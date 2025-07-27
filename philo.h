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
	long			philos_nb;
	long			ttd;
	long			tte;
	long			tts;
	char			rip;
	long			start_timer;
	long			eat_counter;
	char			all_threads_ready;
	long			ready_count;
	long			philos_full_count;
	pthread_mutex_t	*forks;
	pthread_mutex_t	ready_mutex;
	pthread_cond_t	ready_cond;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	full_count_mutex;
	pthread_mutex_t	rip_mutex;
	struct s_philo	*philos;
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

int		is_number(char *str);
int		ft_isdigit(char c);
int		ft_atoi(const char *str);
long	get_time(void);
void	cleaner(t_data *data, t_philo *philo);
void	clean_mutexes(t_data *data);
void	clean_philos(t_philo *philo, int philo_count);
t_philo	*philo_init(t_data *data);
void	init_single_philo(t_philo *philo, t_data *data, int i);
void	assign_forks(t_philo *philo, t_data *data, int i);
t_data	*init_data(t_data *data, char **av);
void	parse_arguments(t_data *data, char **av);
void	init_mutexes(t_data *data);
void	init_forks(t_data *data);
void	valid_input(t_data *data, char **av, int ac);
void	final_supper(t_data *data, t_philo *philo);
int		print_error(char *err);
void	print_state(t_data *data, int philo_id, char *state);
void	ft_usleep(long duration, t_data *data);
int		check_stop(t_philo *philo);
void	pick_fork(t_philo *philo);
void	eat(t_philo *philo);
void	release_fork(t_philo *philo);
void	sleep_philo(t_philo *philo);
void	think(t_philo *philo);
int		check_meals_complete(t_data *data);
void	*single_routine(void *arg);

#endif