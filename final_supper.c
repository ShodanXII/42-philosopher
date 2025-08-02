/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_supper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:39:55 by achat             #+#    #+#             */
/*   Updated: 2025/08/02 19:56:24 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	verify_all_fed(t_data *data)
{
	int	completed_meals;

	if (data->eat_counter <= 0)
		return (0);
	pthread_mutex_lock(&data->full_count_mutex);
	completed_meals = (data->philos_full_count >= data->philos_nb);
	pthread_mutex_unlock(&data->full_count_mutex);
	if (completed_meals)
	{
		pthread_mutex_lock(&data->rip_mutex);
		data->rip = 1;
		pthread_mutex_unlock(&data->rip_mutex);
		return (1);
	}
	return (0);
}

void	*lonely_philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	announce_action(philo->data, philo->id, "is thinking");
	announce_action(philo->data, philo->id, "has taken a fork");
	precise_sleep(philo->data->ttd, philo->data);
	announce_action(philo->data, philo->id, "died");
	return (NULL);
}

static int	start_simulation(t_data *data, t_philo *philo, pthread_t *monitor)
{
	int	i;

	if (data->eat_counter == 0)
		return (0);
	if (data->philos_nb == 1)
	{
		one_philo(philo);
		return (0);
	}
	data->start_timer = current_timestamp();
	data->philos = philo;
	if (pthread_create(monitor, NULL, observe_philosophers, data))
		return (1);
	i = 0;
	while (i < data->philos_nb)
	{
		if (pthread_create(&philo[i].thread, NULL, 
				philosopher_lifecycle, &philo[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	wait_and_cleanup(t_data *data, t_philo *philo, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < data->philos_nb)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	pthread_mutex_lock(&data->rip_mutex);
	data->rip = 1;
	pthread_mutex_unlock(&data->rip_mutex);
	pthread_join(monitor, NULL);
}

void	final_supper(t_data *data, t_philo *philo)
{
	pthread_t	monitor;

	if (start_simulation(data, philo, &monitor) == 0)
		wait_and_cleanup(data, philo, monitor);
}
