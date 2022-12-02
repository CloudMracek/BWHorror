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
extern const uint32_t yay[];
extern const uint32_t victory[];

int i = 0;
int p_ang = 0;

int ghostCount = 0;

OBJECT maze;
OBJECT cube;
VECTOR cubePos = {0, 0, 0};
SVECTOR cubeRot = {0, 0, 0};
VECTOR light_point;

OBJECT duch0;
OBJECT duch1;
OBJECT duch2;
OBJECT duch3;
OBJECT duch4;

VECTOR duch0Pos = {-1000, -400, 5216};
VECTOR duch1Pos = {-9997, -400, 3033};
VECTOR duch2Pos = {-8056, -400, -104};
VECTOR duch3Pos = {-2978, -400, -2008};
VECTOR duch4Pos = {-8086, -400, 7003};

VECTOR putawayPos = {50000, 50000, 50000};

int steps_addr;
int steps_sr;

int yay_addr;
int yay_sr;

int victory_addr;
int victory_sr;

void duchCollision()
{
    if (abs(getCamPosWorld().vx - duch0.pos.vx) < 200 && abs(getCamPosWorld().vz - duch0.pos.vz) < 400)
    {
        duch0.pos = putawayPos;
        ghostCount++;
        if (ghostCount != 5)
            play_sample(yay_addr, yay_sr);
    }
    if (abs(getCamPosWorld().vx - duch1.pos.vx) < 200 && abs(getCamPosWorld().vz - duch1.pos.vz) < 400)
    {
        duch1.pos = putawayPos;
        ghostCount++;
        if (ghostCount != 5)
            play_sample(yay_addr, yay_sr);
    }
    if (abs(getCamPosWorld().vx - duch2.pos.vx) < 200 && abs(getCamPosWorld().vz - duch2.pos.vz) < 400)
    {
        duch2.pos = putawayPos;
        ghostCount++;
        if (ghostCount != 5)
            play_sample(yay_addr, yay_sr);
    }
    if (abs(getCamPosWorld().vx - duch3.pos.vx) < 200 && abs(getCamPosWorld().vz - duch3.pos.vz) < 400)
    {
        duch3.pos = putawayPos;
        ghostCount++;
        if (ghostCount != 5)
            play_sample(yay_addr, yay_sr);
    }
    if (abs(getCamPosWorld().vx - duch4.pos.vx) < 200 && abs(getCamPosWorld().vz - duch4.pos.vz) < 400)
    {
        duch4.pos = putawayPos;
        ghostCount++;
        if (ghostCount != 5)
            play_sample(yay_addr, yay_sr);
    }
    if (ghostCount == 1)
    {
        endGame();
    }
}

void endGame()
{

    play_sample(victory_addr, victory_sr);
    int i = 0;
    while (i < 500)
    {
        SpuSetKey(0, 1 << 0);
        FntPrint(-1, "\n\n\n\n\n\n\n\n\n\n\n\n\n                 YOU WIN!\n");
        display(1);
        i++;
    }
    ghostCount = 0;
    VECTOR camPos = {-350 << 12, -600 << 12, 0};
    VECTOR camRot = {0, 1024 << 12, 0};

    setCamPos(camPos);
    setCamRot(camRot);

    duch0.pos = duch0Pos;
    duch1.pos = duch1Pos;
    duch2.pos = duch2Pos;
    duch3.pos = duch3Pos;
    duch4.pos = duch4Pos;
}

void gameInit()
{
    loadTexture(bandwidth_face, &cube.texture);
    cube.texture.texture_size = 64;
    fillMesh_cube(&cube.mesh);

    duch0.texture.texture_size = 64;

    fillMesh_maze(&maze.mesh);
    maze.texture = cube.texture;
    maze.texture.texture_size = 128;

    loadTexture(duch, &duch0.texture);
    fillMesh_duch(&duch0.mesh);

    duch1.mesh = duch0.mesh;
    duch1.texture = duch0.texture;
    duch2.mesh = duch0.mesh;
    duch2.texture = duch0.texture;
    duch3.mesh = duch0.mesh;
    duch3.texture = duch0.texture;
    duch4.mesh = duch0.mesh;
    duch4.texture = duch0.texture;

    duch0.pos = duch0Pos;
    duch1.pos = duch1Pos;
    duch2.pos = duch2Pos;
    duch3.pos = duch3Pos;
    duch4.pos = duch4Pos;

    VAG_Header *steps_vag = (VAG_Header *)steps;
    steps_addr = upload_sample(&steps_vag[1], SWAP_ENDIAN(steps_vag->size));
    steps_sr = SWAP_ENDIAN(steps_vag->sample_rate);
    int ch = play_sample(steps_addr, steps_sr);

    VAG_Header *yay_vag = (VAG_Header *)yay;
    yay_addr = upload_sample(&yay_vag[1], SWAP_ENDIAN(yay_vag->size));
    yay_sr = SWAP_ENDIAN(yay_vag->sample_rate);

    VAG_Header *victory_vag = (VAG_Header *)victory;
    victory_addr = upload_sample(&victory_vag[1], SWAP_ENDIAN(victory_vag->size));
    victory_sr = SWAP_ENDIAN(victory_vag->sample_rate);

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
    setupReverb(0x7ffe, 0xade0, &rp);
    enableReverbOnChannels(1 << 0);
}

void gameLoop()
{
    setVector(&light_point, (icos(p_ang) >> 2) >> 2, -350, (isin(p_ang) >> 2) >> 2);
    p_ang += 16;
    setLightPosition(getCamPosWorld());

    duchCollision();

    FntPrint(-1, "            Ghosts found: %d/5\n", ghostCount);

    sortObject(&duch0);
    sortObject(&duch1);
    sortObject(&duch2);
    sortObject(&duch3);
    sortObject(&duch4);
    sortObject(&maze);
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
        deltaTime = display(0);
        pollInput(deltaTime);
    }
}
