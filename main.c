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
        if (result > 2147483648)
            return (0);
        i++;
    }
    if (str[i] != '\0')
        return (0);
    return (result);
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
    
}

int main(int argc, char *argv[])
{
    int     i;
    int     argv_num[5];
    t_info  info;

    if (argc < 5 || argc > 6)
    {
        return (print_error("error: wrong argument!\n"));
    }
    i = 0;
    while (i < argc - 1)
    {
        argv_num[i] = ft_atoi(argv[i + 1]);
        if (argv_num[i] == 0)
            return (print_error("error: wrong argument!!\n"));
        i++;
    }
    if (init_info(&info, argc, argv_num) == 1)
        return (1);
    
    return (0);
}