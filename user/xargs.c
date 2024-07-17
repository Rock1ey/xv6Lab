// read lines from the standard input and run a command for each line, supplying the line as arguments to the command.
// Use fork and exec to invoke the command on each line of input. Use wait in the parent to wait for the child to complete the command.
// To read individual lines of input, read a character at a time until a newline ('\n') appears.
// kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "stddef.h"

#define MAXN 1024

int main(int argc, char *argv[])
{
    // 如果参数个数小于 2
    if (argc < 2) {
        // 打印参数错误提示
        printf("usage: xargs command\n");
        exit(1);
    }

    // 存放子进程 exec 的参数
    char * argvs[MAXARG];
    // 索引
    int index = 0;
    // 从xargs后第一个开始，保存命令行参数
    for (int i = 1; i < argc; ++i) {
        argvs[index++] = argv[i];
    }
    
    char buf[MAXN] = {"\0"}; // 缓冲区存放从管道读出的数据
    char ch,*p=buf; // ch用于逐个读取字符，p为指向缓冲区的指针
    argvs[index]=p; // 当前index指向追加的参数

    while (read(0, &ch, sizeof(char))) {
        if (ch=='\n'){
            // 遇到还行符号，执行并清空缓冲区
            *p='\0';

            // 创建子进程
            int pid = fork();
            if(pid==0){
                exec(argvs[0],argvs);
                exit(0);
            } else {
                wait(NULL); // wait for child
                // 清空缓冲区中的参数，并将指针指回缓冲区起始位置
                strcpy(buf,"\0");
                p=buf;
                argvs[index]=p;
            }
        } else {
            // 参数赋值
            *p++=ch;
        }
    }
    // 正常退出
    exit(0);
}
