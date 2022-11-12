#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

#include "display.h"

int i = 0;

void gameLoop() {
    FntPrint(-1, "TEST %d", i);
    i++;
    if(i > 50) {
        i = 0;
    }
}

int main(int argc, const char *argv[])
{
	setGameLoopCallback(gameLoop);
    initDisplay();

    while(true) {
        display();
    }
}
