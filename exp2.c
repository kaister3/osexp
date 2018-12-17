#include <pthread.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

static int sum = 0;//总和
static int mySemid;//信号灯的索引
static int index;//信号灯下标  

union senum{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void P(int semid, int index) {
    //
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;//获取一个资源
    sem.sem_flg = 0;//若资源数量不满足则阻塞
    semop(semid, &sem, 1);
    return;    
}

void V(int semid, int index) {
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;//释放一个资源
    sem.sem_flg = 0;//若资源数量不满足则阻塞
    semop(semid, &sem, 1);
    return;  
}

void *subp1() {
    //print
    for (int i = 1; i <= 100; i++) {
        P(mySemid, 0);
        printf("%d\n", sum);
        V(mySemid, 1);
    }
    exit(0);
}

void *subp2() {
    //add
    for (int i = 1; i <= 100; i++) {
        P(mySemid, 1);
        sum += i;
        V(mySemid, 0);
    }
    exit(0);
}

int main() {
    //main线程
    pthread_t t1, t2; 
    mySemid = semget(666, 2, IPC_CREAT | 0666);//创建信号灯

    union senum init_args1, init_args2;//用于初始化
    init_args1.val = 0;
    init_args2.val = 1;//0:加,1:打印
    //初始状态:可运算不可打印
    //init_args.array = init_array;
    semctl(mySemid, 0, SETVAL, init_args1);//初始化
    semctl(mySemid, 1, SETVAL, init_args2);

    pthread_create(&t1, NULL, subp1, NULL);//开辟线程
    pthread_create(&t2, NULL, subp2, NULL);
    pthread_join(t1, NULL);//等待线程结束
    pthread_join(t2, NULL);

    semctl(mySemid, 0, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}
