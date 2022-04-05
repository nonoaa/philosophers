/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:10:06 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:21:19 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

struct	s_info;

typedef struct s_philo
{
	int				num;
	struct s_info	*info;
	int				eat_cnt;
	size_t			count_time;
	int				full;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}					t_philo;

typedef struct s_info
{
	int				philo_num;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat_num;
	size_t			start_time;
	int				full_count;
	int				end;
	t_philo			*philo;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_main;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_death;
	pthread_mutex_t	mutex_count;
}					t_info;

void	*check_philos(void *info_void);
void	philo_get_forks(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
size_t	get_time_ms(void);
void	print_message(t_philo *philo, char *str);
int		ft_atoi(const char *str);
int		print_error(char *str);
int		init_info(t_info *info, int argc, int *argv_num);
int		init_mutexes(t_info *info);
int		run_thread(t_info *info);

#endif
