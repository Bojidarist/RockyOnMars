#include <gb/gb.h>
#include <stdio.h>
#include "sprites/Rocky.c"

void main() {
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
                break;
            case J_DOWN:
                move_sprite(0, 20, 140);
                break;
            default:
                break;
        }
    }
}
