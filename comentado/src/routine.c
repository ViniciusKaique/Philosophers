/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:17:41 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/09 14:09:03 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h" // Inclui as definições e protótipos necessários

// ---------------------------------------------------------------------------
// O filósofo tenta pegar os dois garfos (mutexes) antes de comer
// ---------------------------------------------------------------------------
static void	take_forks(t_philo *p)
{
	pthread_mutex_lock(p->left_fork);  // Trava o garfo da esquerda
	print_log(p, FORK_MSG);            // Loga a ação "🍴 has taken a fork"

	pthread_mutex_lock(p->right_fork); // Trava o garfo da direita
	print_log(p, FORK_MSG);            // Loga novamente (pegou o segundo garfo)
}

// ---------------------------------------------------------------------------
// O filósofo solta (destrava) os garfos após comer
// ---------------------------------------------------------------------------
static void	let_go_forks(t_philo *p)
{
	pthread_mutex_unlock(p->left_fork);  // Solta o garfo da esquerda
	pthread_mutex_unlock(p->right_fork); // Solta o garfo da direita
}

// ---------------------------------------------------------------------------
// O filósofo realiza o ato de comer
// ---------------------------------------------------------------------------
static void	philo_eat(t_philo *p)
{
	take_forks(p);                    // Primeiro pega os dois garfos
	p->last_meal = get_time_ms();     // Atualiza o tempo da última refeição (para monitorar morte)
	print_log(p, EAT_MSG);            // Imprime a ação "🍝 is eating"
	ft_usleep(p->conf->t_eat);        // Dorme pelo tempo de comer (simula o ato de comer)
	p->eat_count++;                   // Incrementa contador de refeições feitas
	let_go_forks(p);                  // Solta os garfos para os outros filósofos usarem
}

// ---------------------------------------------------------------------------
// O filósofo dorme por um tempo determinado
// ---------------------------------------------------------------------------
static void	philo_sleep(t_philo *p)
{
	print_log(p, SLEEP_MSG);          // Loga a ação "💤 is sleeping"
	ft_usleep(p->conf->t_sleep);      // Espera o tempo de sono definido
}

// ---------------------------------------------------------------------------
// Função executada por cada thread (a rotina principal de um filósofo)
// ---------------------------------------------------------------------------
void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg; // Converte o argumento genérico em ponteiro de t_philo

	// Filósofos ímpares esperam 1ms antes de começar
	// Isso evita que todos tentem pegar garfos ao mesmo tempo (prevenir deadlock)
	if (p->id % 2 != 0)
		ft_usleep(1);

	// Loop principal do filósofo (vida dele)
	while (1)
	{
		// Verifica se a simulação acabou (morte ou todas as refeições feitas)
		if (get_simulation_status(p))
			break ;

		// Ação: comer
		philo_eat(p);

		// Verifica de novo se alguém morreu ou terminou
		if (get_simulation_status(p))
			break ;

		// Ação: dormir
		philo_sleep(p);

		// Verifica novamente o status da simulação
		if (get_simulation_status(p))
			break ;

		// Ação: pensar (não há tempo fixo, apenas loga a ação)
		print_log(p, THINK_MSG);
	}

	return (NULL); // Finaliza a thread (filósofo “morreu” ou acabou a simulação)
}
