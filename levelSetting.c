//
// Created by user on 2018/12/28.
//

#include "levelSetting.h"

void set_level(LevelSetting *setting, int lv, int b_hp, int ene_cnt, int boss_dmg, ALLEGRO_BITMAP *boss,
               ALLEGRO_BITMAP *drop, double drp_rate, float bt_spd, enum flyMode boss_md[], int cnt_of_md,
               int knd_of_ene, int gen_cd[], ALLEGRO_BITMAP *boss_bt, double bs_rate, float boss_spd) {
    setting->level = lv;
    setting->boss_hp = b_hp;
    setting->enemy_count = ene_cnt;
    setting->boss_damage = boss_dmg;
    setting->boss_img = boss;
    setting->dropping_bullet = drop;
    setting->dropping_rate = drp_rate;
    setting->dropping_bullet_speed = bt_spd;
    for (int i = 0; i < cnt_of_md; i++){
        setting->boss_modes[i] = boss_md[i];
    }
    for (int i = cnt_of_md; i < MODE_COUNT; i++){
        setting->boss_modes[i] = stopped;
    }
    setting->count_of_modes = cnt_of_md;
    setting->kind_of_enemy = knd_of_ene;
    for (int i = 0; i < knd_of_ene; i++){
        setting->generate_enemy_cd[i] = gen_cd[i];
    }
    setting->boss_bullet = boss_bt;
    setting->boss_rate = bs_rate;
    setting->boss_bt_speed = boss_spd;
}

void
set_enemy(EnemySetting *ene_set, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *blt_img, int hp, int dmg, int cd, Vector2 pos,
          float spd, float agl, float bt_spd, enum flyMode mds[], int cnt) {
    ene_set->image = img;
    ene_set->bullet = blt_img;
    ene_set->hp = hp;
    ene_set->damage = dmg;
    ene_set->cd = cd;
    ene_set->pos = pos;
    ene_set->speed_base = spd;
    ene_set->angle = agl;
    ene_set->bullet_speed = bt_spd;
    for (int i = 0; i < cnt; i++){
        ene_set->modes[i] = mds[i];
    }
    for (int i = cnt; i < MODE_COUNT; i++){
        ene_set->modes[i] = stopped;
    }
    ene_set->count_of_mode = cnt;
}

void
set_airplane(PlayerSetting *pla_set, ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate,
             float bt_spd, enum flyMode md[], int cnt_of_mode, Skill sk[], int *sk_cd) {
    pla_set->image = img;
    pla_set->hp = hp;
    pla_set->damage = dmg;
    pla_set->bullet = bt_img;
    pla_set->bullet_speed = bt_spd;
    pla_set->shooting_rate = rate;
    for (int i = 0; i < cnt_of_mode; i++){
        pla_set->modes[i] = md[i];
    }
    for (int i = cnt_of_mode; i < MODE_COUNT; i++){
        pla_set->modes[i] = stopped;
    }
    pla_set->count_of_mode = cnt_of_mode;
    for (int i = Q; i <= W; i++){
        pla_set->skill_CD[i] = sk_cd[i];
        pla_set->skill[i] = sk[i];
    }
}
