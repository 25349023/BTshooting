//
// Created by user on 2018/12/28.
//

#include "levelSetting.h"

void set_level(LevelSetting *setting, int lv, int p_hp, int e_hp, int b_hp, int ene_cnt, int ene_dmg, int boss_dmg,
               ALLEGRO_BITMAP *enemy, ALLEGRO_BITMAP *boss, ALLEGRO_BITMAP *drop, double blt_rate, double ene_rate) {
    setting->level = lv;
    setting->player_hp = p_hp;
    setting->enemy_hp = e_hp;
    setting->boss_hp = b_hp;
    setting->enemy_count = ene_cnt;
    setting->enemy_damage = ene_dmg;
    setting->boss_damage = boss_dmg;
    setting->enemy_img = enemy;
    setting->boss_img = boss;
    setting->dropping_bullet = drop;
    setting->bullet_rate = blt_rate;
    setting->enemy_rate_base = ene_rate;
    // TODO: complete this function
}
