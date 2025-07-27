#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	cleaner(t_data *data, t_philo *philo)
{
	int	i;
	int	philo_count;

	philo_count = 0;
	if (data)
		philo_count = data->philos;
	if (data)
	{
		if (data->forks)
		{
			i = 0;
			while (i < data->philos)
			{
				pthread_mutex_destroy(&data->forks[i]);
				i++;
			}
			free(data->forks);
		}
		clean_mutexes(data);
		free(data);
	}
	clean_philos(philo, philo_count);
}

void	clean_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->ready_mutex);
	pthread_cond_destroy(&data->ready_cond);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->full_count_mutex);
	pthread_mutex_destroy(&data->rip_mutex);
}

void	clean_philos(t_philo *philo, int philo_count)
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

t_philo	*philo_init(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * data->philos);
	if (!philo)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < data->philos)
	{
		init_single_philo(&philo[i], data, i);
		i++;
	}
	return (philo);
}

void	init_single_philo(t_philo *philo, t_data *data, int i)
{
	philo->id = i + 1;
	philo->meals_count = 0;
	philo->last_meal = data->start_timer;
	philo->maxim_eaten = 0;
	philo->data = data;
	assign_forks(philo, data, i);
	pthread_mutex_init(&philo->locker, NULL);
}

void	assign_forks(t_philo *philo, t_data *data, int i)
{
	if (i == data->philos - 1)
	{
		philo->l_forks = &data->forks[0];
		philo->r_forks = &data->forks[i];
	}
	else
	{
		philo->l_forks = &data->forks[i];
		philo->r_forks = &data->forks[(i + 1) % data->philos];
	}
}

int	is_number(char *str)
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

t_data	*init_data(t_data *data, char **av)
{
	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	parse_arguments(data, av);
	data->rip = 0;
	data->all_threads_ready = 0;
	data->ready_count = 0;
	data->philos_full_count = 0;
	data->start_timer = get_time();
	init_mutexes(data);
	init_forks(data);
	return (data);
}

void	parse_arguments(t_data *data, char **av)
{
	data->ttd = ft_atoi(av[2]);
	data->philos = ft_atoi(av[1]);
	data->tte = ft_atoi(av[3]);
	data->tts = ft_atoi(av[4]);
	if (av[5])
		data->eat_counter = ft_atoi(av[5]);
	else
		data->eat_counter = -1;
}

void	init_mutexes(t_data *data)
{
	pthread_mutex_init(&data->ready_mutex, NULL);
	pthread_cond_init(&data->ready_cond, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->full_count_mutex, NULL);
	pthread_mutex_init(&data->rip_mutex, NULL);
}

void	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos);
	if (!data->forks)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < data->philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	valid_input(t_data *data, char **av, int ac)
{
	int	i;

	if ((data->philos > 200 || data->tte > INT_MAX || data->tts > INT_MAX
			|| data->ttd > INT_MAX))
	{
		cleaner(data, NULL);
		exit(EXIT_FAILURE);
	}
	i = 1;
	while (i < ac)
	{
		if (!is_number(av[i]))
		{
			cleaner(data, NULL);
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
		data = init_data(data, av);
		if (data->philos < 1 || data->tte < 0 || data->tts < 0
			|| data->ttd < 0)
		{
			cleaner(data, philo);
			exit(EXIT_FAILURE);
		}
		philo = philo_init(data);
		valid_input(data, av, ac);
		final_supper(data, philo);
		cleaner(data, philo);
	}
	else
		exit(EXIT_FAILURE);
	return (0);
}