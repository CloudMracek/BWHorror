#pragma once
#include <psxgte.h>
#include <psxpad.h>
#include <psxspu.h>
#include <stdio.h>

#include "collision.h"

void initInput(void);

void pollInput(int deltaTime);

VECTOR* getCamPos(void);
VECTOR getCamPosWorld(void);
SVECTOR* getCamRot(void);