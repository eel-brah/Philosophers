/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:41:00 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/10 09:41:00 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	handle_error(char *str)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

void	handle_errorEN(int s, char *str)
{
	ft_putstr_fd("Error ", 2);
	ft_putnbr_fd(s, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
		handle_error("gettimeofday");
	return (time.tv_sec * (size_t)1000 + time.tv_usec / (size_t)1000);
}

size_t	get_ct(size_t start)
{
	return (get_time() - start);
}

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

size_t	ft_atoz(const char *str)
{
	size_t	nb;

	nb = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		nb = nb * 10 + (*str++ - '0');
	return (nb);
}
