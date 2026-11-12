/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:45:11 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 00:37:10 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].bouffe_eaten = 0;
		data->philos[i].last_bouffe = 0;
		data->philos[i].data = data;
		data->philos[i].g_fork = &data->forks[i];
		data->philos[i].d_fork = &data->forks[(i + 1) % data->nb_philos];
		if (pthread_mutex_init(&data->philos[i].bouffe_mutex, NULL))
		{
			niq_philo_mutexs(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}
