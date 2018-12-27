//
// Created by user on 2018/12/24.
//

#ifndef FINAL_CLION_GENERAL_H
#define FINAL_CLION_GENERAL_H

typedef struct point {
    float x, y;
} Vector2;

typedef struct circle {
    Vector2 center;
    float radius;
} Circle;

enum flyMode {
    stopped, straight_forward, right_front, left_front, right, left, down, rf_26, lf_26, rf_63, lf_63
};

#endif //FINAL_CLION_GENERAL_H
