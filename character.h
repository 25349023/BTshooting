//
// Created by user on 2018/12/24.
//

#ifndef FINAL_CLION_CHARACTER_H
#define FINAL_CLION_CHARACTER_H

#include <allegro5/allegro.h>
#include "general.h"
#include "levelSetting.h"

typedef struct character {
    int health, damage;
    Vector2 pos;
    Vector2 speed;
    Vector2 size;
    Vector2 firing_point;
    Circle body;
    double shooting_rate;  // for player, boss
    float e_speed;
    float dire_angle;  // in degree
    int CD, shoot_interval;
    ALLEGRO_BITMAP *image;
    ALLEGRO_BITMAP *default_bullet;
    enum flyMode bullet_mode[MODE_COUNT];
    struct character *next;
} Character;

void set_player(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, enum flyMode *md,
                int cnt_of_mode);
void set_boss(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, enum flyMode *md,
                int cnt_of_mode);

Character *create_enemy(const EnemySetting *prefab);
void register_enemy(Character *enemy);
void destroy_enemy(Character *curr, Character **prev);

#endif //FINAL_CLION_CHARACTER_H
