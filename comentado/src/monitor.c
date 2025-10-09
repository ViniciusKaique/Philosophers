/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:17:17 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/09 14:09:57 by vinpache         ###   ########.fr       */
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
		if (check_meals_comple/* ************************************************************************** */
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

#include "philosophers.h" // Inclui o header principal do projeto, com structs e protótipos

// Função que retorna o status atual da simulação (se algum filósofo morreu ou não)
int	get_simulation_status(t_philo *p)
{
	int	status;

	pthread_mutex_lock(&p->conf->death_lock); // Trava o mutex para acessar a variável is_dead com segurança
	status = p->conf->is_dead;                // Lê o status de morte da configuração
	pthread_mutex_unlock(&p->conf->death_lock); // Destrava o mutex
	return (status);                          // Retorna o status (0 = vivo, 1 = morreu)
}

// Função que verifica se algum filósofo morreu
static int	check_death(t_config *conf)
{
	int		i;              // Índice do loop
	size_t	current_time;  // Armazena o tempo atual em ms

	i = -1;
	while (++i < conf->philo_count) // Itera sobre todos os filósofos
	{
		if (get_simulation_status(&conf->philosophers[i])) // Verifica se algum já morreu
			return (1); // Se algum morreu, retorna 1 (simulação deve parar)

		current_time = get_time_ms(); // Pega o tempo atual
		if ((current_time - conf->philosophers[i].last_meal) > conf->t_die) // Verifica se passou do tempo máximo sem comer
		{
			print_log(&conf->philosophers[i], DIE_MSG); // Imprime que o filósofo morreu
			return (1); // Retorna 1 para indicar que a simulação terminou por morte
		}
	}
	return (0); // Nenhum filósofo morreu ainda
}

// Função que verifica se todos os filósofos comeram a quantidade necessária
static int	check_meals_completed(t_config *conf)
{
	int	i;                // Índice do loop
	int	meals_completed;  // Contador de filósofos que já comeram o suficiente

	if (conf->times_to_eat == -1) // Se não há limite de refeições, retorna 0
		return (0);

	meals_completed = 0;
	i = -1;
	while (++i < conf->philo_count) // Itera sobre todos os filósofos
	{
		if (conf->philosophers[i].eat_count >= conf->times_to_eat) // Verifica se cada filósofo comeu o suficiente
			meals_completed++; // Incrementa o contador
	}
	if (meals_completed == conf->philo_count) // Se todos comeram o necessário
	{
		print_log(&conf->philosophers[0], "All philosophers are full"); // Imprime log de todos satisfeitos
		return (1); // Retorna 1 para indicar que simulação deve parar
	}
	return (0); // Ainda há filósofos que não comeram o suficiente
}

// Função principal de monitoramento que espera até a simulação terminar
void	wait_for_completion(t_config *conf)
{
	while (1) // Loop infinito até uma condição de parada
	{
		if (check_death(conf))       // Verifica se algum filósofo morreu
			break ;                 // Sai do loop se houver morte
		if (check_meals_completed(conf)) // Verifica se todos comeram o necessário
			break ;                      // Sai do loop se todos estiverem satisfeitos
		ft_usleep(1); // Pequeno delay para não sobrecarregar o CPU (1 ms)
	}
}
ted(conf))
			break ;
		ft_usleep(1);
	}
}
