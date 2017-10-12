#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "cJSON.h"

#if 0
{
    cmd:"login",
    username:"xxx",
    password:"yyy"
}

{
    cmd:"register",
    username:"xxx",
    password:"yyy"
}
#endif

int main()
{
    CURL *curl = curl_easy_init();
    if(NULL == curl)
    {
        printf("create curl error!\n");
        return -1;
    }

    cJSON *root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "cmd", "register");
    cJSON_AddStringToObject(root, "username", "xxx");
    cJSON_AddStringToObject(root, "password", "yyy");

    char *jsonBuf = cJSON_Print(root);
    printf("jsonBuf is %s\n", jsonBuf);

    cJSON_Delete(root);

    CURLcode code;
    code = curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8888");
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_URL error!\n");
        return -2;
    }

    code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBuf);
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_POSTFIELDS error!\n");
        return -3;
    }

    code = curl_easy_setopt(curl, CURLOPT_POST, 1);
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_POST error!\n");
        return -4;
    }
        
    code = curl_easy_perform(curl);
    if(code != CURLE_OK)
    {
        printf("perform error code = %d\n", (int)code);
        return -5;
    }

    free(jsonBuf);
    curl_easy_cleanup(curl);

    return 0;
}
