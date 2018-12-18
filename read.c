#include "header.h"

int main(){
	int semid;
	semid = semget(semkey, 2, IPC_CREAT|0666);

	int shmid;
	shmid = shmget(shmkey, sizeof(ShareMemory), IPC_CREAT|0666);
	ShareMemory* buf =(ShareMemory*)shmat(shmid, NULL, 0);

	FILE* fp = fopen("input", "rb");
	int i = 0;
	int len;
	while(buf->stop[i] != 1){
		P(semid, 0);
		len = fread(buf->data[i], sizeof(char), BUF_SIZE, fp);
		printf("read  to   buffer %d length = %d\n", i, len);
		buf->length[i] = len;
		i = (i + 1) % 10;
		if (len < BUF_SIZE)
		{
			buf->stop[i] = 1;
		}
		
		V(semid, 1);
	}
	fclose(fp);
	exit(0);
}
