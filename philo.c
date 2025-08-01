/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:40:13 by achat             #+#    #+#             */
/*   Updated: 2025/07/31 20:40:14 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	cleanup_resources(t_data *data, t_philo *philo)
{
	int	i;
	int	philo_count;

	philo_count = 0;
	if (data)
		philo_count = data->philos_nb;
	if (data)
	{
		if (data->forks)
		{
			i = 0;
			while (i < data->philos_nb)
			{
				pthread_mutex_destroy(&data->forks[i]);
				i++;
			}
			free(data->forks);
		}
		destroy_mutexes(data);
		free(data);
	}
	free_philosophers(philo, philo_count);
}

void	destroy_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->ready_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_count_mutex);
	pthread_mutex_destroy(&data->rip_mutex);
}

void	free_philosophers(t_philo *philo, int philo_count)
{
	int	i;

	if (philo && philo_count > 0)
	{
		i = 0;
		while (i < philo_count)
		{
			pthread_mutex_destroy(&philo[i].locker);
			i++;
		}
		free(philo);
	}
}

t_philo	*create_philosophers(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * data->philos_nb);
	if (!philo)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < data->philos_nb)
	{
		setup_philosopher(&philo[i], data, i);
		i++;
	}
	return (philo);
}

void	setup_philosopher(t_philo *philo, t_data *data, int i)
{
	philo->id = i + 1;
	philo->meals_count = 0;
	philo->last_meal = data->start_timer;
	philo->maxim_eaten = 0;
	philo->data = data;
	distribute_utensils(philo, data, i);
	pthread_mutex_init(&philo->locker, NULL);
}

void	distribute_utensils(t_philo *philo, t_data *data, int i)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % data->philos_nb;
	if (left < right)
	{
		philo->l_forks = &data->forks[left];
		philo->r_forks = &data->forks[right];
	}
	else
	{
		philo->l_forks = &data->forks[right];
		philo->r_forks = &data->forks[left];
	}
}

int	validate_numeric(char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+')
		str++;
	if (*str == '-')
		exit(EXIT_FAILURE);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

t_data	*setup_simulation(t_data *data, char **av)
{
	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	process_parameters(data, av);
	data->rip = 0;
	data->philos_full_count = 0;
	data->start_timer = current_timestamp();
	create_mutexes(data);
	prepare_utensils(data);
	return (data);
}

void	process_parameters(t_data *data, char **av)
{
	data->ttd = ft_atoi(av[2]);
	data->philos_nb = ft_atoi(av[1]);
	data->tte = ft_atoi(av[3]);
	data->tts = ft_atoi(av[4]);
	if (av[5])
		data->eat_counter = ft_atoi(av[5]);
	else
		data->eat_counter = -1;
}

void	create_mutexes(t_data *data)
{
	pthread_mutex_init(&data->ready_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->full_count_mutex, NULL);
	pthread_mutex_init(&data->rip_mutex, NULL);
}

void	prepare_utensils(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_nb);
	if (!data->forks)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < data->philos_nb)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	validate_arguments(t_data *data, char **av, int ac)
{
	int	i;

	if ((data->philos_nb > 200 || data->tte > INT_MAX || data->tts > INT_MAX
			|| data->ttd > INT_MAX))
	{
		cleanup_resources(data, NULL);
		exit(EXIT_FAILURE);
	}
	i = 1;
	while (i < ac)
	{
		if (!validate_numeric(av[i]))
		{
			cleanup_resources(data, NULL);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;

	data = NULL;
	philo = NULL;
	if (ac == 5 || ac == 6)
	{
		data = setup_simulation(data, av);
		if (data->philos_nb < 1 || data->tte < 0 || data->tts < 0
			|| data->ttd < 0)
		{
			cleanup_resources(data, philo);
			exit(EXIT_FAILURE);
		}
		philo = create_philosophers(data);
		validate_arguments(data, av, ac);
		final_supper(data, philo);
		cleanup_resources(data, philo);
	}
	else
		exit(EXIT_FAILURE);
	return (0);
}