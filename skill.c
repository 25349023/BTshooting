//
// Created by user on 2019/1/11.
//

#include <allegro5/allegro.h>
#include <math.h>
#include "skill.h"

extern ALLEGRO_BITMAP *bulletImgs[20];
extern Character player;
extern Bullet *enemy_bullet_list;
extern Character *enemy_list;
extern ALLEGRO_TIMER *defendTimer;

void launch_big_fire(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    Bullet *bt;
    int dir = (mode == up ? 1 : -1);
    for (int i = -1; i <= 1; i++){
        bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x + i * 10, pos.y - 20 * dir}, dmg, mul);
        register_bullet(bt, list);
    }
    for (int i = -1; i <= 2; i++){
        for (int j = -2; j <= 2; j++){
            bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x + j * 10, pos.y + i * 10 * dir}, dmg, mul);
            register_bullet(bt, list);
        }
    }
    for (int i = 0; i <= 2; i++){
        bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x + i * 10, pos.y + 30 * dir}, dmg, mul);
        register_bullet(bt, list);
    }
    bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x - 20, pos.y + 30 * dir}, dmg, mul);
    register_bullet(bt, list);
    bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x + 20, pos.y + 40 * dir}, dmg, mul);
    register_bullet(bt, list);
    bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x, pos.y + 40 * dir}, dmg, mul);
    register_bullet(bt, list);
    bt = make_bullet(bulletImgs[rand() % 3], mode, (Vector2) {pos.x, pos.y + 45 * dir}, dmg, mul);
    register_bullet(bt, list);
}

void launch_big_arrow(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    Bullet *bt;
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            bt = make_bullet(bulletImgs[rand() % 3], mode,
                             (Vector2) {pos.x + 12 * i, pos.y + 15 * j + 5 * i}, dmg, mul);
            register_bullet(bt, list);
        }
    }
    for (int i = -1; i >= -5; i--){
        for (int j = 0; j < 3; j++){
            bt = make_bullet(bulletImgs[rand() % 3], mode,
                             (Vector2) {pos.x + 12 * i, pos.y + 15 * j - 5 * i}, dmg, mul);
            register_bullet(bt, list);
        }
    }
}

void defend(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    player.invincible = true;
    player.show_shield = true;
    al_start_timer(defendTimer);
}

void shoot_triangle(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    Bullet *bt;
    for (int i = -1; i <= 1; i++){
        bt = make_bullet(bulletImgs[(rand() % 2 ? 8 : 3)], down,
                (Vector2) {pos.x, pos.y + 20 * i}, dmg, mul);
        register_bullet(bt, list);
    }
    for (int i = -1; i <= 1; i++){
        bt = make_bullet(bulletImgs[(rand() % 2 ? 8 : 3)], right_front,
                (Vector2) {pos.x + 14 * i, pos.y - 14 * i}, dmg, mul);
        register_bullet(bt, list);
    }
    for (int i = -1; i <= 1; i++){
        bt = make_bullet(bulletImgs[(rand() % 2 ? 8 : 3)], left_front,
                (Vector2) {pos.x + 14 * i, pos.y + 14 * i}, dmg, mul);
        register_bullet(bt, list);
    }
}

void big_blow(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    for (Bullet *curr = *list; curr != NULL; curr = curr->next){
        if (curr->mode != stopped){
            curr->mode = up;
        }
    }
    for (Bullet *curr = enemy_bullet_list; curr != NULL; curr = curr->next){
        if (curr->mode != stopped){
            curr->mode = up;
        }
    }
    for (Character *curr = enemy_list; curr != NULL; curr = curr->next){
        if (curr->health > 0){
            curr->dire_angle = 0;
            curr->speed.y = -fabsf(curr->speed.y);
            curr->bullet_mode[0] = stopped;
        }
    }

}

void horizontal_blow(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    for (Bullet *curr = enemy_bullet_list; curr != NULL; curr = curr->next){
        if (curr->pos.x > pos.x){
            curr->mode = right;
        }
        else {
            curr->mode = left;
        }
    }
    for (Character *curr = enemy_list; curr != NULL; curr = curr->next){
        if (curr->health > 0){
            curr->dire_angle = 0;
            curr->speed.y = -fabsf(curr->speed.y);
            curr->bullet_mode[0] = stopped;
        }
    }

}

void thin_circle(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    Bullet *bt;
    for (int m = up; m <= lb_b; m++){
        bt = make_bullet(player.default_bullet, m, pos, dmg, mul);
        register_bullet(bt, list);
    }
}

void thick_circle(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul) {
    Bullet *bt;
    for (int i = 0; i < 5; i++){
        for (int m = up; m <= lb_b; m++){
            bt = make_bullet(player.default_bullet, m, pos, dmg, mul);
            register_bullet(bt, list);
        }
    }
}

