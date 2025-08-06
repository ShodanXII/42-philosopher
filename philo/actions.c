/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by abde-nnour        #+#    #+#             */
/*   Updated: 2025/08/06 16:22:49 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	acquire_forks(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	if (philo->data->philos_nb == 2 && philo->id == 2)
		usleep(100);
	pthread_mutex_lock(philo->l_forks);
	if (should_terminate(philo))
	{
		pthread_mutex_unlock(philo->l_forks);
		return ;
	}
	announce_action(philo->data, philo->id, "has taken a fork");
	if (should_terminate(philo))
	{
		pthread_mutex_unlock(philo->l_forks);
		return ;
	}
	pthread_mutex_lock(philo->r_forks);
	if (should_terminate(philo))
	{
		pthread_mutex_unlock(philo->r_forks);
		pthread_mutex_unlock(philo->l_forks);
		return ;
	}
	announce_action(philo->data, philo->id, "has taken a fork");
}

void	consume_meal(t_philo *philo)
{
	long	eat_start_time;

	if (should_terminate(philo))
		return ;
	eat_start_time = get_time();
	pthread_mutex_lock(&philo->locker);
	pthread_mutex_lock(&philo->data->rip_mutex);
	if (philo->data->rip)
	{
		pthread_mutex_unlock(&philo->data->rip_mutex);
		pthread_mutex_unlock(&philo->locker);
		return ;
	}
	pthread_mutex_unlock(&philo->data->rip_mutex);
	philo->last_meal = eat_start_time;
	philo->meals_count++;
	pthread_mutex_unlock(&philo->locker);
	announce_action(philo->data, philo->id, "is eating");
	ft_usleep(philo->data->tte, philo->data);
}

void	drop_utensils(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_forks);
	pthread_mutex_unlock(philo->r_forks);
}

void	rest_philosopher(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	announce_action(philo->data, philo->id, "is sleeping");
	ft_usleep(philo->data->tts, philo->data);
}

void	thinkinge(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	announce_action(philo->data, philo->id, "is thinking");
}
