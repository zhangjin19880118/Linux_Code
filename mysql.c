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
    
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "test", 3306, NULL, CLIENT_MULTI_STATEMENTS);
    if(NULL == mysql)
    {
        printf("connect error!\n");
        return -2;
    }

    if(mysql_query(mysql, "set names utf8"))
    {
        printf("query error!\n");
        return -3;
    }

    //char *sql = "insert into test2 values(NULL, 8)";
    if(mysql_query(mysql, "insert into test1 values(4, 'zhaoliu', 4); insert into test1 values(5, 'zhuqi', 5)"))
    {
        printf("query error!\n");
        return -4;
    }

    mysql_close(mysql); 

    return 0;
}
