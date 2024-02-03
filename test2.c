#include <libc.h>
#include <pthread.h>
#include <ctype.h>

typedef struct s_thread
{
	pthread_t	tid;
	int			tnum;
	void		*arg;
}	t_thread;

size_t	ft_strlen(const char *s)
{
	char	*ptr;

	ptr = (char *)s;
	while (*ptr)
		ptr++;
	return (ptr - s);
}

void	ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write (fd, &c, 1);
}

static void	ft_putnbr(unsigned int nb, int fd)
{
	if (nb < 10)
	{
		ft_putchar_fd (nb + 48, fd);
		return ;
	}
	ft_putnbr (nb / 10, fd);
	ft_putchar_fd (nb % 10 + 48, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nb;

	if (fd < 0)
		return ;
	nb = n;
	if (n < 0)
	{
		nb *= -1;
		ft_putchar_fd('-', fd);
	}
	ft_putnbr(nb, fd);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s && *s && fd >= 0)
		write(fd, s, ft_strlen(s));
}

void	errorEXITnb(int s, char *str)
{
	ft_putstr_fd("Error ", 2);
	ft_putnbr_fd(s, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}

void	errorEXIT(char *str)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}

void	*thread_start(void *arg)
{
	t_thread	*tinfo;
	char		*str;

	tinfo = arg;
	printf("Thread %d: top of stack near %p; argv_string=%s\n",
			tinfo->tnum, (void *) &tinfo, tinfo->arg);

	str = strdup(tinfo->arg);
	if (str == NULL)
		errorEXIT("strdup");

	for (char *p = str; *p != '\0'; p++)
		*p = toupper(*p);
	// sleep(10);
	return str;
}

int main(int argc, char **argv)
{
	t_thread	*tinfo;
	int			threads_num;
	int			s;
	void		*res;

	threads_num = argc - 1;
	if (!threads_num)
		errorEXIT("args number");
	tinfo = malloc(sizeof(*tinfo) * threads_num);
	if (!tinfo)
		errorEXIT("malloc");
	
	for (int i = 0; i < threads_num; i++) {
		tinfo[i].tnum = i + 1;
		tinfo[i].arg = argv[i + 1];

		s = pthread_create(&tinfo[i].tid, NULL, &thread_start, &tinfo[i]);
		if (s != 0)
			errorEXITnb(s, "pthread_create");
	}
	// sleep(10);
	for (int i = 0; i < threads_num; i++)
	{
		s = pthread_join(tinfo[i].tid, &res);
		if (s != 0)
			errorEXITnb(s, "pthread_join");

		printf("Joined with thread %d; returned value was %s\n",
				tinfo[i].tnum, (char *) res);
		free(res);
	}

	free(tinfo);
	exit(EXIT_SUCCESS);
}