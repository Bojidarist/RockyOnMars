#include <gb/gb.h>
#include "../GameObject.c"

void performant_delay(uint8_t numloops);
unsigned char check_collisions(GameObject* one, GameObject* two);
void render_character(GameObject* character);
void create_ufo(GameObject* ufo, uint8_t spriteIndex, uint8_t firstTileIndex);
void create_rocky(GameObject* rocky, uint8_t spriteIndex, uint8_t firstTileIndex);
void create_hoverboard(GameObject* hoverboard, uint8_t spriteIndex, uint8_t firstTileIndex);