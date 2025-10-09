/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:13:49 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/09 13:59:58 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h" // Inclui o header principal com structs, defines e protótipos

// ---------------------------------------------------------------------------
// Função auxiliar usada apenas quando há UM filósofo na simulação
// ---------------------------------------------------------------------------
static int	handle_one_philo(t_config *conf)
{
	printf("0 1 🍴 has taken a fork\n"); // Imprime que o filósofo 1 pegou um garfo (único disponível)
	ft_usleep(conf->t_die);              // Aguarda o tempo de morte (pois ele nunca poderá comer)
	// um filósofo apenas morre sempre com apenas um garfo
	printf("%lu 1%s\n", conf->t_die, DIE_MSG); // Imprime que o filósofo morreu após t_die milissegundos
	
	free_all(conf);                     // Libera todos os recursos e destrói mutexes
	return (0);                         // Retorna 0 indicando que terminou normalmente
}

// ---------------------------------------------------------------------------
// Cria as threads dos filósofos e inicia a simulação
// ---------------------------------------------------------------------------
int	start_simulation(t_config *conf)
{
	int	i;

	conf->start_time = get_time_ms(); // Marca o tempo inicial da simulação em milissegundos
	i = -1;                           // Começa em -1 para facilitar o pré-incremento no loop
	while (++i < conf->philo_count)   // Loop que cria uma thread para cada filósofo
	{
		// Cria uma thread executando a função 'routine', passando o filósofo i como argumento
		if (pthread_create(&conf->philosophers[i].tid, NULL, &routine,
				&conf->philosophers[i]))
			return (1); // Se a criação falhar, retorna 1 (erro)
	}
	return (0); // Retorna 0 indicando sucesso
}

// ---------------------------------------------------------------------------
// Espera todas as threads dos filósofos terminarem (join)
// ---------------------------------------------------------------------------
void	join_threads(t_config *conf)
{
	int	i;

	i = -1;                            // Índice começa em -1 para usar pré-incremento
	while (++i < conf->philo_count)    // Percorre todas as threads criadas
		pthread_join(conf->philosophers[i].tid, NULL); // Espera cada thread finalizar
}

// ---------------------------------------------------------------------------
// Função principal — ponto de entrada do programa
// ---------------------------------------------------------------------------
int	main(int ac, char **av)
{
	t_config	conf; // Estrutura principal que guarda toda a configuração da simulação

	conf.philosophers = NULL; // Garante que ponteiros começam nulos (segurança contra free indevido)
	conf.forks = NULL;        // Idem para o array de mutexes dos garfos
	
	// -----------------------------------------------------------------------
	// Verifica se a quantidade de argumentos está correta
	// Deve ser 5 ou 6 (programa + 4 ou 5 parâmetros)
	// -----------------------------------------------------------------------
	if (ac < 5 || ac > 6)
	{
		// Mostra a mensagem de uso, definida em macros (USAGE_MSG1 e USAGE_MSG2)
		printf("%s %s\n", USAGE_MSG1, USAGE_MSG2);
		return (1); // Sai com erro
	}

	// -----------------------------------------------------------------------
	// Inicializa toda a estrutura de configuração e os filósofos
	// Faz parsing, valida dados e inicializa mutexes
	// -----------------------------------------------------------------------
	if (init_all(&conf, ac, av))
	{
		free_all(&conf); // Se falhar na inicialização, libera memória e sai
		return (1);
	}

	// -----------------------------------------------------------------------
	// Caso especial: se houver apenas um filósofo, trata separadamente
	// -----------------------------------------------------------------------
	if (conf.philo_count == 1)
		return (handle_one_philo(&conf)); // Executa a rotina especial e encerra

	// -----------------------------------------------------------------------
	// Inicia a simulação criando todas as threads
	// -----------------------------------------------------------------------
	if (start_simulation(&conf))
	{
		free_all(&conf); // Se der erro na criação das threads, libera tudo
		return (1);
	}

	// -----------------------------------------------------------------------
	// Monitora a simulação até que um filósofo morra ou todos comam o suficiente
	// -----------------------------------------------------------------------
	wait_for_completion(&conf); // Função que observa o estado dos filósofos e detecta término
	
	// -----------------------------------------------------------------------
	// Espera todas as threads finalizarem e limpa os recursos
	// -----------------------------------------------------------------------
	join_threads(&conf); // Garante que todas as threads terminaram
	free_all(&conf);     // Destrói mutexes e libera memória alocada
	
	return (0); // Retorno bem-sucedido da função main
}
