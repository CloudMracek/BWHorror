#include <stdint.h>
#include <psxspu.h>
#include <hwregs_c.h>

typedef struct {
	uint32_t magic;			// 0x70474156 ("VAGp") for mono files
	uint32_t version;
	uint32_t interleave;	// Unused in mono files
	uint32_t size;			// Big-endian, in bytes
	uint32_t sample_rate;	// Big-endian, in Hertz
	uint32_t _reserved[3];
	char     name[16];
} VAG_Header;

typedef struct {
	uint16_t dAPF1;   // Reverb APF Offset 1
	uint16_t dAPF2;   // Reverb APF Offset 2
	uint16_t vIIR;    // Reverb Reflection Volume 1
	uint16_t vCOMB1;  // Reverb Comb Volume 1
	uint16_t vCOMB2;  // Reverb Comb Volume 2
	uint16_t vCOMB3;  // Reverb Comb Volume 3
	uint16_t vCOMB4;  // Reverb Comb Volume 4
	uint16_t vWALL;   // Reverb Reflection Volume 2
	uint16_t vAPF1;   // Reverb APF Volume 1
	uint16_t vAPF2;   // Reverb APF Volume 2
	uint16_t mLSAME;  // Reverb Same Side Reflection Address 1 Left
	uint16_t mRSAME;  // Reverb Same Side Reflection Address 1 Right
	uint16_t mLCOMB1; // Reverb Comb Address 1 Left
	uint16_t mRCOMB1; // Reverb Comb Address 1 Right
	uint16_t mLCOMB2; // Reverb Comb Address 2 Left
	uint16_t mRCOMB2; // Reverb Comb Address 2 Right
	uint16_t dLSAME;  // Reverb Same Side Reflection Address 2 Left
	uint16_t dRSAME;  // Reverb Same Side Reflection Address 2 Right
	uint16_t mLDIFF;  // Reverb Different Side Reflect Address 1 Left
	uint16_t mRDIFF;  // Reverb Different Side Reflect Address 1 Right
	uint16_t mLCOMB3; // Reverb Comb Address 3 Left
	uint16_t mRCOMB3; // Reverb Comb Address 3 Right
	uint16_t mLCOMB4; // Reverb Comb Address 4 Left
	uint16_t mRCOMB4; // Reverb Comb Address 4 Right
	uint16_t dLDIFF;  // Reverb Different Side Reflect Address 2 Left
	uint16_t dRDIFF;  // Reverb Different Side Reflect Address 2 Right
	uint16_t mLAPF1;  // Reverb APF Address 1 Left
	uint16_t mRAPF1;  // Reverb APF Address 1 Right
	uint16_t mLAPF2;  // Reverb APF Address 2 Left
	uint16_t mRAPF2;  // Reverb APF Address 2 Right
	uint16_t vLIN;    // Reverb Input Volume Left
	uint16_t vRIN;    // Reverb Input Volume Right
} ReverbPreset;

#define SWAP_ENDIAN(x) ( \
	(((uint32_t) (x) & 0x000000ff) << 24) | \
	(((uint32_t) (x) & 0x0000ff00) <<  8) | \
	(((uint32_t) (x) & 0x00ff0000) >>  8) | \
	(((uint32_t) (x) & 0xff000000) >> 24) \
)

int upload_sample(const void *data, int size);
int play_sample(int addr, int sample_rate);
void setupReverb(int volume, int bufferSize, const ReverbPreset *preset);
void enableReverbOnChannels(uint32_t bits);
