/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:41:14 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/10 09:41:14 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*is_dead(void *args)
{
	t_philo			*pinfo;

	pinfo = args;
	while (1)
	{
		if (!pinfo->done_eating && !pinfo->eating && get_time() - pinfo->last_meal >= pinfo->sim->rotine.tdie)
		{
			// pinfo->done_eating == 0
			sem_wait(pinfo->sim->dead);
			printf("◦ %zu %zu died\n", get_ct(pinfo->sim->start), pinfo->num);
			exit(1);
			// if (pinfo->sim->one_philo)
				// pthread_mutex_unlock(&pinfo->forks.lfork);
		}
		else if (pinfo->done_eating)
			return (NULL);
	}
	return (NULL);
}

// char	creat_monitor(pthread_t *monitor_id, void *args)
// {
// 	int		t;
// 	size_t	j;
// 	t_philo	*pinfo;

// 	pinfo = (t_philo *)args;
// 	t = pthread_create(monitor_id, NULL, monitor, args);
// 	if (t)
// 	{
// 		handle_errorEN(t, "pthread_create");
// 		pthread_mutex_lock(&pinfo->sim->dead_check);
// 		pinfo->sim->state = SMO_DEAD;
// 		pthread_mutex_unlock(&pinfo->sim->dead_check);
// 		j = 0;
// 		while (j < pinfo->sim->philos_num)
// 			pthread_join(pinfo[j++].id, NULL);
// 		j = 0;
// 		while (j < pinfo->sim->philos_num)
// 		{
// 			pthread_mutex_destroy(&pinfo[j].forks.lfork);
// 			pthread_mutex_destroy(&pinfo[j++].eating_check);
// 		}
// 		return (1);
// 	}
// 	return (0);
// }
