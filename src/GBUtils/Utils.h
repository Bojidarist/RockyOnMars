#include <gb/gb.h>
#include "../GameObject.c"

void performant_delay(uint8_t numloops);
unsigned char check_collisions(GameObject* one, GameObject* two);
void render_object(GameObject* character);
void create_ufo(GameObject* ufo);
void create_rocky(GameObject* rocky);
void create_hoverboard(GameObject* hoverboard);
void create_laser(GameObject* laser);
void create_moon(GameObject* moon);
