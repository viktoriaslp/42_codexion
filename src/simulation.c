/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   005_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 18:22:50 by vslyunko         ###   ########.fr       */
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
		if (pthread_create(&data->coders[i].thread, NULL,
				coder_routine, &data->coders[i]) != 0)
		{
			stop_created_coders(data, i);
			return (1);
		}
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
	{
		stop_created_coders(data, i);
		return (1);
	}
	return (0);
}

void	stop_created_coders(t_config *data, int created)
{
	set_end(data, -2);
	wake_all_coders(data);
	join_coders(data, created);
}

int	get_end(t_config *data)
{
	int	end;

	pthread_mutex_lock(&data->end_mutex);
	end = data->end;
	pthread_mutex_unlock(&data->end_mutex);
	return (end);
}

void	set_end(t_config *data, int value)
{
	pthread_mutex_lock(&data->end_mutex);
	if (data->end == 0)
		data->end = value;
	pthread_mutex_unlock(&data->end_mutex);
}
