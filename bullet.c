//
// Created by user on 2018/12/24.
//
#include "bullet.h"

extern int WIDTH;
extern int HEIGHT;

Bullet *make_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt, int dmg, float multi) {
    Bullet *bt = malloc(sizeof(Bullet));
    bt->pos.x = pt.x < -200 ? (rand() % (WIDTH + 1)) : pt.x;
    bt->pos.y = pt.y < -200 ? (rand() % (HEIGHT + 1)) : pt.y;
    bt->size.x = al_get_bitmap_width(img);
    bt->size.y = al_get_bitmap_height(img);
    bt->hit_area.center = (Vector2) {0, 0};
    bt->hit_area.radius = bt->size.x / 3;
    bt->damage = dmg;
    bt->flip = rand() % 2;
    bt->bitmap = img;
    bt->mode = mode;
    bt->next = NULL;
    bt->time = 9;
    bt->speed_multiplier = multi;

    return bt;
}

Bullet *make_firework_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt, int tm, float spd, bool ps) {
    Bullet *bt = malloc(sizeof(Bullet));
    bt->pos.x = pt.x < 0 ? (rand() % (WIDTH + 1)) : pt.x;
    bt->pos.y = pt.y < 0 ? (rand() % (HEIGHT + 1)) : pt.y;
    bt->size.x = al_get_bitmap_width(img);
    bt->size.y = al_get_bitmap_height(img);
    bt->hit_area.center = (Vector2) {0, 0};
    bt->hit_area.radius = bt->size.x / 3;
    bt->flip = rand() % 2;
    bt->bitmap = img;
    bt->mode = mode;
    bt->next = NULL;
    bt->time = tm;
    bt->speed_multiplier = spd;
    bt->pause = ps;

    return bt;
}

void register_bullet(Bullet *bt, Bullet **list) {
    bt->next = *list;
    *list = bt;
}

void destroy_bullet(Bullet *curr, Bullet **prev, Bullet **list) {
    if (curr == *list){
        *list = curr->next;
        free(curr);
        *prev = NULL;
    }
    else {
        (*prev)->next = curr->next;
        free(curr);
    }
}

void free_bullet_list(Bullet **list) {
    for (Bullet *bt = *list; bt != NULL;){
        Bullet *temp = bt;
        bt = bt->next;
        free(temp);
    }

    *list = NULL;
}
