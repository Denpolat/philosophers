/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 01:44:15 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 04:34:27 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	think(t_philo *philo)
{
	long	t_think;

	print_state(philo, "is thinking");
	if (philo->data->nb_philos % 2 == 0)
		return ;
	t_think = philo->data->time_to_eat * 2 - philo->data->time_to_sleep;
	if (t_think < 0)
		t_think = 0;
	ft_usleep(t_think / 2, philo->data);
}

static void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->g_fork < philo->d_fork)
	{
		first = philo->g_fork;
		second = philo->d_fork;
	}
	else
	{
		first = philo->d_fork;
		second = philo->g_fork;
	}
	pthread_mutex_lock(first);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_state(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->bouffe_mutex);
	philo->last_bouffe = get_time();
	philo->bouffe_eaten++;
	pthread_mutex_unlock(&philo->bouffe_mutex);
	print_state(philo, "is eating");
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->g_fork);
	pthread_mutex_unlock(philo->d_fork);
}

void	*lone_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->g_fork);
	print_state(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die, philo->data);
	pthread_mutex_unlock(philo->g_fork);
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philos == 1)
		return (lone_philo(philo));
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2, philo->data);
	while (!is_over(philo->data))
	{
		take_forks(philo);
		eat(philo);
		print_state(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep, philo->data);
		think(philo);
	}
	return (NULL);
}
