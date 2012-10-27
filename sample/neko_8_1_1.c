#include <stdio.h>

int main()
{
    //宣言と同時に初期化
    int a[3] = {10,20,30};
 
    //添字省略
    int b[] = {10,20,30};

    //要素が足りない場合は0がセットされる。
    int c[3] = {10};

    //こうすると全要素が0にセットされる。
    int d[3] = {};

    //初期化せずに宣言だけした場合は、変な値が入っているので危険。
    int e[3];

    int i;
    for (i=0;i<3;i++) {
        printf("a[%d] = %02d  b[%d] = %02d  c[%d] = %02d ", i, a[i], i, b[i], i, c[i]);
        printf("d[%d] = %02d  e[%d] = %02d ", i, d[i], i, e[i]);
        printf("\n");
    }

    return 0;
}

