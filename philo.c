/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:22 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/06 18:47:23 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_error(char *str)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

void	handle_errorEN(int s, char *str)
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
	size_t	tdie;
	size_t	teat;
	size_t	tslp;
	size_t	meals_num;
}	t_rotine;

typedef struct s_simulation
{
	t_SIMstate		state;
	size_t			SIMstart;
	pthread_mutex_t	dead_check;
	t_rotine		rotine;
	char			one_philo;
	char			is_meals_limited;
}	t_simulation;

typedef struct s_forks
{
	pthread_mutex_t	lfork;
	pthread_mutex_t	*rfork;
}	t_forks;

typedef struct s_philo
{
	pthread_t		id;
	int				num;
	t_forks			forks;
	size_t			last_meal;
	char			eating;
	char			done_eating;
	size_t			eaten_meals;
	t_simulation	*sim;
}	t_philo;

typedef struct s_state
{
	int	philos_alive;
	int	philos_dead;
}	t_state;



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
	while (pinfo->sim->state != SMO_DEAD && pinfo->done_eating == 0)
	{
		pthread_mutex_lock(&pinfo->sim->dead_check);
		if (pinfo->sim->state != SMO_DEAD && pinfo->done_eating == 0
			&& !pinfo->eating && get_crent_time(pinfo->sim->SIMstart) - pinfo->last_meal >= pinfo->sim->rotine.tdie)
		{
			printf("◦ %zu %i died\n", get_crent_time(pinfo->sim->SIMstart), pinfo->num);
			pinfo->sim->state = SMO_DEAD;
			if (pinfo->sim->one_philo)
				pthread_mutex_unlock(&pinfo->forks.lfork);
			pthread_mutex_unlock(&pinfo->sim->dead_check);
			return (NULL);
		}
		pthread_mutex_unlock(&pinfo->sim->dead_check);
	}
	return (NULL);
}

void	set_forks(t_philo *pinfo, void **first_fork, void **second_fork)
{
	if (pinfo->num % 2 == 1)
		{
			*first_fork = &pinfo->forks.lfork;
			*second_fork = pinfo->forks.rfork;
		}
		else
		{
			*first_fork = pinfo->forks.rfork;
			*second_fork = &pinfo->forks.lfork;
		}
}

void	*take_forks(t_philo *pinfo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		return (NULL);
	}
	printf("◦ %zu %i has taken a fork\n", get_time() - pinfo->sim->SIMstart, pinfo->num);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		return (NULL);
	}
	printf("◦ %zu %i has taken a fork\n", get_time() - pinfo->sim->SIMstart, pinfo->num);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	return ((void *)1);
}

void	*eating(t_philo *pinfo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (!take_forks(pinfo, first_fork, second_fork))
		return (NULL);
	if (pinfo->sim->rotine.teat > 0 && (pinfo->sim->is_meals_limited == 0 || (pinfo->sim->is_meals_limited && pinfo->sim->rotine.meals_num)))
	{
		pthread_mutex_lock(&pinfo->sim->dead_check);
		if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie)
		{
			pthread_mutex_unlock(first_fork);
			pthread_mutex_unlock(second_fork);
			pthread_mutex_unlock(&pinfo->sim->dead_check);
			return (NULL);
		}
		pinfo->eating = 1;
		printf("◦ %zu %i is eating\n",  get_time() - pinfo->sim->SIMstart, pinfo->num);
		pinfo->last_meal = get_time() - pinfo->sim->SIMstart;
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		usleep(pinfo->sim->rotine.teat * 1000);
		pinfo->eaten_meals++;
		if(pinfo->sim->is_meals_limited && pinfo->eaten_meals == pinfo->sim->rotine.meals_num)
			pinfo->done_eating = 1;
		pinfo->eating = 0;
	}
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	return ((void *)1);
}

void	*sleeping(t_philo *pinfo)
{
	if (pinfo->sim->rotine.tslp > 0)
	{
		pthread_mutex_lock(&pinfo->sim->dead_check);
		if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie)
		{
			pthread_mutex_unlock(&pinfo->sim->dead_check);
			return (NULL);
		}
		printf("◦ %zu %i is sleeping\n", get_time() - pinfo->sim->SIMstart, pinfo->num);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		usleep(pinfo->sim->rotine.tslp * 1000);
	}
	return ((void *)1);
}

void	*thinking(t_philo *pinfo)
{
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie)
	{
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		return (NULL);
	}
	printf("◦ %zu %i is thinking\n", get_time() - pinfo->sim->SIMstart, pinfo->num);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	return ((void *)1);
}

void	*philo_rotine(void *args)
{
	t_philo		*pinfo;
	pthread_t	monitor_id;
	int			t;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	pinfo = (t_philo *)args;
	t = pthread_create(&monitor_id, NULL, monitor, args);
	if (t)
	{
		handle_errorEN(t, "pthread_create");
		pinfo->sim->state = SMO_DEAD;
		return (NULL);
	}
	while (pinfo->sim->state == ALL_ALIVE && (pinfo->sim->is_meals_limited == 0
			|| (pinfo->sim->rotine.meals_num == 0 || pinfo->eaten_meals < pinfo->sim->rotine.meals_num)))
	{
		set_forks(pinfo, (void **)&first_fork, (void **)&second_fork);
		if (!eating(pinfo, first_fork, second_fork))
			return (NULL);
		if (!sleeping(pinfo) || !thinking(pinfo))
			return (NULL);	
	}
	pthread_join(monitor_id, NULL);
	return (NULL);
}

int	init_rotine(char **argv, int argc, size_t *philos_num, t_rotine *rotine)
{
	*philos_num = ft_atoz(argv[1]);
	if (*philos_num == 0)
	{
		printf("🍝 Spaghetti has survived\n   Mession accomplished\n");
		return (0);
	}
	rotine->tdie = ft_atoz(argv[2]);
	// if (rotine->tdie == 0) // ◦ 0 1 died
	// {
	// 	printf("All philosophers have died under mysterious circumstances\n");
	// 	return (0);
	// }
	rotine->teat = ft_atoz(argv[3]);
	// if (rotine->teat == 0) // ◦ 0 1 died
	// {
	// 	printf("All philosophers starved to death\n");
	// 	return (0);
	// }
	rotine->tslp = ft_atoz(argv[4]); // 0
	rotine->meals_num = 0;
	if (argc == 6)
		rotine->meals_num = ft_atoz(argv[5]);
	return (1);
}
#include <string.h>
t_philo	*pre_init_pinfo(int ac, size_t philos_num, t_simulation *sim)
{
	t_philo	*pinfo;
	int		t;

	pinfo = malloc(sizeof(*pinfo) * philos_num);
	if (!pinfo)
	{
		handle_error("malloc");
		return (NULL);
	}
	memset(pinfo, 0, sizeof(*pinfo) * philos_num);
	t = pthread_mutex_init(&sim->dead_check, NULL);
	if (t)
	{
		handle_errorEN(t, "pthread_mutex_init");
		free(pinfo);
		return (NULL);
	}
	sim->state = ALL_ALIVE;
	sim->one_philo = 0;
	if (philos_num == 1)
		sim->one_philo = 1;
	sim->is_meals_limited = 0;
	if (ac == 6)
		sim->is_meals_limited = 1;
	return (pinfo);
}

void	set_rfork(t_philo *pinfo, int i, size_t philos_num)
{
	if (i == 0)
		pinfo[i].forks.rfork = &pinfo[philos_num - 1].forks.lfork;
	else
		pinfo[i].forks.rfork = &pinfo[i - 1].forks.lfork;
}

t_philo	*init_pinfo(int ac, t_philo *pinfo, size_t philos_num, t_simulation *sim)
{
	int		t;
	int		i;

	pinfo = pre_init_pinfo(ac, philos_num, sim);
	if (!pinfo)
		return (NULL);
	i = 0;
	while (i < philos_num)
	{
		pinfo[i].num = i + 1;
		pinfo[i].sim = sim;
		t = pthread_mutex_init(&pinfo[i].forks.lfork, NULL);
		if (t)
		{
			handle_errorEN(t, "pthread_mutex_init");
			pthread_mutex_destroy(&sim->dead_check);
			free(pinfo);
			return (NULL);
		}
		set_rfork(pinfo, i, philos_num);
		i++;
	}
	return (pinfo);
}

int	init_philos(t_philo *pinfo, size_t philos_num, t_simulation *sim)
{
	int	i;
	int	j;

	i = 0;
	sim->SIMstart = get_time();
	while (i < philos_num)
	{
		pinfo[i].last_meal = get_crent_time(sim->SIMstart);
		j = pthread_create(&pinfo[i].id, NULL, philo_rotine, &pinfo[i]);
		if (j)
		{
			handle_errorEN(j, "pthread_create");
			sim->state = SMO_DEAD;
			j = 0;
			while (j < i)
			{
				pthread_join(pinfo[j].id, NULL);
				pthread_mutex_destroy(&pinfo[j++].forks.lfork);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	size_t			philos_num;
	t_philo			*pinfo;
	int				i;
	int				t;

	if (!check_args(argc, argv))
		return (1);
	if (!init_rotine(argv, argc, &philos_num, &sim.rotine))
		return (0);
	pinfo = init_pinfo(argc, pinfo, philos_num, &sim);
	if (!pinfo)
		return (1);
	if (init_philos(pinfo, philos_num, &sim))
	{
		pthread_mutex_destroy(&sim.dead_check);
		free(pinfo);
		return (1);
	}
	i = 0;
	while (i < philos_num)
	{
		t = pthread_join(pinfo[i].id, NULL);
		if (t != 0)
		{
			free(pinfo);
			handle_errorEN(t, "pthread_join");
			return (1);
		}
		pthread_mutex_destroy(&pinfo[i].forks.lfork);
		i++;
	}
	pthread_mutex_destroy(&sim.dead_check);
	free(pinfo);
}
