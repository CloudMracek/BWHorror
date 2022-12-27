#include "sound.h"
#define ALLOC_START_ADDR 0x1010
#define SPU_REVERB_REGS ((volatile uint16_t *) (IOBASE | 0x1dc0))

#define getADSR1(attackMode, attackShift, attackStep, decayShift, sustainLevel) \
	(((attackMode) << 15) | ((attackShift) << 10) | ((attackStep) << 8) | ((decayShift) << 4) | (sustainLevel))
#define getADSR2(sustainMode, sustainDir, sustainShift, sustainStep, releaseMode, releaseShift) \
	(((sustainMode) << 15) | ((sustainDir) << 14) | ((sustainShift) << 8) | ((sustainStep) << 6) | ((releaseMode) << 5) | (releaseShift))

static int next_sample_addr = ALLOC_START_ADDR;
static int next_channel = 0;
static int skipChannel0 = 0;

int upload_sample(const void *data, int size)
{
	// Round the size up to the nearest multiple of 64, as SPU DMA transfers
	// are done in 64-byte blocks.
	int _addr = next_sample_addr;
	int _size = (size + 63) & 0xffffffc0;

	SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
	SpuSetTransferStartAddr(_addr);

	SpuWrite((const uint32_t *)data, _size);
	SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

	next_sample_addr = _addr + _size;
	return _addr;
}

int play_sample(int addr, int sample_rate)
{
	int ch = next_channel;
	if(skipChannel0 && ch == 0) {
		ch = 1;
	}

	// Make sure the channel is stopped.
	SpuSetKey(0, 1 << ch);

	// Set the channel's sample rate and start address. Note that the SPU
	// expects the sample rate to be in 4.12 fixed point format (with
	// 1.0 = 44100 Hz) and the address in 8-byte units; psxspu.h provides the
	// getSPUSampleRate() and getSPUAddr() macros to convert values to these
	// units.
	SPU_CH_FREQ(ch) = getSPUSampleRate(sample_rate);
	SPU_CH_ADDR(ch) = getSPUAddr(addr);

	// Set the channel's volume and ADSR parameters (0x80ff and 0x1fee are
	// dummy values that disable the ADSR envelope entirely).
	SPU_CH_VOL_L(ch) = 0x3fff;
	SPU_CH_VOL_R(ch) = 0x3fff;
	SPU_CH_ADSR1(ch) = getADSR1(0, 0, 0, 15, 15);
	SPU_CH_ADSR2(ch) = getADSR2(0, 0, 0, 0, 0, 0);

	// Start the channel.
	SpuSetKey(1, 1 << ch);

	if(ch==0) {
		skipChannel0 = 1;
	}

	next_channel = (ch + 1) % 24;
	return ch;
}


// Volume must be in 0x0000-0x7fff range.
// Buffer size must be a multiple of 16 bytes and >= 16.
void setupReverb(int volume, int bufferSize, const ReverbPreset *preset) {
	const uint16_t *_preset = (const uint16_t *) preset;
	// This must be done 16 bits at a time, not 8 or 32.
	for (int i = 0; i < (sizeof(ReverbPreset) / 2); i++)
		SPU_REVERB_REGS[i] = _preset[i];

	SPU_REVERB_VOL_L = (uint16_t) volume;
	SPU_REVERB_VOL_R = (uint16_t) volume;
	SpuSetReverbAddr(0x80000 - bufferSize);

	SPU_CTRL |= 1 << 7;
}

void disableReverb(void) {
	SPU_CTRL &= ~(1 << 7);

	SPU_REVERB_VOL_L = 0;
	SPU_REVERB_VOL_R = 0;
	SpuSetReverbAddr(0x7fff0);
}

void enableReverbOnChannels(uint32_t bits) {
	SPU_REVERB_ON1 = (uint16_t) bits;
	SPU_REVERB_ON2 = (uint16_t) (bits >> 16);
}

void enableReverbOnCDAudio(int enable) {
	if (enable)
		SPU_CTRL |= 1 << 2;
	else
		SPU_CTRL &= ~(1 << 2);
}

void freeSpuRAM(void)
{
	SpuWrite((const uint32_t *)NULL, 0x1010 + (((13 << 11) * 4) * 2)) + 64;	
}
