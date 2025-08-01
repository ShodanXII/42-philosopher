/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_supper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:39:55 by achat             #+#    #+#             */
/*   Updated: 2025/07/31 20:39:56 by achat            ###   ########.fr       */
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

void	*philosopher_lifecycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	// Signal that this thread is ready
	pthread_mutex_lock(&philo->data->sync_mutex);
	philo->data->threads_ready++;
	if (philo->data->threads_ready == philo->data->philos_nb)
	{
		// Last thread to start - set synchronized start time
		philo->data->sync_start_time = current_timestamp();
	}
	pthread_mutex_unlock(&philo->data->sync_mutex);
	
	// Wait for all threads to be ready
	while (1)
	{
		pthread_mutex_lock(&philo->data->sync_mutex);
		if (philo->data->sync_start_time != 0)
		{
			philo->last_meal = philo->data->sync_start_time;
			pthread_mutex_unlock(&philo->data->sync_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->sync_mutex);
		usleep(10);
	}
	
	if (philo->id % 2 == 1)
		rest_philosopher(philo);
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
		if (philo->data->philos_nb > 2)
			usleep(1000);
		rest_philosopher(philo);
		contemplate(philo);
	}
	return (NULL);
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

void	final_supper(t_data *data, t_philo *philo)
{
	int			i;
	pthread_t	monitor;

	if (data->eat_counter == 0)
		return ;
	if (data->philos_nb == 1)
	{
		if (pthread_create(&philo[0].thread, NULL, lonely_philosopher_routine, &philo[0]))
			return ;
		pthread_join(philo[0].thread, NULL);
		return ;
	}
	data->philos = philo;
	i = 0;
	while (i < data->philos_nb)
	{
		if (pthread_create(&philo[i].thread, NULL, philosopher_lifecycle, &philo[i]))
			return ;
		i++;
	}
	
	// Wait for all threads to synchronize, then set the global start timer
	while (1)
	{
		pthread_mutex_lock(&data->sync_mutex);
		if (data->sync_start_time != 0)
		{
			data->start_timer = data->sync_start_time;
			pthread_mutex_unlock(&data->sync_mutex);
			break;
		}
		pthread_mutex_unlock(&data->sync_mutex);
		usleep(10);
	}
	
	// Now start the monitor thread after synchronization is complete
	if (pthread_create(&monitor, NULL, observe_philosophers, data))
		return ;
	
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
