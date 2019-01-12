//
// Created by user on 2018/12/24.
//

#ifndef FINAL_CLION_CHARACTER_H
#define FINAL_CLION_CHARACTER_H

#include <allegro5/allegro.h>
#include "general.h"
#include "bullet.h"

typedef void (*Skill)(Vector2, enum flyMode, Bullet **, int, float);

enum skillButton { Q, W, E, R };

typedef struct character {
    int health, damage;
    Vector2 pos;
    Vector2 speed;
    Vector2 size;
    Vector2 firing_point;
    Circle body;
    double shooting_rate;  // for player, boss
    float bullet_speed;
    float e_speed;
    float dire_angle;  // in degree
    int shoot_CD, shoot_interval;  // for enemy
    bool skills[6];  // for boss
    ALLEGRO_BITMAP *image;
    ALLEGRO_BITMAP *default_bullet;
    enum flyMode bullet_mode[MODE_COUNT];
    Skill skill[4];
    bool can[4], show[4];
    int skill_CD[4];
    bool invincible, show_shield;

    struct character *next;
} Character;

typedef struct enemySetting {
    ALLEGRO_BITMAP *image, *bullet;
    int hp, damage, cd;
    Vector2 pos;
    float speed_base, angle, bullet_speed;
    enum flyMode modes[MODE_COUNT];
    int count_of_mode;
} EnemySetting;

typedef struct playerSetting {
    ALLEGRO_BITMAP *image, *bullet;
    int hp, damage;
    float bullet_speed;
    double shooting_rate;
    enum flyMode modes[MODE_COUNT];
    int count_of_mode;
    Skill skill[4];
    int skill_CD[4];
} PlayerSetting;

void set_player(const PlayerSetting *prefab);
void change_player(const PlayerSetting *prefab);
void set_boss(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, float bt_spd,
              enum flyMode *md, int cnt_of_mode);

Character *create_enemy(const EnemySetting *prefab);
void register_enemy(Character *enemy);
void destroy_enemy(Character *curr, Character **prev);

#endif //FINAL_CLION_CHARACTER_H
