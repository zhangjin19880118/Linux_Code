#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int findcard(char *card, char **randcard, int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(0 == strcmp(card, randcard[i]))
            return 0;
    }

    return -1;
}

int isStraight_flush(int a, int b, int c, int d, char **randcard, int n)
{
    if(a <= 4 && b <= 4 && c <= 4 && d <= 4)
        return -1;

    int flag = 0;
    if(a > 4)
    {
        if(0 == findcard("hongA", randcard, n))
        {
            if(0 == findcard("hong2", randcard, n) && 0 == findcard("hong3", randcard, n) && 0 == findcard("hong4", randcard, n) && 0 == findcard("hong5", randcard, n))
            {
                printf("hongA hong2 hong3 hong4 hong5\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei2", randcard, n))
        {
            if(0 == findcard("hei6", randcard, n) && 0 == findcard("hei3", randcard, n) && 0 == findcard("hei4", randcard, n) && 0 == findcard("hei5", randcard, n))
            {
                printf("hei2 hei3 hei4 hei5 hei6\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei3", randcard, n))
        {
            if(0 == findcard("hei6", randcard, n) && 0 == findcard("hei7", randcard, n) && 0 == findcard("hei4", randcard, n) && 0 == findcard("hei5", randcard, n))
            {
                printf("hei3 hei4 hei5 hei6 hei7\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei4", randcard, n))
        {
            if(0 == findcard("hei6", randcard, n) && 0 == findcard("hei7", randcard, n) && 0 == findcard("hei8", randcard, n) && 0 == findcard("hei5", randcard, n))
            {
                printf("hei4 hei5 hei6 hei7 hei8\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei5", randcard, n))
        {
            if(0 == findcard("hei6", randcard, n) && 0 == findcard("hei7", randcard, n) && 0 == findcard("hei8", randcard, n) && 0 == findcard("hei9", randcard, n))
            {
                printf("hei5 hei6 hei7 hei8 hei9\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei6", randcard, n))
        {
            if(0 == findcard("hei10", randcard, n) && 0 == findcard("hei7", randcard, n) && 0 == findcard("hei8", randcard, n) && 0 == findcard("hei9", randcard, n))
            {
                printf("hei6 hei7 hei8 hei9 hei10\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei7", randcard, n))
        {
            if(0 == findcard("hei10", randcard, n) && 0 == findcard("heiJ", randcard, n) && 0 == findcard("hei8", randcard, n) && 0 == findcard("hei9", randcard, n))
            {
                printf("hei7 hei8 hei9 hei10 heiJ\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei8", randcard, n))
        {
            if(0 == findcard("hei10", randcard, n) && 0 == findcard("heiJ", randcard, n) && 0 == findcard("heiQ", randcard, n) && 0 == findcard("hei9", randcard, n))
            {
                printf("hei8 hei9 hei10 heiJ heiQ\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei9", randcard, n))
        {
            if(0 == findcard("hei10", randcard, n) && 0 == findcard("heiJ", randcard, n) && 0 == findcard("heiQ", randcard, n) && 0 == findcard("heiK", randcard, n))
            {
                printf("hei9 hei10 heiJ heiQ heiK\n");
                flag = 1;
            }
        }
        if(0 == findcard("hei10", randcard, n))
        {
            if(0 == findcard("heiK", randcard, n) && 0 == findcard("heiJ", randcard, n) && 0 == findcard("heiQ", randcard, n) && 0 == findcard("heiA", randcard, n))
            {
                printf("hei10 heiJ heiQ heiK heiA\n");
                flag = 1;
            }
        }
    }

    if(b > 4)
    {
        if(0 == findcard("hongA", randcard, n))
        {
            if(0 == findcard("hong2", randcard, n) && 0 == findcard("hong3", randcard, n) && 0 == findcard("hong4", randcard, n) && 0 == findcard("hong5", randcard, n))
            {
                printf("hongA hong2 hong3 hong4 hong5\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong2", randcard, n))
        {
            if(0 == findcard("hong6", randcard, n) && 0 == findcard("hong3", randcard, n) && 0 == findcard("hong4", randcard, n) && 0 == findcard("hong5", randcard, n))
            {
                printf("hong2 hong3 hong4 hong5 hong6\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong3", randcard, n))
        {
            if(0 == findcard("hong6", randcard, n) && 0 == findcard("hong7", randcard, n) && 0 == findcard("hong4", randcard, n) && 0 == findcard("hong5", randcard, n))
            {
                printf("hong3 hong4 hong5 hong6 hong7\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong4", randcard, n))
        {
            if(0 == findcard("hong6", randcard, n) && 0 == findcard("hong7", randcard, n) && 0 == findcard("hong8", randcard, n) && 0 == findcard("hong5", randcard, n))
            {
                printf("hong4 hong5 hong6 hong7 hong8\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong5", randcard, n))
        {
            if(0 == findcard("hong6", randcard, n) && 0 == findcard("hong7", randcard, n) && 0 == findcard("hong8", randcard, n) && 0 == findcard("hong9", randcard, n))
            {
                printf("hong5 hong6 hong7 hong8 hong9\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong6", randcard, n))
        {
            if(0 == findcard("hong10", randcard, n) && 0 == findcard("hong7", randcard, n) && 0 == findcard("hong8", randcard, n) && 0 == findcard("hong9", randcard, n))
            {
                printf("hong6 hong7 hong8 hong9 hong10\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong7", randcard, n))
        {
            if(0 == findcard("hong10", randcard, n) && 0 == findcard("hongJ", randcard, n) && 0 == findcard("hong8", randcard, n) && 0 == findcard("hong9", randcard, n))
            {
                printf("hong7 hong8 hong9 hong10 hongJ\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong8", randcard, n))
        {
            if(0 == findcard("hong10", randcard, n) && 0 == findcard("hongJ", randcard, n) && 0 == findcard("hongQ", randcard, n) && 0 == findcard("hong9", randcard, n))
            {
                printf("hong8 hong9 hong10 hongJ hongQ\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong9", randcard, n))
        {
            if(0 == findcard("hong10", randcard, n) && 0 == findcard("hongJ", randcard, n) && 0 == findcard("hongQ", randcard, n) && 0 == findcard("hongK", randcard, n))
            {
                printf("hong9 hong10 hongJ hongQ hongK\n");
                flag = 1;
            }
        }
        if(0 == findcard("hong10", randcard, n))
        {
            if(0 == findcard("hongK", randcard, n) && 0 == findcard("hongJ", randcard, n) && 0 == findcard("hongQ", randcard, n) && 0 == findcard("hongA", randcard, n))
            {
                printf("hong10 hongJ hongQ hongK hongA\n");
                flag = 1;
            }
        }

    }

    if(c > 4)
    {
        if(0 == findcard("meiA", randcard, n))
        {
            if(0 == findcard("mei2", randcard, n) && 0 == findcard("mei3", randcard, n) && 0 == findcard("mei4", randcard, n) && 0 == findcard("mei5", randcard, n))
            {
                printf("meiA mei2 mei3 mei4 mei5\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei2", randcard, n))
        {
            if(0 == findcard("mei6", randcard, n) && 0 == findcard("mei3", randcard, n) && 0 == findcard("mei4", randcard, n) && 0 == findcard("mei5", randcard, n))
            {
                printf("mei2 mei3 mei4 mei5 mei6\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei3", randcard, n))
        {
            if(0 == findcard("mei6", randcard, n) && 0 == findcard("mei7", randcard, n) && 0 == findcard("mei4", randcard, n) && 0 == findcard("mei5", randcard, n))
            {
                printf("mei3 mei4 mei5 mei6 mei7\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei4", randcard, n))
        {
            if(0 == findcard("mei6", randcard, n) && 0 == findcard("mei7", randcard, n) && 0 == findcard("mei8", randcard, n) && 0 == findcard("mei5", randcard, n))
            {
                printf("mei4 mei5 mei6 mei7 mei8\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei5", randcard, n))
        {
            if(0 == findcard("mei6", randcard, n) && 0 == findcard("mei7", randcard, n) && 0 == findcard("mei8", randcard, n) && 0 == findcard("mei9", randcard, n))
            {
                printf("mei5 mei6 mei7 mei8 mei9\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei6", randcard, n))
        {
            if(0 == findcard("mei10", randcard, n) && 0 == findcard("mei7", randcard, n) && 0 == findcard("mei8", randcard, n) && 0 == findcard("mei9", randcard, n))
            {
                printf("mei6 mei7 mei8 mei9 mei10\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei7", randcard, n))
        {
            if(0 == findcard("mei10", randcard, n) && 0 == findcard("meiJ", randcard, n) && 0 == findcard("mei8", randcard, n) && 0 == findcard("mei9", randcard, n))
            {
                printf("mei7 mei8 mei9 mei10 meiJ\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei8", randcard, n))
        {
            if(0 == findcard("mei10", randcard, n) && 0 == findcard("meiJ", randcard, n) && 0 == findcard("meiQ", randcard, n) && 0 == findcard("mei9", randcard, n))
            {
                printf("mei8 mei9 mei10 meiJ meiQ\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei9", randcard, n))
        {
            if(0 == findcard("mei10", randcard, n) && 0 == findcard("meiJ", randcard, n) && 0 == findcard("meiQ", randcard, n) && 0 == findcard("meiK", randcard, n))
            {
                printf("mei9 mei10 meiJ meiQ meiK\n");
                flag = 1;
            }
        }
        if(0 == findcard("mei10", randcard, n))
        {
            if(0 == findcard("meiK", randcard, n) && 0 == findcard("meiJ", randcard, n) && 0 == findcard("meiQ", randcard, n) && 0 == findcard("meiA", randcard, n))
            {
                printf("mei10 meiJ meiQ meiK meiA\n");
                flag = 1;
            }
        }
    }

    if(d > 4)
    {
        if(0 == findcard("fangA", randcard, n))
        {
            if(0 == findcard("fang2", randcard, n) && 0 == findcard("fang3", randcard, n) && 0 == findcard("fang4", randcard, n) && 0 == findcard("fang5", randcard, n))
            {
                printf("fangA fang2 fang3 fang4 fang5\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang2", randcard, n))
        {
            if(0 == findcard("fang6", randcard, n) && 0 == findcard("fang3", randcard, n) && 0 == findcard("fang4", randcard, n) && 0 == findcard("fang5", randcard, n))
            {
                printf("fang2 fang3 fang4 fang5 fang6\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang3", randcard, n))
        {
            if(0 == findcard("fang6", randcard, n) && 0 == findcard("fang7", randcard, n) && 0 == findcard("fang4", randcard, n) && 0 == findcard("fang5", randcard, n))
            {
                printf("fang3 fang4 fang5 fang6 fang7\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang4", randcard, n))
        {
            if(0 == findcard("fang6", randcard, n) && 0 == findcard("fang7", randcard, n) && 0 == findcard("fang8", randcard, n) && 0 == findcard("fang5", randcard, n))
            {
                printf("fang4 fang5 fang6 fang7 fang8\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang5", randcard, n))
        {
            if(0 == findcard("fang6", randcard, n) && 0 == findcard("fang7", randcard, n) && 0 == findcard("fang8", randcard, n) && 0 == findcard("fang9", randcard, n))
            {
                printf("fang5 fang6 fang7 fang8 fang9\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang6", randcard, n))
        {
            if(0 == findcard("fang10", randcard, n) && 0 == findcard("fang7", randcard, n) && 0 == findcard("fang8", randcard, n) && 0 == findcard("fang9", randcard, n))
            {
                printf("fang6 fang7 fang8 fang9 fang10\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang7", randcard, n))
        {
            if(0 == findcard("fang10", randcard, n) && 0 == findcard("fangJ", randcard, n) && 0 == findcard("fang8", randcard, n) && 0 == findcard("fang9", randcard, n))
            {
                printf("fang7 fang8 fang9 fang10 fangJ\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang8", randcard, n))
        {
            if(0 == findcard("fang10", randcard, n) && 0 == findcard("fangJ", randcard, n) && 0 == findcard("fangQ", randcard, n) && 0 == findcard("fang9", randcard, n))
            {
                printf("fang8 fang9 fang10 fangJ fangQ\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang9", randcard, n))
        {
            if(0 == findcard("fang10", randcard, n) && 0 == findcard("fangJ", randcard, n) && 0 == findcard("fangQ", randcard, n) && 0 == findcard("fangK", randcard, n))
            {
                printf("fang9 fang10 fangJ fangQ fangK\n");
                flag = 1;
            }
        }
        if(0 == findcard("fang10", randcard, n))
        {
            if(0 == findcard("fangK", randcard, n) && 0 == findcard("fangJ", randcard, n) && 0 == findcard("fangQ", randcard, n) && 0 == findcard("fangA", randcard, n))
            {
                printf("fang10 fangJ fangQ fangK fangA\n");
                flag = 1;
            }
        }
    }
    
    if(1 == flag)
        return 0;

    return -1;
}

int main()
{
    char *color[4] = {"hei", "hong", "mei", "fang"};
    char *num[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    char *card[52];
    char *randcard[13];
    for(int i = 0; i < 52; ++i)
        card[i] = (char *)calloc(8, sizeof(char));

    for(int i = 0; i < 13; ++i)
        randcard[i] = (char *)calloc(8, sizeof(char));

    for(int i = 0; i < 4; ++i)
    {   
        for(int j = 0; j < 13; ++j)
        {
            sprintf(card[i * 13 + j], "%s%s", color[i], num[j]);
        }
    }

    for(int i = 0; i < 52; ++i)
    {
        if(0 == i % 13)
            printf("\n");
        printf("%s ", card[i]);
    }

    printf("\n");

    srand((unsigned int)time(NULL));
    for(int i = 0; i < 13; ++i)
    {
        int j = rand() % 52;
        while(0 == strcmp(card[j], "yiqu"))
            j = rand() % 52;
        strcpy(randcard[i], card[j]);
        strcpy(card[j], "yiqu");
    }

    for(int i = 0; i < 13; ++i)
        printf("%s ", randcard[i]);

    printf("\n");

    int a, b, c, d;
    a = b = c = d = 0;
    for(int i = 0; i < 13; ++i)
    {
        if(0 == strncmp("hei", randcard[i], 3))
            ++a;
        if(0 == strncmp("hong", randcard[i], 4))
            ++b;
        if(0 == strncmp("mei", randcard[i], 3))
            ++c;
        if(0 == strncmp("fang", randcard[i], 4))
            ++d;
    }   

    printf("hei = %d, hong = %d, mei = %d, fang = %d\n", a, b, c, d);

    if(-1 == isStraight_flush(a, b, c, d, randcard, 13))
        printf("no straight_flush!\n");

    return 0;
}
