/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 13:26:26 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/25 22:44:47 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_config *data)
{
	int	i;

	data->start_time = get_time_ms();
	i = 0;
	while (i < data->number_of_coders)
	{
		set_last_compile(&data->coders[i], data->start_time);
		if (pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]) != 0)
		{
			stop_created_coders(data, i);
			return (1);
		}
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor, data) != 0)
	{
		stop_created_coders(data, i);
		return (1);
	}
	return (0);
}

void	*coder_routine(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	while (get_compile_count(coder) < coder->config->number_of_compiles_required && get_end(coder->config) == 0)
	{
		if (!take_two_dongles(coder))
			break ;
		if (!begin_compile(coder))
			break ;
		log_event(coder, "is compiling");
		if (!wait_ms(coder->config, coder->config->time_to_compile))
			{
				return_dongles(coder);
				break ;
			}
		increment_compile_count(coder);
		return_dongles(coder);
		log_event(coder, "is debugging");
		if (!wait_ms(coder->config, coder->config->time_to_debug))
			break ;
		log_event(coder, "is refactoring");
		if (!wait_ms(coder->config, coder->config->time_to_refactor))
			break ;
	}
	return (NULL);
}

void	log_event(t_coder *coder, const char *message)
{
	long long	elapsed;

	pthread_mutex_lock(&coder->config->print_mutex);
	if (get_end(coder->config) != 0 && strcmp(message, "burned out") != 0)
	{		
		pthread_mutex_unlock(&coder->config->print_mutex);
		return ;
	}	
	elapsed = get_time_ms() - coder->config->start_time;
	printf("%lld %d %s\n", elapsed, coder->id, message);
	pthread_mutex_unlock(&coder->config->print_mutex);
}

int	wait_ms(t_config *data, int duration)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < duration)
	{
		if (get_end(data) != 0)
			return (0);
		usleep(1000);
	}
	return (1);
}

int	begin_compile(t_coder *coder)
{
	long long	now;

	pthread_mutex_lock(&coder->state_mutex);
	pthread_mutex_lock(&coder->config->end_mutex);

	now = get_time_ms();
	if (coder->config->end != 0)
	{
		pthread_mutex_unlock(&coder->config->end_mutex);
		pthread_mutex_unlock(&coder->state_mutex);
		return (0);
	}
	if (now - coder->last_compile_start >= coder->config->time_to_burnout)
	{
		coder->config->end = coder->id;
		pthread_mutex_unlock(&coder->config->end_mutex);
		pthread_mutex_unlock(&coder->state_mutex);
		return (0);
	}
	coder->last_compile_start = now;

	pthread_mutex_unlock(&coder->config->end_mutex);
	pthread_mutex_unlock(&coder->state_mutex);
	return (1);
}

int	take_two_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = &coder->config->dongles[coder->left_dongle];
	second = &coder->config->dongles[coder->right_dongle];
	if (coder->left_dongle > coder->right_dongle)
	{
		first = &coder->config->dongles[coder->right_dongle];
		second = &coder->config->dongles[coder->left_dongle];
	}
	if (coder->left_dongle == coder->right_dongle)
	{
		if (!take_dongle(coder, first))
			return (0);
		while (get_end(coder->config) == 0)
			usleep(1000);
		return_dongles(coder);
		return (0);
	}
	if (!take_dongle(coder, first))
		return (0);
	if (!take_dongle(coder, second))
	{
		release_dongle(coder, first);
		return (0);
	}
	return (1);
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	timeout;

	pthread_mutex_lock(&dongle->mutex);
	add_to_queue(dongle, coder);
	while ((dongle->queue[0] != coder || dongle->in_use || get_time_ms() < dongle->available_at) && get_end(coder->config) == 0)
	{
		if (dongle->queue[0] != coder)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else if (dongle->in_use)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else
		{
			ms_to_timespec(dongle->available_at, &timeout);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &timeout);
		}
	}
	if (get_end(coder->config) != 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	dongle->in_use = 1;
	pop_from_queue(dongle);
	pthread_mutex_unlock(&dongle->mutex);
	log_event(coder, "has taken a dongle");
	return (1);
}

void	return_dongles(t_coder *coder)
{
	release_dongle(coder, &coder->config->dongles[coder->left_dongle]);
	release_dongle(coder, &coder->config->dongles[coder->right_dongle]);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->available_at = get_time_ms() + coder->config->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
