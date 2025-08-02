/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philosophers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 21:12:48 by achat             #+#    #+#             */
/*   Updated: 2025/08/02 21:24:06 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_mutexes(t_data *data)
{
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->full_count_mutex, NULL);
	pthread_mutex_init(&data->rip_mutex, NULL);
}

void	setup_philosopher(t_philo *philo, t_data *data, int i)
{
	philo->id = i + 1;
	philo->meals_count = 0;
	philo->last_meal = data->start_timer;
	philo->maxim_eaten = 0;
	philo->data = data;
	distribute_utensils(philo, data, i);
	pthread_mutex_init(&philo->locker, NULL);
}

t_philo	*allocate_philosophers(t_data *data)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * data->philos_nb);
	if (!philo)
		return (NULL);
	return (philo);
}

t_philo	*create_philosophers(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = allocate_philosophers(data);
	if (!philo)
		return (NULL);	
	i = 0;
	while (i < data->philos_nb)
	{
		setup_philosopher(&philo[i], data, i);
		i++;
	}
	return (philo);
}
