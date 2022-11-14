#include "texture.h"

void loadTexture(uint32_t* textureData, TEXTURE* texture) {

    GetTimInfo(textureData, &texture->tim);
	
	LoadImage(texture->tim.prect, texture->tim.paddr);
	DrawSync(0);
    
	if (texture->tim.mode & 8) {
		LoadImage(texture->tim.crect, texture->tim.caddr);
		DrawSync(0);
	}
}