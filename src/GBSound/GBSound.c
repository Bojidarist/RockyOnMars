#include <gb/gb.h>
#include "GBSound.h"

void setup_sound(char sound_on, uint8_t channel_volume, uint8_t channels) {
    // https://github.com/bwhitman/pushpin/blob/master/src/gbsound.txt
    // https://gist.github.com/gingemonster/600c33f7dd97ecbf785eca8c84772c9a
    if (sound_on > 0)
    {
        // these registers must be in this specific order!
        NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
        NR50_REG = channel_volume; // sets the volume for both left and right channel just set to max 0x77
        NR51_REG = channels; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
    }
    else {
        NR52_REG = 0x0;
    }
}

void play_move_sound(void) {
    setup_sound(1, 0x66, AUDTERM_1_LEFT);
    NR10_REG = 0x0;
    NR11_REG = 0x20;
    NR12_REG = 0x73;
    NR13_REG = 0x00;
    NR14_REG = 0xC3;
}

void play_enemy_shoot_sound(void) {
    setup_sound(1, 0x44, AUDTERM_2_RIGHT);
    NR21_REG = 0x40;
    NR22_REG = 0x73;
    NR23_REG = 0x00;
    NR24_REG = 0xC3;
}
