// 使用 pipe 创建管道。
// 使用 fork 创建一个子项。
// 使用 read 从管道中读取，使用 write 来写入管道。
// 使用 getpid 查找调用进程的进程 ID。
// 在 Makefile 中将程序添加到 UPROGS。
// xv6 上的用户程序具有一组有限的库 他们可以使用的功能。您可以在 user/user.h 中查看该列表;源（系统调用除外） 
// 位于 user/ulib.c、user/printf.c 中， 和 user/umalloc.c。

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "stddef.h"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    int child_to_parent[2]; // 子进程->父进程
    int parent_to_child[2]; // 父进程->子进程

    // 创建管道
    if(pipe(child_to_parent)==-1||pipe(parent_to_child)){
        printf("Error:Pipe creation failed!\n");
        exit(1);
    }

    int pid = fork(); // 创建子进程

    if(pid < 0) {
        // 子进程创建失败
        printf("Error:Child process creation failed!\n");
        exit(1);
    } else if (pid==0){
        // 子进程
        
        // 关闭父进程的写和子进程的读
        close(parent_to_child[WRITE_END]);
        close(child_to_parent[READ_END]);

        // 从父进程读取一个字节
        char byte;
        read(parent_to_child[READ_END],&byte,sizeof(byte));

        printf("%d: received ping\n",getpid());

        // 子进程读取字节
        write(child_to_parent[WRITE_END],&byte,sizeof(byte));

        // 关闭父进程的写和子进程的读
        close(parent_to_child[READ_END]);
        close(child_to_parent[WRITE_END]);
        exit(0);
    } else {
        // 父进程
        
        // 关闭父进程的读和子进程的写
        close(parent_to_child[READ_END]);
        close(child_to_parent[WRITE_END]);

        // 父进程写入一个字节
        char byte = 'B';
        write(parent_to_child[WRITE_END],&byte,sizeof(byte));

        // 等待子进程写回
        wait(NULL);

        // 子进程读取字节
        read(child_to_parent[READ_END],&byte,sizeof(byte));

        printf("%d: received pong\n",getpid());

        // 关闭父进程的写和子进程的读
        close(parent_to_child[WRITE_END]);
        close(child_to_parent[READ_END]);
        exit(0);
    }
    exit(0);
}
