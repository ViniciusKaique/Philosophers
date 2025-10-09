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

/static int	check_meals_completed(t_config *conf)
{
	int	i;                 // Variável para percorrer os filósofos
	int	meals_completed;   // Contador de filósofos que já comeram o número de vezes desejado

	// Se não há limite de refeições, não faz checagem, retorna 0
	if (conf->times_to_eat == -1)
		return (0);

	meals_completed = 0;   // Inicializa o contador de refeições completas
	i = -1;                // Inicializa o índice do loop

	// Loop para verificar cada filósofo
	while (++i < conf->philo_count)
	{
		// Se o filósofo já comeu pelo menos 'times_to_eat' vezes
		if (conf->philosophers[i].eat_count >= conf->times_to_eat)
			meals_completed++;  // Incrementa o contador
	}

	// Se todos os filósofos comeram a quantidade desejada
	if (meals_completed == conf->philo_count)
	{
		// Imprime a mensagem de todos os filósofos satisfeitos
		print_log(&conf->philosophers[0], "All philosophers are full");

		// Bloqueia o mutex para alterar a variável de controle da simulação
		pthread_mutex_lock(&conf->death_lock);

		// Marca a simulação como finalizada para que os threads saibam que devem parar
		conf->is_dead = 1;

		// Desbloqueia o mutex após alterar a variável
		pthread_mutex_unlock(&conf->death_lock);

		// Retorna 1 indicando que a condição de término foi atingida
		return (1);
	}

	// Retorna 0 se nem todos os filósofos completaram as refeições
	return (0);
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

