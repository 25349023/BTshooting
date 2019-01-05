//
// Created by user on 2018/12/28.
//

#include "levelSetting.h"

void set_level(LevelSetting *setting, int lv, int p_hp, int b_hp, int ene_cnt, int boss_dmg, ALLEGRO_BITMAP *boss,
               ALLEGRO_BITMAP *drop, double drp_rate, int knd_of_ene, int gen_cd[]) {
    setting->level = lv;
    setting->player_hp = p_hp;
    setting->boss_hp = b_hp;
    setting->enemy_count = ene_cnt;
    setting->boss_damage = boss_dmg;
    setting->boss_img = boss;
    setting->dropping_bullet = drop;
    setting->dropping_rate = drp_rate;
    setting->kind_of_enemy = knd_of_ene;
    for (int i = 0; i < knd_of_ene; i++){
        setting->generate_enemy_cd[i] = gen_cd[i];
    }
    // TODO: complete this function
}

void
set_enemy(EnemySetting *ene_set, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *blt_img, int hp, int dmg, int cd, Vector2 pos,
          float spd, float agl, enum flyMode mds[], int cnt) {
    ene_set->image = img;
    ene_set->bullet = blt_img;
    ene_set->hp = hp;
    ene_set->damage = dmg;
    ene_set->cd = cd;
    ene_set->pos = pos;
    ene_set->speed_base = spd;
    ene_set->angle = agl;
    for (int i = 0; i < cnt; i++){
        ene_set->modes[i] = mds[i];
    }
    for (int i = cnt; i < MODE_COUNT; i++){
        ene_set->modes[i] = stopped;
    }
    ene_set->count_of_mode = cnt;
}
