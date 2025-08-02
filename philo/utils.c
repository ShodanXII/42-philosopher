/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by abde-nnour        #+#    #+#             */
/*   Updated: 2025/08/02 21:49:39 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	display_error(char *err)
{
	printf("%s\n", err);
	return (0);
}

void	announce_action(t_data *data, int philo_id, char *state)
{
	long	timestamp;

	pthread_mutex_lock(&data->rip_mutex);
	if (data->rip)
	{
		pthread_mutex_unlock(&data->rip_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->rip_mutex);
	pthread_mutex_lock(&data->print_mutex);
	timestamp = current_timestamp() - data->start_timer;
	printf("%-4ld %-3d %s\n", timestamp, philo_id, state);
	pthread_mutex_unlock(&data->print_mutex);
}

void	precise_sleep(long duration, t_data *data)
{
	long	start;

	start = current_timestamp();
	while (current_timestamp() - start < duration)
	{
		usleep(100);
		pthread_mutex_lock(&data->rip_mutex);
		if (data->rip)
		{
			pthread_mutex_unlock(&data->rip_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->rip_mutex);
	}
}

int	should_terminate(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->rip_mutex);
	stop = philo->data->rip;
	pthread_mutex_unlock(&philo->data->rip_mutex);
	return (stop);
}
