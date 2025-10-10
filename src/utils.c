/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:18:03 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/10 12:24:09 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	ft_atol(const char *str)
{
	long int	res;
	int			sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

size_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(size_t time)
{
	size_t	start;

	start = get_time_ms();
	while (get_time_ms() - start < time)
		usleep(500);
}

void	free_all(t_config *conf)
{
	int	i;

	if (conf->forks)
	{
		i = -1;
		while (++i < conf->philo_count)
			pthread_mutex_destroy(&conf->forks[i]);
	}
	pthread_mutex_destroy(&conf->write_lock);
	pthread_mutex_destroy(&conf->death_lock);
	if (conf->philosophers)
		free(conf->philosophers);
	if (conf->forks)
		free(conf->forks);
}

void	print_log(t_philo *p, char *msg)
{
	const char	*die_msg;
	int			i;

	pthread_mutex_lock(&p->conf->write_lock);
	pthread_mutex_lock(&p->conf->death_lock);
	if (!p->conf->is_dead)
	{
		printf("%lu %d %s\n", get_time_ms() - p->conf->start_time, p->id, msg);
		die_msg = DIE_MSG;
		i = 0;
		while (die_msg[i] && msg[i] && die_msg[i] == msg[i])
			i++;
		if (die_msg[i] == '\0' && msg[i] == '\0')
			p->conf->is_dead = 1;
	}
	pthread_mutex_unlock(&p->conf->death_lock);
	pthread_mutex_unlock(&p->conf->write_lock);
}
