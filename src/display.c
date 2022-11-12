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