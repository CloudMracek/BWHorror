#include "display.h"

typedef struct
{
	DISPENV disp;
	DRAWENV draw;
	uint32_t *ot[OT_LEN];
	char p[PACKET_LEN];
} DB;

DB db[2];
int db_active = 0;
char *db_nextpri;
RECT screen_clip;

MATRIX omtx, mtx, lmtx, llmtx;

VECTOR l_point = {0, 0, -10};

void (*callback)();

void initDisplay(void)
{
	ResetGraph(0);

	// Setup DB 0
	SetDefDispEnv(&db[0].disp, 0, 0, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&db[0].draw, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);

	setRGB0(&db[0].draw, 0, 0, 0);
	db[0].draw.isbg = 1;
	db[0].draw.dtd = 1;

	// Setup DB 1
	SetDefDispEnv(&db[1].disp, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&db[1].draw, 0, 0, SCREEN_XRES, SCREEN_YRES);

	setRGB0(&db[1].draw, 0, 0, 0);
	db[1].draw.isbg = 1;
	db[1].draw.dtd = 1;

	setRECT(&screen_clip, 0, 0, SCREEN_XRES, SCREEN_YRES);

	InitGeom();
	gte_SetGeomOffset(CENTERX, CENTERY);
	gte_SetGeomScreen(CENTERX);

	gte_SetBackColor(0, 0, 0);

#ifdef LOAD_FONT
	FntLoad(960, 0);
	FntOpen(0, 8, 320, 260, 0, 100);
#endif

	db_active = 0;
	db_nextpri = db[0].p;

	ClearOTagR(db[db_active].ot, OT_LEN);
	PutDrawEnv(&(db[0].draw));
	SetDispMask(1);
}

void display(void)
{

	RotMatrix(getCamRot(), &mtx);
	ApplyMatrixLV(&mtx, getCamPos(), getCamPos());
	TransMatrix(&mtx, getCamPos());

	gte_SetRotMatrix(&mtx);
	gte_SetTransMatrix(&mtx);

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

void setGameLoopCallback(void (*ptr)())
{
	callback = ptr;
}

void sortObject(OBJECT *obj)
{
	int i, p, intensity;
	QUAD *pol4;
	VECTOR v_dir;
	SVECTOR v_nrm;

	RotMatrix(&obj->rot, &omtx);
	TransMatrix(&omtx, &obj->pos);

	CompMatrixLV(&mtx, &omtx, &omtx);

	PushMatrix();

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	pol4 = (QUAD *)db_nextpri;
	for (i = 0; i < obj->mesh.faces_num; i++)
	{

		if (abs(getCamPosWorld().vx - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v0].vx) < 2000 &&
			abs(getCamPosWorld().vz - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v0].vz) < 2000)
		{

			v_dir.vx = l_point.vx - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2].vx;
			v_dir.vy = l_point.vy - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2].vy;
			v_dir.vz = l_point.vz - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2].vz;


			gte_ldv3(&obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v0],
					 &obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v1],
					 &obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2]);

			// Perspective
			gte_rtpt();

			// Backface culling
			gte_nclip();

			gte_stopz(&p);
			// Skip drawing this quad if the first 3 vertices are aligned (p = 0)
			// or in counterclockwise (p < 0) order.

			if(p >= 0)
				continue;

			gte_stsxy3(&pol4->x0, &pol4->x1, &pol4->x2);

			// Load last vertex
			gte_ldv0(&obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v3]);
			gte_rtps();

			// Depth sort
			gte_avsz4();
			gte_stotz(&p);

			if( (p>>2) >= OT_LEN )
					continue;


			gte_stsxy(&pol4->x3);

			setRGB0(pol4, 255, 255, 255);
			gte_ldrgb(&pol4->r0);

			gte_ldv0(&obj->mesh.normal_data[(obj->mesh.normal_indices[i]).v0]);
			

			v_dir.vx = l_point.vx - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v0].vx;
			v_dir.vy = l_point.vy - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v0].vy;
			v_dir.vz = l_point.vz - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v0].vz;

			intensity = 4096*4 - ((
									(v_dir.vx * v_dir.vx) +
									(v_dir.vy * v_dir.vy) +
									(v_dir.vz * v_dir.vz)) >>
								7);

			if (intensity < 0)
				intensity = 0;

			llmtx.m[0][0] = intensity;
			llmtx.m[1][0] = intensity;
			llmtx.m[2][0] = intensity;
			gte_SetColorMatrix(&llmtx);

			// Normalize light direction and set it to light matrix
			VectorNormalS(&v_dir, &v_nrm);
			lmtx.m[0][0] = v_nrm.vx;
			lmtx.m[0][1] = v_nrm.vy;
			lmtx.m[0][2] = v_nrm.vz;
			gte_SetLightMatrix(&lmtx);

			gte_nccs();
			//////////////////////////////
			v_dir.vx = l_point.vx - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v1].vx;
			v_dir.vy = l_point.vy - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v1].vy;
			v_dir.vz = l_point.vz - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v1].vz;

			intensity = 4096*4 - ((
									(v_dir.vx * v_dir.vx) +
									(v_dir.vy * v_dir.vy) +
									(v_dir.vz * v_dir.vz)) >>
								7);

			if (intensity < 0)
				intensity = 0;

			llmtx.m[0][0] = intensity;
			llmtx.m[1][0] = intensity;
			llmtx.m[2][0] = intensity;

			gte_strgb(&pol4->r0);

			gte_SetColorMatrix(&llmtx);

			// Normalize light direction and set it to light matrix
			VectorNormalS(&v_dir, &v_nrm);
			lmtx.m[0][0] = v_nrm.vx;
			lmtx.m[0][1] = v_nrm.vy;
			lmtx.m[0][2] = v_nrm.vz;
			gte_SetLightMatrix(&lmtx);

			gte_nccs();
			//////////////////////////////
			v_dir.vx = l_point.vx - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2].vx;
			v_dir.vy = l_point.vy - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2].vy;
			v_dir.vz = l_point.vz - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v2].vz;

			intensity = 4096*4 - ((
									(v_dir.vx * v_dir.vx) +
									(v_dir.vy * v_dir.vy) +
									(v_dir.vz * v_dir.vz)) >>
								7);

			if (intensity < 0)
				intensity = 0;

			llmtx.m[0][0] = intensity;
			llmtx.m[1][0] = intensity;
			llmtx.m[2][0] = intensity;

			gte_strgb(&pol4->r1);

			gte_SetColorMatrix(&llmtx);

			// Normalize light direction and set it to light matrix
			VectorNormalS(&v_dir, &v_nrm);
			lmtx.m[0][0] = v_nrm.vx;
			lmtx.m[0][1] = v_nrm.vy;
			lmtx.m[0][2] = v_nrm.vz;
			gte_SetLightMatrix(&lmtx);

			gte_nccs();
			//////////////////////////////
			v_dir.vx = l_point.vx - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v3].vx;
			v_dir.vy = l_point.vy - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v3].vy;
			v_dir.vz = l_point.vz - obj->mesh.vertex_data[obj->mesh.vertex_indices[i].v3].vz;

			intensity = 4096*4 - ((
									(v_dir.vx * v_dir.vx) +
									(v_dir.vy * v_dir.vy) +
									(v_dir.vz * v_dir.vz)) >>
								7);

			if (intensity < 0)
				intensity = 0;

			llmtx.m[0][0] = intensity;
			llmtx.m[1][0] = intensity;
			llmtx.m[2][0] = intensity;

			gte_strgb(&pol4->r2);

			gte_SetColorMatrix(&llmtx);

			// Normalize light direction and set it to light matrix
			VectorNormalS(&v_dir, &v_nrm);
			lmtx.m[0][0] = v_nrm.vx;
			lmtx.m[0][1] = v_nrm.vy;
			lmtx.m[0][2] = v_nrm.vz;
			gte_SetLightMatrix(&lmtx);

			gte_nccs();

			setUV4(pol4,
				   obj->mesh.uv_data[obj->mesh.uv_indices[i].v0].vx, obj->mesh.uv_data[obj->mesh.uv_indices[i].v0].vy ,
				   obj->mesh.uv_data[obj->mesh.uv_indices[i].v1].vx,  obj->mesh.uv_data[obj->mesh.uv_indices[i].v1].vy ,
				   obj->mesh.uv_data[obj->mesh.uv_indices[i].v2].vx, obj->mesh.uv_data[obj->mesh.uv_indices[i].v2].vy ,
				   obj->mesh.uv_data[obj->mesh.uv_indices[i].v3].vx,  obj->mesh.uv_data[obj->mesh.uv_indices[i].v3].vy );

			pol4->tpage =
				getTPage(obj->texture.tim.mode, 0, obj->texture.tim.prect->x, obj->texture.tim.prect->y);

			setClut(pol4, obj->texture.tim.crect->x, obj->texture.tim.prect->y);
			gte_strgb(&pol4->r3);
			setPolyGT4(pol4);
			addPrim(&(db[db_active].ot)[p >> 2], pol4);

			pol4++;
			//if (pol4 >= (db_nextpri + PACKET_LEN))
			//	break;
		}
	}

	db_nextpri = (char *)pol4;
	PopMatrix();
}

void setLightPosition(VECTOR lightPos)
{
	l_point = lightPos;
}