/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:09:31 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:08:49 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_eat_cnt(t_info *info)
{
	if (info->full_count >= info->philo_num)
	{
		if (pthread_mutex_lock(&(info->mutex_print)) == 0)
		{
			info->end = 1;
			printf("===========================================\n");
			printf("All philosophers finished eating\n");
			pthread_mutex_unlock(&(info->mutex_main));
			return (0);
		}
		return (1);
	}
	return (0);
}

int	check_death(t_info *info, int i)
{
	char	*str;

	if (get_time_ms() - info->philo[i].count_time > (size_t)(info->time_die))
	{
		if (pthread_mutex_lock(&(info->mutex_death)) == 0)
		{
			if (pthread_mutex_lock(&(info->mutex_print)) == 0)
			{
				str = "died............";
				info->end = 1;
				printf("===========================================\n");
				printf("%lums\t[%d]\t[%s]\t[%d]\n",
					get_time_ms() - info->start_time,
					info->philo->num, str, info->philo->eat_cnt);
				usleep(300);
				pthread_mutex_unlock(&(info->mutex_main));
				return (1);
			}
		}
	}
	return (0);
}

void	*check_philos(void *info_void)
{
	t_info	*info;
	int		i;

	info = (t_info *)info_void;
	i = 0;
	while (1)
	{
		if (info->end == 1)
			return (0);
		if (i >= info->philo_num)
			i = 0;
		if (check_eat_cnt(info))
			return ((void *)1);
		if (check_death(info, i))
			return (0);
		i++;
		usleep(300);
	}
}
