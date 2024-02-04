/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:22 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/04 04:20:26 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_error(char *str)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

void	errorEXITnb(int s, char *str)
{
	ft_putstr_fd("Error ", 2);
	ft_putnbr_fd(s, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

int	check_args(int ac, char **av)
{
	char	*pn;

	pn = *av;
	if (!(ac == 5 || ac == 6))
	{
		if (ac == 2 && !ft_strncmp(av[1], "--help", 7))
		{
			printf("Usage: %s number_of_phils time_to_die time_to_eat ti", pn);
			printf("me_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		}
		else
			printf("Wrong number of argiments\nFor help: %s --help\n", pn);
		return (0);
	}
	while (*++av)
	{
		if (!is_all_digits(*av))
		{
			printf("Invalid argiments\nFor help: %s --help\n", pn);
			return (0);
		}
	}
	return (1);
}

int	*get_args(int ac, char **av)
{
	int	*args;
	int	i;

	if (!check_args(ac, av))
		return (NULL);
	i = 0;
	args = malloc(sizeof(*args) * (ac - 1));
	if (!args)
	{
		handle_error("malloc");
		return (NULL);
	}
	while (i < ac - 1)
	{
		args[i] = ft_atoi(av[i + 1]);
		i++;
	}
	return (args);
}

enum e_state
{ 
	P_ALIVE,
	P_DEAD,
	P_JOINED
};

typedef struct s_rotine
{
	int	tdie;
	int	teat;
	int	tslp;
	int meals_num;
}	t_rotine;

typedef struct s_philo
{
	pthread_t		pid;
	int				pnum;
	t_rotine		rotine;
	enum e_state	state;
	pthread_mutex_t lfork;
	pthread_mutex_t *rfork;
	size_t			start;
}	t_philo;

typedef struct s_state
{
	int	philos_alive;
	int	philos_dead;
}	t_state;

void	*philo_rotine(void *args)
{
	t_philo	*pinfo;
	struct timeval timestamp;

	pinfo = (t_philo *)args;
	gettimeofday(&timestamp, NULL);
	while (pinfo->state == P_ALIVE)
	{
		// pthread_mutex_lock(&pinfo->lfork);
		printf("◦ %zu %i is thinking\n", (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000) - pinfo->start, pinfo->pnum);
		usleep(pinfo->rotine.tslp);
		pinfo->state = P_DEAD;
		// printf("◦ %i %i\n", timestamp.tv_usec, pinfo->pnum);
	}
	return (NULL);
}

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

// time_to_die 
// time_to_eat 
// time_to_sleep

// 5 800 300 300

// When a philosopher has finished eating, they put their forks back on the table and
// start sleeping. Once awake, they start thinking again. The simulation stops when
// a philosopher dies of starvation.

// Check if a philo die by checking time_to_die is ended since the last meal of the beggining

// If a philosopher didn’t start eating time_to_die
// milliseconds since the beginning of their last meal or the beginning of the simulation, they die

int	main(int argc, char **argv)
{
	int			philos_num;
	t_philo		*pinfo;
	t_rotine	rotine;
	int			i;
	int			t;
	struct timeval timestamp;

	if (!check_args(argc, argv))
		return (1);
	philos_num = ft_atoi(argv[1]);
	rotine.tdie = ft_atoi(argv[2]);
	rotine.teat = ft_atoi(argv[3]);
	rotine.tslp = ft_atoi(argv[4]);
	if (argc == 6)
		rotine.meals_num = ft_atoi(argv[5]);
	else
		rotine.meals_num = -1;
	pinfo = malloc(sizeof(*pinfo) * philos_num);
	if (!pinfo)
	{
		handle_error("malloc");
		return (1);
	}
	i = 0;
	gettimeofday(&timestamp, NULL);
	size_t start = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
	printf("%lu\n", start);
	// exit(1);
	while (i < philos_num)
	{
		pinfo[i].pnum = i + 1;
		pinfo[i].rotine.tdie = rotine.tdie;
		pinfo[i].rotine.teat = rotine.teat;
		pinfo[i].rotine.tslp = rotine.tslp;
		pinfo[i].rotine.meals_num = rotine.meals_num;
		pinfo[i].state = P_ALIVE;
		pinfo[i].start = start;
		pthread_mutex_init(&pinfo[i].lfork, NULL);
		if (i == 0)
			pinfo[i].rfork = &pinfo[philos_num - 1].lfork;
		else
			pinfo[i].rfork = &pinfo[i - 1].lfork;
		t = pthread_create(&pinfo[i].pid, NULL, philo_rotine, &pinfo[i]);
		if (t != 0)
		{
			free(pinfo);
			errorEXITnb(t, "pthread_create");
			return (1);
		}
		i++;
	}
	i = 0;
	// while (i < philos_num)
	// {
	// 	t = pthread_join(pinfo[i].pid, NULL);
	// 	if (t != 0)
	// 	{
	// 		free(pinfo);
	// 		errorEXITnb(t, "pthread_join");
	// 		return (1);
	// 	}
	// 	printf("Joined with thread %d\n", pinfo[i].pnum);
	// 	i++;
	// }
	t_state state;
	state.philos_alive = philos_num;
	state.philos_dead = 0;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	while (state.philos_alive > 0) 
	{
		gettimeofday(&timestamp, NULL);

		for (i = 0; i < philos_num; i++) 
		{
			if (pinfo[i].state == P_DEAD)
			{
				t = pthread_join(pinfo[i].pid, NULL);
				state.philos_alive--;
				pinfo[i].state = P_JOINED;
				printf("◦ %zu %i died\n", (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000) - start, pinfo[i].pnum);
			}
		}
	}

	// destroy and free
}
