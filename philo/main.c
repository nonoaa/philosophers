/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: byahn <byahn@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:09:43 by byahn             #+#    #+#             */
/*   Updated: 2022/04/05 17:09:51 by byahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_and_destroy_all(t_info *info)
{
	int	i;

	i = 0;
	pthread_mutex_unlock(&(info->mutex_death));
	while (i < info->philo_num)
		pthread_mutex_destroy(&(info->mutex_forks[i++]));
	pthread_mutex_destroy(&(info->mutex_main));
	pthread_mutex_destroy(&(info->mutex_death));
	pthread_mutex_destroy(&(info->mutex_print));
	pthread_mutex_destroy(&(info->mutex_count));
	free(info->mutex_forks);
	free(info->philo);
}

int	main(int argc, char *argv[])
{
	int		i;
	int		argv_num[5];
	t_info	info;

	if (argc < 5 || argc > 6)
		return (print_error("error: wrong argument!\n"));
	i = 0;
	while (i < argc - 1)
	{
		argv_num[i] = ft_atoi(argv[i + 1]);
		if (argv_num[i] == 0)
			return (print_error("error: wrong argument!!\n"));
		i++;
	}
	if (init_info(&info, argc, argv_num))
		return (print_error("error: info initializing failed"));
	if (run_thread(&info))
	{
		free_and_destroy_all(&info);
		return (print_error("error: run thread failed"));
	}
	if (pthread_mutex_lock(&(info.mutex_main)) == 0)
		if (pthread_mutex_unlock(&(info.mutex_main)) == 0)
			free_and_destroy_all(&info);
	return (0);
}
