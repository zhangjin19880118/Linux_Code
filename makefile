all:a.out
a.out:mysql_tool.c
	gcc mysql_tool.c -lmysqlclient -std=c99	
