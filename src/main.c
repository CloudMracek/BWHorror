#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxspu.h>

#include "display.h"
#include "texture.h"
#include "input.h"
#include "sound.h"

#include "../meshes/cube.c"
#include "../meshes/maze.c"
#include "../meshes/duch.c"

extern const uint32_t bandwidth_face[];
extern const uint32_t duch[];

extern const uint32_t steps[];

int i = 0;
int p_ang = 0;

OBJECT maze;
OBJECT cube;
OBJECT duchO;
VECTOR cubePos = {0, 0, 0};
VECTOR duchPos = {-1000, -400, 5216};
SVECTOR cubeRot = {0, 0, 0};
VECTOR light_point;

int steps_addr;
int steps_sr;

void gameInit()
{
    loadTexture(bandwidth_face, &cube.texture);
    cube.texture.texture_size = 64;
    fillMesh_cube(&cube.mesh);

    loadTexture(duch, &duchO.texture);
    fillMesh_duch(&duchO.mesh);
    duchO.pos = duchPos;
    duchO.texture.texture_size = 64;

    fillMesh_maze(&maze.mesh);
    maze.texture = cube.texture;
    maze.texture.texture_size = 128;

    cube.rot = cubeRot;

    VAG_Header *steps_vag = (VAG_Header *) steps;
	steps_addr = upload_sample(&steps_vag[1], SWAP_ENDIAN(steps_vag->size));
	steps_sr = SWAP_ENDIAN(steps_vag->sample_rate);
    int ch = play_sample(steps_addr, steps_sr);

    ReverbPreset rp;

    rp.dAPF1 = 0x01A5;
	rp.dAPF2 = 0x0139;
	rp.vIIR = 0x6000;
	rp.vCOMB1 = 0x5000;
	rp.vCOMB2 = 0x4c00;
	rp.vCOMB3 = 0xB800;
	rp.vCOMB4 = 0xBC00;
	rp.vWALL = 0xc000;
	rp.vAPF1 = 0x6000;
	rp.vAPF2 = 0x5C00;
	rp.mLSAME = 0x15BA;
	rp.mRSAME = 0x11BB;
	rp.mLCOMB1 = 0x14C2;
	rp.mRCOMB1 = 0x10BD;
	rp.mLCOMB2 = 0x11BC;
	rp.mRCOMB2 = 0x0dc1;
	rp.dLSAME = 0x11c0;
	rp.dRSAME = 0x0dc3;
	rp.mLDIFF = 0x0dc0;
	rp.mRDIFF = 0x09c1;
	rp.mLCOMB3 = 0x0bc4;
	rp.mRCOMB3 = 0x07c1;
	rp.mLCOMB4 = 0x0a00;
	rp.mRCOMB4 = 0x06cd;
	rp.dLDIFF = 0x09c2;
	rp.dRDIFF = 0x05c1;
	rp.mLAPF1 = 0x05c0;
	rp.mRAPF1 = 0x041a;
	rp.mLAPF2 = 0x0274;
	rp.mRAPF2 = 0x013a;
	rp.vLIN = 0x8000;
	rp.vRIN = 0x8000;
    setupReverb(0x7ffe, 0xade0 , &rp);
    enableReverbOnChannels(1 << 0);
}

void gameLoop()
{
    setVector(&light_point, (icos(p_ang) >> 2) >> 2, -350, (isin(p_ang) >> 2) >> 2);
    p_ang += 16;
    setLightPosition(getCamPosWorld());

    FntPrint(-1, "Cube: %d %d %d\n", cube.pos.vx, cube.pos.vy, cube.pos.vz);

    sortObject(&duchO);
    sortObject(&maze);
    FntPrint(-1, "TEST %d", i);
    i++;
    if (i > 50)
    {
        i = 0;
    }
}

int deltaTime;
int main(int argc, const char *argv[])
{
    initDisplay();
    SpuInit();
    
    initInput();
    gameInit();
    setGameLoopCallback(&gameLoop);
    while (true)
    {
        deltaTime = display();
        pollInput(deltaTime);
    }
}
