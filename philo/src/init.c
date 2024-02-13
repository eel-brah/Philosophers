/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:41:19 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/10 09:41:19 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_rotine(char **argv, int argc, size_t *philos_num, t_rotine *rotine)
{
	*philos_num = ft_atoz(argv[1]);
	if (*philos_num == 0)
	{
		printf("🍝 Spaghetti has survived\n   Mission accomplished\n");
		return (0);
	}
	rotine->tdie = ft_atoz(argv[2]);
	rotine->teat = ft_atoz(argv[3]);
	rotine->tslp = ft_atoz(argv[4]);
	rotine->meals_num = 0;
	if (argc == 6)
		rotine->meals_num = ft_atoz(argv[5]);
	if ((argc == 6 && rotine->meals_num == 0)
		|| rotine->tslp == 0 || rotine->teat == 0)
	{
		printf("Invalid argiments\nFor help: %s --help\n", *argv);
		return (0);
	}
	return (1);
}

t_philo	*pre_init_pinfo(int ac, size_t philos_num, t_simulation *sim)
{
	t_philo	*pinfo;
	int		t;

	pinfo = pr_malloc(sizeof(*pinfo), philos_num);
	if (!pinfo)
	{
		handle_error("malloc");
		return (NULL);
	}
	memset(pinfo, 0, sizeof(*pinfo) * philos_num);
	t = pthread_mutex_init(&sim->dead_check, NULL);
	if (t)
	{
		handle_erroren(t, "pthread_mutex_init");
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

t_philo	*init_pinfo(int ac, t_philo *pinfo,
		size_t philos_num, t_simulation *sim)
{
	size_t	i;

	pinfo = pre_init_pinfo(ac, philos_num, sim);
	if (!pinfo)
		return (NULL);
	i = 0;
	while (i < philos_num)
	{
		pinfo[i].num = i + 1;
		pinfo[i].sim = sim;
		if (pthread_mutex_init(&pinfo[i].forks.lfork, NULL)
			|| pthread_mutex_init(&pinfo[i].eating_check, NULL))
		{
			handle_error("pthread_mutex_init");
			if (pthread_mutex_destroy(&sim->dead_check))
				handle_error("pthread_mutex_destroy");
			free(pinfo);
			return (NULL);
		}
		set_rfork(pinfo, i, philos_num);
		i++;
	}
	return (pinfo);
}

void	unlock_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}
