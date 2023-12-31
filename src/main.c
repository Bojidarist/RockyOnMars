#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include "GBSound/GBSound.c"
#include "GameObject.c"
#include "GBUtils/Utils.c"
#include <rand.h>
#include "Sprites/MapTiles.c"
#include "Sprites/Map.c"

const uint8_t LASER_OBJECTS_COUNT = 5;
GameObject LASER_OBJECTS[5];
uint8_t currentLaserIndex = 0;
uint8_t fireWaitTime = 180;
uint8_t laserSpeed = 1;
uint32_t score = 0;
GameObject ufo;
GameObject rocky;
GameObject hoverboard;
GameObject moon;
uint8_t isGameOver = 0;
uint8_t backgroundScrollOffsetx1 = 0;
uint8_t backgroundScrollOffsetx2 = 0;

void display_score(void) {
    if(sys_time % 30 == 0) {
        score += 1;
    }
    gotoxy(1, 1); printf("Score:%d", score);
}

void init_game_objects(void) {
    create_ufo(&ufo);
    ufo.x = 140;
    ufo.y = 132;
    render_object(&ufo);

    create_rocky(&rocky);
    rocky.x = 20;
    rocky.y = 140;
    render_object(&rocky);

    create_hoverboard(&hoverboard);
    hoverboard.x = rocky.x;
    hoverboard.y = rocky.y + 8;
    render_object(&hoverboard);

    create_moon(&moon);
    moon.x = 145;
    moon.y = 25;
    render_object(&moon);

    for (size_t i = 0; i < LASER_OBJECTS_COUNT; i++) {
        GameObject laser;
        create_laser(&laser);
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
    set_bkg_tiles(0, 0, 40, 18, Map);
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
    backgroundScrollOffsetx1 = 0;
    backgroundScrollOffsetx2 = 0;
    cls();
    display_score();
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
            fireWaitTime = 80;
            laserSpeed = 1;
            break;
        case 25:
            fireWaitTime = 65;
            laserSpeed = 1;
            break;
        case 50:
            fireWaitTime = 50;
            laserSpeed = 2;
            break;
        case 75:
            fireWaitTime = 45;
            laserSpeed = 2;
            break;
        case 100:
            fireWaitTime = 40;
            laserSpeed = 2;
            break;
        case 125:
            fireWaitTime = 35;
            laserSpeed = 2;
            break;
        case 150:
            fireWaitTime = 30;
            laserSpeed = 3;
            break;
        case 200:
            fireWaitTime = 25;
            laserSpeed = 3;
            break;
        case 300:
            fireWaitTime = 25;
            laserSpeed = 4;
            break;
    }
}

void interrupt_LCD(void) {
    switch (LYC_REG)
    {
        case 0x00:
            move_bkg(0,0);
            LYC_REG = 0x10;
            break;
        case 0x10:
            move_bkg(backgroundScrollOffsetx1, 0);
            LYC_REG = 0x64;
            break;
        case 0x64:
            move_bkg(backgroundScrollOffsetx2, 0);
            LYC_REG = 0x00;
            break;
    }
}

void main(void) {
    setup_sound(1, 0x77, 0xFF);
    display_title_screen();

    uint16_t seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);

    set_bkg_data(0, 6, MapTiles);
    set_bkg_tiles(0, 0, 40, 18, Map);

    STAT_REG = 0x45;
    LY_REG = 0x00;

    disable_interrupts();
    add_LCD(interrupt_LCD);
    enable_interrupts();
    set_interrupts(VBL_IFLAG | LCD_IFLAG);

    init_game_objects();
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
    
    while (1) {
        wait_vbl_done();
        if (isGameOver) {
            display_game_over_screen();
        }

        backgroundScrollOffsetx1 += 1;
        backgroundScrollOffsetx2 += 2;

        display_score();
        handle_rocky_movement();
        handle_ufo_movement();
        handle_lasers();
        check_game_over();
        make_game_harder();
    }
}
