/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 03:45:18 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 03:52:22 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_data *data, int i)
{
	long	last;

	pthread_mutex_lock(&data->philos[i].bouffe_mutex);
	last = data->philos[i].last_bouffe;
	pthread_mutex_unlock(&data->philos[i].bouffe_mutex);
	if (get_time() - last < data->time_to_die)
		return (0);
	pthread_mutex_lock(&data->print_mutex);
	if (!is_over(data))
		printf("%ld %d died\n", get_time() - data->start_time,
			data->philos[i].id);
	set_over(data);
	pthread_mutex_unlock(&data->print_mutex);
	return (1);
}

static int	all_ate(t_data *data)
{
	int	i;
	int	count;

	if (data->must_eat == -1)
		return (0);
	i = 0;
	count = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].bouffe_mutex);
		if (data->philos[i].bouffe_eaten >= data->must_eat)
			count++;
		pthread_mutex_unlock(&data->philos[i].bouffe_mutex);
		i++;
	}
	if (count == data->nb_philos)
	{
		set_over(data);
		return (1);
	}
	return (0);
}

void	monitor(t_data *data)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			if (check_death(data, i))
				return ;
			i++;
		}
		if (all_ate(data))
			return ;
		usleep(500);
	}
}
