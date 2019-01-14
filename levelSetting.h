//
// Created by user on 2018/12/28.
//

#ifndef FINAL_CLION_LEVELOPTION_H
#define FINAL_CLION_LEVELOPTION_H

#include "allegro5/allegro.h"
#include "general.h"
#include "character.h"

typedef struct levelSetting {
    int level;
    int boss_hp;
    int enemy_count;
    int boss_damage;
    ALLEGRO_BITMAP *boss_img;
    ALLEGRO_BITMAP *dropping_bullet, *boss_bullet;
    double dropping_rate, boss_rate;
    float dropping_bullet_speed, boss_bt_speed;

    enum flyMode boss_modes[MODE_COUNT];
    int count_of_modes;
    int kind_of_enemy;
    int generate_enemy_cd[5];
    EnemySetting *enemy_prefab[5];
} LevelSetting;


void set_airplane(PlayerSetting *pla_set, ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate,
                  float bt_spd, enum flyMode md[], int cnt_of_mode, Skill sk[], int *sk_cd);
void
set_enemy(EnemySetting *ene_set, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *blt_img, int hp, int dmg, int cd, Vector2 pos,
          float spd, float agl, float bt_spd, enum flyMode mds[], int cnt);
void set_level(LevelSetting *setting, int lv, int b_hp, int ene_cnt, int boss_dmg, ALLEGRO_BITMAP *boss,
               ALLEGRO_BITMAP *drop, double drp_rate, float bt_spd, enum flyMode boss_md[], int cnt_of_md,
               int knd_of_ene, int gen_cd[], ALLEGRO_BITMAP *boss_bt, double bs_rate, float boss_spd);

#endif //FINAL_CLION_LEVELOPTION_H
