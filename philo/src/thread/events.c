/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsubo <y.otsubo.886@ms.saitama-u.ac.j    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 03:19:15 by yotsubo           #+#    #+#             */
/*   Updated: 2024/10/06 03:19:15 by yotsubo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	died(t_data *data, size_t i, long long now)
{
	pthread_mutex_lock(&(data->msg_mtx->mtx));
	pthread_mutex_lock(&(data->flag_mtx->mtx));
	printf(RED);
	printf("%lld %zu died\n", now - data->start_time, i);
	printf(RESET);
	data->is_finish = TRUE;
	pthread_mutex_unlock(&(data->flag_mtx->mtx));
	pthread_mutex_unlock(&(data->msg_mtx->mtx));
}

void	taking_fork(t_philo *philo, int hand)
{
	if (hand == RIGHT)
		pthread_mutex_lock(&(philo->right->my_mtx->mtx));
	else if (hand == LEFT)
		pthread_mutex_lock(&(philo->left->my_mtx->mtx));
	print_status(philo, TAKE);
}

void	eating(t_philo *philo)
{
	print_status(philo, EAT);
	my_usleep(philo->time_to_eat * 1e3);
	pthread_mutex_lock(&(philo->count_mtx->mtx));
	philo->eat_count++;
	pthread_mutex_unlock(&(philo->count_mtx->mtx));
}

void	sleeping(t_philo *philo)
{
	print_status(philo, SLEEP);
	my_usleep(philo->time_to_sleep * 1e3);
}

void	thinking(t_philo *philo)
{
	print_status(philo, THINK);
	my_usleep(1e3);
}
