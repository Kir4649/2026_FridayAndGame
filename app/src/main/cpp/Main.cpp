#include "DxLib.h"

struct Rect{
    int x;
    int y;
    int w;
    int h;
};//

struct UILayout{
    Rect topLeft;
    Rect topRight;
    Rect bottomLeft;
    Rect bottomRight;
    Rect center;
};
UILayout CreateLayout(int screenW, int screenH){
    const int panelW = 180;
    const int panelH = 90;

    const int centerW = 360;
    const int centerH = 160;
    const int margin = 20;

    UILayout layout;

    layout.topLeft = {
            margin,
            margin,
            panelW,
            panelH,
    };

    layout.topLeft = {
            margin,
            margin,
            panelW,
            panelH,
    };
    layout.topRight =
            {
            //.x screenW - panelW - margin,
            };


    return layout;
}
void DrawUiRect(const Rect& rect, const char* label, int color){
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
            GetColor(255,255,255),
            TRUE
            );
    DrawString(
            rect.x +10,
            rect.y + 10,
            label,
            GetColor(255,255,255)
            );
}

void DrawUiLayout(const UILayout layout){
    DrawUiRect(layout.topLeft,"TopLeft", GetColor(80,170,200));
    DrawUiRect(layout.topRight,"TopRight", GetColor(80,160,120));
    DrawUiRect(layout.bottomLeft,"bottomLeft", GetColor(80,160,120));
    DrawUiRect(layout.bottomRight,"bottomRight", GetColor(80,160,120));


}

int android_main(){
    SetGraphMode(720,1280,32);
    if(DxLib_Init() == -1){
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() == 0){
        ClearDrawScreen();
        int screenW = 720;
        int screenH = 1280;
        DrawString(20,1240,"UI Layout Sample", GetColor(255,255,0));
        ScreenFlip();
    }
    DxLib_End();
    return 0;
}



// Created by student on 2026/05/07.
//
