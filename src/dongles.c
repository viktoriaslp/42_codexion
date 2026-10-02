/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 18:22:04 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	lock_pair(t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
}

static void	unlock_pair(t_dongle *first, t_dongle *second)
{
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}

static void	wait_once(t_coder *coder,
	t_dongle *first, t_dongle *second)
{
	long long		until;
	int				cooldown;
	struct timespec	ts;

	until = max_available_at(coder);
	cooldown = (first->queue[0] == coder
			&& second->queue[0] == coder
			&& !first->in_use && !second->in_use
			&& until > get_time_ms());
	unlock_pair(first, second);
	if (cooldown)
	{
		ms_to_timespec(until, &ts);
		pthread_cond_timedwait(&coder->config->scheduler_cond,
			&coder->config->scheduler_mutex, &ts);
	}
	else
		pthread_cond_wait(&coder->config->scheduler_cond,
			&coder->config->scheduler_mutex);
	lock_pair(first, second);
}

static int	wait_for_pair(t_coder *coder,
	t_dongle *first, t_dongle *second)
{
	while (get_end(coder->config) == 0)
	{
		if (can_take_pair(coder, first, second))
		{
			reserve_pair(coder, first, second);
			return (1);
		}
		wait_once(coder, first, second);
	}
	cancel_request(coder, first, second);
	return (0);
}

int	take_two_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int			success;

	first = &coder->config->dongles[coder->left_dongle];
	second = &coder->config->dongles[coder->right_dongle];
	if (first == second)
	{
		log_event(coder, "has taken a dongle");
		wait_ms(coder->config, coder->config->time_to_burnout);
		return (0);
	}
	pthread_mutex_lock(&coder->config->scheduler_mutex);
	lock_pair(first, second);
	register_request(coder, first, second);
	success = wait_for_pair(coder, first, second);
	unlock_pair(first, second);
	pthread_mutex_unlock(&coder->config->scheduler_mutex);
	if (success)
	{
		log_event(coder, "has taken a dongle");
		log_event(coder, "has taken a dongle");
	}
	return (success);
}
