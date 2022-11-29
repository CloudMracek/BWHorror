#include "input.h"

VECTOR tpos;
SVECTOR trot;

VECTOR cam_pos;
VECTOR cam_rot;

PADTYPE *pad;
uint8_t padBuffer[2][34];

void initInput(void)
{
	InitPAD(padBuffer[0], 34, padBuffer[1], 34);
	StartPAD();
	ChangeClearPAD(0);
}

void pollInput(int deltaTime)
{
	VECTOR positon = getCamPosWorld();
	printf("%d\n", isPlayerOnMesh(&positon));
	pad = (PADTYPE *)&padBuffer[0][0];
	// Parse controller input
	// Divide out fractions of camera rotation
	trot.vx = cam_rot.vx >> 12;
	trot.vy = cam_rot.vy >> 12;
	trot.vz = cam_rot.vz >> 12;

	tpos.vx = -cam_pos.vx >> 12;
	tpos.vy = -cam_pos.vy >> 12;
	tpos.vz = -cam_pos.vz >> 12;
	FntPrint(-1, "X=%d Y=%d Z=%d\n", cam_pos.vx >> 12, cam_pos.vy >> 12,
			 cam_pos.vz >> 12);
	FntPrint(-1, "RX=%d RY=%d\n", cam_rot.vx >> 12, cam_rot.vy >> 12);

	if (pad->stat == 0)
	{

		// For digital pad, dual-analog and dual-shock
		if ((pad->type == 0x4) || (pad->type == 0x5) || (pad->type == 0x7))
		{

			// The button status bits are inverted,
			// so 0 means pressed in this case

			// Look controls
			if (!(pad->btn & PAD_UP))
			{

				// Look up
				cam_rot.vx -= deltaTime * ONE * 16;
			}
			else if (!(pad->btn & PAD_DOWN))
			{

				// Look down
				cam_rot.vx += deltaTime * ONE * 16;
			}

			if (!(pad->btn & PAD_LEFT))
			{

				// Look left
				cam_rot.vy += deltaTime * ONE * 16;
			}
			else if (!(pad->btn & PAD_RIGHT))
			{

				// Look right
				cam_rot.vy -= deltaTime * ONE * 16;
			}

			// Movement controls
			if (!(pad->btn & PAD_TRIANGLE))
			{

				// Move forward
				cam_pos.vx -= deltaTime * ((isin(trot.vy) * icos(trot.vx)) >> 12) << 4;
				cam_pos.vy += deltaTime * isin(trot.vx) << 4;
				cam_pos.vz += deltaTime * ((icos(trot.vy) * icos(trot.vx)) >> 12) << 4;
			}
			else if (!(pad->btn & PAD_CROSS))
			{

				// Move backward
				cam_pos.vx += deltaTime * ((isin(trot.vy) * icos(trot.vx)) >> 12) << 4;
				cam_pos.vy -= deltaTime * isin(trot.vx) << 4;
				cam_pos.vz -= deltaTime * ((icos(trot.vy) * icos(trot.vx)) >> 12) << 4;
			}

			if (!(pad->btn & PAD_SQUARE))
			{

				// Slide left
				cam_pos.vx -= deltaTime * icos(trot.vy) << 4;
				cam_pos.vz -= deltaTime * isin(trot.vy) << 4;
			}
			else if (!(pad->btn & PAD_CIRCLE))
			{

				// Slide right
				cam_pos.vx += deltaTime * icos(trot.vy) << 4;
				cam_pos.vz += deltaTime * isin(trot.vy) << 4;
			}

			if (!(pad->btn & PAD_R1))
			{

				// Slide up
				cam_pos.vx -= deltaTime * ((isin(trot.vy) * isin(trot.vx)) >> 12) << 4;
				cam_pos.vy -= deltaTime * icos(trot.vx) << 4;
				cam_pos.vz += deltaTime * ((icos(trot.vy) * isin(trot.vx)) >> 12) << 4;
			}

			if (!(pad->btn & PAD_R2))
			{

				// Slide down
				cam_pos.vx += deltaTime * ((isin(trot.vy) * isin(trot.vx)) >> 12) << 4;
				cam_pos.vy += deltaTime * icos(trot.vx) << 4;
				cam_pos.vz -= deltaTime * ((icos(trot.vy) * isin(trot.vx)) >> 12) << 4;
			}
		}

		// For dual-analog and dual-shock (analog input)
		if ((pad->type == 0x5) || (pad->type == 0x7))
		{

			// Moving forwards and backwards
			if (((pad->ls_y - 128) < -16) || ((pad->ls_y - 128) > 16))
			{

				cam_pos.vx += deltaTime *
					(((isin(trot.vy) * icos(trot.vx)) >> 12) * (pad->ls_y - 128)) >> 5;
				cam_pos.vy -= deltaTime * (isin(trot.vx) * (pad->ls_y - 128)) >> 5;
				cam_pos.vz -= deltaTime *
					(((icos(trot.vy) * icos(trot.vx)) >> 12) * (pad->ls_y - 128)) >> 5;
			}

			// Strafing left and right
			if (((pad->ls_x - 128) < -16) || ((pad->ls_x - 128) > 16))
			{
				cam_pos.vx += deltaTime * (icos(trot.vy) * (pad->ls_x - 128)) >> 5;
				cam_pos.vz += deltaTime * (isin(trot.vy) * (pad->ls_x - 128)) >> 5;
			}

			// Look up and down
			if (((pad->rs_y - 128) < -16) || ((pad->rs_y - 128) > 16))
			{
				cam_rot.vx += deltaTime * (pad->rs_y - 128) << 9;
			}

			// Look left and right
			if (((pad->rs_x - 128) < -16) || ((pad->rs_x - 128) > 16))
			{
				cam_rot.vy -= deltaTime * (pad->rs_x - 128) << 9;
			}
		}
	}
}

VECTOR *getCamPos(void)
{
	return &tpos;
}

VECTOR getCamPosWorld(void) 
{
	VECTOR negCamPos = {-tpos.vx, -tpos.vy, -tpos.vz};
	negCamPos.vx = cam_pos.vx >> 12;
	negCamPos.vy = cam_pos.vy >> 12;
	negCamPos.vz = cam_pos.vz >> 12;
	return negCamPos;
}

SVECTOR *getCamRot(void)
{
	return &trot;
}