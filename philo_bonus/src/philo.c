/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:22 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/11 16:17:32 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	check_args(int ac, char **av)
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
		exit(1);
	}
	while (*++av)
	{
		if (!is_all_digits(*av))
		{
			printf("Invalid argiments\nFor help: %s --help\n", pn);
			exit(1);
		}
	}
}

void	philo_rotine(void *args)
{
	t_philo			*pinfo;
	pthread_t		monitor_id;

	pinfo = (t_philo *)args;
	pinfo->last_meal = get_time();
	// if (pinfo->num % 2 == 0)
	// {
	// 	printf("◦ %zu %zu is thinking\n", get_ct(pinfo->sim->start), pinfo->num);
	// 	ft_msleep(pinfo->sim->rotine.teat / 2);
	// }
	pthread_create(&monitor_id, NULL, is_dead, pinfo);
	pthread_detach(monitor_id);
	while (1)
	{
		eating(pinfo);
		sleeping(pinfo);
		thinking(pinfo);
	}
	exit(0);
}

// void	exting(t_philo *pinfo, size_t philos_num)
// {
// 	size_t	i;
// 	int		t;

// 	i = 0;
// 	while (i < philos_num)
// 	{
// 		t = pthread_join(pinfo[i].id, NULL);
// 		if (t)
// 			handle_errorEN(t, "pthread_join");
// 		i++;
// 	}
// 	i = 0;
// 	while (i < philos_num)
// 	{
// 		if (pthread_mutex_destroy(&pinfo[i].forks.lfork) 
// 			|| pthread_mutex_destroy(&pinfo[i].eating_check))
// 			handle_error("pthread_mutex_destroy");
// 		i++;
// 	}
// 	if (pthread_mutex_destroy(&pinfo->sim->dead_check))
// 		handle_error("pthread_mutex_destroy");
// }

int	main(int argc, char **argv)
{
	t_simulation	sim;
	t_philo			*pinfo;
	int				val;
	size_t			i;

	pinfo = NULL;
	check_args(argc, argv);
	init_rotine(argv, argc, &sim.philos_num, &sim.rotine);
	pinfo = init_pinfo(argc, pinfo, sim.philos_num, &sim);
	init_sems(pinfo);
	start_philos(pinfo, sim.philos_num, &sim);

	val = 0;
	i = 0;
	pid_t	r;
	size_t	num = 0;
	while (num < sim.philos_num)
	{
		r = waitpid(-1, &val, 0);
		if (r == -1)
		{
			handle_error("waitpid");
			i = 0;
			while (i < sim.philos_num)
				kill(pinfo[i++].id, 2);
			break ;
		}
		if ((val >> 8) == 1)
		{
			i = 0;
			while (i < sim.philos_num)
				kill(pinfo[i++].id, 2);
			break ;
		}
		num++;
	}
	sem_close(pinfo->sim->dead);
	sem_unlink(SEM_F002);
	sem_close(pinfo->sim->eating);
	sem_unlink(SEM_F003);
	sem_close(pinfo->sim->forks);
	sem_unlink(SEM_F001);
	free(pinfo);
	exit(0);
}
