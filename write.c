#include "header.h"

int main() {
    //获取信号灯
    int mySemid = semget(semkey, 2, IPC_CREAT | 0666);
    //获取共享内存
    int myShmid = shmget(shmkey, sizeof(shareMemory), IPC_CREAT | 0666);
    shareMemory *sharebuf = (shareMemory *)shmat(myShmid, NULL, 0);
    FILE *fp2 = fopen("output.txt", "wb");
    if (fp2 == NULL) { 
        printf("f2 open error\n");
    }
    int len;
    int i = 0;
    while(sharebuf->stop != 1){
        P(mySemid, 1);
        fwrite(sharebuf->data[i], sizeof(char), BUF_SIZE, fp2);
        printf("writing : buffer %d, length = %d\n", i, len);
        i = (i + 1) % 10;
        V(mySemid, 0);
    }
    while(sharebuf->length[i] > 0) {
        P(mySemid, 1);
        len = fwrite(sharebuf->data[i], sizeof(char), sharebuf->length[i], fp2);
        printf("writing : buffer %d, length = %d\n", i, len);
        i = (i + 1) % 10;
        V(mySemid, 0);
    }
    fclose(fp2);
    exit(0);
}
