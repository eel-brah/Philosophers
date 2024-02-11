/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 20:26:03 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/11 20:50:19 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	fork_fail(t_philo *pinfo, size_t i)
{
	size_t	j;

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

int	start_philos(t_philo *pinfo, size_t philos_num, t_simulation *sim)
{
	size_t	i;

	i = 0;
	sim->start = get_time();
	while (i < philos_num)
	{
		pinfo[i].id = fork();
		if (pinfo[i].id == -1)
		{
			handle_error("fork");
			fork_fail(pinfo, i);
		}
		else if (pinfo[i].id == 0)
			philo_rotine(&pinfo[i]);
		i++;
	}
	return (0);
}

size_t	get_ct(size_t start)
{
	return (get_time() - start);
}
