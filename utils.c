/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 05:00:32 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/05 07:02:00 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0)
	{
		if (!*s1 || !*s2 || *(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
		n--;
	}
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	is_all_digits(char *s)
{
	int	j;

	j = 0;
	if (s[j] == '+' && s[j + 1])
		j++;
	while (s[j])
	{
		if (!ft_isdigit(s[j]))
			return (0);
		j++;
	}
	return (1);
}

static int	ft_isspace(char c)
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

size_t	ft_strlen(const char *s)
{
	char	*ptr;

	ptr = (char *)s;
	while (*ptr)
		ptr++;
	return (ptr - s);
}

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
