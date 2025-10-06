/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:13:03 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/06 14:13:33 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	assign_values_from_args(t_config *conf, int ac, char **av)
{
	long int	values[5];

	values[0] = ft_atol(av[1]);
	values[1] = ft_atol(av[2]);
	values[2] = ft_atol(av[3]);
	values[3] = ft_atol(av[4]);
	if (ac == 6)
		values[4] = ft_atol(av[5]);
	else
		values[4] = -1;
	if (values[0] <= 0 || values[1] <= 0 || values[2] <= 0 || values[3] <= 0
		|| (ac == 6 && values[4] <= 0))
	{
		printf("Error: All values must be positive integers.\n");
		return (1);
	}
	conf->philo_count = values[0];
	conf->t_die = values[1];
	conf->t_eat = values[2];
	conf->t_sleep = values[3];
	conf->times_to_eat = values[4];
	return (0);
}

static int	allocate_memory(t_config *conf)
{
	conf->philosophers = malloc(sizeof(t_philo) * conf->philo_count);
	conf->forks = malloc(sizeof(pthread_mutex_t) * conf->philo_count);
	if (!conf->philosophers || !conf->forks)
		return (1);
	return (0);
}

static void	init_philo_data(t_philo *p, t_config *conf, int i)
{
	p->conf = conf;
	p->id = i + 1;
	p->eat_count = 0;
	p->last_meal = get_time_ms();
	p->left_fork = &conf->forks[i];
	p->right_fork = &conf->forks[(i + 1) % conf->philo_count];
}

static void	init_mutexes(t_config *conf)
{
	int	i;

	pthread_mutex_init(&conf->write_lock, NULL);
	pthread_mutex_init(&conf->death_lock, NULL);
	i = -1;
	while (++i < conf->philo_count)
		pthread_mutex_init(&conf->forks[i], NULL);
}

int	init_all(t_config *conf, int ac, char **av)
{
	int	i;

	if (assign_values_from_args(conf, ac, av))
		return (1);
	conf->is_dead = 0;
	if (allocate_memory(conf))
		return (1);
	init_mutexes(conf);
	i = -1;
	while (++i < conf->philo_count)
		init_philo_data(&conf->philosophers[i], conf, i);
	return (0);
}
