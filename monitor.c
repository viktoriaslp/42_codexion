/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:26:23 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/23 23:13:33 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    *monitor(void *arg)
{
    t_config    *data;

    data = (t_config *)arg;
    while (sim_running(data))
    {
        check_burn(data);
		if (data->end == 0)
			check_done(data);
        usleep(1000);
    }
    return (NULL);
}

void    check_burn(t_config *data)
{
    int	i;
	long long	time_sc;

	i = 0;
	while (i < data->number_of_coders)
	{
		time_sc = get_time_ms() - data->coders[i].last_compile_start;
		if (time_sc > data->time_to_burnout)
		{
			data->end = ++i;
			return ;
		}
		i++;
	}
	return ;
}

void	wake_all_coders(t_config *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_cond_broadcast(&data->dongles[i].cond);
	}
}

void    check_done(t_config *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].compile_count >= data->number_of_compiles_required)
			count++;
		i++;
	}
	if (count == data->number_of_coders)
	{
		data->end = -1;
		return ;
	}
	return ;
}

int sim_running(t_config *data)
{
	if (data->end > 0)
	{
		log_event(&data->coders[data->end - 1], "burned out");
		return (0);
	}
	else if (data->end == -1)
	{
		printf("Programming ended\n");
		return (0);
	}
	return (1);
}