//
// Created by user on 2019/1/11.
//

#include <allegro5/allegro.h>
#include "skill.h"

extern ALLEGRO_BITMAP *bulletImgs[20];

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

