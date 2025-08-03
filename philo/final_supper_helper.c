/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_supper_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:44 by achat             #+#    #+#             */
/*   Updated: 2025/08/02 22:52:11 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	initialize_philosopher(t_philo *philo)
{
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = current_timestamp();
	pthread_mutex_unlock(&philo->locker);
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
	int		should_die;

	should_die = 0;
	time_since_last_meal = 0;
	pthread_mutex_lock(&philo->locker);
	time_since_last_meal = current_timestamp() - philo->last_meal;
	if (time_since_last_meal >= data->ttd)
		should_die = 1;
	pthread_mutex_unlock(&philo->locker);

	if (should_die)
	{
		pthread_mutex_lock(&data->rip_mutex);
		if (!data->rip)
		{
			data->rip = 1;
			pthread_mutex_unlock(&data->rip_mutex);
			pthread_mutex_lock(&data->print_mutex);
			printf("%-4ld %-3d died\n", current_timestamp() - data->start_timer, philo->id);
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->rip_mutex);
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
