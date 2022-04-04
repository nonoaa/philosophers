#include "philo.h"

int print_error(char *str)
{
    printf("%s", str);

    return (1);
}

int ft_atoi(const char *str)
{
    long    result;
    size_t  i;

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
    size_t          ms;

	gettimeofday(&tv, NULL);
    ms = (size_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}

int malloc_philos_forks(t_info *info)
{
    info->philo = (t_philo *)malloc(sizeof(t_philo) * info->philo_num);
    if (info->philo == NULL)
        return (1);
    info->mutex_forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (info->philo_num));
    if (info->mutex_forks == NULL)
    {
        free(info->philo);
        return (1);
    }
    return (0);
}

int init_philos_forks(t_info *info)
{
    int i;

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

int init_mutexes(t_info *info)
{
    if (pthread_mutex_init(&(info->mutex_main), NULL))
        return (1);
    if (pthread_mutex_init(&(info->mutex_print), NULL))
        return (1);
    pthread_mutex_lock(&(info->mutex_main));
    return (0);
}

int init_info(t_info *info, int argc, int *argv_num)
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
    printf("TIME\tPHILO\tSTATUS\t\t\tEAT_CNT\n");
	printf("===============================================\n");
    return (0);
}

void print_message(t_philo *philo, char *str)
{
    if (pthread_mutex_lock(&(philo->info->mutex_print)) == 0)
    {
        printf("%llums\t[%d]\t[%s]\t[%d]\n", get_time_ms() - philo->info->start_time, philo->num, str, philo->eat_cnt);
        pthread_mutex_unlock(&(philo->info->mutex_print));
    }
}

void philo_get_forks(t_philo *philo)
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
	while (get_time_ms() - philo->count_time < philo->info->time_eat)
        usleep(100);
	(philo->eat_cnt)++;
	if (philo->eat_cnt == philo->info->must_eat_num && philo->full == 0)
	{
		philo->full = 1;
		(philo->info->full_count)++;
	}
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	philo_sleep(t_philo *philo)
{
    size_t  start_time;

    start_time = get_time_ms();
	print_message(philo, "is sleeping...");
	while (get_time_ms() - start_time < philo->info->time_sleep)
        usleep(100);
}

void	philo_think(t_philo *philo)
{
	print_message(philo, "is thinking...");
}

void *philo_behave(void *philo_void)
{
    t_philo *philo;

    philo = (t_philo *)philo_void;
    philo->count_time = philo->info->start_time;
    while (philo->info->end == 0)
    {
        philo_get_forks(philo);
        philo_eat(philo);
        philo_sleep(philo);
        philo_think(philo);
        usleep(100);
    }
    return ((void *)0);
}


int run_philo(t_info *info, int p_num)
{
    t_philo     *p;
    pthread_t   tid;

    while (p_num < info->philo_num)
    {
        p = &(info->philo[p_num]);
        if (pthread_create(&tid, NULL, &philo_behave, p))
            return (1);
        pthread_detach(tid);
        p_num += 2;
    }
    if (p_num % 2 == 0)
        usleep(500 * info->time_eat);
    return (0);
}

int check_eat_cnt(t_info *info)
{
    if (info->full_count >= info->philo_num)
    {
        if (pthread_mutex_lock(&(info->mutex_print)) == 0)
        {
            printf("===========================================\n");
            printf("All philosophers finished eating\n");
            usleep(100);
            pthread_mutex_unlock(&(info->mutex_main));
            return (0);
        }
        return (1);
    }
    return (0);
}

int check_death(t_info *info, int i)
{
    if (get_time_ms() - info->philo[i].count_time > info->time_die)
    {
        if (pthread_mutex_lock(&(info->mutex_print)) == 0)
        {
            char    *str;
            
            str = "died............";
            printf("===========================================\n");
            printf("%llums\t[%d]\t[%s]\t[%d]\n", get_time_ms() - info->start_time, info->philo->num, str, info->philo->eat_cnt);
            usleep(100);
            pthread_mutex_unlock(&(info->mutex_main));
            return (1);
        }
    }
    return (0);
}

void *check_philos(void *info_void)
{
    t_info  *info;
    int     i;

    info = (t_info *)info_void;
    i = 0;
    while (1)
    {
        if (i >= info->philo_num)
            i = 0;
        if (check_eat_cnt(info))
            return ((void *)1);
        if (check_death(info, i))
            return (0);
        i++;
        usleep(100);
    }
}

int run_thread(t_info *info)
{
    pthread_t   tid;

    info->start_time = get_time_ms();
    if (run_philo(info, 0) || run_philo(info, 1))
        return (1);
    if (pthread_create(&tid, NULL, &check_philos, info))
        return (1);
    return (0);
}

void free_and_destroy_all(t_info *info)
{
    int i;

    i = 0;
    while (i < info->philo_num)
		pthread_mutex_destroy(&(info->mutex_forks[i++]));
	pthread_mutex_destroy(&(info->mutex_print));
	pthread_mutex_destroy(&(info->mutex_main));
	free(info->mutex_forks);
	free(info->philo);
}

int main(int argc, char *argv[])
{
    int     i;
    int     argv_num[5];
    t_info  info;

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
    if (init_mutexes(&info))
        return (print_error("error: mutex initializing failed"));
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