#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int count=0;

void reg_count(const char *path)
{
    DIR *dir=opendir(path);

    if(NULL==dir)
        return;

    while(1)
    {
        struct dirent *entry= readdir(dir);

        if(NULL==entry)
            break;

        if('.'==entry->d_name[0])
            continue;

        if(DT_REG==entry->d_type)
        {
            ++count;
            continue;
        } 

        if(DT_DIR==entry->d_type)
        {
            char buf[1024];
            sprintf(buf,"%s/%s",path,entry->d_name);
            reg_count(buf);
        }
    }

    closedir(dir);
}

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("arg error!\n");
        return -1;
    }

    const char *src_filename=argv[1];

    struct stat st;
    stat(src_filename,&st);

    if(!S_ISDIR(st.st_mode))
    {
        printf("src_filename is not a dir\n");
        return -2;
    }

    reg_count(src_filename);

    printf("regular count is %d\n",count);

    return 0;
}
