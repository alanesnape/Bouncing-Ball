// bounce_fixed.c - 修正版本
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "gfx.h"

#define CIRCLE_RADIUS 20
#define INITIAL_VELOCITY 3
#define SLEEP_TIME 20

typedef struct {
    double x, y;
    double vx, vy;
    int radius;
} Ball;

void update_ball(Ball *ball, int width, int height) {
    ball->x += ball->vx;
    ball->y += ball->vy;

    if (ball->x - ball->radius <= 0) {
        ball->x = ball->radius;
        ball->vx = fabs(ball->vx);
    } else if (ball->x + ball->radius >= width) {
        ball->x = width - ball->radius;
        ball->vx = -fabs(ball->vx);
    }

    if (ball->y - ball->radius <= 0) {
        ball->y = ball->radius;
        ball->vy = fabs(ball->vy);
    } else if (ball->y + ball->radius >= height) {
        ball->y = height - ball->radius;
        ball->vy = -fabs(ball->vy);
    }
}

void set_random_velocity(Ball *ball) {
    do {
        ball->vx = (rand() % 9) - 4;
        ball->vy = (rand() % 9) - 4;
    } while (fabs(ball->vx) < 1.0 && fabs(ball->vy) < 1.0);

    double speed = sqrt(ball->vx * ball->vx + ball->vy * ball->vy);
    ball->vx = (ball->vx / speed) * INITIAL_VELOCITY;
    ball->vy = (ball->vy / speed) * INITIAL_VELOCITY;
}

int main() {
    const int width = 800, height = 600;
    Ball ball;

    ball.x = width / 2;
    ball.y = height / 2;
    ball.radius = CIRCLE_RADIUS;
    ball.vx = INITIAL_VELOCITY;
    ball.vy = INITIAL_VELOCITY;

    srand(time(NULL));

    gfx_open(width, height, "Bouncing Ball - Fixed");
    printf("Ball is bouncing automatically!\n");
    printf("Click to move ball, press 'q' to quit\n");

    while (1) {
        // === 关键：非阻塞事件处理 ===
        // 使用 PeekMessage 代替 gfx_event_waiting/gfx_wait
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // 手动处理消息
            if (msg.message == WM_LBUTTONDOWN) {
                int x = LOWORD(msg.lParam);
                int y = HIWORD(msg.lParam);
                ball.x = x;
                ball.y = y;
                set_random_velocity(&ball);
                printf("Ball moved to: (%d, %d)\n", x, y);
            }
            else if (msg.message == WM_KEYDOWN) {
                char key = (char)msg.wParam;
                printf("Key pressed: %c\n", key);
                if (key == 'q' || key == 'Q' || key == 27) {
                    printf("Game ended\n");
                    return 0;
                }
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 更新球的位置（无论是否有事件）
        update_ball(&ball, width, height);

        // 绘制
        gfx_clear();
        gfx_color(255, 0, 0);  // 红色球
        gfx_circle((int)ball.x, (int)ball.y, ball.radius);

        // 显示信息
        gfx_color(0, 255, 0);  // 绿色文字
        gfx_text(20, 30, "Bouncing Ball");
        gfx_text(20, 60, "Click to move ball");
        gfx_text(20, 90, "Press 'q' to quit");

        gfx_flush();
        Sleep(SLEEP_TIME);
    }

    return 0;
}