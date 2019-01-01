//
// Created by user on 2018/12/24.
//

#include <math.h>
#include "character.h"

extern int WIDTH, HEIGHT;
extern Character player;
extern Character *enemy_list;

void set_character(ALLEGRO_BITMAP *img, int hp, int dmg, ALLEGRO_BITMAP *bt_img, double rate, enum flyMode md) {
    player.image = img;
    player.size.x = al_get_bitmap_width(img);
    player.size.y = al_get_bitmap_height(img);
    player.firing_point.x = player.size.x / 2;
    player.firing_point.y = player.size.y / 3;
    player.body.center = (Vector2) {player.size.x / 2, player.size.y / 2};
    player.body.radius = player.size.x < player.size.y ? (player.size.x / 3) : (player.size.y / 3);
    player.health = hp;
    player.default_damage = dmg;
    player.default_bullet = bt_img;
    player.shooting_rate = rate;
    player.bullet_mode = md;
}

Character *create_enemy(ALLEGRO_BITMAP *img, int hp, int dmg, Vector2 pos, float spd, float angle,
                        ALLEGRO_BITMAP *bt_img, int cd, enum flyMode md) {
    Character *enemy = malloc(sizeof(Character));
    enemy->image = img;
    enemy->pos.x = pos.x < 0 ? ((rand() % 18 + 1) * 20 + 10) : pos.x;
    enemy->pos.y = pos.y < 0 ? (rand() % (HEIGHT + 1)) : pos.y;
    enemy->size.x = al_get_bitmap_width(img);
    enemy->size.y = al_get_bitmap_height(img);
    enemy->firing_point.x = enemy->size.x / 2;
    enemy->firing_point.y = enemy->size.y / 2;
    enemy->health = hp;
    enemy->default_damage = dmg;
    enemy->body.center = (Vector2) {enemy->size.x / 2, enemy->size.y / 2};
    enemy->body.radius = enemy->size.x < enemy->size.y ? (enemy->size.x / 3) : (enemy->size.y / 3);
    enemy->e_speed = spd;
    enemy->speed.x = enemy->e_speed * sinf((float) (ALLEGRO_PI / 180 * angle));
    enemy->speed.y = enemy->e_speed * -cosf((float) (ALLEGRO_PI / 180 * angle));
    enemy->dire_angle = angle;
    enemy->default_bullet = bt_img;
    enemy->CD = cd;
    enemy->shoot_interval = 0;
    enemy->bullet_mode = md;

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
