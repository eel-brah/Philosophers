/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:52:22 by eel-brah          #+#    #+#             */
/*   Updated: 2024/02/03 05:01:21 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_error(char *str)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (0);
}

int	check_args(int ac, char **av)
{
	char	*pn;

	pn = *av;
	if (!(ac == 5 || ac == 6))
	{
		if (ac == 2 && !ft_strncmp(av[1], "--help", 7))
		{
			printf("Usage: %s number_of_phils time_to_die time_to_eat ti", pn);
			printf("me_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		}
		else
			printf("Wrong number of argiments\nFor help: %s --help\n", pn);
		return (0);
	}
	while (*++av)
	{
		if (!is_all_digits(*av))
		{
			printf("Invalid argiments\nFor help: %s --help\n", pn);
			return (0);
		}
	}
	return (1);
}

int	*get_args(int ac, char **av)
{
	int	*args;
	int	i;

	if (!check_args(ac, av))
		return (NULL);
	i = 0;
	args = malloc(sizeof(*args) * (ac - 1));
	if (!args)
	{
		handle_error("malloc");
		return (NULL);
	}
	while (i < ac - 1)
	{
		args[i] = ft_atoi(av[i + 1]);
		i++;
	}
	return (args);
}

int	main(int argc, char **argv)
{
	int	*args;
	int	i;

	args = get_args(argc, argv);
	if (!args)
		return (1);
	i = 0;
	while (i < argc - 1)
	{
		printf("%i\n", args[i++]);
	}
	free(args);
}
