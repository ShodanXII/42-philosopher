/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by abde-nnour        #+#    #+#             */
/*   Updated: 2025/08/02 19:45:18 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	acquire_utensils(t_philo *philo)
{
	if (philo->data->philos_nb == 2 && philo->id == 2)
		usleep(100);
	pthread_mutex_lock(philo->l_forks);
	announce_action(philo->data, philo->id, "has taken a fork");
	if (should_terminate(philo))
	{
		pthread_mutex_unlock(philo->l_forks);
		return ;
	}
	pthread_mutex_lock(philo->r_forks);
	announce_action(philo->data, philo->id, "has taken a fork");
}

void	consume_meal(t_philo *philo)
{
	long	eat_start_time;

	eat_start_time = current_timestamp();
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = eat_start_time;
	philo->meals_count++;
	pthread_mutex_unlock(&philo->locker);
	announce_action(philo->data, philo->id, "is eating");
	precise_sleep(philo->data->tte, philo->data);
}

void	drop_utensils(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_forks);
	pthread_mutex_unlock(philo->r_forks);
}

void	rest_philosopher(t_philo *philo)
{
	announce_action(philo->data, philo->id, "is sleeping");
	precise_sleep(philo->data->tts, philo->data);
}

void	contemplate(t_philo *philo)
{
	announce_action(philo->data, philo->id, "is thinking");
}
