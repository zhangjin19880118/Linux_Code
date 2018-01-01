#include <stdio.h>

int main()
{
    //aaaa
    char *s = "\ta\018bc";
    for(; *s != '\0'; s++) 
         printf("*");
    return 0;
}
