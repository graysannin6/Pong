#include "raylib.h"
#include <string>
#include "Paddle.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
const char* TITLE = "Pong";

int main()
{

    SetTargetFPS(60);
    InitWindow(WIDTH,HEIGHT,TITLE);

    Paddle paddle{100,100,180,30,10,WHITE};

    while (!WindowShouldClose())
    {
        paddle.updateBounds();
        //Inputs
        if (IsKeyDown(KEY_A) && paddle.l_corner_x >= 0){ paddle.x -= paddle.speed; }
        if (IsKeyDown(KEY_D) && paddle.r_corner_x <= WIDTH){ paddle.x += paddle.speed; }
        
        BeginDrawing();
        ClearBackground(BLACK);
        //Game loop
        DrawRectangle(paddle.x,paddle.y,paddle.width,paddle.height,paddle.color);

        EndDrawing();
    }
    CloseWindow();
}