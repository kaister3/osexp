#include "header.h"

int main() {
    //主进程
    pid_t pid1, pid2;
    //创建信号灯
    int mySemid = semget(semkey, 2, IPC_CREAT | 0666);
    union senum init_args1, init_args2;//用于初始化
    init_args1.val = 10;//初始状态可读入缓冲区
    init_args2.val = 0;//初始状态不可写入文件
    //init_args.array = init_array;
    semctl(mySemid, 0, SETVAL, init_args1);
    semctl(mySemid, 1, SETVAL, init_args2);
    //创建共享内存
    int myShmid = shmget(shmkey, sizeof(shareMemory), IPC_CREAT | 0666);
    shareMemory *shmBuf = (shareMemory*)shmat(myShmid, NULL, 0);
    shmBuf->stop = 0;

    if((pid1=fork()) == 0) {
        //p1
        printf("read created, pid = %d\n", getpid());
        execv("read", NULL);
    }
    else if(pid1 == -1){
        printf("p1 error\n");
    }
    else if ((pid2=fork()) == 0) {
        //p2
        printf("write created, pid = %d\n", getpid());
        execv("write", NULL);
    }
    else if(pid2 == -1) {
        printf("p2 error\n");
    }
    //parent
    int a, b;
    wait(&a);
    wait(&b);
    semctl(mySemid, IPC_RMID, 0);
    shmctl(myShmid, IPC_RMID, 0);
    printf("transmission over! main pid = %d\n", getpid());
    exit(0);
}
