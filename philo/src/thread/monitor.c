/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsubo <y.otsubo.886@ms.saitama-u.ac.j    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 03:20:08 by yotsubo           #+#    #+#             */
/*   Updated: 2024/10/06 03:20:08 by yotsubo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static void	set_start(t_data *data)
{
	pthread_mutex_lock(&(data->flag_mtx->mtx));
	data->is_start = TRUE;
	data->start_time = get_time();
	pthread_mutex_unlock(&(data->flag_mtx->mtx));
}

static void	*set_finish(t_data *data)
{
	pthread_mutex_lock(&(data->flag_mtx->mtx));
	data->is_finish = TRUE;
	pthread_mutex_unlock(&(data->flag_mtx->mtx));
	return (NULL);
}

void	set_all_philo_dead(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_of_philo)
		data->philos[i++]->is_dead = TRUE;
}

static t_bool	is_philo_dead(t_data *data, size_t i)
{
	long long	now;

	now = get_time();
	if (now - data->philos[i]->last_eat >= data->philos[i]->time_to_die)
	{
		all_philos_mutex_lock(data, i);
		set_all_philo_dead(data);
		died(data, i, now);
		return (TRUE);
	}
	return (FALSE);
}

void	all_philos_mutex_lock(t_data *data, size_t num)
{
	size_t	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		if (i == num)
			continue ;
		pthread_mutex_lock(&(data->count_mtxs[i]->mtx));
		i++;
	}
}

void	all_philos_mutex_unlock(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_of_philo)
		pthread_mutex_unlock(&(data->count_mtxs[i++]->mtx));
}

void	died(t_data *data, size_t i, long long now)
{
	pthread_mutex_lock(&(data->msg_mtx->mtx));
	printf(RESET);
	printf("%lld %zu died\n", get_time() - now, i);
	printf(RED);
	pthread_mutex_unlock(&(data->msg_mtx->mtx));
}

void	*monitor(void *arg)
{
	t_data	*data;
	size_t	i;
	size_t	totla_eat;

	data = (t_data *)arg;
	set_start(data);
	while (1)
	{
		i = 0;
		totla_eat = 0;
		while (i < (size_t)data->num_of_philo)
		{
			pthread_mutex_lock(&(data->count_mtxs[i]->mtx));
			if (is_philo_dead(data, i))
			{
				all_philos_mutex_unlock(data);
				return (set_finish(data));
			}
			if (data->must_eat > 0 && data->philos[i]->eat_count >= data->must_eat)
				totla_eat++;
			pthread_mutex_unlock(&(data->count_mtxs[i]->mtx));
			i++;
		}
		if (totla_eat == (size_t)data->num_of_philo)
			return (set_finish(data));
	}
}