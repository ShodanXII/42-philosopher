/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_case.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:53:59 by achat             #+#    #+#             */
/*   Updated: 2025/08/02 19:55:23 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_philo *philo)
{
	philo->data->start_timer = current_timestamp();
	if (pthread_create(&philo[0].thread, NULL, 
			lonely_philosopher_routine, &philo[0]))
		return ;
	pthread_join(philo[0].thread, NULL);
	return ;
}
