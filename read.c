#include "header.h"

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
