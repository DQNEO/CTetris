#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define KEY_LEFT  'j'
#define KEY_RIGHT 'l'
#define KEY_DOWN  ' '
#define KEY_ROTATE 'x'

#define INTERVAL 2000

//関数プロトタイプ宣言
void init();
int  block_new();
void block_operate();
int  is_attached(int, int);
void block_move(int, int);
int  block_rotate();
int  block_drop();
void block_lock();
void check_and_delete();

void view_render();
void view_gameover(); 
void view_clear();

//グローバル変数
int background[21][12];     // 壁と固定済みブロック用
int block[4][4];            // 現在落下中のブロック
int view_data[21][12];      // 描画するデータ。background[][]を背景としてその上にblock[][]を重ねたもの

int y = 0; //ブロックのy座標
int x = 4; //ブロックのx座標

int oneline, twoline, threeline, fourline; //得点を記憶する変数たち(消したラインの数を記録)


//７種類のブロックのデータ
const int block_list[7][4][4] = {
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
    },

    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,1,0,0}
    },

    {
        {0,0,1,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    },

    {
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
    },

    {
        {0,0,0,0},
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0}
    },

    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },

    {
        {0,0,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,1,0}
    }
};

int main()
{
    int time = 0; //タイマをリセット

    init();

    //ゲームオーバーになるまで無限ループ
    while (1) {

        //キー入力があればブロックを動かす
        if(kbhit()){
            block_operate();
        }

        if (time < INTERVAL) {
            time++;
        } else {
            //ブロックを１マス下げる
            if (!block_drop()) {
                //ゲームオーバーの場合
                break;
            }
            //タイマをリセット
            time = 0;
        }
    }

    view_gameover();
    return 0;
}

//ゲーム起動直後の初期設定。
//画面と壁のデータを初期化
void init()
{
    int i, j;

    //画面と壁を初期設定
    for(i = 0; i <= 20; i++) {
        for(j = 0; j <= 11; j++) {
            if((j == 0) || (j == 11) || (i == 20)) {
                background[i][j] = 9;
            } else {
                background[i][j] = 0;
            }
            
            view_data[i][j] = background[i][j];
        }
    }


    block_new(); //最初のブロックを生成
    view_render();
}

//新しいブロックを生成して次のブロックに発生させる
//@return 0:ゲームオーバー, 1:ゲーム続く
int block_new()
{
    int i, j; //forループ制御用の変数
    int block_type; //ブロックの種類用。０～６の乱数を入れる

    //まずブロックの座標を初期位置にリセット
    y = 0;
    x = 4;

    //乱数を発生させ、その乱数を7で割った余り（０～６まで）でブロックの種類を決定
    srand((unsigned)time(NULL));
    block_type = rand() % 7;

    //ブロックデータの中からblock_typeに応じた種類のブロックを読み込む
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            block[i][j] = 0;
            block[i][j] = block_list[block_type][i][j];
        }
    }
    //壁＋ブロックをフィールドへ
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[i][j+4] = background[i][j+4] + block[i][j];

            //初期位置に置いたブロックが既に固定ブロックに重なっていればゲームオーバー
            if(view_data[i][j+4] > 1) {
                return 0;
            }

        }
    }

    return 1;
}

//画面表示
//view_data[][]の内容を画面に出力する
void view_render()
{
    int i, j;

    view_clear();

    //データに応じてブロックや空白を画面表示
    for(i = 0; i<21; i++) {
        for(j = 0; j < 12; j++) {

            switch(view_data[i][j]) {
                case 0:
                    printf("  ");
                    break;
                case 9:
                    printf("□");
                    break;
                default:
                    printf("■");
                    break;
            }
        }
        printf("\n");
    }

    //得点表示
    //printf("\n１行消し：%d回  ２行消し：%d回  ３行消し：%d回  ４行消し：%d回\n", oneline, twoline, threeline, fourline);
}


//ゲームオーバー画面を表示する
void view_gameover()
{
    view_clear();
    printf("\n\n==========\nGAME OVER\n==========\n\n");
}

//画面を全消去
void view_clear()
{
    system("cls");
}

//キー入力を受けてブロックを操作する(移動/回転)
void block_operate()
{
    char key; //受け付けたキーを保存する変数

    key = getch(); //キーから一文字入力

    //キーに応じて各方向へブロックを移動したり、回転させたりする
    switch(key) {

        case KEY_RIGHT: // move right
            if(!is_attached(x+1, y)) {
                block_move(x+1, y);
            }
            break;
        case KEY_LEFT: // move left
            if(!is_attached(x-1, y)) {
                block_move(x-1, y);
            }
            break;
        case KEY_DOWN: // move down
            if(!is_attached(x, y+1)) {
                block_move(x, y+1);
            }
            break;
        case KEY_ROTATE: // rotate
            block_rotate();

    }
}

//重なり検査
//落下中のブロックが壁や固定済みブロックに接触していないか判別
int is_attached(int x2, int y2)
{
    int i, j; //forループ制御用変数

    //ブロックが向かう位置に、固定ブロックもしくは壁があるかどうかを検査
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            if(block[i][j]) {
                if(background[y2 + i][x2 + j] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

//落下中ブロックを移動
void block_move(int x2, int y2)
{
    int i, j; //forループ制御用変数

    //今までのブロックを消して
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[y+i][x+j] -= block[i][j];
        }
    }
    //ブロックの座標を更新
    x = x2;
    y = y2;

    //新しい座標にブロックを入れなおし
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[y+i][x+j] += block[i][j];
        }
    }

    view_render();
}

//ブロックを回転する
int block_rotate()
{
    int i, j; //forループ制御用
    int temp[4][4] = {0}; //ブロックを一時保存するための配列

    //ブロックを回転する前にtempにセーブ
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            temp[i][j] = block[i][j];
        }
    }

    //ブロックを回転
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            block[i][j] = temp[3-j][i];
        }
    }
    //重なってるブロックが出てしまったらブロックを回転前に戻して中止
    if(is_attached(x, y)) {
        for(i = 0; i<4; i++) {
            for(j = 0; j<4; j++) {
                block[i][j] = temp[i][j];
            }
        }
        return 1;
    }

    //一旦フィールドからブロック消して回転後のブロックを再表示
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[y+i][x+j] -= temp[i][j];
            view_data[y+i][x+j] += block[i][j];
        }
    }

    view_render();

    return 0;
}

//ブロックを落下させる。
//下に接地した場合はその位置に固定
int block_drop()
{
    //重なりあるか判定
    if(!is_attached(x, y+1)) {
        // なければ移動
        block_move(x, y+1);
    } else {
        // あれば壁にする
        block_lock();
        if (! block_new()) {
            return 0;
        }
        view_render();
    }
    
    return 1;
}

//着地したブロックを固定し、横一列がそろってるかの判定を呼び出す
void block_lock()
{
    int i, j; //forループ制御用変数

    //ブロックを壁に加える
    for(i = 0; i<21; i++) {
        for(j = 0; j<12; j++) {
            background[i][j] = view_data[i][j];
        }
    }

    check_and_delete(); //横一列がそろってるか判定して処理する関数を呼ぶ

    //列完成判定後の壁をフィールドへ
    for(i = 0; i<21; i++) {
        for(j = 0; j<12; j++) {
            view_data[i][j] = background[i][j];
        }
    }
}

//横一段が完成しているか検査。
//そろっていればその段を消して上のブロック群を下ろす
void check_and_delete()
{
    int i, j, k; //forループ制御用
    int comp; //横一列がそろっていれば１、一つでも隙間があると０になる
    int lines = 0; //同時に消したラインの数

    while(1) {
        for(i = 0; i<20; i++) {
            comp = 1;

            for(j = 1; j<11; j++) {
                if(background[i][j] == 0) {
                    comp = 0;
                }
            }
            if(comp == 1) break;
        }
        if(comp == 0) break;

        lines++;


        // 列を消去
        for(j = 1; j<11; j++) {
            background[i][j] = 0;
        }


        //消えた列より上にあった固定ブロックを列の消えたところへ下ろす
        for(k = i; k>0; k--) {
            for(j = 1; j<11; j++) {
                background[k][j] = background[k-1][j];
            }
        }
    }

    //同時に消したラインの数をカウント
    switch(lines) {
        case 1:
            oneline++;
            break;
        case 2:
            twoline++;
            break;
        case 3:
            threeline++;
            break;
        case 4:
            fourline++;
            break;
        default:
            break;
    }

}

 