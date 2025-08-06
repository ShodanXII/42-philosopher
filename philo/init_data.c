/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 21:14:32 by achat             #+#    #+#             */
/*   Updated: 2025/08/06 16:22:27 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_numeric(char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+')
		str++;
	if (*str == '-')
		return (-1);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

t_data	*allocate_data(void)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	return (data);
}

t_data	*init_data(t_data *data, char **av)
{
	data = allocate_data();
	if (!data)
		return (NULL);
	process_parameters(data, av);
	data->rip = 0;
	data->philos_full_count = 0;
	data->start_timer = get_time();
	create_mutexes(data);
	prepare_utensils(data);
	return (data);
}

int	validate_ranges(t_data *data)
{
	if (data->philos_nb > 200 || data->tte > INT_MAX || 
		data->tts > INT_MAX || data->ttd > INT_MAX)
		return (0);
	return (1);
}

void	validate_arguments(t_data *data, char **av, int ac)
{
	int	i;

	if (!validate_ranges(data))
	{
		cleanup_resources(data, NULL);
		return ;
	}
	i = 1;
	while (i < ac)
	{
		if (!validate_numeric(av[i]))
		{
			cleanup_resources(data, NULL);
			return ;
		}
		i++;
	}
}
