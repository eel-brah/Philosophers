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

void	_dead(t_philo *pinfo)
{
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state != SMO_DEAD && pinfo->done_eating == 0)
	{
		printf("◦ %zu %zu died\n", get_ct(pinfo->sim->SIMstart), pinfo->num);
		pinfo->sim->state = SMO_DEAD;
		if (pinfo->sim->one_philo)
			pthread_mutex_unlock(&pinfo->forks.lfork);
	}
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	pthread_mutex_unlock(&pinfo->eating_check);
}

void	*monitor(void *args)
{
	t_philo	*pinfo;
	size_t	i;

	pinfo = (t_philo *)args;
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&pinfo[i].eating_check);
		if (!pinfo[i].eating && get_time() - pinfo[i].last_meal >= pinfo[i].sim->rotine.tdie)
		{
			_dead(&pinfo[i]);
			return (NULL);
		}
		pthread_mutex_unlock(&pinfo[i].eating_check);
		i++;
		if (i == pinfo[i - 1].sim->philos_num)
			i = 0;
	}
	return (NULL);
}

char	creat_monitor(pthread_t *monitor_id, void *args)
{
	int	t;

	t = pthread_create(monitor_id, NULL, monitor, args);
	if (t)
	{
		handle_errorEN(t, "pthread_create");
		((t_philo *)args)->sim->state = SMO_DEAD;
		return (1);
	}
	return (0);
}
