#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//�O���[�o���ϐ�
int stage[21][12] = {0}; //�ǂƌŒ�ς݃u���b�N�p
int block[4][4] =  {0}; //���ݗ������̃u���b�N������
int field[21][12] = {0}; //�`�悷��f�[�^�Bstage[][]��block[][]���d�˂�����

int y = 0; //�u���b�N�̉�ʏ�ł�y���W
int x = 4; //�u���b�N�̉�ʏ�ł�x���W
int oneline, twoline, threeline, fourline; //���������C���̐�
int gameover = 0; //�Q�[���I�[�o�[����B�V�����u���b�N�������ʒu�ɒu���Ȃ����1�ɂȂ�B

//�֐��v���g�^�C�v�錾
void Initialize(); //�Q�[���N������̏����ݒ���s���֐��B��ʂƕǂ̃f�[�^��������
int CreateBlock(); //�V�����u���b�N�𐶐����Ď��̃u���b�N�ɔ���������
void ShowGameField(); //field[][]�̒��g�ɉ����āA��ʂ�`�悷��
void ControlBlock(); //�L�[���͂ɉ����ău���b�N�Ɉړ����]���̏������s�킹��
int CheckOverlap(int, int); //�������̃u���b�N���ǂ�Œ�ς݃u���b�N�ɐڐG���Ă��Ȃ�������
void MoveBlock(int, int); //�������u���b�N����U�����āA�C�ӂ̍��W�Ɉړ�������
int TurnBlock(); //�u���b�N�̉�]����������
void DropBlock(); //�u���b�N�𗎉�������B���Ɉړ��ł��Ȃ��ꍇ�u���b�N�����̈ʒu�ɌŒ�
void LockBlock(); //���n�����u���b�N���Œ�ς݃u���b�N�ɉ�����֐�
void CheckLines(); //�u���b�N�������ɂ��낦�΂�����������A��̃u���b�N�������ɉ��낷

//�V��ނ̃u���b�N�̃f�[�^
int block_list[7][4][4] =

{
    {{0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0}},

    {{0,0,0,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,1,0,0}},

    {{0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}},

    {{0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}},

    {{0,0,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0}},

    {{0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}},

    {{0,0,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,0}}
};


int main()
{
    int time = 0; //���Ԃ��J�E���g

    Initialize(); //������

    //�Q�[���I�[�o�[�ɂȂ�܂ŃQ�[�����s
    while(!gameover){
        //�L�[���͂�����΂���ɉ����đ���
        if(kbhit()){
            ControlBlock();
        }
        //���Ԃ�������u���b�N���P�}�X����������B���Ԃ����ĂȂ���Ύ��Ԃ��P�����C���N�������g�B
        if(time < 20000){
            time++;
        } else {
            DropBlock();
            time = 0;
        }
    }
    return 0;
}

//�Q�[���̏�����
void Initialize()
{
int i, j; //for���[�v����p�ϐ�

//��ʂƕǂ������ݒ�
for(i = 0; i <= 20; i++) {
for(j = 0; j <= 11; j++) {
if((j == 0) || (j == 11) || (i == 20)) {
field[i][j] = stage[i][j] = 9;
}
else {
field[i][j] = stage[i][j] = 0;
}
}
}

CreateBlock(); //�ŏ��̃u���b�N����������
ShowGameField(); //�Q�[������̉�ʂ�`��
}

//�u���b�N�쐬
int CreateBlock()
{
int i, j; //for���[�v����p�̕ϐ�
int block_type; //�u���b�N�̎�ޗp�B�O�`�U�̗���������

//�܂��u���b�N�̍��W�������ʒu�Ƀ��Z�b�g
y = 0;
x = 4;

//�����𔭐������A���̗�����7�Ŋ������]��i�O�`�U�܂Łj�Ńu���b�N�̎�ނ�����
srand((unsigned)time(NULL));
block_type = rand() % 7;

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
field[i][j+4] = stage[i][j+4] + block[i][j];

//�����ʒu�ɒu�����u���b�N�����ɌŒ�u���b�N�ɏd�Ȃ��Ă���΃Q�[���I�[�o�[
if(field[i][j+4] > 1) {
gameover = 1;
return 1;
}

}
}
return 0;
}

//��ʕ\��
void ShowGameField()
{
int i, j; //for���[�v����p�ϐ�

//��ʂ���U�N���A
system("cls");

//�f�[�^�ɉ����ău���b�N��󔒂���ʕ\��
for(i = 0; i<21; i++) {
for(j = 0; j < 12; j++) {

switch(field[i][j]) {
case 0:
printf("");
break;
case 9:
printf("��");
break;
default:
printf("��");
break;
}
}
printf("\n");
}

//���_�\��
printf("\n�P�s�����F%d��Q�s�����F%d��R�s�����F%d��S�s�����F%d��\n", 
 oneline, twoline, threeline, fourline);

//�Q�[���I�[�o�[�̏ꍇ��GAME OVER��\��
if(gameover){
system("cls");
printf("\n\n\n\n\nGAME OVER\n\n");
}

}

//�L�[���͂ɉ����ău���b�N������
void ControlBlock()
{
    char key; //�󂯕t�����L�[��ۑ�����ϐ�

    key = getch(); //�L�[����ꕶ������

    //�L�[�ɉ����Ċe�����փu���b�N���ړ�������A��]�������肷��
    switch(key) {

        case 'l':
            if(!CheckOverlap(x+1, y)) {
                MoveBlock(x+1, y);
            }
            break;
        case 'j':
            if(!CheckOverlap(x-1, y)) {
                MoveBlock(x-1, y);
            }
            break;
        case ' ':
            if(!CheckOverlap(x, y+1)) {
                MoveBlock(x, y+1);
            }
            break;
        case 'k':
            TurnBlock();
    }
}

//�d�Ȃ茟��
int CheckOverlap(int x2, int y2)
{
int i, j; //for���[�v����p�ϐ�

//�u���b�N���������ʒu�ɁA�Œ�u���b�N�������͕ǂ����邩�ǂ���������
for(i = 0; i<4; i++) {
for(j = 0; j<4; j++) {
if(block[i][j]) {
if(stage[y2 + i][x2 + j] != 0) {
return 1;
}
}
}
}
return 0;
}

//�ړ�
void MoveBlock(int x2, int y2)
{
int i, j; //for���[�v����p�ϐ�

//���܂ł̃u���b�N��������
for(i = 0; i<4; i++) {
for(j = 0; j<4; j++) {
field[y+i][x+j] -= block[i][j];
}
}
//�u���b�N�̍��W���X�V
x = x2;
y = y2;

//�V�������W�Ƀu���b�N�����Ȃ���
for(i = 0; i<4; i++) {
for(j = 0; j<4; j++) {
field[y+i][x+j] += block[i][j];
}
}

ShowGameField();
}

//�u���b�N����]���鏈��
int TurnBlock()
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
if(CheckOverlap(x, y)) {
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
field[y+i][x+j] -= temp[i][j];
field[y+i][x+j] += block[i][j];
}
}

ShowGameField();

return 0;
}

//�u���b�N�𗎂Ƃ�
void DropBlock()
{
//�d�Ȃ肪�Ȃ���Έړ�
if(!CheckOverlap(x, y+1)) {
MoveBlock(x, y+1);
}
//�d�Ȃ肪����Εǂɂ���
else{
LockBlock();
CreateBlock();
ShowGameField();
}
}

//���n��̃u���b�N���Œ肵�A����񂪂�����Ă邩�̔�����Ăяo��
void LockBlock()
{
int i, j; //for���[�v����p�ϐ�

//�u���b�N��ǂɉ�����
for(i = 0; i<21; i++) {
for(j = 0; j<12; j++) {
stage[i][j] = field[i][j];
}
}

CheckLines(); //����񂪂�����Ă邩���肵�ď�������֐����Ă�

//�񊮐������̕ǂ��t�B�[���h��
for(i = 0; i<21; i++) {
for(j = 0; j<12; j++) {
field[i][j] = stage[i][j];
}
}
}

//����񂪊������Ă��邩�����B������Ă���΂����������ď�̃u���b�N�Q�����낷
void CheckLines()
{
int i, j, k; //for���[�v����p
int comp; //����񂪂�����Ă���΂P�A��ł����Ԃ�����ƂO�ɂȂ�
int lines = 0; //�����ɏ��������C���̐�

while(1) {
for(i = 0; i<20; i++) {
comp = 1;

for(j = 1; j<11; j++) {
if(stage[i][j] == 0) {
comp = 0;
}
}
if(comp == 1) break;
}
if(comp == 0) break;

lines++;


// �������
for(j = 1; j<11; j++) {
stage[i][j] = 0;
}


//�����������ɂ������Œ�u���b�N���̏������Ƃ���։��낷
for(k = i; k>0; k--) {
for(j = 1; j<11; j++) {
stage[k][j] = stage[k-1][j];
}
}
}

//�����ɏ��������C���̐����J�E���g
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

 