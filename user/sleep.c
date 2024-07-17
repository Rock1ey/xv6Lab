// sleep

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc <= 1){
    // 用户忘记输入参数，给出错误提示
    printf("error: loss of argument!\nUsage: sleep <seconds>\n");
    exit(1);
  } else if(argc>=3) {
    // 输入了两个或以上的参数
    printf("error: too many arguments!\nUsage: sleep <seconds>\n");
    exit(1);
  } else {
    // 参数个数正确
    int sleep_time=atoi(argv[1]); // 将参数字符串转化为数字

    if(sleep_time<=0){
      // 输入的秒数非法
      printf("Please enter a positive integer of seconds!\n");
      exit(1);
    } else {
      // printf("Sleeping for %d seconds...\n", seconds);
      sleep(sleep_time);
      // printf("Awake!\n");
    }
  }

  // 退出程序
  exit(0);
}


