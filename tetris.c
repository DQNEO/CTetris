#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define KEY_LEFT   'j'
#define KEY_RIGHT  'k'
#define KEY_DOWN   ' '
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
int  block_lock();
int  check_and_delete();
void view_render();
void view_gameover(); 
void view_clear();
int  rand_block_type();

void copy_block(int src[4][4], int dst[4][4]);


//グローバル変数
const int NUM_ROWS = 20;
const int NUM_COLS = 10;
const char *TILE_SPACE = "  ";
const char *TILE_WALL  = "□";
const char *TILE_BLOCK = "■";

int background[21][12];     // 壁と固定済みブロック
int view_data[21][12];      // 画面データ。background[][]を背景としてその上にmyblock.pattern[][]を重ねたもの

//ブロックのx,y座標
struct position {
    int x;
    int y;
};

//現在落下中のブロックを扱う構造体
struct block {
    //位置データ
    struct position pos;
    //形状データ
    int pattern[4][4];
};

struct block myblock;


int total_point = 0; //合計得点
int add_points[5] = {0,100,300,500,1000}; //同時消しの加算ポイント


//７種類のブロックの形状データ
int block_patterns[7][4][4] = {
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
    int row, col;

    //画面と壁を初期設定
    for(row = 0; row <= NUM_ROWS; row++) {
        for(col = 0; col <= NUM_COLS + 1; col++) {
            if((col == 0) || (col == NUM_COLS + 1) || (row == NUM_ROWS)) {
                background[row][col] = 9; //壁
            } else {
                background[row][col] = 0; //真空
            }
            
            view_data[row][col] = background[row][col];
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

    //ブロックの種類。ランダムに決まる。
    int block_type = rand_block_type();
    
    //まずブロックの座標を初期位置にリセット
    myblock.pos.x = 4;
    myblock.pos.y = 0;

    //ブロックパターンの中からblock_typeに応じた種類のブロックを読み込む
    copy_block(block_patterns[block_type], myblock.pattern);

    //壁＋ブロックをフィールドへ
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[i][j+4] = background[i][j+4] + myblock.pattern[i][j];
        }
    }

    //ゲームオーバー判定
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            //初期位置に置いたブロックが既に固定ブロックに重なっていればゲームオーバー
            if(view_data[i][j+4] > 1) {
                return 0;
            }

        }
    }

    return 1;
}

void copy_block(int src[4][4], int dst[4][4])
{
    int i,j;
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

//乱数を発生させ、その乱数を7で割った余り（０〜６まで）でブロックの種類を決定
int rand_block_type()
{
    srand((unsigned)time(NULL));
    return rand() % 7;
}

//キー入力を受けてブロックを操作する(移動/回転)
void block_operate()
{
    char key; //受け付けたキーを保存する変数

    key = getch(); //キーから一文字入力

    //キーに応じて各方向へブロックを移動したり、回転させたりする
    switch(key) {

        case KEY_RIGHT: // move right
        case 'l': // move right
            if(!is_attached(+1, 0)) {
                block_move(1, 0);
            }
            break;

        case KEY_LEFT: // move left
            if(!is_attached(-1, 0)) {
                block_move(-1, 0);
            }
            break;
        case KEY_DOWN: // move down
            if(!is_attached(0, +5)) {
                block_move(0, 5);
            }
            break;
        case KEY_ROTATE: // rotate
        case 'c': // rotate
            block_rotate();

    }
}

//重なり検査
//落下中のブロックが壁や固定済みブロックに接触していないか判別
int is_attached(int dx, int dy)
{
    int i, j; //forループ制御用変数

    //ブロックが向かう位置に、固定ブロックもしくは壁があるかどうかを検査
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            if(myblock.pattern[i][j]) {
                if(background[myblock.pos.y + dy + i][myblock.pos.x + dx + j] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

//落下中ブロックを移動
void block_move(int dx, int dy)
{
    int i, j; //forループ制御用変数

    //一度ブロックを消して
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[myblock.pos.y+i][myblock.pos.x+j] -= myblock.pattern[i][j];
        }
    }
    //ブロックの座標を更新
    myblock.pos.x += dx;
    myblock.pos.y += dy;

    //新しい座標にブロックを入れなおし
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[myblock.pos.y+i][myblock.pos.x+j] += myblock.pattern[i][j];
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
    copy_block(myblock.pattern, temp);

    //ブロックを回転
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            myblock.pattern[i][j] = temp[3-j][i];
        }
    }
    //重なってるブロックが出てしまったらブロックを回転前に戻して中止
    if(is_attached(0, 0)) {
        copy_block(temp, myblock.pattern);
        return 1;
    }

    //一旦フィールドからブロック消して回転後のブロックを再表示
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[myblock.pos.y+i][myblock.pos.x+j] -= temp[i][j];
            view_data[myblock.pos.y+i][myblock.pos.x+j] += myblock.pattern[i][j];
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
    if(!is_attached(0, 1)) {
        // なければ下に移動
        block_move(0, 1);
    } else {
        // あれば壁にする
        int num_lines = block_lock();
        total_point += add_points[num_lines];
        
        //新ブロックを生成
        if (block_new()) {
            view_render();
        } else {
            //生成時点で接地したらゲームオーバー
            return 0;
        }
    }
    
    return 1;
}

//着地したブロックを固定し、横一列がそろってるかの判定を呼び出す
int block_lock()
{
    int i, j, num_lines;

    //ブロックを壁に加える
    for(i = 0; i<NUM_ROWS + 1; i++) {
        for(j = 0; j<NUM_COLS + 2; j++) {
            background[i][j] = view_data[i][j];
        }
    }

    num_lines = check_and_delete(); //横一列がそろってるか判定して処理する関数を呼ぶ

    //列完成判定後の壁を画面データへ
    for(i = 0; i< NUM_ROWS+1; i++) {
        for(j = 0; j< NUM_COLS + 2; j++) {
            view_data[i][j] = background[i][j];
        }
    }

    //消したラインの数
    return num_lines;
}

//横一段が完成しているか検査。
//そろっていればその段を消して上のブロック群を下ろす
int check_and_delete()
{
    int row, col, k; //forループ制御用
    int is_complete; //横一列がそろっていれば１、一つでも隙間があると０になる
    int num_lines = 0; //同時に消したラインの数

    while(1) {
        for(row = 0; row<NUM_ROWS; row++) {
            is_complete = 1;

            for(col = 1; col<NUM_COLS + 1; col++) {
                if(background[row][col] == 0) {
                    is_complete = 0;
                }
            }
            if( is_complete) break;
        }
        if (! is_complete) break;

        num_lines++;


        // 列を消去
        for(col = 1; col< NUM_COLS + 1; col++) {
            background[row][col] = 0;
        }


        //消えた列より上にあった固定ブロックを列の消えたところへ下ろす
        for(k = row; k>0; k--) {
            for(col = 1; col< NUM_COLS + 1; col++) {
                background[k][col] = background[k-1][col];
            }
        }
    }

    return num_lines;
}

//画面表示
//view_data[][]の内容を画面に出力する
void view_render()
{
    int row, col;
    char output_string[25*30] ="";


    //view_dataの中身(壁、ブロック、真空)を画面表示
    for(row = 0; row<NUM_ROWS + 1; row++) {
        strcat(output_string ,"          ");
        for(col = 0; col < NUM_COLS + 2; col++) {

            switch(view_data[row][col]) {
                case 0:
                    strcat(output_string, TILE_SPACE);
                    break;
                case 9:
                    strcat(output_string, TILE_WALL);
                    break;
                case 1:
                    strcat(output_string, TILE_BLOCK);
                    break;
            }
        }

        strcat(output_string, "\n");
    }

    strcat(output_string, "\0");

    view_clear();
    printf("%s", output_string);

    printf("              Total Point : %d\n", total_point);
}


//ゲームオーバー画面を表示する
void view_gameover()
{
    printf("\n              GAME OVER  \n");
}

//画面を全消去
void view_clear()
{
    system("cls");
}

 