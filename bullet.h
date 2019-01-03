//
// Created by user on 2018/12/24.
//

#ifndef FINAL_CLION_BULLET_H
#define FINAL_CLION_BULLET_H

#include <allegro5/allegro.h>
#include "general.h"

typedef struct bullet {
    Vector2 pos;
    Vector2 size;
    Circle hit_area;
    int damage;
    ALLEGRO_BITMAP *bitmap;
    int flip;
    enum flyMode mode;
    struct bullet *next;
    int time;  // for special use
    float speed_multiplier;  // for special use
    bool pause;  // for special use
} Bullet;

Bullet *make_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt, int dmg);
Bullet *make_firework_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt, int tm, float spd, bool ps);
void register_bullet(Bullet *bt, Bullet **list);
void destroy_bullet(Bullet *curr, Bullet **prev, Bullet **list);
void free_bullet_list(Bullet **list);

#endif //FINAL_CLION_BULLET_H
