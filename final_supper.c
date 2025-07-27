#include "philo.h"

int	check_meals_complete(t_data *data)
{
	int	completed_meals;
	int	i;

	completed_meals = 1;
	if (data->eat_counter <= 0)
		return (0);
	pthread_mutex_lock(&data->full_count_mutex);
	i = 0;
	while (i < data->philos_nb)
	{
		if (data->philos_full_count < data->philos_nb)
		{
			completed_meals = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->full_count_mutex);
	if (completed_meals)
	{
		pthread_mutex_lock(&data->rip_mutex);
		data->rip = 1;
		pthread_mutex_unlock(&data->rip_mutex);
		return (1);
	}
	return (0);
}

void	*single_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_state(philo->data, philo->id, "is thinking");
	print_state(philo->data, philo->id, "has taken a fork");
	ft_usleep(philo->data->ttd, philo->data);
	print_state(philo->data, philo->id, "died");
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->locker);
	think(philo);
	if (philo->id % 2 == 1)
		usleep((philo->data->tte / 2) * 1000);
	while (1)
	{
		if (check_stop(philo))
			return (NULL);
		pick_fork(philo);
		eat(philo);
		if (philo->data->eat_counter != -1 && 
			philo->meals_count >= philo->data->eat_counter &&
			!philo->maxim_eaten)
		{
			pthread_mutex_lock(&philo->locker);
			philo->maxim_eaten = 1;
			pthread_mutex_unlock(&philo->locker);
			pthread_mutex_lock(&philo->data->full_count_mutex);
			philo->data->philos_full_count++;
			pthread_mutex_unlock(&philo->data->full_count_mutex);
		}
		release_fork(philo);
		sleep_philo(philo);
		think(philo);
	}
	return (NULL);
}

int	check_philosopher_death(t_philo *philo, t_data *data)
{
	long	time_since_last_meal;

	pthread_mutex_lock(&philo->locker);
	time_since_last_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->locker);
	if (time_since_last_meal >= data->ttd)
	{
		pthread_mutex_lock(&data->rip_mutex);
		if (!data->rip)
		{
			print_state(data, philo->id, "died");
			data->rip = 1;
			pthread_mutex_unlock(&data->rip_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->rip_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_deaths(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->rip)
	{
		if (check_meals_complete(data))
			return (NULL);
		i = 0;
		while (i < data->philos_nb)
		{
			if (check_philosopher_death(&data->philos[i], data))
				return (NULL);
			i++;
			usleep(100);
		}
	}
	return (NULL);
}

void final_supper(t_data *data, t_philo *philo)
{
	int i;
	pthread_t monitor;
	
	if (data->eat_counter == 0)
		return;
	if (data->philos_nb == 1)
	{
		if (pthread_create(&philo[0].thread, NULL, single_routine, &philo[0]))
			return ;
		pthread_join(philo[0].thread, NULL);
		return;
	}
	
	data->start_timer = get_time();
	data->philos = philo;
	
	if (pthread_create(&monitor, NULL, monitor_deaths, data))
		return ;
	
	i = 0;
	while (i < data->philos_nb)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]))
			return ;
		i++;
	}
	
	i = 0;
	while (i < data->philos_nb)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	
	pthread_mutex_lock(&data->rip_mutex);
	data->rip = 1;
	pthread_mutex_unlock(&data->rip_mutex);
	pthread_join(monitor, NULL);
}

