#include "raylib.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

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

Vector2 Vector2Scale(Vector2 v, float scale) {
    Vector2 result = { v.x * scale, v.y * scale };
    return result;
}

float Vector2Length(Vector2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}