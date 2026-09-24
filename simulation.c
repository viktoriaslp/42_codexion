/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 13:26:26 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/23 23:27:42 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_config *data)
{
	int	i;

	data->start_time = get_time_ms();
	i = 0;
	while(i < data->number_of_coders)
	{
		data->coders[i].last_compile_start = data->start_time;
		if(pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(data->coders[i].thread, NULL);
			return (1);
		}
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor, data) != 0)
		return (1);
	return (0);
}

void	*coder_routine(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
    while (coder->compile_count < coder->config->number_of_compiles_required && coder->config->end == 0)
    {
		if (!take_two_dongles(coder))
			break ;
        log_event(coder, "is compiling");
		coder->last_compile_start = get_time_ms();
        usleep(coder->config->time_to_compile * 1000);
        coder->compile_count++;
		return_dongles(coder);
        log_event(coder, "is debugging");
        usleep(coder->config->time_to_debug * 1000);
        log_event(coder, "is refactoring");
        usleep(coder->config->time_to_refactor * 1000);
    }
	return (NULL);
}

void    log_event(t_coder *coder, const char *message)
{
    long long elapsed;

    elapsed = get_time_ms() - coder->config->start_time;
	pthread_mutex_lock(&coder->config->print_mutex);
    printf("%lld %d %s\n", elapsed, coder->id, message);
	pthread_mutex_unlock(&coder->config->print_mutex);
}

int	take_two_dongles(t_coder *coder)
{
	if (!take_dongle(coder, &coder->config->dongles[coder->left_dongle]) || !take_dongle(coder, &coder->config->dongles[coder->right_dongle]))
		return (0);
	return (1);
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	timeout;

	pthread_mutex_lock(&dongle->mutex);
	while ((dongle->in_use || get_time_ms() < dongle->available_at) && coder->config->end == 0)
	{
		if (dongle->in_use)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else
		{
			ms_to_timespec(dongle->available_at, &timeout);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &timeout);
		}
	}
	if (coder->config->end != 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	dongle->in_use = 1;
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
