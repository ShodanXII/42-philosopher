/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abde-nnour <abde-nnour@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by abde-nnour        #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by abde-nnour       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(char *err)
{
	printf("%s\n", err);
	return (0);
}

void	print_state(t_data *data, int philo_id, char *state)
{
	long	timestamp;

	pthread_mutex_lock(&data->print_mutex);
	timestamp = get_time() - data->start_timer;
	if (!data->rip)
		printf("%-4ld %-3d %s\n", timestamp, philo_id, state);
	pthread_mutex_unlock(&data->print_mutex);
}

void	ft_usleep(long duration, t_data *data)
{
	long	start;

	start = get_time();
	while (get_time() - start < duration)
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

int	check_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->rip_mutex);
	stop = philo->data->rip;
	pthread_mutex_unlock(&philo->data->rip_mutex);
	return (stop);
}
