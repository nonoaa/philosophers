/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:10:01 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:18:03 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	malloc_philos_forks(t_info *info)
{
	info->philo = (t_philo *)malloc(sizeof(t_philo) * info->philo_num);
	if (info->philo == NULL)
		return (1);
	info->mutex_forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* (info->philo_num));
	if (info->mutex_forks == NULL)
	{
		free(info->philo);
		return (1);
	}
	return (0);
}

int	init_philos_forks(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_num)
	{
		info->philo[i].num = i + 1;
		info->philo[i].eat_cnt = 0;
		info->philo[i].info = info;
		info->philo[i].full = 0;
		info->philo[i].r_fork = &(info->mutex_forks[i]);
		if (i + 1 == info->philo_num)
			info->philo[i].l_fork = &(info->mutex_forks[0]);
		else
			info->philo[i].l_fork = &(info->mutex_forks[i + 1]);
		if (pthread_mutex_init(&(info->mutex_forks[i]), NULL))
			return (1);
		i++;
	}
	return (0);
}

int	init_mutexes(t_info *info)
{
	if (pthread_mutex_init(&(info->mutex_main), NULL))
		return (1);
	if (pthread_mutex_init(&(info->mutex_print), NULL))
		return (1);
	if (pthread_mutex_init(&(info->mutex_death), NULL))
		return (1);
	if (pthread_mutex_init(&(info->mutex_count), NULL))
		return (1);
	pthread_mutex_lock(&(info->mutex_main));
	return (0);
}

int	init_info(t_info *info, int argc, int *argv_num)
{
	info->philo_num = argv_num[0];
	info->time_die = argv_num[1];
	info->time_eat = argv_num[2];
	info->time_sleep = argv_num[3];
	if (argc == 6)
		info->must_eat_num = argv_num[4];
	else
		info->must_eat_num = -1;
	info->end = 0;
	info->full_count = 0;
	if (malloc_philos_forks(info))
		return (1);
	if (init_philos_forks(info))
		return (1);
	if (init_mutexes(info))
		return (1);
	printf("TIME\tPHILO\tSTATUS\t\t\tEAT_CNT\n");
	printf("===============================================\n");
	return (0);
}
