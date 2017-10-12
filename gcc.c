#define A 5
#include "gcc.h"
#include "xx.h"
#include <stdio.h>

int foo()
{
    int *p = 0;
    *p = 10;
    int a = A;
    return 0;
}

int main()
{
    int a=XXXX;

    foo();

    int c = a + 5;
    printf("%d\n", c);

    return 0;
}
