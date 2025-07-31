/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by abde-nnour        #+#    #+#             */
/*   Updated: 2025/07/31 15:42:40 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_fork(t_philo *philo)
{
	if (philo->data->philos_nb == 2 && philo->id == 2)
		usleep(100);
	pthread_mutex_lock(philo->l_forks);
	print_state(philo->data, philo->id, "has taken a fork");
	if (check_stop(philo))
	{
		pthread_mutex_unlock(philo->l_forks);
		return;
	}
	pthread_mutex_lock(philo->r_forks);
	print_state(philo->data, philo->id, "has taken a fork");
}

void	eat(t_philo *philo)
{
	long eat_start_time;

	eat_start_time = get_time();
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = eat_start_time;
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
