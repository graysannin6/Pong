#include <utility>
#include <cmath>
#include <cstdlib>
#include "Utility.hpp"
struct GameState {
    Vector2 window_size;
    Rectangle paddle;
    Rectangle ai_paddle;
    float paddle_speed;
    Rectangle ball;
    Vector2 ball_dir;
    float ball_speed;
    float ai_target_y;
    float ai_reaction_delay;
    float ai_reaction_counter;
    int score_player;
    int score_cpu;
    float boost_timer;
};

// Function prototypes
void reset(GameState &gs);
std::pair<Vector2, bool> BallDirCalculate(Rectangle ball, Rectangle paddle);

class ScopedAudioDevice {
public:
    ScopedAudioDevice() {
        InitAudioDevice();
    }
    ~ScopedAudioDevice() {
        CloseAudioDevice();
    }
};

int main() {
    GameState gs = {
        {1280, 720},    //window_size
        {0, 0, 30, 80}, //paddle
        {0, 0, 30, 80}, //ai_paddle
        10.0f,          //paddle_speed    
        {0, 0, 30, 30}, //ball
        {0, 0},         //ball_dir
        10.0f,          //ball_speed
        0.0f,           //ai_target_y
        0.1f,           //ai_reaction_delay
        0.0f,           //ai_reaction_counter
        0,              //score_player
        0,              //score_cpu
        0.0f            //boost_timer
    };

    InitWindow(static_cast<int>(gs.window_size.x), static_cast<int>(gs.window_size.y), "PONG");
    SetTargetFPS(60);

    ScopedAudioDevice audioDevice;
    
    Sound sfx_hit = LoadSound("hit.wav");
    Sound sfx_win = LoadSound("win.wav");
    Sound sfx_lose = LoadSound("lose.wav");


    reset(gs);
    
    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        
        // Update timers
        gs.ai_reaction_counter += delta_time;
        gs.boost_timer -= delta_time;

        // Paddle movement
        if (IsKeyDown(KEY_UP)) gs.paddle.y -= gs.paddle_speed;
        if (IsKeyDown(KEY_DOWN)) gs.paddle.y += gs.paddle_speed;

        if (IsKeyPressed(KEY_SPACE))
        {
            if (gs.boost_timer < 0)
            {
                gs.boost_timer = 0.2f;
            }
            
        }
        

        gs.paddle.y = Clamp(gs.paddle.y, 0.0f, gs.window_size.y - gs.paddle.height);

       // AI movement
        if (gs.ai_reaction_counter >= gs.ai_reaction_delay) {
            gs.ai_reaction_counter = 0.0f;
            float ball_mid = gs.ball.y + gs.ball.height / 2;
            if (gs.ball_dir.x < 0) {
                gs.ai_target_y = ball_mid - gs.ai_paddle.height / 2;
                gs.ai_target_y += static_cast<float>(rand() % 41 - 20); 
            } else {
                gs.ai_target_y = gs.window_size.y / 2 - gs.ai_paddle.height / 2;
            }
        }
        

        float ai_paddle_mid = gs.ai_paddle.y + gs.ai_paddle.height / 2;
        float target_diff = gs.ai_target_y - gs.ai_paddle.y;
        gs.ai_paddle.y += Clamp(target_diff, -gs.paddle_speed, gs.paddle_speed) * 0.65f;
        gs.ai_paddle.y = Clamp(gs.ai_paddle.y, 0.0f, gs.window_size.y - gs.ai_paddle.height);

        // Calculate next ball position
        Rectangle next_ball_rect = gs.ball;
        next_ball_rect.x += gs.ball_speed * gs.ball_dir.x;
        next_ball_rect.y += gs.ball_speed * gs.ball_dir.y;

        // Ball collision with top and bottom
        if (next_ball_rect.y >= gs.window_size.y - gs.ball.height || next_ball_rect.y <= 0) {
            gs.ball_dir.y *= -1;
        }

        // Ball goes off screen
        if (next_ball_rect.x >= gs.window_size.x - gs.ball.width) {
            gs.score_cpu++;
            PlaySound(sfx_lose);
            reset(gs);
        }
        if (next_ball_rect.x < 0) {
            gs.score_player++;
            PlaySound(sfx_win);
            reset(gs);
        }

        // Ball collision with paddles
        auto [new_dir, did_hit] = BallDirCalculate(next_ball_rect, gs.paddle);
        if (did_hit) 
        {
             if (gs.boost_timer > 0) {
                float d = 1 + gs.boost_timer / 0.2f;
                new_dir = Vector2Scale(new_dir, d);
            }
            gs.ball_dir = new_dir;
            PlaySound(sfx_hit);
        }
        
        auto [new_dir_ai, did_hit_ai] = BallDirCalculate(next_ball_rect, gs.ai_paddle);
        if (did_hit_ai) 
        {
            gs.ball_dir = new_dir_ai;
            PlaySound(sfx_hit);
        }

        // Update ball position
        gs.ball.x += gs.ball_speed * gs.ball_dir.x;
        gs.ball.y += gs.ball_speed * gs.ball_dir.y;

         // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw paddles with boost visual feedback
        if (gs.boost_timer > 0) {
            DrawRectangleRec(gs.paddle, {static_cast<unsigned char>(255 * (0.2f / gs.boost_timer)), 255, 255, 255});
        } else {
            DrawRectangleRec(gs.paddle, WHITE);
        }
        DrawRectangleRec(gs.ai_paddle, WHITE);

        // Draw ball with speed visual feedback
        DrawRectangleRec(gs.ball, {255, static_cast<unsigned char>(255 - 255 / Vector2Length(gs.ball_dir)), 0, 255});

        // Draw scores
        DrawText(TextFormat("%i", gs.score_cpu), 12, 12, 32, WHITE);
        DrawText(TextFormat("%i", gs.score_player), static_cast<int>(gs.window_size.x) - 28, 12, 32, WHITE);

        EndDrawing();
    }
    UnloadSound(sfx_hit);
    UnloadSound(sfx_win);
    UnloadSound(sfx_lose);
    CloseWindow();
    return 0;
}

void reset(GameState &gs) {
    float angle = static_cast<float>(rand() % 91 - 45);
    if (rand() % 2 == 0) angle += 180;
    float r = DEG2RAD * angle;

    gs.ball_dir.x = cos(r);
    gs.ball_dir.y = sin(r);

    gs.ball.x = gs.window_size.x / 2 - gs.ball.width / 2;
    gs.ball.y = gs.window_size.y / 2 - gs.ball.height / 2;

    float paddle_margin = 50.0f;

    gs.paddle.x = gs.window_size.x - (gs.paddle.width + paddle_margin);
    gs.paddle.y = gs.window_size.y / 2 - gs.paddle.height / 2;

    gs.ai_paddle.x = paddle_margin;
    gs.ai_paddle.y = gs.window_size.y / 2 - gs.ai_paddle.height / 2;
}

std::pair<Vector2, bool> BallDirCalculate(Rectangle ball, Rectangle paddle) {
    if (CheckCollisionRecs(ball, paddle)) {
        Vector2 ball_center = { ball.x + ball.width / 2, ball.y + ball.height / 2 };
        Vector2 paddle_center = { paddle.x + paddle.width / 2, paddle.y + paddle.height / 2 };
        Vector2 new_dir = Vector2Normalize(Vector2Subtract(ball_center, paddle_center));
        return { new_dir, true };
    }
    return { {0, 0}, false };
}

