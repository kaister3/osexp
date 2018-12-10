#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

pid_t pid1, pid2;
int isEnd = 0;
int pf1 = 0;
int pf2 = 0;

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
	char m[20];
	char msg[20];

	while((pid1 = fork()) == -1);
	while((pid2 = fork()) == -1);

	if (pid1 == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1, pid1Killed);
		int x = 1;
		while(1) {
			sprintf(m, "I send you %d times", x);
			write(pipefd[1], m, sizeof(m));
			x++;
			sleep(1);
		}
	}
	else if (pid2 == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGINT , pid2Killed);
		while(1) {
			ssize_t len = read(pipefd[0], msg, sizeof(msg));
			if (len != 0)
			{
				msg[len] = '\0';
				printf("%s\n", msg);
			}
		}
	}
	else {
		//Parent
		signal(SIGINT, killProcess);
		waitpid(pid1, &quitPid, 0);
		waitpid(pid2, &quitPid, 0);
		close(pipefd[1]);
		close(pipefd[0]);
		printf("Parent process is killed\n");
		exit(0);
	}
}