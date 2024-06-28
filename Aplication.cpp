#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

struct GameState
{
    Vector2 window_size;
    Rectangle paddle;
    float paddle_speed;
    Rectangle ball;
    Vector2 ball_dir;
    float ball_speed;
};

// Function prototypes
void reset(GameState &gs);
template<typename T>
T Clamp(T value, T min, T max);
Vector2 Vector2Normalize(Vector2 v);
Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);

int main()
{
    GameState gs = {
        {1280, 720},
        {0, 0, 30, 80},
        10.0f,
        {0, 0, 30, 30},
        {0, 0},
        10.0f
    };

    reset(gs);
    
    InitWindow(static_cast<int>(gs.window_size.x),static_cast<int>(gs.window_size.y),"PONG");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        //Paddle movement
        if (IsKeyDown(KEY_UP)){ gs.paddle.y -= gs.paddle_speed; }
        if (IsKeyDown(KEY_DOWN)){ gs.paddle.y += gs.paddle_speed; }
        
        gs.paddle.y = Clamp(gs.paddle.y,0.0f,gs.window_size.y - gs.paddle.height);

        // Calculate next ball position
        Rectangle next_ball_rect = gs.ball;
        next_ball_rect.x += gs.ball_speed * gs.ball_dir.x;
        next_ball_rect.y += gs.ball_speed * gs.ball_dir.y;

        // Ball collision with top and bottom
        if (next_ball_rect.y >= 720 - gs.ball.height || next_ball_rect.y <= 0) {
            gs.ball_dir.y *= -1;
        }

        // Ball goes off screen
        if (next_ball_rect.x >= gs.window_size.x - gs.ball.width) {
            reset(gs);
        }
        if (next_ball_rect.x < 0) {
            reset(gs);
        }

        // Ball collision with paddle
        if (CheckCollisionRecs(next_ball_rect, gs.paddle)) {
            Vector2 ball_center = { next_ball_rect.x + gs.ball.width / 2, next_ball_rect.y + gs.ball.height / 2 };
            Vector2 paddle_center = { gs.paddle.x + gs.paddle.width / 2, gs.paddle.y + gs.paddle.height / 2 };
            gs.ball_dir = Vector2Normalize(Vector2Subtract(ball_center, paddle_center));
        }

        gs.ball.x += gs.ball_speed * gs.ball_dir.x;
        gs.ball.y += gs.ball_speed * gs.ball_dir.y;

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(gs.paddle, WHITE);
        DrawRectangleRec(gs.ball, RED);
        EndDrawing();

    }
    CloseWindow();
}

void reset(GameState &gs)
{
    float angle = -45 + static_cast<float>(rand() / (static_cast<float>(RAND_MAX / (90))));
    float r = angle * (PI / 180.0f);

    gs.ball_dir.x = cos(r);
    gs.ball_dir.y = sin(r);

    gs.ball.x = gs.window_size.x / 2 - gs.ball.width / 2;
    gs.ball.y = gs.window_size.y / 2 - gs.ball.height / 2;

    gs.paddle.x = gs.window_size.x - 80;
    gs.paddle.y = gs.window_size.y / 2 - gs.paddle.height / 2;
}


template<typename T>
T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Vector2 Vector2Normalize(Vector2 v) {
    float length = sqrt(v.x * v.x + v.y * v.y);
    if (length != 0.0f) {
        v.x /= length;
        v.y /= length;
    }
    return v;
}

Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
    Vector2 result = { v1.x - v2.x, v1.y - v2.y };
    return result;
}