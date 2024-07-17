// Your goal is to find all the files in a directory tree with a specific name
// Look at user/ls.c to see how to read directories.
// Use recursion to allow find to descend into sub-directories.
// Don't recurse into "." and "..".
// Changes to the file system persist across runs of qemu; 
// to get a clean file system run and then . make cleanmake qemu

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "stddef.h"

void find_files (const char* dir_path,const char* file_name){
    // 在指定目录下查找名称对应文件
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // 打开目录，并给文件描述符赋值
    if((fd = open(dir_path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir_path);
        return;
    }

    // 文件状态复制到st上
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", dir_path);
        close(fd);
        return;
    }

    // 不是目录
    if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", dir_path);
        close(fd);
        return;
    }

    // 如果路径过长放不入缓冲区，则报错提示
    if(strlen(dir_path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find: directory too long\n");
        close(fd);
        return;
    }

    // 将目录路径复制到 buf
    strcpy(buf, dir_path);
    // 加 "/" 前缀拼接在 buf 的后面
    p = buf + strlen(buf);
    *p++ = '/';

    // 读取 fd ，如果 read 返回字节数与 de 长度相等则循环
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
            continue;

        // 不要递归 "." 和 ".."
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        // 把文件名复制到p上
        memmove(p, de.name, DIRSIZ);
        // 设置文件名结束符
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        // 如果是目录类型，递归查找
        if (st.type == T_DIR)
        {
            find_files(buf, file_name);
        }
        // 如果是文件类型 并且 名称与要查找的文件名相同
        else if (st.type == T_FILE && !strcmp(de.name, file_name))
        {
            // 打印缓冲区存放的路径
            printf("%s\n", buf);
        } 
    }
}

int main(int argc, char *argv[])
{
    if(argc!=3){
        // 参数个数不正确
        printf("Usage:find direcroty filename\n");
        exit(1);
    }

    find_files(argv[1],argv[2]);

    exit(0);
}
