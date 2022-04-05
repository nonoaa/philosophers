/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behave.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:09:56 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:13:51 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_get_forks(t_philo *philo)
{
	if (pthread_mutex_lock(philo->r_fork) == 0)
		print_message(philo, "has taken a fork");
	else
		pthread_mutex_unlock(&(philo->info->mutex_main));
	if (pthread_mutex_lock(philo->l_fork) == 0)
		print_message(philo, "has taken a fork");
	else
		pthread_mutex_unlock(&(philo->info->mutex_main));
}

void	philo_eat(t_philo *philo)
{
	philo->count_time = get_time_ms();
	print_message(philo, "is eating.....");
	while (get_time_ms() - philo->count_time < (size_t)(philo->info->time_eat))
		usleep(300);
	(philo->eat_cnt)++;
	if (philo->eat_cnt == philo->info->must_eat_num && philo->full == 0)
	{
		if (pthread_mutex_lock(&(philo->info->mutex_count)) == 0)
		{
			philo->full = 1;
			(philo->info->full_count)++;
			pthread_mutex_unlock(&(philo->info->mutex_count));
		}
	}
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	philo_sleep(t_philo *philo)
{
	size_t	start_time;

	start_time = get_time_ms();
	print_message(philo, "is sleeping...");
	while (get_time_ms() - start_time < (size_t)(philo->info->time_sleep))
		usleep(500);
}

void	philo_think(t_philo *philo)
{
	print_message(philo, "is thinking...");
}
