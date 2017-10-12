#include <stdio.h>
#include <mysql/mysql.h>

int main()
{
    MYSQL *mysql = mysql_init(NULL);
    if(NULL == mysql)
    {
        printf("create mysql error!\n");
        return -1;
    }

    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "test", 3306, NULL, 0);
    if(NULL == mysql)
    {
        printf("connect error!\n");
        return -2;
    }

    if(mysql_query(mysql, "set names utf8"))
    {
        printf("query set names utf8!\n");
        return -3;
    }

    if(mysql_query(mysql, "select * from test1"))
    {
        printf("query select * from test1!\n");
        return -4;
    }

    MYSQL_RES *result = mysql_store_result(mysql);
    if(NULL == result)
    {
        printf("result error!\n");
        return -5;
    }

    int count = mysql_num_fields(result);

    MYSQL_FIELD *field;
    while((field = mysql_fetch_field(result)))
    {
        printf("%s\t", field->name);
    }
    printf("\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
       for(int i = 0; i < count; ++i)
       {
           printf("%s\t", row[i]);
       }
       printf("\n");
    }

    mysql_free_result(result);

    mysql_close(mysql);

    return 0;
}
