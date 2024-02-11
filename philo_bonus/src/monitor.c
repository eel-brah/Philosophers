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
		sem_wait(pinfo->sim->eating);
		if (pinfo->done_eating)
		{
			sem_post(pinfo->sim->eating);
			return (NULL);
		}
		if (!pinfo->done_eating && !pinfo->eating && get_time() - pinfo->last_meal >= pinfo->sim->rotine.tdie)
		{
			sem_wait(pinfo->sim->dead);
			printf("◦ %zu %zu died\n", get_ct(pinfo->sim->start), pinfo->num);
			if (pinfo->sim->rotine.tdie == 0)
				ft_msleep(1);
			exit(1);
		}
		sem_post(pinfo->sim->eating);
	}
	return (NULL);
}
