//
// Created by user on 2018/12/28.
//

#ifndef FINAL_CLION_LEVELOPTION_H
#define FINAL_CLION_LEVELOPTION_H

#include "allegro5/allegro.h"
#include "general.h"

// TODO: complete this struct, add a set_enemy func,
//       and go to rewrite create_enemy func
typedef struct enemySetting {
    ALLEGRO_BITMAP *image, *bullet;
    int hp, damage;
    enum flyMode *modes;
} EnemySetting;

typedef struct levelSetting {
    int level;
    int player_hp, enemy_hp, boss_hp;
    int enemy_count;
    int enemy_damage, boss_damage;
    ALLEGRO_BITMAP *enemy_img;
    ALLEGRO_BITMAP *boss_img;
    ALLEGRO_BITMAP *dropping_bullet;
    double enemy_rate_base, bullet_rate;
} LevelSetting;

void set_level(LevelSetting *setting, int lv, int p_hp, int e_hp, int b_hp, int ene_cnt, int ene_dmg, int boss_dmg,
               ALLEGRO_BITMAP *enemy, ALLEGRO_BITMAP *boss, ALLEGRO_BITMAP *drop, double blt_rate, double ene_rate);

#endif //FINAL_CLION_LEVELOPTION_H
