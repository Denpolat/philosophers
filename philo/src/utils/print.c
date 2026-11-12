/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 01:15:47 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 01:40:47 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->state_mutex);
	if (!philo->data->simu_over)
		printf("%ld %d %s\n",
			get_time() - philo->data->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->state_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
