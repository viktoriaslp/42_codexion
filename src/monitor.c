/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   006_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 18:48:26 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	check_burn(t_config *data)
{
	int			i;
	long long	time_sc;

	i = 0;
	while (i < data->number_of_coders)
	{
		time_sc = get_time_ms() - get_last_compile(&data->coders[i]);
		if (time_sc > data->time_to_burnout)
		{
			set_end(data, ++i);
			return ;
		}
		i++;
	}
	return ;
}

static void	check_done(t_config *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (get_compile_count(&data->coders[i])
			< data->number_of_compiles_required)
			return ;
		i++;
	}
	set_end(data, -1);
}

void	*monitor_routine(void *arg)
{
	t_config	*data;
	int			end;

	data = (t_config *)arg;
	while (get_end(data) == 0 && data->number_of_compiles_required > 0)
	{
		check_burn(data);
		if (get_end(data) == 0)
			check_done(data);
		if (get_end(data) != 0)
			break ;
		usleep(1000);
	}
	end = get_end(data);
	if (end > 0)
		log_event(&data->coders[end - 1], "burned out");
	wake_all_coders(data);
	return (NULL);
}

void	wake_all_coders(t_config *data)
{
	pthread_mutex_lock(&data->scheduler_mutex);
	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
}

void	log_event(t_coder *coder, const char *message)
{
	long long	elapsed;

	pthread_mutex_lock(&coder->config->print_mutex);
	if (get_end(coder->config) == 0
		|| strcmp(message, "burned out") == 0)
	{
		elapsed = get_time_ms() - coder->config->start_time;
		printf("%lld %d %s\n", elapsed, coder->id, message);
	}
	pthread_mutex_unlock(&coder->config->print_mutex);
}
