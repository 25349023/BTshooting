//
// Created by user on 2018/12/24.
//

#include <math.h>
#include "character.h"

extern int WIDTH, HEIGHT;
extern Character player, boss;
extern Character *enemy_list;

void set_player(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, float bt_spd, enum flyMode *md,
                int cnt_of_mode, Skill sk_q, int sk_cd[4], int sk_lv) {
    player.image = img;
    player.size.x = al_get_bitmap_width(img);
    player.size.y = al_get_bitmap_height(img);
    player.firing_point.x = player.size.x / 2;
    player.firing_point.y = player.size.y / 3;
    player.body.center = (Vector2) {player.size.x / 2, player.size.y / 2};
    player.body.radius = player.size.x < player.size.y ? (player.size.x / 4) : (player.size.y / 4);
    player.health = hp;
    player.damage = dmg;
    player.default_bullet = bt_img;
    player.shooting_rate = rate;
    player.bullet_speed = bt_spd;
    for (int i = 0; i < cnt_of_mode; i++){
        player.bullet_mode[i] = md[i];
    }
    for (int i = cnt_of_mode; i < MODE_COUNT; i++){
        player.bullet_mode[i] = stopped;
    }

    player.skill_Q = sk_q;
    player.can_Q = false;
    player.can_W = false;
    player.can_E = false;
    player.can_R = false;
    switch (sk_lv){
        case 4:
            player.can_R = true;
        case 3:
            player.can_E = true;
        case 2:
            player.can_W = true;
        case 1:
            player.can_Q = true;
            break;
        default:
            break;
    }
    player.cd_Q = sk_cd[0];
    player.cd_W = sk_cd[1];
    player.cd_E = sk_cd[2];
    player.cd_R = sk_cd[3];

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
    enemy->CD = prefab->cd;
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
