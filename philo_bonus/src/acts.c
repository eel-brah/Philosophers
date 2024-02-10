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

void	take_forks(t_philo *pinfo)
{
	sem_wait(pinfo->forks);
	if (pinfo->done_eating || !pinfo->sim->rotine.tdie)
	{
		if (pinfo->done_eating)
		{
			sem_post(pinfo->forks);
			exit(2);
		}
		exit(1);
	}
	sem_wait(pinfo->sim->dead);
	printf("◦ %zu %zu has taken a fork\n", get_ct(pinfo->sim->start), pinfo->num);
	sem_post(pinfo->sim->dead);
	sem_wait(pinfo->forks);
	// if (is_dead(pinfo) || !pinfo->sim->rotine.tdie)
	// 	exit(1);
	sem_wait(pinfo->sim->dead);
	printf("◦ %zu %zu has taken a fork\n", get_ct(pinfo->sim->start), pinfo->num);
	sem_post(pinfo->sim->dead);
}

void	eating(t_philo *pinfo)
{
	take_forks(pinfo);
	if (pinfo->sim->rotine.teat > 0 && (pinfo->sim->is_meals_limited == 0
		|| (pinfo->sim->is_meals_limited && pinfo->sim->rotine.meals_num)))
	{
		// sem_wait(pinfo->sim->eating);
		pinfo->eating = 1;
		sem_wait(pinfo->sim->dead);
		printf("◦ %zu %zu is eating\n", get_ct(pinfo->sim->start), pinfo->num);
		sem_post(pinfo->sim->dead);
		pinfo->eaten_meals++;
		if(pinfo->sim->is_meals_limited && pinfo->eaten_meals == pinfo->sim->rotine.meals_num)
			pinfo->done_eating = 1;
		// if (is_dead(pinfo) || !pinfo->sim->rotine.tdie)
		// 	exit(1);
		pinfo->last_meal = get_time();
		ft_usleep(pinfo->sim->rotine.teat);
		pinfo->eating = 0;
	}
	sem_post(pinfo->forks);
	sem_post(pinfo->forks);
}

// void	philo_sleep(size_t ms, t_philo *pinfo)
// {
// 	size_t	start;
// 	(void)pinfo;
// 	start = get_time();
// 	while (get_ct(start) < ms)
// 	{
// 		// is_dead(pinfo);
// 		usleep(200);
// 	}
// }

void	sleeping(t_philo *pinfo)
{
	if (pinfo->sim->rotine.tslp > 0)
	{
		// if (is_dead(pinfo) || !pinfo->sim->rotine.tdie)
		// 	exit(1);
		sem_wait(pinfo->sim->dead);
		printf("◦ %zu %zu is sleeping\n", get_ct(pinfo->sim->start), pinfo->num);
		sem_post(pinfo->sim->dead);
		ft_usleep(pinfo->sim->rotine.tslp);
	}
}

void	thinking(t_philo *pinfo)
{
	// if (is_dead(pinfo) || !pinfo->sim->rotine.tdie)
	// 	exit(1);
	sem_wait(pinfo->sim->dead);
	printf("◦ %zu %zu is thinking\n", get_ct(pinfo->sim->start), pinfo->num);
	sem_post(pinfo->sim->dead);
	// if (pinfo->sim->philos_num % 2 == 1 && p
}
