# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <fcntl.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

#define SSM "/ssm"

int main()
{
	sem_t	*sem;
	pid_t	child1;
	pid_t	child2;
	pid_t	rt;
	int val;

	sem = sem_open(SSM, O_CREAT | O_EXCL, 666, 0);
	if (sem == SEM_FAILED)
	{
		perror("sem_open: ");
		exit(EXIT_FAILURE);
	}
	child1 = fork();
	if (child1 == -1)
	{
		perror("fork: ");
		exit(EXIT_FAILURE);
	}
	if (child1 == 0)
	{
		sem_wait(sem);
		printf("Child1 %i\n", getppid());
		exit(4);
	}
	child2 = fork();
	if (child2 == -1)
	{
		perror("fork: ");
		waitpid(child1, &val, 0);
		exit(EXIT_FAILURE);
	}
	if (child2 == 0)
	{
		sem_post(sem);
		printf("Child2 %i\n", getppid());
		exit(3);
	}
	if (waitpid(child1, &val, 0) == -1)
		exit(EXIT_FAILURE);
	if (waitpid(child2, &val, 0) == -1)
		exit(EXIT_FAILURE);
	printf("Prent: %i %i val %i\n", child1, child2, val);

	if (sem_close(sem) == -1)
		exit(EXIT_FAILURE);
	if (sem_unlink(SSM) == -1)
		exit(EXIT_FAILURE);
	printf("extting...\n");
	exit(EXIT_SUCCESS);
}