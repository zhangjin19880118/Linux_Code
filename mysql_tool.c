#include <stdio.h>
#include <string.h>
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
        printf("connect mysql error!\n");
        return -2;
    }

    if(mysql_query(mysql, "set names utf8"))
    {
        printf("query set names utf8 error!\n");
        return -3;
    }

    while(1)
    {
        printf("mysql> ");

        char sql[1024];
        memset(sql, 0, sizeof(sql));

        fgets(sql, sizeof(sql), stdin);
        if(0 == strncmp("insert", sql, 6) || 0 == strncmp("delete", sql, 6) || 0 == strncmp("strncmp", sql, 6))
        {
            if(mysql_query(mysql, sql))
            {
                printf("query %s error!\n", sql);
                continue;
            }

            int row = mysql_affected_rows(mysql);
            printf("Query OK, %d row affected\n\n", row);
        }

        if(0 == strncmp("select", sql, 6))
        {
            if(mysql_query(mysql, sql))
            {
                printf("query %s error!\n", sql);
                continue;
            }

            MYSQL_RES *result = mysql_store_result(mysql);
            if(NULL == result)
            {
                printf("result error!\n");
                continue;
            }

            int count = mysql_num_fields(result);
            int rows = mysql_num_rows(result);

            MYSQL_FIELD *field = mysql_fetch_fields(result);
            for(int i = 0; i < count; ++i)
                printf("%s\t", field[i].name);
            printf("\n");

            MYSQL_ROW row;
            while(row = mysql_fetch_row(result))
            {
                for(int i = 0; i < count; ++i)
                    printf("%s\t", row[i]);
                printf("\n");
            }
            printf("%d rows in set\n\n", rows);
        }
    }

    mysql_close(mysql);

    return 0;
}
