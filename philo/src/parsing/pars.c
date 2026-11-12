/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 15:07:38 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/11 17:08:46 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	tkt_nombre_chef(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static	long	ft_atol(char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		res = res * 10 + (str[i] - '0');
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return (res);
}

int	pars_arg(int ac, char **av, t_data *data)
{
	int	i;

	if (ac > 6 || ac < 5)
		return (1);
	i = 1;
	while (i < ac)
	{
		if (!tkt_nombre_chef(av[i]) || ft_atol(av[i]) <= 0)
			return (1);
		i++;
	}
	data->nb_philos = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	data->must_eat = -1;
	if (ac == 6)
		data->must_eat = ft_atol(av[5]);
	return (0);
}
