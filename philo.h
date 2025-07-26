#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>

typedef struct s_philo t_philo;
typedef struct s_data t_data;

struct s_data
{
	long			philos;
	long			ttd;
	long			tte;
	long			tts;
	char			rip;
	long			start_timer;
	long			eat_counter;
	char			all_threads_ready;
	long			ready_count;
	pthread_mutex_t	*forks;
	pthread_mutex_t	ready_mutex;
	pthread_cond_t	ready_cond;
};

struct s_philo
{
	int				id;
	long			last_meal;
	long			meals_count;
	char			maxim_eaten;
	char			threads_read;
	t_data			*data;
	pthread_t		thread;
	pthread_mutex_t	*l_forks;
	pthread_mutex_t	*r_forks;
	pthread_mutex_t	locker;
};

int	is_number(char *str);
int	ft_isdigit(char c);
int	ft_atoi(const char *str);
long	get_time(void);

#endif