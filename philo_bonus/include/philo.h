/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:42 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/11 20:49:21 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <semaphore.h>
# include <signal.h>

# define SEM_F001 "/sem_f001"
# define SEM_F002 "/sem_f002"
# define SEM_F003 "/sem_f003"
# define SEM_F004 "/sem_f004"

typedef enum e_state
{
	ALL_ALIVE,
	SMO_DEAD,
}	t_state;

typedef struct s_rotine
{
	size_t	tdie;
	size_t	teat;
	size_t	tslp;
	size_t	meals_num;
}	t_rotine;

typedef struct s_simulation
{
	t_rotine		rotine;
	t_state			state;
	sem_t			*dead;
	sem_t			*eating;
	sem_t			*forks;
	size_t			start;
	char			one_philo;
	char			is_meals_limited;
	size_t			philos_num;
}	t_simulation;

typedef struct s_philo
{
	t_simulation	*sim;
	pid_t			id;
	size_t			last_meal;
	size_t			eaten_meals;
	size_t			num;
	char			eating;
	char			done_eating;
}	t_philo;

size_t	ft_atoz(const char *str);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
int		is_all_digits(char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putnbr_fd(int n, int fd);
void	*is_dead(void *args);
void	take_forks(t_philo *pinfo);
void	eating(t_philo *pinfo);
void	sleeping(t_philo *pinfo);
void	thinking(t_philo *pinfo);
char	creat_monitor(pthread_t *monitor_id, void *args);
int		start_philos(t_philo *pinfo, size_t philos_num, t_simulation *sim);
t_philo	*init_pinfo(int ac, t_philo *pinfo,
			size_t philos_num, t_simulation *sim);
void	init_rotine(char **argv, int argc,
			size_t *philos_num, t_rotine *rotine);
void	handle_error(char *str);
void	handle_erroren(int s, char *str);
size_t	get_time(void);
size_t	get_ct(size_t start);
void	philo_rotine(void *args);
int		ft_isspace(char c);
void	ft_msleep(size_t milliseconds);
void	init_sems(t_philo *pinfo);
#endif