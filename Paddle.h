#pragma once

#include "raylib.h"

struct Paddle
{
    int x;
    int y;
    int width;
    int height;
    int speed;
    Color color;

    //bounds
    int l_corner_x;
    int r_corner_x;
    int u_corner_y;
    int b_corner_y;

    void updateBounds()
    {
        l_corner_x = x;
        r_corner_x = x + width;
        u_corner_y = y;
        b_corner_y = y + height;
    }
};