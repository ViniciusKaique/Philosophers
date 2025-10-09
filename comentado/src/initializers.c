/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:13:03 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/09 14:00:49 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h" // Inclui as definições de structs, funções e macros do projeto

// ---------------------------------------------------------------------------
// Lê e valida os argumentos da linha de comando, salvando os valores no 'conf'
// ---------------------------------------------------------------------------
static int	assign_values_from_args(t_config *conf, int ac, char **av)
{
	long int	values[5]; // Array temporário para guardar os valores convertidos

	// Converte cada argumento string para long int usando ft_atol
	values[0] = ft_atol(av[1]); // Número de filósofos
	values[1] = ft_atol(av[2]); // Tempo até morrer (ms)
	values[2] = ft_atol(av[3]); // Tempo para comer (ms)
	values[3] = ft_atol(av[4]); // Tempo para dormir (ms)

	// Se o argumento opcional "times_to_eat" foi passado, lê ele também
	if (ac == 6)
		values[4] = ft_atol(av[5]);
	else
		values[4] = -1; // Se não foi passado, define como -1 (significa infinito)

	// Verifica se todos os valores são positivos
	if (values[0] <= 0 || values[1] <= 0 || values[2] <= 0 || values[3] <= 0
		|| (ac == 6 && values[4] <= 0))
	{
		printf("Error: All values must be positive integers.\n");
		return (1); // Retorna erro se algum argumento for inválido
	}

	// Atribui os valores validados à struct de configuração
	conf->philo_count = values[0];
	conf->t_die = values[1];
	conf->t_eat = values[2];
	conf->t_sleep = values[3];
	conf->times_to_eat = values[4];

	return (0); // Sucesso
}

// ---------------------------------------------------------------------------
// Aloca memória para os arrays de filósofos e de mutexes (garfos)
// ---------------------------------------------------------------------------
static int	allocate_memory(t_config *conf)
{
	// Aloca espaço para N filósofos
	conf->philosophers = malloc(sizeof(t_philo) * conf->philo_count);
	// Aloca espaço para N mutexes, um para cada garfo
	conf->forks = malloc(sizeof(pthread_mutex_t) * conf->philo_count);

	// Verifica se houve erro na alocação
	if (!conf->philosophers || !conf->forks)
		return (1); // Falhou em alocar memória
	return (0); // Sucesso
}

// ---------------------------------------------------------------------------
// Inicializa os dados individuais de cada filósofo
// ---------------------------------------------------------------------------
static void	init_philo_data(t_philo *p, t_config *conf, int i)
{
	p->conf = conf;                      // Cada filósofo recebe um ponteiro para a configuração global
	p->id = i + 1;                       // ID do filósofo (1 a N)
	p->eat_count = 0;                    // Inicia contador de refeições em 0
	p->last_meal = get_time_ms();        // Registra o tempo atual como a última refeição inicial
	p->left_fork = &conf->forks[i];      // Define o garfo da esquerda (índice igual ao filósofo)
	p->right_fork = &conf->forks[(i + 1) % conf->philo_count]; 
	// O garfo da direita é o próximo no array (usando módulo para circular)
}

// ---------------------------------------------------------------------------
// Inicializa todos os mutexes (forks e locks globais)
// ---------------------------------------------------------------------------
static void	init_mutexes(t_config *conf)
{
	int	i;

	// Mutex para sincronizar escrita no terminal (evita que prints se misturem)
	pthread_mutex_init(&conf->write_lock, NULL);
	// Mutex para proteger o acesso à variável global "is_dead"
	pthread_mutex_init(&conf->death_lock, NULL);

	i = -1;
	// Cria um mutex para cada garfo (um por filósofo)
	while (++i < conf->philo_count)
		pthread_mutex_init(&conf->forks[i], NULL);
}

// ---------------------------------------------------------------------------
// Função principal de inicialização do programa
// ---------------------------------------------------------------------------
int	init_all(t_config *conf, int ac, char **av)
{
	int	i;

	// 1️⃣ Lê e valida todos os argumentos
	if (assign_values_from_args(conf, ac, av))
		return (1); // Retorna erro se algum valor for inválido

	// 2️⃣ Define estado inicial da simulação (ninguém morto)
	conf->is_dead = 0;

	// 3️⃣ Aloca memória para filósofos e garfos
	if (allocate_memory(conf))
		return (1); // Retorna erro se falhar na alocação

	// 4️⃣ Inicializa mutexes (write, death e garfos)
	init_mutexes(conf);

	// 5️⃣ Inicializa os dados individuais de cada filósofo
	i = -1;
	while (++i < conf->philo_count)
		init_philo_data(&conf->philosophers[i], conf, i);

	return (0); // Tudo pronto com sucesso
}
