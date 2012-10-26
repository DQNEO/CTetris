#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define KEY_LEFT  'j'
#define KEY_RIGHT 'k'
#define KEY_DOWN  ' '
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
void block_lock();
void check_and_delete();

void view_render();
void view_gameover(); 
void view_clear();

int rand_block_type();

//�O���[�o���ϐ�

const char *TILE_SPACE = "  ";
const char *TILE_WALL  = "��";
const char *TILE_BLOCK = "��";

int background[21][12];     // �ǂƌŒ�ς݃u���b�N�p
int block[4][4];            // ���ݗ������̃u���b�N
int view_data[21][12];      // �`�悷��f�[�^�Bbackground[][]��w�i�Ƃ��Ă��̏��block[][]���d�˂�����

int y; //�u���b�N��y���W
int x; //�u���b�N��x���W

int total_point = 0; //���v���_
int add_points[5] = {0,100,300,500,1000}; //���������̉��Z�|�C���g


//�V��ނ̃u���b�N�̃f�[�^
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
    for(row = 0; row <= 20; row++) {
        for(col = 0; col <= 11; col++) {
            if((col == 0) || (col == 11) || (row == 20)) {
                background[row][col] = 9; //��
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
    int i, j; //for���[�v����p�̕ϐ�

    //�u���b�N�̎�ށB�����_���Ɍ��܂�B
    int block_type = rand_block_type();
    
    //�܂��u���b�N�̍��W�������ʒu�Ƀ��Z�b�g
    y = 0;
    x = 4;


    //�u���b�N�f�[�^�̒�����block_type�ɉ�������ނ̃u���b�N��ǂݍ���
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            block[i][j] = 0;
            block[i][j] = block_list[block_type][i][j];
        }
    }
    //�ǁ{�u���b�N���t�B�[���h��
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[i][j+4] = background[i][j+4] + block[i][j];

            //�����ʒu�ɒu�����u���b�N�����ɌŒ�u���b�N�ɏd�Ȃ��Ă���΃Q�[���I�[�o�[
            if(view_data[i][j+4] > 1) {
                return 0;
            }

        }
    }

    return 1;
}

//�����𔭐������A���̗�����7�Ŋ������]��i�O�`�U�܂Łj�Ńu���b�N�̎�ނ�����
int rand_block_type()
{
    srand((unsigned)time(NULL));
    return rand() % 7;
}

//��ʕ\��
//view_data[][]�̓��e����ʂɏo�͂���
void view_render()
{
    int row, col;
    char output_string[25*30] ="";


    //view_data�̒��g(�ǁA�u���b�N�A�^��)����ʕ\��
    for(row = 0; row<21; row++) {
        strcat(output_string ,"   ");
        for(col = 0; col < 12; col++) {

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

    printf("        Total Point : %d\n", total_point);
}


//�Q�[���I�[�o�[��ʂ�\������
void view_gameover()
{
    printf("\n      GAME OVER  \n");
}

//��ʂ�S����
void view_clear()
{
    system("cls");
}

//�L�[���͂��󂯂ău���b�N�𑀍삷��(�ړ�/��])
void block_operate()
{
    char key; //�󂯕t�����L�[��ۑ�����ϐ�

    key = getch(); //�L�[����ꕶ������

    //�L�[�ɉ����Ċe�����փu���b�N���ړ�������A��]�������肷��
    switch(key) {

        case KEY_RIGHT: // move right
        case 'l': // move right
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
            if(!is_attached(x, y+5)) {
                block_move(x, y+5);
            }
            break;
        case KEY_ROTATE: // rotate
        case 'c': // rotate
            block_rotate();

    }
}

//�d�Ȃ茟��
//�������̃u���b�N���ǂ�Œ�ς݃u���b�N�ɐڐG���Ă��Ȃ�������
int is_attached(int x2, int y2)
{
    int i, j; //for���[�v����p�ϐ�

    //�u���b�N���������ʒu�ɁA�Œ�u���b�N�������͕ǂ����邩�ǂ���������
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

//�������u���b�N���ړ�
void block_move(int x2, int y2)
{
    int i, j; //for���[�v����p�ϐ�

    //���܂ł̃u���b�N��������
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[y+i][x+j] -= block[i][j];
        }
    }
    //�u���b�N�̍��W���X�V
    x = x2;
    y = y2;

    //�V�������W�Ƀu���b�N�����Ȃ���
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[y+i][x+j] += block[i][j];
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
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            temp[i][j] = block[i][j];
        }
    }

    //�u���b�N����]
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            block[i][j] = temp[3-j][i];
        }
    }
    //�d�Ȃ��Ă�u���b�N���o�Ă��܂�����u���b�N����]�O�ɖ߂��Ē��~
    if(is_attached(x, y)) {
        for(i = 0; i<4; i++) {
            for(j = 0; j<4; j++) {
                block[i][j] = temp[i][j];
            }
        }
        return 1;
    }

    //��U�t�B�[���h����u���b�N�����ĉ�]��̃u���b�N���ĕ\��
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[y+i][x+j] -= temp[i][j];
            view_data[y+i][x+j] += block[i][j];
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
    if(!is_attached(x, y+1)) {
        // �Ȃ���Έړ�
        block_move(x, y+1);
    } else {
        // ����Εǂɂ���
        block_lock();
        
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

//���n�����u���b�N���Œ肵�A����񂪂�����Ă邩�̔�����Ăяo��
void block_lock()
{
    int i, j; //for���[�v����p�ϐ�

    //�u���b�N��ǂɉ�����
    for(i = 0; i<21; i++) {
        for(j = 0; j<12; j++) {
            background[i][j] = view_data[i][j];
        }
    }

    check_and_delete(); //����񂪂�����Ă邩���肵�ď�������֐����Ă�

    //�񊮐������̕ǂ��t�B�[���h��
    for(i = 0; i<21; i++) {
        for(j = 0; j<12; j++) {
            view_data[i][j] = background[i][j];
        }
    }
}

//����i���������Ă��邩�����B
//������Ă���΂��̒i�������ď�̃u���b�N�Q�����낷
void check_and_delete()
{
    int i, j, k; //for���[�v����p
    int comp; //����񂪂�����Ă���΂P�A��ł����Ԃ�����ƂO�ɂȂ�
    int lines = 0; //�����ɏ��������C���̐�

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


        // �������
        for(j = 1; j<11; j++) {
            background[i][j] = 0;
        }


        //�����������ɂ������Œ�u���b�N���̏������Ƃ���։��낷
        for(k = i; k>0; k--) {
            for(j = 1; j<11; j++) {
                background[k][j] = background[k-1][j];
            }
        }
    }

    //�����ɏ��������C���̐����J�E���g
    total_point += add_points[lines];

}

 