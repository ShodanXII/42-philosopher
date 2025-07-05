#include "philo.h"

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
		philo[i].last_meal = 0;
		philo[i].data = data;
		philo[i].l_forks = &data->forks[i];
		philo[i].r_forks = &data->forks[(i + 1) % data->philos];
		pthread_mutex_init(&philo[i].locker, NULL);
		i++;
	}
	return (philo);
}

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_number(char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+' || *str == '-')
		str++;
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
	data = malloc(sizeof(t_data));
	data->ttd = ft_atoi(av[2]);
	data->philos = ft_atoi(av[1]);
	data->tte = ft_atoi(av[3]);
	data->tts = ft_atoi(av[4]);
	if(av[5])
		data->eat_counter = ft_atoi(av[5]);
	data->rip = 0;
	return (data);
}

void valid_input(t_data *data, char **av, int ac)
{
	if((data->philos > 200 || data->tte > INT_MAX || data->tts > INT_MAX || data->ttd > INT_MAX))
		exit(EXIT_FAILURE);
	int i = 0;
	while(i < ac)
	{
		if(!is_number(av[i]))
			exit(EXIT_FAILURE);
		i++;
	}
	printf("failed\n");
}

int main(int ac, char **av)
{
	t_data 	*data;
	t_philo	*philo;
	int i = 0;
	if(ac == 5 || ac == 6)
	{
		data = init_data(data ,av);
		if(data->philos < 1 || data->tte < 0 || data->tts < 0 || data->ttd < 0)
			exit(EXIT_FAILURE); // zid cleaner
		philo = philo_init(data);
		valid_input(data,av, ac);
		sim(data);
	}
	else
		exit(EXIT_FAILURE);
	return 0;
}