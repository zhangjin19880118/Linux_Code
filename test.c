#include <stdio.h>

int func(int x)
{
    int countx = 0;
    while(x)
    {
        countx++;
        x = x&(x-1);
    }
    return countx;
}

int main()
{
    printf("%d\n",func(9999));
    return 0;
}
