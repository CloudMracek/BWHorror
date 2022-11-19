#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

#include "display.h"
#include "texture.h"
#include "input.h"

#include "../meshes/cube.c"
#include "../meshes/floor.c"

extern const uint32_t bandwidth_face[];

int i = 0;
int p_ang = 0;

OBJECT floor;
OBJECT cube;
VECTOR cubePos = {0, 0, 0};
SVECTOR cubeRot = {0, 0, 0};
VECTOR light_point;

void gameInit()
{
    loadTexture(bandwidth_face, &cube.texture);
    cube.texture.texture_size = 64;
    fillMesh_cube(cube.mesh);

    fillMesh_floor(floor.mesh);
    floor.texture = cube.texture;

    cube.pos = cubePos;
    cube.rot = cubeRot;
    
}

void gameLoop()
{
    setVector(&light_point, (icos(p_ang) >> 2) >> 2, -350, (isin(p_ang) >> 2) >> 2);
	p_ang += 16;

    setLightPosition(light_point);
    //cube.pos = light_point;
    floor.pos.vz = 1000;
    floor.pos.vy = -1000;

    //sortObject(&cube);
    sortObject(&floor);
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
