#include "header.h"

/*int lengthOfFile(char *filename){
	struct stat statbuf;
	stat(filename, &statbuf);
	int len = statbuf.st_size;
	return len;
}*/

int main(){
	int semid;
	semid = semget(semkey, 2, IPC_CREAT|0666);

	int shmid;
	shmid = shmget(shmkey, sizeof(ShareMemory), IPC_CREAT|0666);
	ShareMemory* buf = (ShareMemory*)shmat(shmid, NULL, 0);


	/*int fileLength = 0;
	char *filePath = "input";
	fileLength = lengthOfFile(filePath);*/

	FILE* fp = fopen("output", "wb");
	int i = 0;
	int len;
	while(buf->stop[i] != 1){
		P(semid, 1);
		len = fwrite(buf->data[i], sizeof(char), buf->length[i], fp);
		printf("write from buffer %d length = %d\n", i, len);
		i = (i + 1) % 10;
		V(semid, 0);
	}

	/*while (buf->length[i] != 0)
	{
		fwrite(buf->data[i], sizeof(char), buf->length[i], fp);
		printf("write from buffer %d\n", i);
	}*/

	fclose(fp);
	exit(0);
}
