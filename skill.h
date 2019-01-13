//
// Created by user on 2019/1/11.
//

#ifndef FINAL_CLION_SKILL_H
#define FINAL_CLION_SKILL_H

#include "general.h"
#include "bullet.h"
#include "character.h"

void launch_big_fire(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void launch_big_arrow(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void defend(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void shoot_triangle(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void big_blow(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void horizontal_blow(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void thin_circle(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void thick_circle(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void landmine(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void ten_thousands_arrows(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void product_rule(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void plus_point2(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);
void cure(Vector2 pos, enum flyMode mode, Bullet **list, int dmg, float mul);

#endif //FINAL_CLION_SKILL_H
