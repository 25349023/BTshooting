//
// Created by user on 2018/12/24.
//

#include <math.h>
#include "character.h"

extern int WIDTH, HEIGHT;
extern Character player, boss;
extern Character *enemy_list;
extern bool hintOut[2];
extern ALLEGRO_TIMER *defendTimer, *thickCircleTimer, *thousandsArrowTimer, *cureTimer;
extern int defend_sparkle_times, thick_circle_times, thousand_arrow_times, cure_times;

void set_player(const PlayerSetting *prefab) {
    player.image = prefab->image;
    player.size.x = al_get_bitmap_width(prefab->image);
    player.size.y = al_get_bitmap_height(prefab->image);
    player.firing_point.x = player.size.x / 2;
    player.firing_point.y = player.size.y / 3;
    player.body.center = (Vector2) {player.size.x / 2, player.size.y / 2};
    player.body.radius = player.size.x < player.size.y ? (player.size.x / 4) : (player.size.y / 4);
    player.health = prefab->hp;
    player.damage = prefab->damage;
    player.default_bullet = prefab->bullet;
    player.shooting_rate = prefab->shooting_rate;
    player.bullet_speed = prefab->bullet_speed;
    for (int i = 0; i < prefab->count_of_mode; i++){
        player.bullet_mode[i] = prefab->modes[i];
    }
    for (int i = prefab->count_of_mode; i < MODE_COUNT; i++){
        player.bullet_mode[i] = stopped;
    }

    player.skill[Q] = prefab->skill[Q];
    player.skill[W] = prefab->skill[W];
    for (int i = Q; i <= W; i++){
        player.skill_CD[i] = prefab->skill_CD[i];
        player.can[i] = hintOut[i];
        player.show[i] = hintOut[i];
    }
    player.invincible = false;
    player.show_shield = false;
    if (defendTimer && al_get_timer_started(defendTimer)){
        al_stop_timer(defendTimer);
        defend_sparkle_times = 0;
    }
    if (thickCircleTimer && al_get_timer_started(thickCircleTimer)){
        al_stop_timer(thickCircleTimer);
        thick_circle_times = 0;
    }
    if (thousandsArrowTimer && al_get_timer_started(thousandsArrowTimer)){
        al_stop_timer(thousandsArrowTimer);
        thousand_arrow_times = 0;
    }
    if (cureTimer && al_get_timer_started(cureTimer)){
        al_stop_timer(cureTimer);
        cure_times = 0;
    }
}

Character *create_enemy(const EnemySetting *prefab) {
    Character *enemy = malloc(sizeof(Character));
    enemy->image = prefab->image;
    Vector2 pos = prefab->pos;
    enemy->pos.x = pos.x < 0 ? ((rand() % 18 + 1) * 20 + 10) : pos.x;
    enemy->pos.y = pos.y < 0 ? (rand() % (HEIGHT + 1)) : pos.y;
    enemy->size.x = al_get_bitmap_width(prefab->image);
    enemy->size.y = al_get_bitmap_height(prefab->image);
    enemy->health = prefab->hp;
    enemy->damage = prefab->damage;
    enemy->body.center = (Vector2) {enemy->size.x / 2, enemy->size.y / 2};
    enemy->body.radius = enemy->size.x < enemy->size.y ? (enemy->size.x / 3) : (enemy->size.y / 3);
    enemy->firing_point = enemy->body.center;
    enemy->e_speed = prefab->speed_base;
    enemy->speed.x = enemy->e_speed * sinf((float) (ALLEGRO_PI / 180 * prefab->angle));
    enemy->speed.y = enemy->e_speed * -cosf((float) (ALLEGRO_PI / 180 * prefab->angle));
    enemy->dire_angle = prefab->angle;
    enemy->default_bullet = prefab->bullet;
    enemy->shoot_CD = prefab->cd;
    enemy->shoot_interval = 0;
    enemy->bullet_speed = prefab->bullet_speed;
    for (int i = 0; i < prefab->count_of_mode; i++){
        enemy->bullet_mode[i] = prefab->modes[i];
    }
    for (int i = prefab->count_of_mode; i < MODE_COUNT; i++){
        enemy->bullet_mode[i] = stopped;
    }

    return enemy;
}

void register_enemy(Character *enemy) {
    enemy->next = enemy_list;
    enemy_list = enemy;
}

void destroy_enemy(Character *curr, Character **prev) {
    if (curr == enemy_list){
        enemy_list = curr->next;
        free(curr);
        *prev = NULL;
    }
    else {
        (*prev)->next = curr->next;
        free(curr);
    }
}

void set_boss(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, float bt_spd, enum flyMode *md,
              int cnt_of_mode) {
    boss.image = img;
    boss.size.x = al_get_bitmap_width(img);
    boss.size.y = al_get_bitmap_height(img);
    boss.pos = (Vector2) {rand() % 301 + 50, -60};
    boss.body.radius = boss.size.x < boss.size.y ? (boss.size.x / 3) : (boss.size.y / 3);
    boss.body.center = (Vector2) {boss.size.x / 2, boss.size.y / 2};
    boss.firing_point = boss.body.center;
    boss.health = hp;
    boss.damage = dmg;
    boss.default_bullet = bt_img;
    boss.shooting_rate = rate;
    boss.bullet_speed = bt_spd;
    for (int i = 0; i < cnt_of_mode; i++){
        boss.bullet_mode[i] = md[i];
    }
    for (int i = cnt_of_mode; i < MODE_COUNT; i++){
        boss.bullet_mode[i] = stopped;
    }
    boss.e_speed = 1;
    boss.speed = (Vector2) {0, boss.e_speed};
}

void change_player(const PlayerSetting *prefab) {
    player.image = prefab->image;
    player.health = (player.health < prefab->hp ? player.health : prefab->hp);
    player.size.x = al_get_bitmap_width(prefab->image);
    player.size.y = al_get_bitmap_height(prefab->image);
    player.firing_point.x = player.size.x / 2;
    player.firing_point.y = player.size.y / 3;
    player.body.center = (Vector2) {player.size.x / 2, player.size.y / 2};
    player.body.radius = player.size.x < player.size.y ? (player.size.x / 4) : (player.size.y / 4);
    player.damage = (player.health < 0 ? 0 : prefab->damage);
    player.default_bullet = prefab->bullet;
    player.shooting_rate = prefab->shooting_rate;
    player.bullet_speed = prefab->bullet_speed;
    for (int i = 0; i < prefab->count_of_mode; i++){
        player.bullet_mode[i] = prefab->modes[i];
    }
    for (int i = prefab->count_of_mode; i < MODE_COUNT; i++){
        player.bullet_mode[i] = stopped;
    }

    player.skill[Q] = prefab->skill[Q];
    player.skill[W] = prefab->skill[W];
    for (int i = Q; i <= W; i++){
        player.skill_CD[i] = prefab->skill_CD[i];
    }
    if (defendTimer && al_get_timer_started(defendTimer)){
        al_stop_timer(defendTimer);
        defend_sparkle_times = 0;
    }
    if (thickCircleTimer && al_get_timer_started(thickCircleTimer)){
        al_stop_timer(thickCircleTimer);
        thick_circle_times = 0;
    }
    if (thousandsArrowTimer && al_get_timer_started(thousandsArrowTimer)){
        al_stop_timer(thousandsArrowTimer);
        thousand_arrow_times = 0;
    }
    if (cureTimer && al_get_timer_started(cureTimer)){
        al_stop_timer(cureTimer);
        cure_times = 0;
    }
}
