#include "philo.h"

int	threads(t_philo *philo)
{

}

//  the dinner sim
void	dinner(t_philo *philo)
{

	if(threads(philo))
		 
}


void    final_supper(t_data *data, t_philo *philo)
{
	int i;

	i = 0;

	if(0 >= data->eat_counter)
	   return;
	else if(data->philos == 1)
		return;
	while(i < data->philos)
	{
		if (pthread_create(&philo[i].thread, NULL, dinner, &philo[i]) != 0)
		{
			cleaner(data, philo);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

