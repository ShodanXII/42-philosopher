#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>

typedef struct s_philo t_philo;
typedef struct s_data t_data;

struct s_data
{
	long	philos;
	long	ttd;
	long	tte;
	long	tts;
	int		rip;
	long	start_timer;
	long	eat_counter;
	pthread_mutex_t	*forks;
};

struct s_philo
{
	int				id;
	long			meals_count;
	long			last_meal;
	t_data			*data;
	pthread_t		thread;
	pthread_mutex_t	*l_forks;
	pthread_mutex_t	*r_forks;
	pthread_mutex_t	locker;
};


int	is_number(char *str);
int	ft_isdigit(char c);

#endif