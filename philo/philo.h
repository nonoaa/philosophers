#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

struct  s_info;

typedef struct  s_philo
{
    int             num;
    struct s_info   *info;
    int             eat_cnt;
    size_t          count_time;
    int             full;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
}               t_philo;

typedef struct  s_info
{
    int             philo_num;
    int             time_die;
    int             time_eat;
    int             time_sleep;
    int             must_eat_num;
    size_t          start_time;
    int             full_count;
    int             end;
    t_philo         *philo;
    pthread_mutex_t *mutex_forks;
    pthread_mutex_t mutex_main;
    pthread_mutex_t mutex_print;
}               t_info;

#endif