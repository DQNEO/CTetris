#include <stdio.h>

int main()
{
    //�錾�Ɠ����ɏ�����
    int a[3] = {10,20,30};
 
    //�Y���ȗ�
    int b[] = {10,20,30};

    //�v�f������Ȃ��ꍇ��0���Z�b�g�����B
    int c[3] = {10};

    //��������ƑS�v�f��0�ɃZ�b�g�����B
    int d[3] = {};

    //�����������ɐ錾���������ꍇ�́A�ςȒl�������Ă���̂Ŋ댯�B
    int e[3];

    int i;
    for (i=0;i<3;i++) {
        printf("a[%d] = %02d  b[%d] = %02d  c[%d] = %02d ", i, a[i], i, b[i], i, c[i]);
        printf("d[%d] = %02d  e[%d] = %02d ", i, d[i], i, e[i]);
        printf("\n");
    }

    return 0;
}

