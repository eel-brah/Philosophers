/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:22 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/05 07:02:50 by eel-brah         ###   ########.fr       */
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

// int	*get_args(int ac, char **av)
// {
// 	int	*args;
// 	int	i;

// 	if (!check_args(ac, av))
// 		return (NULL);
// 	i = 0;
// 	args = malloc(sizeof(*args) * (ac - 1));
// 	if (!args)
// 	{
// 		handle_error("malloc");
// 		return (NULL);
// 	}
// 	while (i < ac - 1)
// 	{
// 		args[i] = ft_atoi(av[i + 1]);
// 		i++;
// 	}
// 	return (args);
// }

enum e_state
{ 
	P_ALIVE,
	P_DEAD,
	P_JOINED
};

typedef enum e_SIMstate
{ 
	ALL_ALIVE,
	SMO_DEAD,
} t_SIMstate;

typedef struct s_rotine
{
	int	tdie;
	int	teat;
	int	tslp;
	int meals_num;
}	t_rotine;

typedef struct s_philo
{
	pthread_t		id;
	int				num;
	t_rotine		*rotine;
	t_SIMstate		*state;
	pthread_mutex_t lfork;
	pthread_mutex_t *rfork;
	size_t			SIMstart;
	size_t			last_meal;
	char			eating;
	pthread_mutex_t *dead_check;
	char			done_eating;
}	t_philo;

typedef struct s_state
{
	int	philos_alive;
	int	philos_dead;
}	t_state;

typedef struct s_simulation
{
	t_SIMstate	state;
	t_philo		*philo;
}	t_simulation;

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * (size_t)1000 + time.tv_usec / (size_t)1000);
}

size_t	get_crent_time(size_t start)
{
	return (get_time() - start);
}

void	*monitor(void *args)
{
	t_philo	*pinfo;

	pinfo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(pinfo->dead_check);
		if (*(pinfo->state) != SMO_DEAD && !pinfo->eating && pinfo->done_eating == 0 && get_crent_time(pinfo->SIMstart) - pinfo->last_meal >= pinfo->rotine->tdie)
		{
			printf("◦ %zu %i died\n", get_crent_time(pinfo->SIMstart), pinfo->num);
			*(pinfo->state) = SMO_DEAD;
			// printf("%zu %d\n", get_crent_time(pinfo->SIMstart) - pinfo->last_meal , pinfo->rotine.tdie);
			return (NULL);
		}
		pthread_mutex_unlock(pinfo->dead_check);
	}
	return (NULL);
}

void	*philo_rotine(void *args)
{
	t_philo	*pinfo;

	pinfo = (t_philo *)args;
	int eaten_meals = 0;
	pthread_t	monitor_id;
	pthread_create(&monitor_id, NULL, monitor, args);
	size_t	time;
	while (*(pinfo->state) == ALL_ALIVE
		&& (pinfo->rotine->meals_num == -1 || eaten_meals < pinfo->rotine->meals_num))
	{
		if (*(pinfo->state) == SMO_DEAD)
			return (NULL);
		if (pinfo->num % 2 == 1)
		{
			pthread_mutex_lock(&pinfo->lfork);
			printf("◦ %zu %i has taken a fork\n", get_time() - pinfo->SIMstart, pinfo->num);
			if (*(pinfo->state) == SMO_DEAD)
				return (NULL);
			pthread_mutex_lock(pinfo->rfork);
			printf("◦ %zu %i has taken a fork\n", get_time() - pinfo->SIMstart, pinfo->num);
		}
		else
		{
			pthread_mutex_lock(pinfo->rfork);
			printf("◦ %zu %i has taken a fork\n", get_time() - pinfo->SIMstart, pinfo->num);
			if (*(pinfo->state) == SMO_DEAD)
				return (NULL);
			pthread_mutex_lock(&pinfo->lfork);
			printf("◦ %zu %i has taken a fork\n", get_time() - pinfo->SIMstart, pinfo->num);
		}
		pinfo->eating = 1;
		time = get_time() - pinfo->SIMstart;
		printf("◦ %zu %i is eating\n", time, pinfo->num);
		pinfo->last_meal =  time;
		usleep(pinfo->rotine->teat * 1000);
		eaten_meals++;
		if(pinfo->rotine->meals_num != -1 && eaten_meals == pinfo->rotine->meals_num)
			pinfo->done_eating = 1;
		pinfo->eating = 0;
		if (pinfo->num % 2 == 1)
		{
			pthread_mutex_unlock(&pinfo->lfork);
			pthread_mutex_unlock(pinfo->rfork);
		}
		else
		{
			pthread_mutex_unlock(pinfo->rfork);
			pthread_mutex_unlock(&pinfo->lfork);
		}
		if (*(pinfo->state) == SMO_DEAD)
			return (NULL);
		if (pinfo->rotine->tslp > 0)
		{
			printf("◦ %zu %i is sleeping\n", get_time() - pinfo->SIMstart, pinfo->num);
			usleep(pinfo->rotine->tslp * 1000);
		}
		if (*(pinfo->state) == SMO_DEAD)
				return (NULL);
		printf("◦ %zu %i is thinking\n", get_time() - pinfo->SIMstart, pinfo->num);
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

int	init_rotine(char **argv, int argc, size_t *philos_num, t_rotine *rotine)
{
	*philos_num = ft_atoz(argv[1]);
	if (philos_num == 0)
	{
		printf("🍝 Spaghetti has survived\n   Mession accomplished\n");
		return (0);
	}
	rotine->tdie = ft_atoz(argv[2]);
	if (rotine->tdie == 0) // ◦ 0 1 died
	{
		printf("All philosophers have died under mysterious circumstances\n");
		return (0);
	}
	rotine->teat = ft_atoz(argv[3]);
	if (rotine->teat == 0) // ◦ 0 1 died
	{
		printf("All philosophers starved to death\n");
		return (0);
	}
	rotine->tslp = ft_atoz(argv[4]); // 0
	if (argc == 6)
		rotine->meals_num = ft_atoz(argv[5]);
	else
		rotine->meals_num = -1;
	if (rotine->meals_num == 0) // ◦ 0 1 died
	{
		printf("All philosophers starved to death\n");
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	size_t			philos_num;
	t_philo			*pinfo;
	t_rotine		rotine;
	int				i;
	int				t;

	if (!check_args(argc, argv))
		return (1);
	if (!init_rotine(argv, argc, &philos_num, &rotine))
		return (0);
	pinfo = malloc(sizeof(*pinfo) * philos_num);
	if (!pinfo)
	{
		handle_error("malloc");
		return (1);
	}
	i = 0;
	size_t SIMstart = get_time();
	t_SIMstate	sstate = ALL_ALIVE;
	pthread_mutex_t dead_check;
	pthread_mutex_init(&dead_check, NULL);
	while (i < philos_num)
	{
		pinfo[i].num = i + 1;
		pinfo[i].rotine = &rotine;
		pinfo[i].state = &sstate;
		pinfo[i].SIMstart = SIMstart;
		pinfo[i].last_meal = get_crent_time(SIMstart);
		pinfo[i].eating = 0;
		pinfo[i].dead_check = &dead_check;
		pinfo[i].done_eating = 0;
		pthread_mutex_init(&pinfo[i].lfork, NULL);
		if (i == 0)
			pinfo[i].rfork = &pinfo[philos_num - 1].lfork;
		else
			pinfo[i].rfork = &pinfo[i - 1].lfork;
		t = pthread_create(&pinfo[i].id, NULL, philo_rotine, &pinfo[i]);
		if (t != 0)
		{
			free(pinfo);
			errorEXITnb(t, "pthread_create");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < philos_num)
	{
		t = pthread_join(pinfo[i].id, NULL);
		if (t != 0)
		{
			free(pinfo);
			errorEXITnb(t, "pthread_join");
			return (1);
		}
		// printf("Joined with thread %d\n", pinfo[i].num);
		i++;
	}
	// t_state state;
	// state.philos_alive = philos_num;
	// state.philos_dead = 0;
	// while (state.philos_alive > 0) 
	// {
	// 	for (i = 0; i < philos_num; i++) 
	// 	{
	// 		if (pinfo[i].state == P_DEAD)
	// 		{
	// 			t = pthread_join(pinfo[i].id, NULL);
	// 			state.philos_alive--;
	// 			pinfo[i].state = P_JOINED;
	// 			// printf("◦ %zu %i died\n", (time.tv_sec * 1000 + time.tv_usec / 1000) - start, pinfo[i].num);
	// 		}
	// 	}
	// }

	// destroy and free
}
