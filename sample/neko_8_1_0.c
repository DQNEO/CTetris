#include <stdio.h>

int main()
{
    int a[3];
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    
    int i;
    for (i=0;i<3;i++) printf("a[%d] = %d\n", i,a[i]);
    return 0;
}
