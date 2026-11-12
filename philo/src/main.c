/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:48:57 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 04:07:19 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->nb_philos)
	{
		pthread_mutex_destroy(&d->forks[i]);
		pthread_mutex_destroy(&d->philos[i].bouffe_mutex);
		i++;
	}
	pthread_mutex_destroy(&d->print_mutex);
	pthread_mutex_destroy(&d->state_mutex);
	free(d->forks);
	free(d->philos);
}

static void	start_simulation(t_data *d)
{
	int	i;
	int	created;

	d->start_time = get_time();
	created = 0;
	while (created < d->nb_philos)
	{
		d->philos[created].last_bouffe = d->start_time;
		if (pthread_create(&d->philos[created].thread, NULL,
				routine, &d->philos[created]))
		{
			set_over(d);
			break ;
		}
		created++;
	}
	if (created == d->nb_philos)
		monitor(d);
	i = 0;
	while (i < created)
	{
		pthread_join(d->philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
	{
		printf("Usage: ./philo nb die eat sleep [must_eat]\n");
		return (1);
	}
	if (pars_arg(ac, av, &data))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (init_data(&data))
	{
		printf("Error: init failed\n");
		return (1);
	}
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
