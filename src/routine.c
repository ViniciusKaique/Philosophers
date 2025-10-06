/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:17:41 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/06 14:17:56 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	take_forks(t_philo *p)
{
	pthread_mutex_lock(p->left_fork);
	print_log(p, FORK_MSG);
	pthread_mutex_lock(p->right_fork);
	print_log(p, FORK_MSG);
}

static void	let_go_forks(t_philo *p)
{
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
}

static void	philo_eat(t_philo *p)
{
	take_forks(p);
	p->last_meal = get_time_ms();
	print_log(p, EAT_MSG);
	ft_usleep(p->conf->t_eat);
	p->eat_count++;
	let_go_forks(p);
}

static void	philo_sleep(t_philo *p)
{
	print_log(p, SLEEP_MSG);
	ft_usleep(p->conf->t_sleep);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 != 0)
		ft_usleep(1);
	while (1)
	{
		if (get_simulation_status(p))
			break ;
		philo_eat(p);
		if (get_simulation_status(p))
			break ;
		philo_sleep(p);
		if (get_simulation_status(p))
			break ;
		print_log(p, THINK_MSG);
	}
	return (NULL);
}
