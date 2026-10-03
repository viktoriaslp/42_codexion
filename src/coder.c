/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/02 13:58:16 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	begin_compile(t_coder *coder)
{
	long long	now;
	int			success;

	success = 0;
	pthread_mutex_lock(&coder->state_mutex);
	pthread_mutex_lock(&coder->config->end_mutex);
	now = get_time_ms();
	if (coder->config->end == 0)
	{
		if (now - coder->last_compile_start
			> coder->config->time_to_burnout)
			coder->config->end = coder->id;
		else
		{
			coder->last_compile_start = now;
			success = 1;
		}
	}
	pthread_mutex_unlock(&coder->config->end_mutex);
	pthread_mutex_unlock(&coder->state_mutex);
	return (success);
}

static int	compile_phase(t_coder *coder)
{
	if (!take_two_dongles(coder))
		return (0);
	if (!begin_compile(coder))
	{
		release_dongles(coder);
		return (0);
	}
	log_event(coder, "is compiling");
	if (!wait_ms(coder->config, coder->config->time_to_compile))
	{
		release_dongles(coder);
		return (0);
	}
	increment_compile_count(coder);
	release_dongles(coder);
	return (1);
}

void	*coder_routine(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	while (coder->config->number_of_compiles_required > 0
		&& get_end(coder->config) == 0)
	{
		if (!compile_phase(coder))
			break ;
		log_event(coder, "is debugging");
		if (!wait_ms(coder->config, coder->config->time_to_debug))
			break ;
		log_event(coder, "is refactoring");
		if (!wait_ms(coder->config, coder->config->time_to_refactor))
			break ;
	}
	return (NULL);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long long	available_at;

	first = &coder->config->dongles[coder->left_dongle];
	second = &coder->config->dongles[coder->right_dongle];
	pthread_mutex_lock(&coder->config->scheduler_mutex);
	lock_pair(first, second);
	available_at = get_time_ms() + coder->config->dongle_cooldown;
	first->in_use = 0;
	second->in_use = 0;
	first->available_at = available_at;
	second->available_at = available_at;
	unlock_pair(first, second);
	pthread_cond_broadcast(&coder->config->scheduler_cond);
	pthread_mutex_unlock(&coder->config->scheduler_mutex);
}
