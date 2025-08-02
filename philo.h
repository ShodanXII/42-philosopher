/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:40:17 by achat             #+#    #+#             */
/*   Updated: 2025/07/31 20:40:17 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_philo	t_philo;
typedef struct s_data	t_data;

struct s_data
{
	long			philos_nb;
	long			ttd;
	long			tte;
	long			tts;
	char			rip;
	long			start_timer;
	long			eat_counter;
	long			philos_full_count;
	pthread_mutex_t	*forks;
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

int		validate_numeric(char *str);
int		ft_isdigit(char c);
int		ft_atoi(const char *str);
long	current_timestamp(void);
void	cleanup_resources(t_data *data, t_philo *philo);
void	destroy_mutexes(t_data *data);
void	free_philosophers(t_philo *philo, int philo_count);
t_philo	*create_philosophers(t_data *data);
void	setup_philosopher(t_philo *philo, t_data *data, int i);
void	distribute_utensils(t_philo *philo, t_data *data, int i);
t_data	*init_data(t_data *data, char **av);
void	process_parameters(t_data *data, char **av);
void	create_mutexes(t_data *data);
void	prepare_utensils(t_data *data);
void	validate_arguments(t_data *data, char **av, int ac);
void	final_supper(t_data *data, t_philo *philo);
int		display_error(char *err);
void	announce_action(t_data *data, int philo_id, char *state);
void	precise_sleep(long duration, t_data *data);
int		should_terminate(t_philo *philo);
void	acquire_utensils(t_philo *philo);
void	consume_meal(t_philo *philo);
void	drop_utensils(t_philo *philo);
void	one_philo(t_philo *philo);
void	*philosopher_lifecycle(void *arg);
void	*observe_philosophers(void *arg);
void	rest_philosopher(t_philo *philo);
int		detect_starvation(t_philo *philo, t_data *data);
void	contemplate(t_philo *philo);
int		verify_all_fed(t_data *data);
void	create_mutexes(t_data *data);
void	*lonely_philosopher_routine(void *arg);

#endif