/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:09:51 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:12:16 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philo *philo, char *str)
{
	if (philo->info->end)
		return ;
	if (pthread_mutex_lock(&(philo->info->mutex_print)) == 0)
	{
		printf("%lums\t[%d]\t[%s]\t[%d]\n",
			get_time_ms() - philo->info->start_time,
			philo->num, str, philo->eat_cnt);
		pthread_mutex_unlock(&(philo->info->mutex_print));
	}
}

int	print_error(char *str)
{
	printf("%s", str);
	return (1);
}

int	ft_atoi(const char *str)
{
	long	result;
	size_t	i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		result = result * 10 + str[i] - '0';
		if (result > (long)2147483648)
			return (0);
		i++;
	}
	if (str[i] != '\0')
		return (0);
	return (result);
}

size_t	get_time_ms(void)
{
	struct timeval	tv;
	size_t			ms;

	gettimeofday(&tv, NULL);
	ms = (size_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}
