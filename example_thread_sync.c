// Example of how to use the thread synchronization

#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;
    
    // 1. Signal that this thread is ready
    pthread_mutex_lock(&data->ready_mutex);
    data->ready_count++;
    
    // 2. Check if all threads are ready
    if (data->ready_count == data->philos)
    {
        data->all_threads_ready = 1;  // Set boolean flag
        data->start_timer = get_time(); // Reset start time for accurate timing
        pthread_cond_broadcast(&data->ready_cond); // Wake up all waiting threads
    }
    
    // 3. Wait until all threads are ready
    while (!data->all_threads_ready)
    {
        pthread_cond_wait(&data->ready_cond, &data->ready_mutex);
    }
    pthread_mutex_unlock(&data->ready_mutex);
    
    // 4. Now ALL threads start the simulation simultaneously
    philo->last_meal = data->start_timer; // Update last meal time
    
    // 5. Main philosopher loop (eating, sleeping, thinking)
    while (!data->rip && !philo->maxim_eaten)
    {
        // Eating logic
        // Sleeping logic  
        // Thinking logic
    }
    
    return NULL;
}

// In your final_supper function:
void final_supper(t_data *data, t_philo *philo)
{
    int i = 0;
    
    // Create all philosopher threads
    while (i < data->philos)
    {
        pthread_create(&philo[i].thread, NULL, philosopher_routine, &philo[i]);
        i++;
    }
    
    // Wait for all threads to complete
    i = 0;
    while (i < data->philos)
    {
        pthread_join(philo[i].thread, NULL);
        i++;
    }
}
