#include <stdio.h>

int main()
{
    //éŒ¾‚Æ“¯‚É‰Šú‰»
    int a[3] = {10,20,30};
 
    //“YšÈ—ª
    int b[] = {10,20,30};

    int i;
    for (i=0;i<3;i++) {
        printf("a[%d] = %d  b[%d] = %d \n", i, a[i], i, b[i]);
    }

    return 0;
}

