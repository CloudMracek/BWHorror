#pragma once
#include <stdint.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <psxpad.h>
#include <inline_c.h>

#include "options.h"

void setGameLoopCallback(void (*ptr)());

void initDisplay(void);
void display(void);