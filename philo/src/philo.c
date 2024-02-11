/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:22 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/11 20:52:38 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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

void	*philo_rotine(void *args)
{
	t_philo			*pinfo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	pinfo = (t_philo *)args;
	if (pinfo->num % 2 == 0)
	{
		printf("◦ %zu %zu is thinking\n",
			get_ct(pinfo->sim->start), pinfo->num);
		ft_msleep(pinfo->sim->rotine.teat / 2);
	}
	set_forks(pinfo, (void **)&first_fork, (void **)&second_fork);
	while (1)
	{
		if (!eating(pinfo, first_fork, second_fork))
			return (NULL);
		if (!sleeping(pinfo) || !thinking(pinfo))
			return (NULL);
	}
	return (NULL);
}

void	exting(t_philo *pinfo, size_t philos_num)
{
	size_t	i;
	int		t;

	i = 0;
	while (i < philos_num)
	{
		t = pthread_join(pinfo[i].id, NULL);
		if (t)
			handle_erroren(t, "pthread_join");
		i++;
	}
	i = 0;
	while (i < philos_num)
	{
		if (pthread_mutex_destroy(&pinfo[i].forks.lfork)
			|| pthread_mutex_destroy(&pinfo[i].eating_check))
			handle_error("pthread_mutex_destroy");
		i++;
	}
	if (pthread_mutex_destroy(&pinfo->sim->dead_check))
		handle_error("pthread_mutex_destroy");
	free(pinfo);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	t_philo			*pinfo;
	pthread_t		monitor_id;

	pinfo = NULL;
	if (!check_args(argc, argv))
		return (1);
	if (!init_rotine(argv, argc, &sim.philos_num, &sim.rotine))
		return (1);
	pinfo = init_pinfo(argc, pinfo, sim.philos_num, &sim);
	if (!pinfo)
		return (1);
	if (start_philos(pinfo, sim.philos_num, &sim)
		|| creat_monitor(&monitor_id, (void *)pinfo))
	{
		pthread_mutex_destroy(&sim.dead_check);
		free(pinfo);
		return (1);
	}
	pthread_join(monitor_id, NULL);
	exting(pinfo, sim.philos_num);
	return (0);
}
