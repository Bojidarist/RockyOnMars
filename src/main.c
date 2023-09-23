#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include "GBSound/GBSound.c"
#include "GameObject.c"
#include "GBUtils/Utils.c"
#include <rand.h>

const uint8_t LASER_OBJECTS_COUNT = 5;
GameObject LASER_OBJECTS[5];
uint8_t currentLaserIndex = 0;
uint8_t fireWaitTime = 180;
uint8_t laserSpeed = 1;
uint32_t score = 0;
GameObject ufo;
GameObject rocky;
GameObject hoverboard;
uint8_t isGameOver = 0;

void display_score(void) {
    if(sys_time % 30 == 0) {
        score += 1;
        gotoxy(1, 1); printf("Score:%d", score);
    }
}

void init_game_objects(void) {
    create_ufo(&ufo, 0, 0);
    ufo.x = 140;
    ufo.y = 132;
    render_object(&ufo);

    create_rocky(&rocky, 4, 16);
    rocky.x = 20;
    rocky.y = 140;
    render_object(&rocky);

    create_hoverboard(&hoverboard, 5, 17);
    hoverboard.x = rocky.x;
    hoverboard.y = rocky.y + 8;
    render_object(&hoverboard);

    for (size_t i = 0; i < LASER_OBJECTS_COUNT; i++) {
        GameObject laser;
        create_laser(&laser, 6 + i, 21 + i);
        LASER_OBJECTS[i] = laser;
    }
}

void reset_game(void) {
    cls();
    isGameOver = 0;
    score = 0;
    for (size_t i = 0; i < LASER_OBJECTS_COUNT; i++) {
        LASER_OBJECTS[i].x = 240;
        render_object(&LASER_OBJECTS[i]);
    }
}

void handle_rocky_movement(void) {
    switch (joypad()) {
        case J_UP:
            if (rocky.y != 128) {
                rocky.y = 128;
                hoverboard.y = rocky.y + 8;
                play_move_sound();
            }
            break;
        case J_DOWN:
            if (rocky.y != 140) {
                rocky.y = 140;
                hoverboard.y = rocky.y + 8;
                play_move_sound();
            }
            break;
    }
    render_object(&rocky);
    render_object(&hoverboard);
}

void handle_ufo_movement(void) {
    if(sys_time % 20 == 0) {
        uint8_t r = rand() % 2;
        if (r) {
            ufo.y = 120;
        }
        else {
            ufo.y = 132;
        }
    }
    render_object(&ufo);
}

void handle_lasers(void) {
    if(sys_time % fireWaitTime == 0) {
        LASER_OBJECTS[currentLaserIndex].x = ufo.x;
        LASER_OBJECTS[currentLaserIndex].y = ufo.y + 9; // So it looks like it is shot from the UFO's gun
        currentLaserIndex++;
        if (currentLaserIndex >= LASER_OBJECTS_COUNT) {
            currentLaserIndex = 0;
        }

        play_enemy_shoot_sound();
    }

    for (size_t i = 0; i < LASER_OBJECTS_COUNT; i++) {
        if (LASER_OBJECTS[i].x < 180) {
            LASER_OBJECTS[i].x -= laserSpeed;
        }
        render_object(&LASER_OBJECTS[i]);
    }
}

void check_game_over(void) {
    if (isGameOver) {
        return;
    }
    
    for (size_t i = 0; i < LASER_OBJECTS_COUNT; i++) {
        if (check_collisions(&rocky, &LASER_OBJECTS[i])) {
            isGameOver = 1;
            return;
        }
    }
    
}

void display_title_screen(void) {
    cls();
    gotoxy(3, 3);
    printf("Rocky");
    performant_delay(50);
    printf(" On");
    performant_delay(50);
    printf(" Mars");
    performant_delay(50);

    unsigned char ps_on = 1;
    while (joypad() != J_START) {
        if (sys_time % 60 == 0) {
            gotoxy(4, 16);
            if (ps_on) {
                printf("PRESS START");
            }
            else {
                printf("           ");
            }
            ps_on = !ps_on;
        }
    }
    cls();
}

void display_game_over_screen(void) {
    HIDE_SPRITES;
    gotoxy(5,8);
    printf("GAME OVER");

    unsigned char ps_on = 1;
    while (joypad() != J_START) {
        if (sys_time % 60 == 0) {
            gotoxy(4, 16);
            if (ps_on) {
                printf("PRESS START");
            }
            else {
                printf("           ");
            }
            ps_on = !ps_on;
        }
    }

    reset_game();
    SHOW_SPRITES;
}

void make_game_harder(void) { 
    switch (score) {
        case 0:
            fireWaitTime = 180;
            laserSpeed = 1;
            break;
        case 50:
            fireWaitTime = 150;
            laserSpeed = 2;
            break;
        case 100:
            fireWaitTime = 100;
            laserSpeed = 2;
            break;
        case 150:
            fireWaitTime = 80;
            laserSpeed = 3;
            break;
        case 200:
            fireWaitTime = 60;
            laserSpeed = 4;
            break;
        case 300:
            fireWaitTime = 40;
            laserSpeed = 5;
            break;
    }
    
}

void main(void) {
    setup_sound(1, 0x77, 0xFF);
    // display_title_screen();
    init_game_objects();
    SHOW_SPRITES;

    while (1) {
        wait_vbl_done();
        if (isGameOver) {
            display_game_over_screen();
        }

        display_score();
        handle_rocky_movement();
        handle_ufo_movement();
        handle_lasers();
        check_game_over();
        make_game_harder();
    }
}
