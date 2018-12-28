//
// Created by user on 2018/12/24.
//
#include "bullet.h"

extern int WIDTH;
extern int HEIGHT;

Bullet *make_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt) {
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
    bt->time = 9;

    return bt;
}

Bullet *make_firework_bullet(ALLEGRO_BITMAP *img, enum flyMode mode, Vector2 pt, int tm) {
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
