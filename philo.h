#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "libft/libft.h"
#include <limits.h>


typedef struct s_data t_data;

struct s_data
{
	long	philos;
	long	ttd;
	long	tte;
	long	tts;
	int		rip;
	long	eat_counter;
};


int	is_number(char *str);
int	ft_isdigit(char c);

#endif