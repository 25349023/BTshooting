//
// Created by user on 2018/12/24.
//

#ifndef FINAL_CLION_CHARACTER_H
#define FINAL_CLION_CHARACTER_H

#include <allegro5/allegro.h>
#include "general.h"

// TODO: make the bullet_mode an array or a pointer
typedef struct character {
    int health, default_damage;
    Vector2 pos;
    Vector2 speed;
    Vector2 size;
    Vector2 firing_point;
    Circle body;
    double shooting_rate;
    float e_speed;
    float dire_angle;  // in degree
    int CD, shoot_interval;
    ALLEGRO_BITMAP *image;
    ALLEGRO_BITMAP *default_bullet;
    enum flyMode bullet_mode;
    struct character *next;
} Character;

void set_character(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, enum flyMode md);

Character *create_enemy(ALLEGRO_BITMAP *img, int hp, int dmg, Vector2 pos, float spd, float angle,
                        ALLEGRO_BITMAP *bt_img, int cd, enum flyMode md);
void register_enemy(Character *enemy);
void destroy_enemy(Character *curr, Character **prev);

#endif //FINAL_CLION_CHARACTER_H
