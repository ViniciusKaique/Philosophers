/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:13:49 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/06 14:23:54 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	handle_one_philo(t_config *conf)
{
	printf("0 1 🍴 has taken a fork\n");
	ft_usleep(conf->t_die);
	printf("%lu 1%s\n", conf->t_die, DIE_MSG);
	free_all(conf);
	return (0);
}

int	start_simulation(t_config *conf)
{
	int	i;

	conf->start_time = get_time_ms();
	i = -1;
	while (++i < conf->philo_count)
	{
		if (pthread_create(&conf->philosophers[i].tid, NULL, &routine,
				&conf->philosophers[i]))
			return (1);
	}
	return (0);
}

void	join_threads(t_config *conf)
{
	int	i;

	i = -1;
	while (++i < conf->philo_count)
		pthread_join(conf->philosophers[i].tid, NULL);
}

int	main(int ac, char **av)
{
	t_config	conf;

	conf.philosophers = NULL;
	conf.forks = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("%s %s\n", USAGE_MSG1, USAGE_MSG2);
		return (1);
	}
	if (init_all(&conf, ac, av))
	{
		free_all(&conf);
		return (1);
	}
	if (conf.philo_count == 1)
		return (handle_one_philo(&conf));
	if (start_simulation(&conf))
	{
		free_all(&conf);
		return (1);
	}
	wait_for_completion(&conf);
	join_threads(&conf);
	free_all(&conf);
	return (0);
}
