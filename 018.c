#include <stdio.h>

int main()
{
    char *s = "\ta\018bc";
    for(; *s != '\0'; s++) 
         printf("*");
    return 0;
}
