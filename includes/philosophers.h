/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:06:54 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/06 14:27:07 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define FORK_MSG "\033[1;33m🍴 has taken a fork\033[0m"
# define EAT_MSG "\033[1;32m🍽️  is eating\033[0m"
# define SLEEP_MSG "\033[1;35m💤 is sleeping\033[0m"
# define THINK_MSG "\033[1;34m🤔 is thinking\033[0m"
# define DIE_MSG "\033[1;31m💀 died\033[0m"
# define USAGE_MSG1 "\033[1;33mUsage: ./philo num_philo time_die\033[0m"
# define USAGE_MSG2 "\033[1;33mtime_eat time_sleep [times_to_eat]\033[0m"

typedef struct s_configuration
{
	int						philo_count;
	uint64_t				t_die;
	uint64_t				t_eat;
	uint64_t				t_sleep;
	int						times_to_eat;
	int						is_dead;
	uint64_t				start_time;
	pthread_mutex_t			*forks;
	struct s_philosopher	*philosophers;
	pthread_mutex_t			write_lock;
	pthread_mutex_t			death_lock;
}							t_config;

typedef struct s_philosopher
{
	t_config				*conf;
	uint64_t				last_meal;
	int						id;
	int						eat_count;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	pthread_t				tid;
}							t_philo;

int							init_all(t_config *conf, int ac, char **av);
void						*routine(void *arg);
int							start_simulation(t_config *conf);
void						wait_for_completion(t_config *conf);
long int					ft_atol(const char *str);
uint64_t					get_time_ms(void);
void						print_log(t_philo *p, char *msg);
void						ft_usleep(uint64_t time);
void						free_all(t_config *conf);
void						join_threads(t_config *conf);
int							get_simulation_status(t_philo *p);

#endif