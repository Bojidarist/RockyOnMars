#include <gb/gb.h>
#include "../GameObject.c"
#include "../Sprites/UFO.c"
#include "../Sprites/Rocky.c"
#include "../Sprites/Hoverboard.c"
#include "../Sprites/Laser.c"
#include "../Sprites/Moon.c"

uint8_t utilsCurrentSpriteIndex = 0;
uint8_t utilsCurrentTileIndex = 0;

void performant_delay(uint8_t numLoops) {
    uint8_t i;
    for(i = 0; i < numLoops; i++) {
        wait_vbl_done();
    }     
}

unsigned char check_collisions(GameObject* one, GameObject* two) {
    return one->x < two->x + two->width && one->x + one->width > two->x && one->y < two->y + two->height && one->y + one->height > two->y;
}

void render_object(GameObject* character) {
    // https://gbdev.gg8.se/wiki/articles/GBDK_Sprite_Tutorial
    // https://github.com/mrombout/gbdk_playground/tree/master
    if (character->currentFrame >= character->nbOfFrames - 1) {
        character->currentFrame = 0;
    }
    else {
        character->currentFrame++;
    }

    if (character->isMetaSprite) {
        set_sprite_tile(character->spriteIndex, character->firstTileIndex + (4 * character->currentFrame));
        move_sprite(character->spriteIndex, character->x, character->y);

        set_sprite_tile(character->spriteIndex + 1, character->firstTileIndex + (4 * character->currentFrame) + 1);
        move_sprite(character->spriteIndex + 1, character->x, character->y + 8);

        set_sprite_tile(character->spriteIndex + 2, character->firstTileIndex + (4 * character->currentFrame) + 2);
        move_sprite(character->spriteIndex + 2, character->x + 8, character->y);

        set_sprite_tile(character->spriteIndex + 3, character->firstTileIndex + (4 * character->currentFrame) + 3);
        move_sprite(character->spriteIndex + 3, character->x + 8, character->y + 8);
    }
    else {
        set_sprite_tile(character->spriteIndex, character->firstTileIndex + character->currentFrame);
        move_sprite(character->spriteIndex, character->x, character->y);
    }
}

void manageSpriteTileIndex(GameObject* gameObject) {
    if (gameObject->isMetaSprite) {
        utilsCurrentTileIndex += gameObject->nbOfFrames * 4;
        utilsCurrentSpriteIndex += 4;
    }
    else {
        utilsCurrentTileIndex += gameObject->nbOfFrames;
        utilsCurrentSpriteIndex += 1;
    }
}

void create_ufo(GameObject* ufo) {
    ufo->isMetaSprite = 1;
    ufo->nbOfFrames = 4;
    ufo->currentFrame = 0;
    ufo->spriteIndex = utilsCurrentSpriteIndex;
    ufo->firstTileIndex = utilsCurrentTileIndex;
    ufo->height = 16;
    ufo->width = 16;
    ufo->x = 0;
    ufo->y = 0;

    manageSpriteTileIndex(ufo);
    set_sprite_data(ufo->firstTileIndex, 4*4, UFO);
}

void create_rocky(GameObject* rocky) {
    rocky->isMetaSprite = 0;
    rocky->nbOfFrames = 1;
    rocky->currentFrame = 0;
    rocky->spriteIndex = utilsCurrentSpriteIndex;
    rocky->firstTileIndex = utilsCurrentTileIndex;
    rocky->height = 8;
    rocky->width = 8;
    rocky->x = 0;
    rocky->y = 0;

    manageSpriteTileIndex(rocky);
    set_sprite_data(rocky->firstTileIndex, 1, Rocky);
}

void create_hoverboard(GameObject* hoverboard) {
    hoverboard->isMetaSprite = 0;
    hoverboard->nbOfFrames = 4;
    hoverboard->currentFrame = 0;
    hoverboard->spriteIndex = utilsCurrentSpriteIndex;
    hoverboard->firstTileIndex = utilsCurrentTileIndex;
    hoverboard->height = 8;
    hoverboard->width = 8;
    hoverboard->x = 0;
    hoverboard->y = 0;

    manageSpriteTileIndex(hoverboard);
    set_sprite_data(hoverboard->firstTileIndex, 4, Hoverboard);
}

void create_laser(GameObject* laser) {
    laser->isMetaSprite = 0;
    laser->nbOfFrames = 1;
    laser->currentFrame = 0;
    laser->spriteIndex = utilsCurrentSpriteIndex;
    laser->firstTileIndex = utilsCurrentTileIndex;
    laser->height = 8;
    laser->width = 8;
    laser->x = 0;
    laser->y = 0;

    manageSpriteTileIndex(laser);
    set_sprite_data(laser->firstTileIndex, 1, Laser);
}

void create_moon(GameObject* moon) {
    moon->isMetaSprite = 1;
    moon->nbOfFrames = 1;
    moon->currentFrame = 0;
    moon->spriteIndex = utilsCurrentSpriteIndex;
    moon->firstTileIndex = utilsCurrentTileIndex;
    moon->height = 16;
    moon->width = 16;
    moon->x = 0;
    moon->y = 0;

    manageSpriteTileIndex(moon);
    set_sprite_data(moon->firstTileIndex, 4, Moon);
}
