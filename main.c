#include "header.h"

int main(){
	int shmid = shmget(shmkey, sizeof(ShareMemory), IPC_CREAT|0666);
	ShareMemory* buf = (ShareMemory*)shmat(shmid, NULL, 0);
	for (int i = 0; i < leng; ++i)
	{
		buf->stop[i] = 0;
	}

	int semid = semget(semkey, 2, IPC_CREAT|0666);

	union senum arg;
	arg.val = 10;
	semctl(semid, 0, SETVAL, arg);
	arg.val = 0;
	semctl(semid, 1, SETVAL, arg);

	pid_t p1, p2;

	if ((p1 = fork()) == -1){
		printf("p1 error\n");
	}
	else if(p1 == 0){
		//p1
		printf("read process created! id = %d\n", getpid());
		execv("read", NULL);
	}
	else if((p2 = fork()) == -1){
		printf("p2 error\n");
	}
	else if (p2 == 0){
		//p2
		printf("write process created! id = %d\n", getpid());
		execv("write", NULL);
	}
	else{
		//main
		wait(&p1);
		wait(&p2);
		semctl(semid, 1, IPC_RMID, 0);
		shmctl(shmid, IPC_RMID, 0);
	}
	return 0;
}