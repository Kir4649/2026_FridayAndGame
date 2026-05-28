
#include "DxLib.h"
#include <math.h>

struct Player
{
    float x;
    float y;

    int w;
    int h;

    float vx;
    float vy;
};

struct MobileInput
{
    bool isTouching;

    int startX;
    int startY;

    int currentX;
    int currentY;
};


//==============================
// 距離
//==============================
float GetDistance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return sqrtf(dx * dx + dy * dy);
}


//==============================
// 入力初期化
//==============================
void InitMobileInput(MobileInput* input)
{
    input->isTouching = false;

    input->startX = 0;
    input->startY = 0;

    input->currentX = 0;
    input->currentY = 0;
}


//==============================
// タッチ更新
//==============================
void UpdateMobileInput(MobileInput* input)
{
    int touchNum = GetTouchInputNum();

    // タッチなし
    if (touchNum == 0)
    {
        input->isTouching = false;
        return;
    }

    int x;
    int y;

    GetTouchInput(0, &x, &y, NULL, NULL);

    // タッチ開始
    if (!input->isTouching)
    {
        input->isTouching = true;

        input->startX = x;
        input->startY = y;
    }

    input->currentX = x;
    input->currentY = y;
}


//==============================
// プレイヤー描画
//==============================
void DrawPlayer(const Player& p)
{
    DrawBox(
            (int)p.x,
            (int)p.y,
            (int)p.x + p.w,
            (int)p.y + p.h,
            GetColor(255, 0, 0),
            TRUE
    );

    DrawBox(
            (int)p.x,
            (int)p.y,
            (int)p.x + p.w,
            (int)p.y + p.h,
            GetColor(255,255,255),
            FALSE
    );

    DrawString(
            (int)p.x + 10,
            (int)p.y + 10,
            "PLAYER",
            GetColor(255,255,255)
    );
}


//==============================
// プレイヤーに触れたか
//==============================
bool IsTouchPlayer(const Player& p, int tx, int ty)
{
    return (
            tx >= p.x &&
            tx <= p.x + p.w &&
            ty >= p.y &&
            ty <= p.y + p.h
    );
}


//==============================
// メイン
//==============================
int android_main()
{
    SetGraphMode(720, 1280, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    //---------------------------------
    // 入力
    //---------------------------------
    MobileInput input;
    InitMobileInput(&input);

    //---------------------------------
    // プレイヤー
    //---------------------------------
    Player player;

    player.x = 300;
    player.y = 900;

    player.w = 100;
    player.h = 100;

    player.vx = 0;
    player.vy = 0;

    //---------------------------------
    // ドラッグ状態
    //---------------------------------
    bool dragging = false;

    //---------------------------------
    // 前フレームのタッチ状態
    //---------------------------------
    bool prevTouch = false;

    //---------------------------------
    // メインループ
    //---------------------------------
    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        //---------------------------------
        // 入力更新
        //---------------------------------
        UpdateMobileInput(&input);

        //---------------------------------
        // タッチ開始
        //---------------------------------
        if (input.isTouching && !prevTouch)
        {
            if (IsTouchPlayer(player, input.currentX, input.currentY))
            {
                dragging = true;

                // 停止
                player.vx = 0;
                player.vy = 0;
            }
        }

        //---------------------------------
        // ドラッグ中
        //---------------------------------
        if (dragging && input.isTouching)
        {
            player.x = input.currentX - player.w / 2;
            player.y = input.currentY - player.h / 2;
        }

        //---------------------------------
        // 指を離した瞬間
        //---------------------------------
        if (!input.isTouching && prevTouch && dragging)
        {
            dragging = false;

            float dx = input.currentX - input.startX;
            float dy = input.currentY - input.startY;

            //---------------------------------
            // 最大距離制限
            //---------------------------------
            float length = GetDistance(
                    0,
                    0,
                    dx,
                    dy
            );

            const float maxLength = 250.0f;

            if (length > maxLength)
            {
                dx *= maxLength / length;
                dy *= maxLength / length;
            }

            //---------------------------------
            // 発射
            //---------------------------------
            float power = 0.15f;

            // 逆方向へ飛ばす
            player.vx = -dx * power;
            player.vy = -dy * power;
        }

        //---------------------------------
        // 移動
        //---------------------------------
        player.x += player.vx;
        player.y += player.vy;

        //---------------------------------
        // 摩擦
        //---------------------------------
        player.vx *= 0.98f;
        player.vy *= 0.98f;

        //---------------------------------
        // 壁反射
        //---------------------------------

        // 左
        if (player.x < 0)
        {
            player.x = 0;
            player.vx *= -1;
        }

        // 右
        if (player.x + player.w > 720)
        {
            player.x = 720 - player.w;
            player.vx *= -1;
        }

        // 上
        if (player.y < 0)
        {
            player.y = 0;
            player.vy *= -1;
        }

        // 下
        if (player.y + player.h > 1280)
        {
            player.y = 1280 - player.h;
            player.vy *= -1;
        }

        //---------------------------------
        // プレイヤー描画
        //---------------------------------
        DrawPlayer(player);

        //---------------------------------
        // 引っ張り線
        //---------------------------------
        if (dragging)
        {
            DrawLine(
                    (int)player.x + player.w / 2,
                    (int)player.y + player.h / 2,
                    input.currentX,
                    input.currentY,
                    GetColor(255,255,0)
            );
        }

        //---------------------------------
        // デバッグ
        //---------------------------------
        DrawFormatString(
                20,
                20,
                GetColor(255,255,255),
                "Touch : %d",
                input.isTouching
        );

        //---------------------------------
        // タッチ保存
        //---------------------------------
        prevTouch = input.isTouching;

        //---------------------------------
        // 画面更新
        //---------------------------------
        ScreenFlip();
    }

    DxLib_End();

    return 0;
}