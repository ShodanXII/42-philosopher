#include "philo.h"

void wait_for_all_threads(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->ready_mutex);
	
	philo->data->ready_count++;
	
	if (philo->data->ready_count == philo->data->philos)
	{
		philo->data->all_threads_ready = 1;
		philo->data->start_timer = get_time();
		pthread_cond_broadcast(&philo->data->ready_cond);
	}
	else
	{
		while (!philo->data->all_threads_ready)
			pthread_cond_wait(&philo->data->ready_cond, &philo->data->ready_mutex);
	}
	
	pthread_mutex_unlock(&philo->data->ready_mutex);
}

void print_action(t_philo *philo, char *action)
{
	long timestamp;
	int should_print = 0;
	
	pthread_mutex_lock(&philo->data->print_mutex);
	
	pthread_mutex_lock(&philo->data->rip_mutex);
	if (!philo->data->rip)
		should_print = 1;
	pthread_mutex_unlock(&philo->data->rip_mutex);
	
	if (should_print)
	{
		timestamp = get_time() - philo->data->start_timer;
		printf("%ld %d %s\n", timestamp, philo->id, action);
	}
	
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void philo_eat(t_philo *philo)
{
	pthread_mutex_t *first_fork, *second_fork;
	
	if (philo->l_forks < philo->r_forks)
	{
		first_fork = philo->l_forks;
		second_fork = philo->r_forks;
	}
	else
	{
		first_fork = philo->r_forks;
		second_fork = philo->l_forks;
	}
	
	pthread_mutex_lock(first_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_action(philo, "has taken a fork");
	
	print_action(philo, "is eating");
	
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = get_time();
	philo->meals_count++;
	
	if (philo->data->eat_counter != -1 && 
		philo->meals_count >= philo->data->eat_counter &&
		!philo->maxim_eaten)
	{
		philo->maxim_eaten = 1;
		
		pthread_mutex_lock(&philo->data->full_count_mutex);
		philo->data->philos_full_count++;
		pthread_mutex_unlock(&philo->data->full_count_mutex);
	}
	pthread_mutex_unlock(&philo->locker);
	
	usleep(philo->data->tte * 1000);
	
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void *dinner(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	int simulation_running = 1;
	
	wait_for_all_threads(philo);
	
	if (philo->id % 2 == 0)
		usleep(50 * 1000);
	
	while (simulation_running && !philo->maxim_eaten)
	{
		pthread_mutex_lock(&philo->data->rip_mutex);
		simulation_running = !philo->data->rip;
		pthread_mutex_unlock(&philo->data->rip_mutex);
		
		if (!simulation_running || philo->maxim_eaten)
			break;
		
		philo_eat(philo);
		
		print_action(philo, "is sleeping");
		usleep(philo->data->tts * 1000);
		
		print_action(philo, "is thinking");
	}
	
	return NULL;
}

typedef struct s_monitor_args {
	t_data *data;
	t_philo *philos;
} t_monitor_args;

void *monitor_deaths(void *arg)
{
	t_monitor_args *args = (t_monitor_args *)arg;
	t_data *data = args->data;
	t_philo *philos = args->philos;
	int i;
	long current_time;
	long time_since_last_meal;
	int simulation_running = 1;
	
	while (simulation_running)
	{
		pthread_mutex_lock(&data->rip_mutex);
		simulation_running = !data->rip;
		pthread_mutex_unlock(&data->rip_mutex);
		
		if (!simulation_running)
			break;
			
		if (data->eat_counter != -1)
		{
			pthread_mutex_lock(&data->full_count_mutex);
			if (data->philos_full_count >= data->philos)
			{
				pthread_mutex_lock(&data->print_mutex);
				printf("reached final meal\n");
				pthread_mutex_unlock(&data->print_mutex);
				
				pthread_mutex_lock(&data->rip_mutex);
				data->rip = 1;
				pthread_mutex_unlock(&data->rip_mutex);
				
				pthread_mutex_unlock(&data->full_count_mutex);
				break;
			}
			pthread_mutex_unlock(&data->full_count_mutex);
		}
		
		current_time = get_time();
		i = 0;
		
		while (i < data->philos)
		{
			pthread_mutex_lock(&data->rip_mutex);
			simulation_running = !data->rip;
			pthread_mutex_unlock(&data->rip_mutex);
			
			if (!simulation_running)
				break;
				
			pthread_mutex_lock(&philos[i].locker);
			time_since_last_meal = current_time - philos[i].last_meal;
			pthread_mutex_unlock(&philos[i].locker);
			
			if (time_since_last_meal > data->ttd)
			{
				pthread_mutex_lock(&data->print_mutex);
				pthread_mutex_lock(&data->rip_mutex);
				if (!data->rip)
				{
					printf("%ld %d died\n", current_time - data->start_timer, philos[i].id);
					data->rip = 1;
				}
				pthread_mutex_unlock(&data->rip_mutex);
				pthread_mutex_unlock(&data->print_mutex);
				break;
			}
			i++;
		}
		
		usleep(5000);
	}
	
	return NULL;
}

void final_supper(t_data *data, t_philo *philo)
{
	int i;
	pthread_t monitor;
	t_monitor_args monitor_args;
	
	if (data->eat_counter == 0)
		return;
	if (data->philos == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(data->ttd * 1000);
		printf("%ld 1 died\n", data->ttd);
		return;
	}
	
	monitor_args.data = data;
	monitor_args.philos = philo;
	
	if (pthread_create(&monitor, NULL, monitor_deaths, &monitor_args) != 0)
	{
		cleaner(data, philo);
		exit(EXIT_FAILURE);
	}
	
	i = 0;
	while (i < data->philos)
	{
		if (pthread_create(&philo[i].thread, NULL, dinner, &philo[i]) != 0)
		{
			cleaner(data, philo);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	
	i = 0;
	while (i < data->philos)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	
	pthread_mutex_lock(&data->rip_mutex);
	data->rip = 1;
	pthread_mutex_unlock(&data->rip_mutex);
	pthread_join(monitor, NULL);
}

