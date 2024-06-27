#pragma once 

#include "raylib.h"

struct Ball
{
    int posX;
    int posY;
    int width;
    int height;
    int speed;
    Vector2 direction;
};