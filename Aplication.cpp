#include <string>
#include "Paddle.h"
#include "Ball.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
const char* TITLE = "Pong";

int main()
{
    Paddle paddle{100,100,180,30,10,WHITE};
    Ball ball{100,150,30,30,10,{0,-1}};
    
    InitWindow(WIDTH,HEIGHT,TITLE);
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        paddle.updateBounds();
        //Paddle movement
        if (IsKeyDown(KEY_A) && paddle.l_corner_x >= 0){ paddle.x -= paddle.speed; }
        if (IsKeyDown(KEY_D) && paddle.r_corner_x <= WIDTH){ paddle.x += paddle.speed; }
        
        // Predict the next position of the ball
        Rectangle next_ball_rect = { static_cast<float>(ball.posX), static_cast<float>(ball.posY), static_cast<float>(ball.width), static_cast<float>(ball.height) };
        next_ball_rect.y += ball.speed * ball.direction.y;

        // Ball collision with screen edges
        if (next_ball_rect.y >= HEIGHT - ball.height || next_ball_rect.y <= 0)
        {
            ball.direction.y *= -1;
        }

        // Ball collision with paddle
        Rectangle paddle_rect = { static_cast<float>(paddle.x), static_cast<float>(paddle.y), static_cast<float>(paddle.width), static_cast<float>(paddle.height) };
        if (CheckCollisionRecs(next_ball_rect, paddle_rect))
        {
            ball.direction.y *= -1;
        }

        // Update ball position
        ball.posY += ball.speed * ball.direction.y;

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(paddle.x, paddle.y, paddle.width, paddle.height, paddle.color);
        DrawRectangle(ball.posX, ball.posY, ball.width, ball.height, RED);
        EndDrawing();
    }
    CloseWindow();
}