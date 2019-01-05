//
// Created by user on 2018/12/24.
//

#ifndef FINAL_CLION_GENERAL_H
#define FINAL_CLION_GENERAL_H

#define MODE_COUNT 20

typedef struct point {
    float x, y;
} Vector2;

typedef struct circle {
    Vector2 center;
    float radius;
} Circle;

enum flyMode {
    stopped, up, right_front, left_front, right, left, right_back, left_back, down,
    rf_f, lf_f, rf_r, lf_l, rb_r, lb_l, rb_b, lb_b
};

#endif //FINAL_CLION_GENERAL_H
