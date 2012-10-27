#define KEY_LEFT   'j'
#define KEY_RIGHT  'k'
#define KEY_DOWN   ' '
#define KEY_ROTATE 'x'

#define INTERVAL 2000

//�֐��v���g�^�C�v�錾
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

//�O���[�o���ϐ�
const int NUM_ROWS = 20;
const int NUM_COLS = 10;

const char *tiles[3] = {
 "  ",
 "��",
 "��",
};

//�u���b�N��r,c���W
struct position {
    int r;
    int c;
};

//���ݗ������̃u���b�N�������\����
struct block {
    //�ʒu�f�[�^
    struct position pos;
    //�`��f�[�^
    int pattern[4][4];
};


const int add_points[5] = {0,100,300,500,1000}; //���������̉��Z�|�C���g

// �ǂƌŒ�ς݃u���b�N
int background[21][12] ={0};
// ��ʃf�[�^�Bbackground[][]��w�i�Ƃ��Ă��̏��myblock.pattern[][]���d�˂�����
int view_data[21][12];

struct block myblock;

int total_point = 0; //���v���_


//�V��ނ̃u���b�N�̌`��f�[�^
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

