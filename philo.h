#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct  s_info
{
    int     philo_num;
    int     time_die;
    int     time_eat;
    int     time_sleep;
    int     must_eat_num;

}               t_info;

#endif