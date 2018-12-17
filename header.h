#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024//单个缓冲区的大小

const key_t semkey = 0x2234;//?网上说必须是32位整数
key_t shmkey = 234;

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

#endif
