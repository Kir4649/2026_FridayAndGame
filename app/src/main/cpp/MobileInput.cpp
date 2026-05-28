//
// Created by k2-takeshita on 2026/05/06.
//
#include "DxLib.h"
#include <math.h>

struct TouchPoint
{
    int x;
    int y;
};
struct Player{
    int x;
    int y;

    int w;
    int h;//大きさ
};

struct MobileInput
{
    bool isTouching;

    bool tap;
    bool swipe;
    bool flick;
    bool pinchIn;
    bool pinchOut;

    int startX;
    int startY;
    int currentX;
    int currentY;

    int startTime;
    int touchCount;

    float previousPinchDistance;
};

void DrawUiRect(const Player& rect, const char* label, int color){
    DrawBox(
            rect.x,
            rect.y,
            rect.x + rect.w,
            rect.y + rect.h,
            color,
            TRUE
    );
    DrawBox(
            rect.x,
            rect.y,
            rect.x + rect.w,
            rect.y + rect.h,
            GetColor(255,100,255),
            FALSE
    );
    DrawString(
            rect.x +10,
            rect.y + 10,
            label,
            GetColor(255,100,255)
    );
}

float GetDistance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

void InitMobileInput(MobileInput* input)
{
    input->isTouching = false;

    input->tap = false;
    input->swipe = false;
    input->flick = false;
    input->pinchIn = false;
    input->pinchOut = false;

    input->startX = 0;
    input->startY = 0;
    input->currentX = 0;
    input->currentY = 0;

    input->startTime = 0;
    input->touchCount = 0;

    input->previousPinchDistance = 0.0f;
}

void UpdateMobileInput(MobileInput* input)
{
    const int swipeDistance = 80;
    const int flickTime = 200;
    const float pinchThreshold = 5.0f;

    input->tap = false;
    input->swipe = false;
    input->flick = false;
    input->pinchIn = false;
    input->pinchOut = false;

    int touchNum = GetTouchInputNum();
    input->touchCount = touchNum;

    if (touchNum == 0)
    {
        if (input->isTouching)
        {
            int dx = input->currentX - input->startX;
            int dy = input->currentY - input->startY;
            float distance = GetDistance(
                    (float)input->startX,
                    (float)input->startY,
                    (float)input->currentX,
                    (float)input->currentY
            );

            int elapsedTime = GetNowCount() - input->startTime;

            if (distance < swipeDistance)
            {
                input->tap = true;

            }
            else
            {
                input->swipe = true;

                if (elapsedTime <= flickTime)
                {
                    input->flick = true;
                }
            }
        }

        input->isTouching = false;
        input->previousPinchDistance = 0.0f;
        return;
    }

    int x = 0;
    int y = 0;

    GetTouchInput(0, &x, &y, NULL, NULL);

    if (!input->isTouching)
    {
        input->isTouching = true;

        input->startX = x;
        input->startY = y;
        input->currentX = x;
        input->currentY = y;

        input->startTime = GetNowCount();
    }
    else
    {
        input->currentX = x;
        input->currentY = y;
    }

    if (touchNum >= 2)
    {
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        GetTouchInput(0, &x1, &y1, NULL, NULL);
        GetTouchInput(1, &x2, &y2, NULL, NULL);

        float currentDistance = GetDistance(
                (float)x1,
                (float)y1,
                (float)x2,
                (float)y2
        );

        if (input->previousPinchDistance == 0.0f)
        {
            input->previousPinchDistance = currentDistance;
            return;
        }

        float diff = currentDistance - input->previousPinchDistance;

        if (fabsf(diff) > pinchThreshold)
        {
            if (diff > 0)
            {
                input->pinchOut = true;
            }
            else
            {
                input->pinchIn = true;
            }
        }

        input->previousPinchDistance = currentDistance;
    }
}

void DrawInputDebug(const MobileInput& input)
{
    DrawFormatString(20, 20, GetColor(255, 255, 255), "Touch Count : %d", input.touchCount);
    DrawFormatString(20, 50, GetColor(255, 255, 255), "Current : %d, %d", input.currentX, input.currentY);
    DrawFormatString(20, 80, GetColor(255, 255, 255), "Start   : %d, %d", input.startX, input.startY);

    if (input.tap)
    {
        DrawString(20, 140, "Tap", GetColor(255, 255, 0));
    }

    if (input.swipe)
    {
        DrawString(20, 170, "Swipe", GetColor(0, 255, 255));
    }

    if (input.flick)
    {
        DrawString(20, 200, "Flick", GetColor(255, 120, 0));
    }

    if (input.pinchIn)
    {
        DrawString(20, 230, "Pinch In", GetColor(255, 100, 255));
    }

    if (input.pinchOut)
    {
        DrawString(20, 260, "Pinch Out", GetColor(100, 255, 100));
    }

    if (input.isTouching)
    {
        DrawCircle(input.currentX, input.currentY, 20, GetColor(255, 255, 255), TRUE);
        DrawLine(input.startX, input.startY, input.currentX, input.currentY, GetColor(255, 255, 0));
    }
}

int android_main()
{
    SetGraphMode(720, 1280, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    MobileInput input;
    InitMobileInput(&input);


// プレイヤー作成
    Player player;
    player.x = 300;
    player.y = 1000;
    player.w = 100;
    player.h = 100;

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        UpdateMobileInput(&input);

        // プレイヤー描画
        DrawUiRect(player, "PLAYER", GetColor(255,0,0));

        DrawInputDebug(input);

        ScreenFlip();
    }

    //for(int startX = 0; x < )

    DxLib_End();
    return 0;
}

