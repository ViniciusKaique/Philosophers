/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:18:03 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/09 14:08:28 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h" // Inclui definições, structs, macros e protótipos usados aqui

// ---------------------------------------------------------------------------
// Converte string em long int (versão própria de atoi, chamada ft_atol)
// ---------------------------------------------------------------------------
long int	ft_atol(const char *str)
{
	long int	res;  // Resultado final
	int			sign; // Sinal do número (+ ou -)

	res = 0;
	sign = 1;

	// Ignora espaços e caracteres de controle (tab, newline, etc)
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;

	// Detecta sinal negativo
	if (*str == '-')
		sign = -1;

	// Se houver '+' ou '-', avança o ponteiro
	if (*str == '-' || *str == '+')
		str++;

	// Converte os caracteres numéricos em número
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0'); // Multiplica o acumulado por 10 e soma o novo dígito
		str++;
	}
	return (res * sign); // Retorna o resultado final considerando o sinal
}

// ---------------------------------------------------------------------------
// Retorna o tempo atual em milissegundos (baseado em gettimeofday)
// ---------------------------------------------------------------------------
size_t	get_time_ms(void)
{
	struct timeval	tv; // Estrutura que guarda segundos e microssegundos

	gettimeofday(&tv, NULL); // Pega o tempo atual
	// Converte segundos e microssegundos para milissegundos
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// ---------------------------------------------------------------------------
// Faz uma pausa (sleep) em milissegundos, com precisão melhor que usleep puro
// ---------------------------------------------------------------------------
void	ft_usleep(size_t time)
{
	size_t	start;

	start = get_time_ms(); // Marca o tempo inicial
	while (get_time_ms() - start < time) // Loop até atingir o tempo desejado
		usleep(500); // Pequenas pausas (500 µs) para evitar travar a CPU
}

// ---------------------------------------------------------------------------
// Libera memória e destrói mutexes (usado no final ou em erro de inicialização)
// ---------------------------------------------------------------------------
void	free_all(t_config *conf)
{
	int	i;

	// Se os mutexes de garfos foram criados, destrói um por um
	if (conf->forks)
	{
		i = -1;
		while (++i < conf->philo_count)
			pthread_mutex_destroy(&conf->forks[i]);
	}

	// Destrói os mutexes globais
	pthread_mutex_destroy(&conf->write_lock);
	pthread_mutex_destroy(&conf->death_lock);

	// Libera o array de filósofos, se alocado
	if (conf->philosophers)
		free(conf->philosophers);

	// Libera o array de garfos, se alocado
	if (conf->forks)
		free(conf->forks);
}

// ---------------------------------------------------------------------------
// Imprime mensagens da simulação (sincronizadas e seguras entre threads)
// ---------------------------------------------------------------------------
void	print_log(t_philo *p, char *msg)
{
	// Trava o mutex de escrita para evitar que várias threads imprimam ao mesmo tempo
	pthread_mutex_lock(&p->conf->write_lock);

	// Trava o mutex de morte para checar ou alterar a variável is_dead
	pthread_mutex_lock(&p->conf->death_lock);

	// Só imprime se ainda ninguém morreu
	if (!p->conf->is_dead)
	{
		// Mostra o tempo desde o início, o ID do filósofo e a mensagem (ex: "🍴 took fork")
		printf("%lu %d %s\n", get_time_ms() - p->conf->start_time, p->id, msg);

		// Se a mensagem for DIE_MSG, marca que a simulação acabou (alguém morreu)
		if (strcmp(msg, DIE_MSG) == 0)
			p->conf->is_dead = 1;
	}

	// Libera os mutexes na ordem inversa do lock
	pthread_mutex_unlock(&p->conf->death_lock);
	pthread_mutex_unlock(&p->conf->write_lock);
}
