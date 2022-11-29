#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

#include "display.h"
#include "texture.h"
#include "input.h"

#include "../meshes/cube.c"
#include "../meshes/maze.c"
#include "../meshes/duch.c"

extern const uint32_t bandwidth_face[];
extern const uint32_t duch[];

int i = 0;
int p_ang = 0;

OBJECT maze;
OBJECT cube;
OBJECT duchO;
VECTOR cubePos = {0, 0, 0};
VECTOR duchPos = {-1000, -400, 5216};
SVECTOR cubeRot = {0, 0, 0};
VECTOR light_point;

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
    initInput();
    gameInit();
    setGameLoopCallback(&gameLoop);
    while (true)
    {
        deltaTime = display();
        pollInput(deltaTime);
    }
}
