/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abde-nnour <abde-nnour@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by abde-nnour        #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by abde-nnour       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->l_forks);
	print_state(philo->data, philo->id, "has taken a fork");
	pthread_mutex_lock(philo->r_forks);
	print_state(philo->data, philo->id, "has taken a fork");
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->locker);
	print_state(philo->data, philo->id, "is eating");
	ft_usleep(philo->data->tte, philo->data);
}

void	release_fork(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_forks);
	pthread_mutex_unlock(philo->r_forks);
}

void	sleep_philo(t_philo *philo)
{
	print_state(philo->data, philo->id, "is sleeping");
	ft_usleep(philo->data->tts, philo->data);
}

void	think(t_philo *philo)
{
	print_state(philo->data, philo->id, "is thinking");
}
