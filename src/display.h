#pragma once
#include <stdint.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <psxpad.h>
#include <inline_c.h>

#include "options.h"
#include "input.h"

typedef struct {
    short v0,v1,v2,v3;
} INDEX;

typedef struct {
    int texture_size;
    TIM_IMAGE tim;
} TEXTURE;

typedef struct {
    int faces_num;
    SVECTOR *vertex_data;
    SVECTOR *normal_data;
    SVECTOR *uv_data;
    INDEX *vertex_indices;
    INDEX *uv_indices;
    int *normal_indices;
} MESH;

typedef struct {
    VECTOR pos;
    SVECTOR rot;
    MESH *mesh;
    TEXTURE texture;
} OBJECT;

void setGameLoopCallback(void (*ptr)());

void initDisplay(void);
void display(void);

void sortObject(OBJECT *obj);