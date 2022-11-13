#pragma once
#include <psxgte.h>
#include <psxpad.h>
#include <stdio.h>

void initInput(void);

void pollInput(void);

VECTOR* getCamPos(void);
SVECTOR* getCamRot(void);