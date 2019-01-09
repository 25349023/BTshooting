//
// Created by user on 2018/12/28.
//

#ifndef FINAL_CLION_LEVELOPTION_H
#define FINAL_CLION_LEVELOPTION_H

#include "allegro5/allegro.h"
#include "general.h"

// TODO: edit this struct if necessary
typedef struct enemySetting {
    ALLEGRO_BITMAP *image, *bullet;
    int hp, damage, cd;
    Vector2 pos;
    float speed_base, angle, bullet_speed;
    enum flyMode modes[MODE_COUNT];
    int count_of_mode;
} EnemySetting;

typedef struct levelSetting {
    int level;
    int player_hp, boss_hp;
    int enemy_count;
    int boss_damage;
    ALLEGRO_BITMAP *boss_img;
    ALLEGRO_BITMAP *dropping_bullet;
    double dropping_rate;
    float dropping_bullet_speed;

    int kind_of_enemy;
    int generate_enemy_cd[5];
    EnemySetting *enemy_prefab[5];
} LevelSetting;


void
set_enemy(EnemySetting *ene_set, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *blt_img, int hp, int dmg, int cd, Vector2 pos,
          float spd, float agl, float bt_spd, enum flyMode mds[], int cnt);
void set_level(LevelSetting *setting, int lv, int p_hp, int b_hp, int ene_cnt, int boss_dmg, ALLEGRO_BITMAP *boss,
               ALLEGRO_BITMAP *drop, double drp_rate, float bt_spd, int knd_of_ene, int gen_cd[]);

#endif //FINAL_CLION_LEVELOPTION_H
