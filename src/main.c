#include <gb/gb.h>
#include <stdio.h>
#include "Sprites/Rocky.c"
#include "GBSound/GBSound.c"

void main(void) {
    setup_sound(1, 0x77, 0xFF);

    set_sprite_data(0, 2, Rocky);
    set_sprite_tile(0, 0);
    move_sprite(0, 20, 140);
    SHOW_SPRITES;

    while (1)
    {
        switch (joypad())
        {
            case J_UP:
                move_sprite(0, 20, 120);
                play_move_sound();
                break;
            case J_DOWN:
                move_sprite(0, 20, 140);
                play_enemy_shoot_sound();
                break;
            default:
                break;
        }
    }
}
