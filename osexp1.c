#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid1, pid2;//创两个进程

void killProcess()
{
	kill(pid1, SIGUSR1);
	kill(pid2, SIGUSR2);
}

void pid1Killed() {
	printf("Child process 1 is killed by parent\n");
	exit(0);
}

void pid2Killed() {
	printf("Child process 2 is killed by parent\n");
	exit(0);
}

int main() {
	int quitPid;
	int pipefd[2];
	pipe(pipefd);
	char m[50];
	char msg[50];

	if ((pid1 = fork()) == -1);
	else if (pid1 == 0)
	{//p1进程
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1, pid1Killed);
		int x = 1;
		while(1) {
			lockf(pipefd[1], 1, 0);
			sprintf(m, "I send you %d times", x);
			write(pipefd[1], m, 50);
			x++;
			sleep(1);
			lockf(pipefd[1], 0, 0);
		}
	}
    else if((pid2 = fork()) == -1);
	else if (pid2 == 0)
	{//p2进程
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR2, pid2Killed);
		while(1) {
			lockf(pipefd[0], 1, 0);
			read(pipefd[0], msg, 50);
			printf("%s\n", msg);
			lockf(pipefd[0], 0, 0);
		}
	}
    else{
        //Parent进程
        signal(SIGINT, killProcess);
        int a, b;
        wait(&a);//不能用waitpid
        wait(&b);//如果参数是NULL就错了，我也不知道为啥
        close(pipefd[1]);//关闭管道
        close(pipefd[0]);
        printf("Parent process is killed\n");
        exit(0);
    }
}
