/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:41:24 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/10 09:41:24 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*take_forks(t_philo *pinfo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie || pinfo->done_eating)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		return (NULL);
	}
	printf("◦ %zu %zu has taken a fork\n", get_ct(pinfo->sim->SIMstart), pinfo->num);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie || pinfo->done_eating)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		return (NULL);
	}
	printf("◦ %zu %zu has taken a fork\n", get_time() - pinfo->sim->SIMstart, pinfo->num);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	return ((void *)1);
}

int	is_dead(t_philo *pinfo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&pinfo->sim->dead_check);
	if (pinfo->sim->state == SMO_DEAD || !pinfo->sim->rotine.tdie || pinfo->done_eating)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		return (1);
	}
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	return (0);
}

void	*eating(t_philo *pinfo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (!take_forks(pinfo, first_fork, second_fork))
		return (NULL);
	if (pinfo->sim->rotine.teat > 0 && (pinfo->sim->is_meals_limited == 0
		|| (pinfo->sim->is_meals_limited && pinfo->sim->rotine.meals_num)))
	{
		if (is_dead(pinfo, first_fork, second_fork))
			return (NULL);
		pthread_mutex_lock(&pinfo->eating_check);
		pinfo->eating = 1;
		pinfo->eaten_meals++;
		if(pinfo->sim->is_meals_limited && pinfo->eaten_meals == pinfo->sim->rotine.meals_num)
			pinfo->done_eating = 1;
		printf("◦ %zu %zu is eating\n", get_ct(pinfo->sim->SIMstart), pinfo->num);
		pinfo->last_meal = get_time();
		pthread_mutex_unlock(&pinfo->eating_check);
		ft_usleep(pinfo->sim->rotine.teat);
		pthread_mutex_lock(&pinfo->eating_check);
		pinfo->eating = 0;
		pthread_mutex_unlock(&pinfo->eating_check);
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
		printf("◦ %zu %zu is sleeping\n", get_ct(pinfo->sim->SIMstart), pinfo->num);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
		ft_usleep(pinfo->sim->rotine.tslp);
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
	printf("◦ %zu %zu is thinking\n", get_ct(pinfo->sim->SIMstart), pinfo->num);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
	if (pinfo->sim->philos_num % 2 == 1 && pinfo->num % 2 == 1)
		ft_usleep(pinfo->sim->rotine.teat / 4);
	return ((void *)1);
}
