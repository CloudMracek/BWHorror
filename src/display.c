#include "display.h"

typedef struct {
    DISPENV disp;
    DRAWENV draw;
    uint32_t *ot[OT_LEN];
    char p[PACKET_LEN];
} DB;

DB db[2];
int db_active = 0;
char *db_nextpri;
RECT screen_clip;

MATRIX omtx, mtx;

void (*callback)();

void initDisplay(void) {
    ResetGraph(0);

    // Setup DB 0
    SetDefDispEnv(&db[0].disp, 0, 0, SCREEN_XRES, SCREEN_YRES);
    SetDefDrawEnv(&db[0].draw, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);

    setRGB0(&db[0].draw, 63, 0, 127);
    db[0].draw.isbg = 1;
    db[0].draw.dtd = 1;

     // Setup DB 1
    SetDefDispEnv(&db[1].disp, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&db[1].draw, 0, 0, SCREEN_XRES, SCREEN_YRES);

    setRGB0(&db[1].draw, 63, 0, 127);
    db[1].draw.isbg = 1;
    db[1].draw.dtd = 1;
    
    setRECT(&screen_clip, 0, 0, SCREEN_XRES, SCREEN_YRES);

    InitGeom();
    gte_SetGeomOffset(CENTERX, CENTERY);
    gte_SetGeomScreen(CENTERX);

    gte_SetBackColor(63, 63, 63);
    
    #ifdef LOAD_FONT
    FntLoad(960, 0);
    FntOpen(0, 8, 320, 260 , 0, 100);
    #endif

    db_active = 0;
    db_nextpri = db[0].p;

    ClearOTagR(db[db_active].ot, OT_LEN);
    PutDrawEnv(&(db[0].draw));
    SetDispMask(1);
}

void display(void) {

    DrawSync(0);
    VSync(0);

    db_active ^= 1;
    db_nextpri = db[db_active].p;

    ClearOTagR(db[db_active].ot, OT_LEN);
    PutDrawEnv(&(db[db_active].draw));
	PutDispEnv(&(db[db_active].disp));

	DrawOTag(db[db_active ^ 1].ot + (OT_LEN - 1));

    #ifdef LOAD_FONT
    FntFlush(-1);
    #endif

    // Call the game loop
    (*callback)();
}

void setGameLoopCallback(void (*ptr)()) {
    callback = ptr;
}

void sortObject(OBJECT *obj) {
	int i, p;
	QUAD *pol4;

	RotMatrix(&obj->rot, &omtx);
	TransMatrix(&omtx, &obj->pos);

	CompMatrixLV(&mtx, &omtx, &omtx);

	PushMatrix();

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	pol4 = (QUAD *)db_nextpri;
	printf("test %d\n", obj->mesh->vertex_data[0].vx);
	for (i = 0; i < obj->mesh->faces_num; i++) {
		printf("test1\n");
		gte_ldv3(&obj->mesh->vertex_data[obj->mesh->vertex_indices[i].v0],
						 &obj->mesh->vertex_data[obj->mesh->vertex_indices[i].v1],
						 &obj->mesh->vertex_data[obj->mesh->vertex_indices[i].v2]);
		gte_rtpt();
		gte_nclip();
		gte_stopz(&p);

		// Skip drawing this quad if the first 3 vertices are aligned (p = 0)
		// or in counterclockwise (p < 0) order.
		if (p <= 0)
			continue;

		gte_avsz3();
		gte_stotz(&p);

		// Skip drawing this face if it's too far from or too close to the
		// camera and wouldn't fit in the OT.
		if (((p >> 2) <= 0) || ((p >> 2) >= OT_LEN))
			continue;

		gte_stsxy0(&pol4->x0);
		gte_stsxy1(&pol4->x1);
		gte_stsxy2(&pol4->x2);
		gte_ldv0(&obj->mesh->vertex_data[obj->mesh->vertex_indices[i].v3]);
		gte_rtps();
		gte_stsxy(&pol4->x3);

		gte_ldrgb(&pol4->r0);
		gte_ldv0(&obj->mesh->normal_data[obj->mesh->normal_indices[i]]);
		gte_ncs();
		gte_strgb(&pol4->r0);

		pol4->tpage =
				getTPage(obj->texture->tim->mode, 0, obj->texture->tim->prect->x, obj->texture->tim->prect->y);
		setClut(pol4, obj->texture->tim->crect->x, obj->texture->tim->crect->y);
		setUV4(pol4,
					obj->mesh->uv_data[obj->mesh->uv_indices[i].v0].vx, obj->texture->texture_size-1-obj->mesh->uv_data[obj->mesh->uv_indices[i].v0].vy,
					obj->mesh->uv_data[obj->mesh->uv_indices[i].v1].vx, obj->texture->texture_size-1-obj->mesh->uv_data[obj->mesh->uv_indices[i].v1].vy,
					obj->mesh->uv_data[obj->mesh->uv_indices[i].v2].vx, obj->texture->texture_size-1-obj->mesh->uv_data[obj->mesh->uv_indices[i].v2].vy,
					obj->mesh->uv_data[obj->mesh->uv_indices[i].v3].vx, obj->texture->texture_size-1-obj->mesh->uv_data[obj->mesh->uv_indices[i].v3].vy);
		setRGB0( pol4, 255, 255, 255 );
		gte_avsz4();
		gte_stotz(&p);
		addPrim(&(db[db_active].ot)[p >> 2], pol4);

		pol4++;
		//if (pol4 >= (db_nextpri + PACKET_LEN))
			//break;
			printf("test3");
	}

	db_nextpri = (char *)pol4;
	PopMatrix();
}