// Your goal is to use pipe and fork to set up the pipeline. 
// The first process feeds the numbers 2 through 35 into the pipeline. 
// For each prime number, you will arrange to create one process that reads 
// from its left neighbor over a pipe and writes to its right neighbor over another pipe. 
// Since xv6 has limited number of file descriptors and processes, the first process can stop at 35.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "stddef.h"

#define READ_END 0
#define WRITE_END 1

#define MAX_NUM 35

void filter_process(int read_fd){
    // 定义变量获取管道中的数字
    int first,next;

    if(read(read_fd,&first,sizeof(int))){
        printf("prime %d\n",first); // 第一个一定是素数

        int num_pipe[2]; // 用于给下一子进程读取的管道
        pipe(num_pipe);

        int pid = fork(); // 创建子进程

        if(pid == 0) {
            // child process
            close(num_pipe[WRITE_END]); // 关闭写端
            filter_process(num_pipe[READ_END]); // 递归
            close(num_pipe[READ_END]);  // 关闭读端
        } else if (pid < 0) {
            printf("Error: process creation failed!");
            exit(1);
        } else {
            // parent process
            close(num_pipe[READ_END]);
            while (read(read_fd,&next,sizeof(int)))
            {
                // 如果不为第一个数的倍数，则写入右边管道中
                if(next%first!=0){
                    write(num_pipe[WRITE_END],&next,sizeof(int));
                }
            }
            close(num_pipe[WRITE_END]);

            wait(NULL); // 等待子进程打印完毕

            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    // 创建管道
    int num_pipe[2];
    pipe(num_pipe);

    int pid = fork(); // 创建子进程

    if(pid == 0) {
        // 第一个子进程
        close(num_pipe[WRITE_END]); // 关闭写端
        filter_process(num_pipe[READ_END]);
        close(num_pipe[READ_END]);  // 关闭读端
    } else if (pid < 0) {
        printf("Error: process creation failed!");
        exit(1);
    }

    close(num_pipe[READ_END]);

    for(int i=2;i<=MAX_NUM;i++){
        // 父进程将所有数写入管道中
        write(num_pipe[WRITE_END],&i,sizeof(int));
    }

    close(num_pipe[WRITE_END]);

    wait(NULL); // 等待子进程打印完毕

    exit(0);
}
