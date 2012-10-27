#include <stdio.h>

int ary_copy(int x[][2], int y[][2]);

int main()
{
    int a[][2] = {
        1,2,
        3,4,
        5,6,
    };

    int b[3][2];

    printf("b[1][1] = %d\n", b[1][1]);

    ary_copy(a,b);

    printf("a[1][1] = %d\n", a[1][1]);
    printf("b[1][1] = %d\n", b[1][1]);
}

//
int ary_copy(int x[][2], int y[][2])
{
    int i,j;
    for(i=0;i<3;i++) {
        for(j=0;j<2;j++){
            printf("x[%d][%d] = %d\n", i,j,x[i][j]);
            y[i][j] = x[i][j];
        }
    }
    
    return 0;
}

