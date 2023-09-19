#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include "GBSound/GBSound.c"
#include "GameObject.c"
#include "GBUtils/Utils.c"
#include <rand.h>

void main(void) {
    setup_sound(1, 0x77, 0xFF);
    uint16_t score = 0;
    uint8_t fireWaitTime = 180;

    GameObject ufo;
    create_ufo(&ufo, 0, 0);
    ufo.x = 140;
    ufo.y = 132;
    render_object(&ufo);

    GameObject rocky;
    create_rocky(&rocky, 4, 16);
    rocky.x = 20;
    rocky.y = 140;
    render_object(&rocky);

    GameObject hoverboard;
    create_hoverboard(&hoverboard, 5, 17);
    hoverboard.x = rocky.x;
    hoverboard.y = rocky.y + 8;
    render_object(&hoverboard);

    SHOW_SPRITES;

    while (1)
    {
        wait_vbl_done();
        if(sys_time % 30 == 0) {
            score += 1;
            gotoxy(1, 1); printf("Score:%d", score);
        }
        
        switch (joypad())
        {
            case J_UP:
                if (rocky.y != 120) {
                    rocky.y = 120;
                    hoverboard.y = rocky.y + 8;
                    render_object(&rocky);
                    render_object(&hoverboard);
                    play_move_sound();
                }
                break;
            case J_DOWN:
                if (rocky.y != 140) {
                    rocky.y = 140;
                    hoverboard.y = rocky.y + 8;
                    render_object(&rocky);
                    render_object(&hoverboard);
                    play_move_sound();
                }
                break;
        }

        if(sys_time % 20 == 0) {
            uint8_t r = rand() % 2;
            if (r) {
                ufo.y = 124;
            }
            else {
                ufo.y = 132;
            }
        }
        render_object(&ufo);
        if(sys_time % fireWaitTime == 0) {
            play_enemy_shoot_sound();
        }
    }
}
