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
const char *TILE_SPACE = "  ";
const char *TILE_WALL  = "��";
const char *TILE_BLOCK = "��";

int background[21][12];     // �ǂƌŒ�ς݃u���b�N
int view_data[21][12];      // ��ʃf�[�^�Bbackground[][]��w�i�Ƃ��Ă��̏��myblock.pattern[][]���d�˂�����

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

struct block myblock;


int total_point = 0; //���v���_
int add_points[5] = {0,100,300,500,1000}; //���������̉��Z�|�C���g


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

int main()
{
    int time = 0; //�^�C�}�����Z�b�g

    init();

    //�Q�[���I�[�o�[�ɂȂ�܂Ŗ������[�v
    while (1) {

        //�L�[���͂�����΃u���b�N�𓮂���
        if(kbhit()){
            block_operate();
        }

        if (time < INTERVAL) {
            time++;
        } else {
            //�u���b�N���P�}�X������
            if (!block_drop()) {
                //�Q�[���I�[�o�[�̏ꍇ
                break;
            }
            //�^�C�}�����Z�b�g
            time = 0;
        }
    }

    view_gameover();
    return 0;
}

//�Q�[���N������̏����ݒ�B
//��ʂƕǂ̃f�[�^��������
void init()
{
    int row, col;

    //��ʂƕǂ������ݒ�
    for(row = 0; row <= NUM_ROWS; row++) {
        for(col = 0; col <= NUM_COLS + 1; col++) {
            if((col == 0) || (col == NUM_COLS + 1) || (row == NUM_ROWS)) {
                background[row][col] = 2; //��
            } else {
                background[row][col] = 0; //�^��
            }
            
            view_data[row][col] = background[row][col];
        }
    }


    block_new(); //�ŏ��̃u���b�N�𐶐�
    view_render();
}

//�V�����u���b�N�𐶐����Ď��̃u���b�N�ɔ���������
//@return 0:�Q�[���I�[�o�[, 1:�Q�[������
int block_new()
{
    int r, c; //for���[�v����p�̕ϐ�

    
    //�܂��u���b�N�̍��W�������ʒu�Ƀ��Z�b�g
    myblock.pos.c = 4;
    myblock.pos.r = 0;

    //�u���b�N�̎�ށB�����_���Ɍ��܂�B
    int new_block_type = rand_block_type();
    //�u���b�N�p�^�[���̒�����block_type�ɉ�������ނ̃u���b�N��ǂݍ���
    copy_block(block_patterns[new_block_type], myblock.pattern);

    //�ǁ{�u���b�N���t�B�[���h��
    //TODO:�����̏������킩��ɂ����B�����I�łȂ��B
    int overlay;
    for(r = 0; r<4; r++) {
        for(c = 0 + myblock.pos.c ; c< 4 +  myblock.pos.c ; c++) {
            overlay = background[r][c] + myblock.pattern[r][c - myblock.pos.c];
            //�o�ꂵ���V�u���b�N�����ɌŒ�u���b�N�ɏd�Ȃ��Ă���΃Q�[���I�[�o�[
            //TODO:1���}�W�b�N�i���o�[�Ȃ̂łȂ�Ƃ��������B
            if (overlay > 1) {
                return 0;
            }
            view_data[r][c] = overlay;
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

//�����𔭐������A���̗�����7�Ŋ������]��i�O�`�U�܂Łj�Ńu���b�N�̎�ނ�����
int rand_block_type()
{
    srand((unsigned)time(NULL));
    return rand() % 7;
}

//�L�[���͂��󂯂ău���b�N�𑀍삷��(�ړ�/��])
void block_operate()
{
    char key = getch(); //�L�[����ꕶ������

    //�L�[�ɉ����Ċe�����փu���b�N���ړ�������A��]�������肷��
    switch(key) {

        case KEY_RIGHT: // move right
            if(!is_attached(+1, 0)) {
                block_move(+1, 0);
            }
            break;

        case KEY_LEFT: // move left
            if(!is_attached(-1, 0)) {
                block_move(-1, 0);
            }
            break;
        case KEY_DOWN: // move down
            if(!is_attached(0, +5)) {
                block_move(0, +5);
            }
            break;
        case KEY_ROTATE: // rotate
            block_rotate();

    }
}

//�d�Ȃ茟��
//�������̃u���b�N���ǂ�Œ�ς݃u���b�N�ɐڐG���Ă��Ȃ�������
//�u���b�N���������ʒu�ɁA�Œ�u���b�N�������͕ǂ����邩�ǂ���������
int is_attached(int dx, int dy)
{
    int r, c;

    for(r = 0; r<4; r++) {
        for(c = 0; c<4; c++) {
            if(myblock.pattern[r][c]) {
                if(background[myblock.pos.r + dy + r][myblock.pos.c + dx + c] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

//�������u���b�N���ړ�
void block_move(int dx, int dy)
{
    int r, c;

    //��x�u���b�N��������
    for(r = 0; r<4; r++) {
        for(c = 0; c<4; c++) {
            view_data[myblock.pos.r+r][myblock.pos.c+c] -= myblock.pattern[r][c];
        }
    }
    //�u���b�N�̍��W���X�V
    myblock.pos.c += dx;
    myblock.pos.r += dy;

    //�V�������W�Ƀu���b�N�����Ȃ���
    for(r = 0; r<4; r++) {
        for(c = 0; c<4; c++) {
            view_data[myblock.pos.r+r][myblock.pos.c+c] += myblock.pattern[r][c];
        }
    }

    view_render();
}

//�u���b�N����]����
int block_rotate()
{
    int i, j; //for���[�v����p
    int temp[4][4] = {0}; //�u���b�N���ꎞ�ۑ����邽�߂̔z��

    //�u���b�N����]����O��temp�ɃZ�[�u
    copy_block(myblock.pattern, temp);

    //�u���b�N����]
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            myblock.pattern[i][j] = temp[3-j][i];
        }
    }
    //�d�Ȃ��Ă�u���b�N���o�Ă��܂�����u���b�N����]�O�ɖ߂��Ē��~
    if(is_attached(0, 0)) {
        copy_block(temp, myblock.pattern);
        return 1;
    }

    //��U�t�B�[���h����u���b�N�����ĉ�]��̃u���b�N���ĕ\��
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[myblock.pos.r+i][myblock.pos.c+j] -= temp[i][j];
            view_data[myblock.pos.r+i][myblock.pos.c+j] += myblock.pattern[i][j];
        }
    }

    view_render();

    return 0;
}

//�u���b�N�𗎉�������B
//���ɐڒn�����ꍇ�͂��̈ʒu�ɌŒ�
int block_drop()
{
    //�d�Ȃ肠�邩����
    if(!is_attached(0, +1)) {
        // �Ȃ���Ή��Ɉړ�
        block_move(0, +1);
    } else {
        // ����Εǂɂ���
        int num_lines = block_lock();
        total_point += add_points[num_lines];
        
        //�V�u���b�N�𐶐�
        if (block_new()) {
            view_render();
        } else {
            //�������_�Őڒn������Q�[���I�[�o�[
            return 0;
        }
    }
    
    return 1;
}

void copy_whole(int src[21][12], int dst[21][12])
{
    int i,j;

    //�u���b�N��ǂɉ�����
    for(i = 0; i<NUM_ROWS + 1; i++) {
        for(j = 0; j<NUM_COLS + 2; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

//���n�����u���b�N���Œ肵�A����񂪂�����Ă邩�̔�����Ăяo��
int block_lock()
{
    int i, j, num_lines;

    //�u���b�N��w�i�ɍ��̂�����
    copy_whole(view_data, background);
    
    //����񂪂�����Ă邩���肵�ď�������֐����Ă�
    num_lines = check_and_delete();

    //�񊮐������̔w�i����ʃf�[�^�֔��f
    copy_whole(background, view_data);

    //���������C���̐�
    return num_lines;
}

//����i���������Ă��邩�����B
//������Ă���΂��̒i�������ď�̃u���b�N�Q�����낷
int check_and_delete()
{
    int row, col, k; //for���[�v����p
    int is_complete; //����񂪂�����Ă���΂P�A��ł����Ԃ�����ƂO�ɂȂ�
    int num_lines = 0; //�����ɏ��������C���̐�

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


        // �������
        for(col = 1; col< NUM_COLS + 1; col++) {
            background[row][col] = 0;
        }


        //�����������ɂ������Œ�u���b�N���̏������Ƃ���։��낷
        for(k = row; k>0; k--) {
            for(col = 1; col< NUM_COLS + 1; col++) {
                background[k][col] = background[k-1][col];
            }
        }
    }

    return num_lines;
}

//��ʕ\��
//view_data[][]�̓��e����ʂɏo�͂���
void view_render()
{
    int r, c;
    char *cell;
    
    char output[25*30] ="";
    char offset [] = "        ";

    //view_data����ʕ\��
    for(r = 0; r<NUM_ROWS + 1; r++) {
        strcat(output, offset);
        for(c = 0; c < NUM_COLS + 2; c++) {

            //switch�ł͂Ȃ��z��Œl�����肵�����B
            switch(view_data[r][c]) {
                case 0:
                    cell = (char *)TILE_SPACE;
                    break;
                case 2:
                    cell = (char *)TILE_WALL;
                    break;
                case 1:
                    cell = (char *) TILE_BLOCK;
                    break;
            };

            //1�Z���ǉ�
            strcat(output, cell);
        }
        strcat(output, "\n");
    }

    view_clear();
    printf("%s", output);

    printf("              Total Point : %d\n", total_point);
}


//�Q�[���I�[�o�[��ʂ�\������
void view_gameover()
{
    printf("\n              GAME OVER  \n");
}

//��ʂ�S����
void view_clear()
{
    system("cls");
}

 