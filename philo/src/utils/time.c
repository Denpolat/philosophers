/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 01:05:57 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 03:43:33 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long ms, t_data *data)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
	{
		if (is_over(data))
			break ;
		usleep(500);
	}
}

int	is_over(t_data *data)
{
	int	over;

	pthread_mutex_lock(&data->state_mutex);
	over = data->simu_over;
	pthread_mutex_unlock(&data->state_mutex);
	return (over);
}

void	set_over(t_data *data)
{
	pthread_mutex_lock(&data->state_mutex);
	data->simu_over = 1;
	pthread_mutex_unlock(&data->state_mutex);
}
