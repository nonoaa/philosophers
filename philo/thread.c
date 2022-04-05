/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:10:10 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:22:52 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_behave(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	philo->count_time = philo->info->start_time;
	while (philo->info->end == 0)
	{
		philo_get_forks(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
		usleep(500);
	}
	return ((void *)0);
}

int	run_philo(t_info *info, int p_num)
{
	t_philo		*p;
	pthread_t	tid;

	while (p_num < info->philo_num)
	{
		p = &(info->philo[p_num]);
		if (pthread_create(&tid, NULL, &philo_behave, p))
			return (1);
		pthread_detach(tid);
		p_num += 2;
	}
	if (p_num % 2 == 0)
		usleep(500 * (info->time_eat));
	return (0);
}

int	run_thread(t_info *info)
{
	pthread_t	tid;

	info->start_time = get_time_ms();
	if (run_philo(info, 0) || run_philo(info, 1))
		return (1);
	if (pthread_create(&tid, NULL, &check_philos, info))
		return (1);
	return (0);
}
