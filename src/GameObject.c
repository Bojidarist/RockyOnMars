#pragma once
#include <gb/gb.h>

typedef struct GameObject {
    unsigned char isMetaSprite;
    uint8_t nbOfFrames;
    uint8_t currentFrame;
    uint8_t spriteIndex;
    uint8_t firstTileIndex;
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} GameObject;
