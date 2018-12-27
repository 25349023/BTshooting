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
    int flip;
    ALLEGRO_BITMAP *bitmap;
    enum flyMode mode;
    struct bullet *next;
} Bullet;

Bullet *make_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt);
void register_bullet(Bullet *bt, Bullet **list);
void destroy_bullet(Bullet *curr, Bullet **prev, Bullet **list);

#endif //FINAL_CLION_BULLET_H
