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
void copy_whole(int src[21][12], int dst[21][12]);

//グローバル変数
const int NUM_ROWS = 20;
const int NUM_COLS = 10;

const char *tiles[3] = {
 "  ",
 "■",
 "□",
};

//ブロックのr,c座標
struct position {
    int r;
    int c;
};

//現在落下中のブロックを扱う構造体
struct block {
    //位置データ
    struct position pos;
    //形状データ
    int pattern[4][4];
};


const int add_points[5] = {0,100,300,500,1000}; //同時消しの加算ポイント

// 壁と固定済みブロック
int background[21][12] ={0};
// 画面データ。background[][]を背景としてその上にmyblock.pattern[][]を重ねたもの
int view_data[21][12];

struct block myblock;

int total_point = 0; //合計得点


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

