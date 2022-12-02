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
void setCamPos(VECTOR pos);
SVECTOR* getCamRot(void);
void setCamRot(VECTOR rot);
