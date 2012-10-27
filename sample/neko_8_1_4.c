#include <stdio.h>

int main()
{
    //éŒ¾‚Æ“¯‚É‰Šú‰»
    int a[4] = {10,20,30,40};
    int i;
    int *p;
    
    p = a;
    
    for (i=0;i<4;i++) {
        printf("&a[%d] = %p, a[%d] = %d, *(p + %d) = %d, %p\n"
                ,i,&a[i]  ,i,a[i]     , i,*(p+i), p+i
                );
        
    }
    
    printf("\n");
    printf("a = %p\n", a);
    printf("p = %p\n", p);
 
    return 0;
}

