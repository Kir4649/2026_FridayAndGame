このゲームはPlayerとなるブロックを引っ張ってはじく引っ張りハンティングゲームです。
仕様プラットフォーム　Android；
今後実装予定の仕様は以下の通りです。
敵の実装と攻撃
スタート画面の実装
クリア＆ゲームオーバー画面の実装



// ドラッグ中

        if (dragging && input.isTouching)
        {
            player.x = input.currentX - player.w / 2;
            player.y = input.currentY - player.h / 2;
        }
//触ってるかの確認
 // 指を離した瞬間

        if (!input.isTouching && prevTouch && dragging)
        {
            dragging = false;

            float dx = input.currentX - input.startX;
            float dy = input.currentY - input.startY;


            // 最大距離制限

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


            // 発射

            float power = 0.15f;

            // 逆方向へ飛ばす
            player.vx = -dx * power;
            player.vy = -dy * power;
        }
        // 指を離した瞬間起きる処理
