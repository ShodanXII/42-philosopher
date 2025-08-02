/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 21:11:25 by achat             #+#    #+#             */
/*   Updated: 2025/08/02 21:18:59 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philosophers(t_philo *philo, int philo_count)
{
	int	i;

	if (philo && philo_count > 0)
	{
		i = 0;
		while (i < philo_count)
		{
			pthread_mutex_destroy(&philo[i].locker);
			i++;
		}
		free(philo);
	}
}

void	destroy_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_count_mutex);
	pthread_mutex_destroy(&data->rip_mutex);
}

void	cleanup_forks(t_data *data)
{
	int	i;

	if (data && data->forks)
	{
		i = 0;
		while (i < data->philos_nb)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
}

void	cleanup_resources(t_data *data, t_philo *philo)
{
	int	philo_count;

	philo_count = 0;
	if (data)
		philo_count = data->philos_nb;
	cleanup_forks(data);
	if (data)
	{
		destroy_mutexes(data);
		free(data);
	}
	free_philosophers(philo, philo_count);
}
