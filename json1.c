#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

struct Person
{
    int age;
    char name[32];
    int sex;
};

#if 0
{
    age:18,
    name:"tom",
    man:ture
}
#endif

int main()
{
    struct Person p;
    p.age = 18;
    strcpy(p.name, "tom");
    p.sex = 1;

    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "age", p.age);
    cJSON_AddStringToObject(root, "name", p.name);
    cJSON_AddBoolToObject(root, "man", p.sex);

    char *jsonBuf = cJSON_Print(root);

    printf("jsonBuf is %s\n", jsonBuf);

    free(jsonBuf);
    cJSON_Delete(root);

    return 0;
}
