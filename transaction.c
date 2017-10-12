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
        printf("query set names utf8 error!\n");
        return -3;
    }

    if(mysql_query(mysql, "start transaction"))
    {
        printf("query start transaction error!\n");
        return -4;
    }

    int ret1 = mysql_query(mysql, "insert into test1 values(6, 'liuliu', 6)");
    int ret2 = mysql_query(mysql, "insert into test1 values(7, 'qiqi', 7)");
    if(ret1 || ret2)
    {
        if(mysql_query(mysql, "rollback"))
        {
            printf("query rollback error!\n");
            return -5;
        }
    }

    if(mysql_query(mysql, "commit"))
    {
        printf("query commit error!\n");
        return -6;
    }

    mysql_close(mysql);

    return 0;
}
