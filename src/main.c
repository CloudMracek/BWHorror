#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

#include "display.h"
#include "texture.h"
#include "input.h"

#include "../meshes/cube.c"

extern const uint32_t bandwidth_face[];

int i = 0;

OBJECT cube;
VECTOR cubePos = {0, 0, 0};
SVECTOR cubeRot = {0, 0, 0};

void gameInit()
{
    loadTexture(bandwidth_face, &cube.texture);
    fillMesh_cube(cube.mesh);
    cube.pos = cubePos;
    cube.rot = cubeRot;
}

void gameLoop()
{
    sortObject(&cube);
    FntPrint(-1, "TEST %d", i);
    i++;
    if (i > 50)
    {
        i = 0;
    }
}

int main(int argc, const char *argv[])
{
    initDisplay();
    initInput();
    gameInit();
    setGameLoopCallback(&gameLoop);
    while (true)
    {
        display();
        pollInput();
    }
}
