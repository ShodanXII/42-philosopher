/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_supper_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:44 by achat             #+#    #+#             */
/*   Updated: 2025/08/02 21:29:22 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	initialize_philosopher(t_philo *philo)
{
	philo->last_meal = current_timestamp();
	if (philo->id % 2 == 1)
		rest_philosopher(philo);
}

static void	*run_philosopher_simulation(t_philo *philo)
{
	while (1)
	{
		if (should_terminate(philo))
			return (NULL);
		contemplate(philo);
		acquire_utensils(philo);
		consume_meal(philo);
		pthread_mutex_lock(&philo->locker);
		if (philo->data->eat_counter != -1
			&& philo->meals_count >= philo->data->eat_counter
			&& !philo->maxim_eaten)
		{
			philo->maxim_eaten = 1;
			pthread_mutex_unlock(&philo->locker);
			pthread_mutex_lock(&philo->data->full_count_mutex);
			philo->data->philos_full_count++;
			pthread_mutex_unlock(&philo->data->full_count_mutex);
		}
		else
			pthread_mutex_unlock(&philo->locker);
		drop_utensils(philo);
		rest_philosopher(philo);
		contemplate(philo);
	}
	return (NULL);
}

void	*philosopher_lifecycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	initialize_philosopher(philo);
	return (run_philosopher_simulation(philo));
}

int	detect_starvation(t_philo *philo, t_data *data)
{
	long	time_since_last_meal;

	pthread_mutex_lock(&philo->locker);
	time_since_last_meal = current_timestamp() - philo->last_meal;
	pthread_mutex_unlock(&philo->locker);
	if (time_since_last_meal >= data->ttd)
	{
		pthread_mutex_lock(&data->rip_mutex);
		if (!data->rip)
		{
			announce_action(data, philo->id, "died");
			data->rip = 1;
			pthread_mutex_unlock(&data->rip_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->rip_mutex);
		return (1);
	}
	return (0);
}

void	*observe_philosophers(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->rip)
	{
		if (verify_all_fed(data))
			return (NULL);
		i = 0;
		while (i < data->philos_nb)
		{
			if (detect_starvation(&data->philos[i], data))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
