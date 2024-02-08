#include "../include/philo.h"

void	_dead(t_philo *pinfo)
{
	printf("◦ %zu %zu died\n", get_crent_time(pinfo->sim->SIMstart), pinfo->num);
	pinfo->sim->state = SMO_DEAD;
	if (pinfo->sim->one_philo)
		pthread_mutex_unlock(&pinfo->forks.lfork);
	pthread_mutex_unlock(&pinfo->eating_check);
	pthread_mutex_unlock(&pinfo->sim->dead_check);
}

void	*monitor(void *args)
{
	t_philo	*pinfo;

	pinfo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(&pinfo->sim->dead_check);
		pthread_mutex_lock(&pinfo->eating_check);
		if (pinfo->sim->state != SMO_DEAD && pinfo->done_eating == 0
			&& !pinfo->eating && get_crent_time(pinfo->sim->SIMstart) - pinfo->last_meal >= pinfo->sim->rotine.tdie)
		{
			_dead(pinfo);
			return (NULL);
		}
		else if (pinfo->sim->state == SMO_DEAD || pinfo->done_eating)
		{
			pthread_mutex_unlock(&pinfo->eating_check);
			pthread_mutex_unlock(&pinfo->sim->dead_check);
			return (NULL);
		}
		pthread_mutex_unlock(&pinfo->eating_check);
		pthread_mutex_unlock(&pinfo->sim->dead_check);
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
