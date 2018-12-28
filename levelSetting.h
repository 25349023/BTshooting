//
// Created by user on 2018/12/28.
//

#ifndef FINAL_CLION_LEVELOPTION_H
#define FINAL_CLION_LEVELOPTION_H

#include "allegro5/allegro.h"

typedef struct levelSetting {
    int level;
    int enemy_count;
    ALLEGRO_BITMAP *enemy_img;
    ALLEGRO_BITMAP *boss_img;
    ALLEGRO_BITMAP *dropping_bullet;
    float bullte_rate;
} LevelSetting;

void set_level(LevelSetting *setting, int lv, int ene_cnt, ALLEGRO_BITMAP *enemy, ALLEGRO_BITMAP *boss, ALLEGRO_BITMAP *drop,
               float rate);

#endif //FINAL_CLION_LEVELOPTION_H
