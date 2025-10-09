/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:06:54 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/09 12:56:42 by vinpache         ###   ########.fr       */
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

typedef struct s_configuration //t_config contém tudo compartilhado entre threads (parâmetros e sincronização).
{
	int						philo_count; //número de filosofos
	uint64_t				t_die; //tempo em millisegundos para morrer
	uint64_t				t_eat; //tempo em millisegundos para comer
	uint64_t				t_sleep; //tempo em millisegundos para domir
	int						times_to_eat; //opcional quantas vezes cada filosofo come (-1 infinito)
	int						is_dead; //flag que algum filosofo morreu
	uint64_t				start_time; //timestamp inicio de simulação
	pthread_mutex_t			*forks; //ponteiro para o array de mutexs
	struct s_philosopher	*philosophers; //array para o t_philo (estado de cada filosofo) 
	pthread_mutex_t			write_lock; //mutex para serializar printf (evitar saída embaralhada).
	pthread_mutex_t			death_lock; //mutex para proteger is_dead (evitar race conditions).
}							t_config;

typedef struct s_philosopher
{
	t_config				*conf; //ponteiro da confuguração global compartilhadas para threads
	uint64_t				last_meal; //quando começou a ultima ref
	int						id; //id do filosofo para controle
	int						eat_count; //quantidade de ref finalizadas
	pthread_mutex_t			*left_fork; //ponteiro para o mutex da esquerda do filosofo
	pthread_mutex_t			*right_fork; //ponteiro para o mutex da direita do filosofo
	pthread_t				tid; //idnetificador da thread
}							t_philo;

int							init_all(t_config *conf, int ac, char **av); //Parseia args, aloca estruturas, inicializa mutexes e preenche conf/philosophers.
void						*routine(void *arg); //Função que cada thread/philosopher executa: loop comer → dormir → pensar.
int							start_simulation(t_config *conf); //Cria as threads dos filósofos e dá partida na simulação.
void						wait_for_completion(t_config *conf); //Função monitor (no main): observa mortes ou término (times_to_eat) e termina simulação.
long int					ft_atol(const char *str); //Converte string para long int (parsing seguro de argumentos).
uint64_t					get_time_ms(void); //Retorna tempo atual em milissegundos (usa gettimeofday).
void						print_log(t_philo *p, char *msg); //Imprime mensagens sincronizadas (usa write_lock e verifica/atualiza is_dead com death_lock).
void						ft_usleep(uint64_t time); //Sleep preciso em milissegundos (loop que evita imprecisão de usleep).
void						free_all(t_config *conf); //Destrói mutexes e libera memória alocada.
void						join_threads(t_config *conf); //pthread_join em todas as threads para esperar saída limpa.
int							get_simulation_status(t_philo *p); //Retorna se a simulação continua/permite ações para esse filósofo (verifica is_dead e possivelmente times_to_eat).

#endif