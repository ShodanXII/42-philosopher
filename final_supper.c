#include "philo.h"

// Thread synchronization - wait for all threads to be ready
void wait_for_all_threads(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->ready_mutex);
	
	philo->data->ready_count++;
	
	// If this is the last thread to be ready
	if (philo->data->ready_count == philo->data->philos)
	{
		philo->data->all_threads_ready = 1;
		philo->data->start_timer = get_time(); // Reset start time
		pthread_cond_broadcast(&philo->data->ready_cond);
	}
	else
	{
		// Wait for all threads to be ready
		while (!philo->data->all_threads_ready)
			pthread_cond_wait(&philo->data->ready_cond, &philo->data->ready_mutex);
	}
	
	pthread_mutex_unlock(&philo->data->ready_mutex);
}

// Print philosopher actions (thread-safe)
void print_action(t_philo *philo, char *action)
{
	long timestamp;
	
	pthread_mutex_lock(&philo->data->print_mutex);
	
	// Check if simulation is still running
	if (philo->data->rip)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return;
	}
	
	timestamp = get_time() - philo->data->start_timer;
	printf("%ld %d %s\n", timestamp, philo->id, action);
	
	pthread_mutex_unlock(&philo->data->print_mutex);
}

// Philosopher eating routine (fixed deadlock prevention)
void philo_eat(t_philo *philo)
{
	// Deadlock prevention: lower ID philosopher takes left fork first
	// higher ID philosopher takes right fork first
	if (philo->id % 2 == 0)
	{
		// Even ID: take right fork first
		pthread_mutex_lock(philo->r_forks);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_forks);
		print_action(philo, "has taken a fork");
	}
	else
	{
		// Odd ID: take left fork first
		pthread_mutex_lock(philo->l_forks);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_forks);
		print_action(philo, "has taken a fork");
	}
	
	// Start eating
	print_action(philo, "is eating");
	
	pthread_mutex_lock(&philo->locker);
	philo->last_meal = get_time();
	philo->meals_count++;
	
	// Check if reached max meals
	if (philo->data->eat_counter != -1 && 
		philo->meals_count >= philo->data->eat_counter &&
		!philo->maxim_eaten)
	{
		philo->maxim_eaten = 1;
		
		// Increment global full count atomically
		pthread_mutex_lock(&philo->data->full_count_mutex);
		philo->data->philos_full_count++;
		pthread_mutex_unlock(&philo->data->full_count_mutex);
	}
	pthread_mutex_unlock(&philo->locker);
	
	// Sleep for eating time (convert milliseconds to microseconds for usleep)
	usleep(philo->data->tte * 1000);
	
	// Release forks in reverse order
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->l_forks);
		pthread_mutex_unlock(philo->r_forks);
	}
	else
	{
		pthread_mutex_unlock(philo->r_forks);
		pthread_mutex_unlock(philo->l_forks);
	}
}

// Main philosopher routine
void *dinner(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	
	// Wait for all threads to be ready
	wait_for_all_threads(philo);
	
	// Main simulation loop
	while (!philo->data->rip && !philo->maxim_eaten)
	{
		// Eat
		philo_eat(philo);
		
		// Sleep
		print_action(philo, "is sleeping");
		usleep(philo->data->tts * 1000); // Convert ms to microseconds
		
		// Think
		print_action(philo, "is thinking");
	}
	
	return NULL;
}

// Structure to pass both data and philos to monitor
typedef struct s_monitor_args {
	t_data *data;
	t_philo *philos;
} t_monitor_args;

// Monitor for philosopher deaths (PROPERLY IMPLEMENTED)
void *monitor_deaths(void *arg)
{
	t_monitor_args *args = (t_monitor_args *)arg;
	t_data *data = args->data;
	t_philo *philos = args->philos;
	int i;
	long current_time;
	long time_since_last_meal;
	
	while (!data->rip)
	{
		// First check if all philosophers have eaten enough
		if (data->eat_counter != -1)
		{
			pthread_mutex_lock(&data->full_count_mutex);
			if (data->philos_full_count >= data->philos)
			{
				// All philosophers have eaten enough - end simulation cleanly
				pthread_mutex_lock(&data->print_mutex);
				printf("reached final meal\n");
				pthread_mutex_unlock(&data->print_mutex);
				data->rip = 1;
				pthread_mutex_unlock(&data->full_count_mutex);
				break;
			}
			pthread_mutex_unlock(&data->full_count_mutex);
		}
		
		// Then check for deaths
		current_time = get_time();
		i = 0;
		
		while (i < data->philos && !data->rip)
		{
			pthread_mutex_lock(&philos[i].locker);
			time_since_last_meal = current_time - philos[i].last_meal;
			pthread_mutex_unlock(&philos[i].locker);
			
			// Check if this philosopher died
			if (time_since_last_meal > data->ttd)
			{
				pthread_mutex_lock(&data->print_mutex);
				if (!data->rip)
				{
					printf("%ld %d died\n", current_time - data->start_timer, philos[i].id);
					data->rip = 1;
				}
				pthread_mutex_unlock(&data->print_mutex);
				break;
			}
			i++;
		}
		
		usleep(5000); // Check every 5ms to reduce CPU overhead
	}
	
	return NULL;
}

void final_supper(t_data *data, t_philo *philo)
{
	int i;
	pthread_t monitor;
	t_monitor_args monitor_args;
	
	// Handle edge cases
	if (data->eat_counter == 0)
		return;
	if (data->philos == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(data->ttd * 1000); // Convert to microseconds
		printf("%ld 1 died\n", data->ttd);
		return;
	}
	
	// Setup monitor arguments
	monitor_args.data = data;
	monitor_args.philos = philo;
	
	// Create death monitor thread
	if (pthread_create(&monitor, NULL, monitor_deaths, &monitor_args) != 0)
	{
		cleaner(data, philo);
		exit(EXIT_FAILURE);
	}
	
	// Create philosopher threads
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
	
	// Wait for all philosopher threads to finish
	i = 0;
	while (i < data->philos)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	
	// Stop monitor and wait for it
	data->rip = 1;
	pthread_join(monitor, NULL);
}

