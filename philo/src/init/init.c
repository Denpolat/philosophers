/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:09:11 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 04:24:26 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexs_2(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		niq_forks(data, data->nb_philos);
		return (1);
	}
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		niq_forks(data, data->nb_philos);
		return (1);
	}
	return (0);
}


static int	init_mutexs(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			return (1);
		}
		i++;
	}
	return (init_mutexs_2(data));
}



int	init_data(t_data *data)
{
	data->simu_over = 0;
	if (init_mutexs(data))
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		niq_mutexs(data);
		return (1);
	}
	if (init_philo(data))
	{
		free(data->philos);
		niq_mutexs(data);
		return (1);
	}
	return (0);
}