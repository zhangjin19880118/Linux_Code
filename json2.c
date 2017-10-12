#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

#if 0
{
    name:"tom",
    age:18,
    mobile:{
        branch:"apple",
        version:6
    }
}
#endif

int main()
{
    cJSON *root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "name", "tom");
    cJSON_AddNumberToObject(root, "age", 18);

    cJSON *item = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "mobile", item);
    cJSON_AddStringToObject(item, "branch", "apple");
    cJSON_AddNumberToObject(item, "version", 6);

    char *jsonBuf = cJSON_Print(root);

    //printf("jsonBuf is %s\n", jsonBuf);
    {
        cJSON *root = cJSON_Parse(jsonBuf);

        cJSON *name = cJSON_GetObjectItem(root, "name");
        printf("%s=%s\n", name->string, name->valuestring);

        cJSON *age = cJSON_GetObjectItem(root, "age");
        printf("%s=%d, %s=%g\n", age->string, age->valueint, age->string, age->valuedouble);

        cJSON_Delete(root);
    }

    free(jsonBuf);
    cJSON_Delete(root);

    return 0;
}
