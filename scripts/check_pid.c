#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

static int chk_pid(void *arg)
{
	return (getpid() != *(int *)arg) ? 0 : 1;
}

int main(int argc, char **argv)
{
	int pid, kid, status;
	static char stk[2 << 12];

	pid = getpid();
	kid = clone(chk_pid, stk + sizeof(stk), CLONE_VM | SIGCHLD, &pid);
	if (kid < 0) {
		printf("n\n");
		return 1;
	}
	waitpid(kid, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		printf("n\n");
		return 1;
	}

	printf("y\n");
	return 0;
}
