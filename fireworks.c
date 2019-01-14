//
// Created by user on 2018/12/29.
//

#include "fireworks.h"
#include "bullet.h"

extern ALLEGRO_BITMAP *bulletImgs[];
extern Bullet *fireworks_bullet_list;

void draw_fireworks1(int size, Vector2 pt) {
    for (int i = 1; i <= 8; i++){  // eight direction
        Bullet *bt = make_firework_bullet(bulletImgs[rand() % 3], i, pt, size, 1, false);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_fireworks2(int size, Vector2 pt) {
    for (int i = 1; i <= 16; i++){  // eight direction
        Bullet *bt = make_firework_bullet(bulletImgs[rand() % 3], i, pt, size, 1, false);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_H(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 160 : 250);
    Bullet *bt;
    bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                              (Vector2) {pt.x - 30, pt.y}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                              (Vector2) {pt.x + 30, pt.y}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    for (int i = 1; i <= 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x - 30, pt.y - i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x + 30, pt.y - i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x - 30, pt.y + i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x + 30, pt.y + i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_A(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 160 : 250);
    Bullet *bt;
    bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                              (Vector2) {pt.x - 28, pt.y + 10}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                              (Vector2) {pt.x + 28, pt.y + 10}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                              (Vector2) {pt.x - 30, pt.y - 10}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                              (Vector2) {pt.x + 30, pt.y - 20}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);

    bt = make_firework_bullet(bulletImgs[rand() % 3], right_front,
                              (Vector2) {pt.x - 15, pt.y - 20}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], right_front,
                              (Vector2) {pt.x, pt.y - 35}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], right_front,
                              (Vector2) {pt.x + 15, pt.y - 50}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);

    bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                              (Vector2) {pt.x + 30, pt.y - 40}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                              (Vector2) {pt.x + 30, pt.y - 50}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);

    for (int i = 0; i < 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x + 10 - i * 20, pt.y - 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }

    for (int i = 1; i <= 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x - 30, pt.y + i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x + 30, pt.y + i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_P2(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 160 : 250);
    Bullet *bt;
    // for P
    bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                              (Vector2) {pt.x - 40, pt.y - 5}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                              (Vector2) {pt.x - 40, pt.y - 50}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    for (int i = 0; i <= 1; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                                  (Vector2) {pt.x + 20 * i, pt.y - 5}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                                  (Vector2) {pt.x + 20 * i, pt.y - 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
    for (int i = 0; i < 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x - 40, pt.y - 25 * i}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x - 40, pt.y + 20 * i}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
    bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                              (Vector2) {pt.x - 40, pt.y + 60}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                              (Vector2) {pt.x + 20, pt.y - 50}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                              (Vector2) {pt.x + 20, pt.y}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);

    // for 2
    bt = make_firework_bullet(bulletImgs[3], right,
                              (Vector2) {pt.x + 55, pt.y - 75}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[3], right_front,
                              (Vector2) {pt.x + 55, pt.y - 75}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[3], left_back,
                              (Vector2) {pt.x + 30, pt.y - 50}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[3], left,
                              (Vector2) {pt.x + 30, pt.y - 50}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
}

void draw_Y(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 160 : 250);
    Bullet *bt;
    bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                              (Vector2) {pt.x, pt.y + 10}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    for (int i = 1; i < 4; ++i){

        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x, pt.y + i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
    for (int i = 1; i <= 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], right_front,
                                  (Vector2) {pt.x + 15 * i, pt.y - 15 * i - 10}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], left_front,
                                  (Vector2) {pt.x - 15 * i, pt.y - 15 * i - 10}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_2(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 100 : 120);
    Bullet *bt;
    for (int i = 0; i < 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x - i * 20 + 10, pt.y + 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                                  (Vector2) {pt.x + i * 20 - 10, pt.y}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x - i * 20 + 10, pt.y - 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x + 30, pt.y - 20 * (i + 1)}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x - 30, pt.y + 20 * (i + 1)}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_0(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 100 : 120);
    Bullet *bt;
    for (int i = 0; i < 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x + 30, pt.y - 20 * (i + 1)}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x + 30, pt.y + 20 * (i + 1)}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x - 30, pt.y - 20 * (i + 1)}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x - 30, pt.y + 20 * (i + 1)}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                                  (Vector2) {pt.x + i * 20 - 10, pt.y - 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x - i * 20 + 10, pt.y + 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_1(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 100 : 120);
    Bullet *bt;
    bt = make_firework_bullet(bulletImgs[rand() % 3], right_front,
                              (Vector2) {pt.x, pt.y - 60}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], left_back,
                              (Vector2) {pt.x - 25, pt.y - 35}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    bt = make_firework_bullet(bulletImgs[rand() % 3], left_back,
                              (Vector2) {pt.x - 40, pt.y - 20}, tm, speed, true);
    register_bullet(bt, &fireworks_bullet_list);
    for (int i = 1; i <= 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x, pt.y - i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x, pt.y + i * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
    for (int i = 1; i <= 2; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x - i * 20 - 5, pt.y + 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                                  (Vector2) {pt.x + i * 20 - 5, pt.y + 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

void draw_9(Vector2 pt, bool standing, int speed) {
    int tm = (standing ? 100 : 120);
    Bullet *bt;
    for (int i = 0; i < 3; i++){
        bt = make_firework_bullet(bulletImgs[rand() % 3], right,
                                  (Vector2) {pt.x + i * 20 - 10, pt.y - 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x - i * 20 + 10, pt.y}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], left,
                                  (Vector2) {pt.x - i * 20 + 5, pt.y + 50}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x + 30, pt.y - (i + 1) * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], up,
                                  (Vector2) {pt.x - 30, pt.y - (i + 1) * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
        bt = make_firework_bullet(bulletImgs[rand() % 3], down,
                                  (Vector2) {pt.x + 30, pt.y + (i + 1) * 20}, tm, speed, true);
        register_bullet(bt, &fireworks_bullet_list);
    }
}

