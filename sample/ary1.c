#include <stdio.h>

int showint(int x[][2]);

int main()
{
    int a[][2] = {
        1,2,
        3,4,
        5,6,
    };

    //アドレス渡しなので、破壊的に作用する
    showint(a);
    printf("a[0][0] = %d\n", a[0][0]);
}

//
int showint(int m[][2])
{
    int i,j;
    for(i=0;i<3;i++) {
        for(j=0;j<2;j++){
            printf("m[%d][%d] = %d\n", i,j,m[i][j]);
        }
    }
    
    m[0][0] = 9;
    return 0;
}
