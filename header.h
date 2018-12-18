#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUF_SIZE 128//单个缓冲区的大小
#define leng 10

const key_t shmkey = 0x2234;//?网上说必须是32位整数
key_t semkey = 1;

typedef struct ShareMemory{
    char data[leng][BUF_SIZE];
    int length[leng];//每个缓冲区读取的长度
    int stop[leng];
}ShareMemory;//共享内存

union senum{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/*P操作*/
void P(int semid, int index){
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = -1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
	return;
}

/*V操作*/
void V(int semid, int index){
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
	return;
}

#endif