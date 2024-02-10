/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:41:08 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/10 09:41:08 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write (fd, &c, 1);
}

static void	ft_putnbr(unsigned int nb, int fd)
{
	if (nb < 10)
	{
		ft_putchar_fd (nb + 48, fd);
		return ;
	}
	ft_putnbr (nb / 10, fd);
	ft_putchar_fd (nb % 10 + 48, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nb;

	if (fd < 0)
		return ;
	nb = n;
	if (n < 0)
	{
		nb *= -1;
		ft_putchar_fd('-', fd);
	}
	ft_putnbr(nb, fd);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s && *s && fd >= 0)
		write(fd, s, ft_strlen(s));
}
