/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:17:17 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/06 14:17:31 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_simulation_status(t_philo *p)
{
	int	status;

	pthread_mutex_lock(&p->conf->death_lock);
	status = p->conf->is_dead;
	pthread_mutex_unlock(&p->conf->death_lock);
	return (status);
}

static int	check_death(t_config *conf)
{
	int		i;
	size_t	current_time;

	i = -1;
	while (++i < conf->philo_count)
	{
		if (get_simulation_status(&conf->philosophers[i]))
			return (1);
		current_time = get_time_ms();
		if ((current_time - conf->philosophers[i].last_meal) > conf->t_die)
		{
			print_log(&conf->philosophers[i], DIE_MSG);
			return (1);
		}
	}
	return (0);
}

static int	check_meals_completed(t_config *conf)
{
	int	i;
	int	meals_completed;

	if (conf->times_to_eat == -1)
		return (0);
	meals_completed = 0;
	i = -1;
	while (++i < conf->philo_count)
	{
		if (conf->philosophers[i].eat_count >= conf->times_to_eat)
			meals_completed++;
	}
	if (meals_completed == conf->philo_count)
	{
		print_log(&conf->philosophers[0], "All philosophers are full");
		return (1);
	}
	return (0);
}

void	wait_for_completion(t_config *conf)
{
	while (1)
	{
		if (check_death(conf))
			break ;
		if (check_meals_completed(conf))
			break ;
		ft_usleep(1);
	}
}
