#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct recvData
{
    char *buf;
    int size;
    int dataSize;
}recvData;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    recvData *data = (recvData *)userdata;

    size_t dataSize = nmemb * size;

    memcpy(data->buf + data->dataSize, ptr, dataSize);
    data->dataSize += dataSize;

    return dataSize;
}

int main()
{
    recvData data;
    data.buf = (char *)calloc(8192, sizeof(char));
    data.size = 8192 * sizeof(char);
    data.dataSize = 0;


    CURL *curl = curl_easy_init();
    if(NULL == curl)
    {
        printf("create curl error!\n");
        return -1;
    }


    CURLcode code;
    code = curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1/cgi-bin/a.out");
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_URL error!\n");
        return -2;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_WRITEFUNCTION error!\n");
        return -3;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_WRITEHEADER error!\n");
        return -4;
    }

    code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "username=aaa&&password=bbb");
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_POSTFIELDS error!\n");
        return -5;
    }

    code = curl_easy_setopt(curl, CURLOPT_POST, 1);
    if(code != CURLE_OK)
    {
        printf("setopt CURLOPT_POST error!\n");
        return -5;
    }

    code = curl_easy_perform(curl);
    if(code != CURLE_OK)
    {
        printf("perform error code = %d\n", (int)code);
        return -7;
    }
    
    data.buf[data.dataSize] = 0;
    printf("recv data is %s\n", data.buf);

    free(data.buf);

    curl_easy_cleanup(curl);
    
    return 0;
}
