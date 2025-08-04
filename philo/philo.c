/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:40:13 by achat             #+#    #+#             */
/*   Updated: 2025/08/04 21:27:21 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	distribute_utensils(t_philo *philo, t_data *data, int i)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % data->philos_nb;
	philo->l_forks = &data->forks[right];
	philo->r_forks = &data->forks[left];
}

void	process_parameters(t_data *data, char **av)
{
	data->ttd = ft_atoi(av[2]);
	data->philos_nb = ft_atoi(av[1]);
	data->tte = ft_atoi(av[3]);
	data->tts = ft_atoi(av[4]);
	if (av[5])
		data->eat_counter = ft_atoi(av[5]);
	else
		data->eat_counter = -1;
}

void	prepare_utensils(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_nb);
	if (!data->forks)
		return ;
	i = 0;
	while (i < data->philos_nb)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;
	int		i;

	if (ac != 5 && ac != 6)
		return (1);
	i = 1;
	while (i < ac)
	{
		if (!validate_numeric(av[i]))
			return (1);
		i++;
	}
	data = init_data(NULL, av);
	if (data->philos_nb < 1 || data->tte < 0 || data->tts < 0 
		|| data->ttd < 0 || (av[5] && data->eat_counter < 0))
	{
		cleanup_resources(data, NULL);
		return (1);
	}
	philo = create_philosophers(data);
	final_supper(data, philo);
	cleanup_resources(data, philo);
	return (0);
}
