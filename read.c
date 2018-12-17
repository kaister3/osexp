#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64//单个缓冲区的大小

const key_t semkey = 0x2234;//?网上说必须是32位整数
key_t shmkey = 2;

typedef struct shareMemory{
    char data[10][BUF_SIZE];
    int length[10];
    int stop;
}shareMemory;

union senum{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void P(int semid, int index) {
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

int main() {
    //获取信号灯
    int mySemid = semget(semkey, 2, IPC_CREAT | 0666);
    //获取共享内存
    int myShmid = shmget(shmkey, sizeof(shareMemory), IPC_CREAT | 0666);
    shareMemory *sharebuf = (shareMemory *)shmat(myShmid, NULL, 0);
    FILE *fp1 = fopen("input.txt", "rb");
    if (fp1 == NULL){
        printf("f1 open error\n");
    }
    int len;
    int i = 0;
    while(sharebuf->stop != 1){
        P(mySemid, 0);
        len = fread(sharebuf->data[i], sizeof(char), BUF_SIZE, fp1);
        sharebuf->length[i] = len;
        printf("reading : buffer %d, length = %d\n", i, len);
        if(len < BUF_SIZE) {
            sharebuf->stop = 1;
        }
        i = (i + 1) % 10;
        V(mySemid, 1);
    }
    fclose(fp1);
    exit(0);
}