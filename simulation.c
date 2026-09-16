/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 13:26:26 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/16 23:25:02 by vslyunko         ###   ########.fr       */
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
		take_dongles(coder);
        log_event(coder, "is compiling");
		coder->last_compile_start = get_time_ms();
        usleep(coder->config->time_to_compile * 1000);
        //TODO: leave each of the dongles and set the cooldown time
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

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->config->dongles[coder->left_dongle].mutex);
	log_event(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->config->dongles[coder->right_dongle].mutex);
	log_event(coder, "has taken a dongle");
}

void	return_dongles(t_coder *coder)
{
	coder->config->dongles[coder->left_dongle].last_release_time = get_time_ms();
	pthread_mutex_unlock(&coder->config->dongles[coder->left_dongle].mutex);
	coder->config->dongles[coder->right_dongle].last_release_time = get_time_ms();
	pthread_mutex_unlock(&coder->config->dongles[coder->right_dongle].mutex);
}

// has_compiled_n = 1;

// for (int i = 0; i < n_programers; i++)
// 	if (programmers[i].n_comp >= noc)
// 		counter++;
		
// return counter == n_programmers