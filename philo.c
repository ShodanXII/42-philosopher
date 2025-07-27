#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void cleaner(t_data *data, t_philo *philo)
{
	int i;
	
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
		pthread_mutex_destroy(&data->ready_mutex);
		pthread_cond_destroy(&data->ready_cond);
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->full_count_mutex);
		free(data);
	}
	if (philo && data)  // Only clean philo if we have data to know the count
	{
		i = 0;
		while (i < data->philos)
		{
			pthread_mutex_destroy(&philo[i].locker);
			i++;
		}
		free(philo);
	}
	printf("cleaner\n");
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
		philo[i].id = i + 1;
		philo[i].meals_count = 0;
		philo[i].last_meal = data->start_timer; // Initialize with start time
		philo[i].maxim_eaten = 0;  // Initialize boolean flag
		philo[i].data = data;
		philo[i].l_forks = &data->forks[i];
		philo[i].r_forks = &data->forks[(i + 1) % data->philos];
		pthread_mutex_init(&philo[i].locker, NULL);
		i++;
	}
	return (philo);
}

int	is_number(char *str)
{
	if (!str || *str == '\0') 
		return (0);
	if (*str == '+')
		str++;
	if(*str == '-')
		exit(EXIT_FAILURE);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

//number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

t_data *init_data(t_data *data, char **av)
{
	int i;
	
	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	data->ttd = ft_atoi(av[2]); // Keep in milliseconds
	data->philos = ft_atoi(av[1]); // how many philos 3ana
	data->tte = ft_atoi(av[3]); // Keep in milliseconds  
	data->tts = ft_atoi(av[4]); // Keep in milliseconds
	if(av[5])
		data->eat_counter = ft_atoi(av[5]);
	else
		data->eat_counter = -1; // No limit if not specified
	data->rip = 0;
	data->all_threads_ready = 0;  // Boolean flag
	data->ready_count = 0;        // Counter for ready threads
	data->philos_full_count = 0;  // Count of full philosophers
	data->start_timer = get_time(); // Initialize start time
	
	// Initialize synchronization mutexes
	pthread_mutex_init(&data->ready_mutex, NULL);
	pthread_cond_init(&data->ready_cond, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->full_count_mutex, NULL);
	pthread_mutex_init(&data->rip_mutex, NULL);
	
	// Initialize forks array
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos);
	if (!data->forks)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < data->philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}

	return (data);
}

void valid_input(t_data *data, char **av, int ac)
{
	if((data->philos > 200 || data->tte > INT_MAX || data->tts > INT_MAX || data->ttd > INT_MAX))
	{
		cleaner(data, NULL);  // Pass NULL instead of uninitialized philo
		exit(EXIT_FAILURE);
	}
	int i = 1;  // Start from 1, not 0 (av[0] is program name)
	while(i < ac)
	{
		if(!is_number(av[i]))
		{
			cleaner(data, NULL);  // Pass NULL instead of uninitialized philo
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

int main(int ac, char **av)
{
	t_data 	*data = NULL;  // Initialize to NULL
	t_philo	*philo = NULL; // Initialize to NULL
	
	if(ac == 5 || ac == 6)
	{
		data = init_data(data ,av); // parse and init the data
		if(data->philos < 1 || data->tte < 0 || data->tts < 0 || data->ttd < 0)
		{
			cleaner(data, philo);
			exit(EXIT_FAILURE);
		}
		philo = philo_init(data);
		valid_input(data,av, ac);// validate the input
		final_supper(data, philo);
	}
	else
		exit(EXIT_FAILURE);
	return 0;
}