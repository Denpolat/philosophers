/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denpolat <denpolat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:48:25 by denpolat          #+#    #+#             */
/*   Updated: 2026/11/12 04:09:31 by denpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
 #define PHILO_H
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <limits.h>

typedef struct	s_data	t_data;
typedef struct	s_philo	t_philo;

typedef struct s_data
{
	int				nb_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat;// -1 si y a r bg
	long			start_time;
	int				simu_over;// 0 = continue 1 = fini
	pthread_mutex_t	*forks;//tab de N mutex
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex; //protege simu over
	t_philo			*philos;
}	t_data;

typedef struct	s_philo
{
	int				id;// de 1 a N
	int				bouffe_eaten;
	long			last_bouffe;//timestamp de la derniere graillance
	pthread_mutex_t	*g_fork;
	pthread_mutex_t	*d_fork;
	pthread_mutex_t	bouffe_mutex;// protege last et eaten
	pthread_t		thread;
	t_data			*data;
}	t_philo;

int	pars_arg(int ac, char **av, t_data *data);
int	init_data(t_data *data);
int	init_philo(t_data *data);

void	*routine(void *arg);
void	*lone_philo(t_philo *philo);
void	eat(t_philo *philo);



void	niq_forks(t_data *data, int count);
void	niq_mutexs(t_data *data);
void	niq_philo_mutexs(t_data *data, int count);

void	monitor(t_data *data);


long	get_time(void);
void	ft_usleep(long ms, t_data *data);
void	print_state(t_philo *philo, char *msg);
int	is_over(t_data *data);
void	set_over(t_data *data);

#endif