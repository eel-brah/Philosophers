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

void	init_rotine(char **argv, int argc, size_t *philos_num, t_rotine *rotine)
{
	*philos_num = ft_atoz(argv[1]);
	if (*philos_num == 0)
	{
		printf("🍝 Spaghetti has survived\n   Mission accomplished\n");
		exit(1);
	}
	rotine->tdie = ft_atoz(argv[2]);
	rotine->teat = ft_atoz(argv[3]);
	rotine->tslp = ft_atoz(argv[4]);
	rotine->meals_num = 0;
	if (argc == 6)
		rotine->meals_num = ft_atoz(argv[5]);
}

void	init_sems(t_philo *pinfo)
{
	sem_unlink(SEM_F002);
	sem_unlink(SEM_F001);
	sem_unlink(SEM_F003);
	pinfo->sim->dead = sem_open(SEM_F002, O_CREAT | O_EXCL, 0666, 1);
	if (pinfo->sim->dead == SEM_FAILED)
	{
		handle_error("sem_open");
		free(pinfo);
		exit(1);
	}
	pinfo->sim->eating = sem_open(SEM_F003, O_CREAT | O_EXCL, 0666, 1);
	if (pinfo->sim->eating == SEM_FAILED)
	{
		handle_error("sem_open");
		sem_close(pinfo->sim->dead);
		sem_unlink(SEM_F002);
		free(pinfo);
		exit(1);
	}
	pinfo->sim->forks = sem_open(SEM_F001, O_CREAT | O_EXCL, 0666, pinfo->sim->philos_num);
	if (pinfo->sim->forks == SEM_FAILED)
	{
		handle_error("sem_open");
		sem_close(pinfo->sim->dead);
		sem_unlink(SEM_F002);
		sem_close(pinfo->sim->eating);
		sem_unlink(SEM_F003);
		free(pinfo);
		exit(1);
	}
}

t_philo	*init_pinfo(int ac, t_philo *pinfo, size_t philos_num, t_simulation *sim)
{
	size_t	i;

	pinfo = malloc(sizeof(*pinfo) * philos_num);
	if (!pinfo)
	{
		handle_error("malloc");
		exit(1);
	}
	memset(pinfo, 0, sizeof(*pinfo) * philos_num);
	sim->state = ALL_ALIVE;
	sim->one_philo = 0;
	if (philos_num == 1)
		sim->one_philo = 1;
	sim->is_meals_limited = 0;
	if (ac == 6)
		sim->is_meals_limited = 1;
	i = 0;
	while (i < philos_num)
	{
		pinfo[i].num = i + 1;
		pinfo[i].sim = sim;
		i++;
	}
	return (pinfo);
}

int	start_philos(t_philo *pinfo, size_t philos_num, t_simulation *sim)
{
	size_t	i;
	size_t	j;

	i = 0;
	sim->start = get_time();
	while (i < philos_num)
	{
		// sem_wait(pinfo->sim->eating);
		pinfo[i].id = fork();
		if (pinfo[i].id == -1)
		{
			handle_error("fork");
			j = 0;
			while (j < i)
				kill(pinfo[j++].id, 2);
			sem_close(pinfo->sim->dead);
			sem_unlink(SEM_F002);
			sem_close(pinfo->sim->eating);
			sem_unlink(SEM_F003);
			sem_close(pinfo->sim->forks);
			sem_unlink(SEM_F001);
			free(pinfo);
			exit(1);
		}
		else if (pinfo[i].id == 0)
			philo_rotine(&pinfo[i]);
		// usleep(1000);
		i++;
	}
	return (0);
}
