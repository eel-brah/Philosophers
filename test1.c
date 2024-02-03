#include <libc.h>
#include <pthread.h>

void	*fn(void *arg)
{
	printf("%s\n", arg);
	pthread_exit(arg);
	return arg;
}

void	errorEXIT(int s, char *str)
{
	printf("Error %i: %s\n", s, str); // stderr
	exit(EXIT_FAILURE);
}

int main()
{
	pthread_t tid;
	int s;
	void *r;
	printf("start\n");
	s = pthread_create(&tid, NULL, fn, "Hello");
	if (s)
		errorEXIT(s, "pthread_create");
	s = pthread_join(tid, &r);
	if (s)
		errorEXIT(s, "pthread_join");
	// sleep(2);
	printf("end\n");
	printf("%s", r);
}