/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:00:18 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/11 21:00:30 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	create_threads_fail(t_philo *pinfo, size_t i)
{
	size_t	j;

	pthread_mutex_lock(&pinfo->sim->dead_check);
	pinfo->sim->state = SMO_DEAD;
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	j = 0;
	while (j < i)
		pthread_join(pinfo[j++].id, NULL);
	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&pinfo[j].forks.lfork);
		pthread_mutex_destroy(&pinfo[j++].eating_check);
	}
}

int	start_philos(t_philo *pinfo, size_t philos_num, t_simulation *sim)
{
	size_t	i;
	size_t	j;

	i = 0;
	sim->start = get_time();
	while (i < philos_num)
	{
		pinfo[i].last_meal = get_time();
		j = pthread_create(&pinfo[i].id, NULL, philo_rotine, &pinfo[i]);
		if (j)
		{
			handle_erroren(j, "pthread_create");
			create_threads_fail(pinfo, i);
			return (1);
		}
		i++;
	}
	return (0);
}
