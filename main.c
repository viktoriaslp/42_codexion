/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/22 22:24:22 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	data;
	int			i;

	if (argc != 9)
		return (print_usage());
	if (init_config(argv, argc, &data) != 0)
		return (1);
	if (start_simulation(&data) == 1)
	{
		clean_up(1, &data);
		return (1);
	}
	i = 0;
	while (i < data.number_of_coders)
	{
		pthread_join(data.coders[i].thread, NULL);
		i++;
	}
	pthread_join(data.monitor_thread, NULL);
	clean_up(0, &data);
}

int	clean_up(int status, t_config *data)
{	
	pthread_mutex_destroy(&data->burn_mutex);
	pthread_mutex_destroy(&data->ncr_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	if (data->coders)
		free(data->coders);
	if (data->dongles)
		clean_dongles(data);
	return (status);
}	

void	clean_dongles(t_config *data)
{
	c_m_destroy(data->number_of_coders, data->dongles);
	free(data->dongles);
}

long long get_time_ms()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	ms_to_timespec(long long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000;
}

int	print_usage(void)
{
	fprintf(stderr,
		"Error: invalid arguments\n"
		"Usage: ./codexion <number_of_coders> <time_to_burnout> "
		"<time_to_compile> <time_to_debug> <time_to_refactor> "
		"<number_of_compiles_required> <dongle_cooldown> <fifo|edf>\n"
		"Numeric arguments must be valid integers and cannot be negative.\n");
	return (1);
}
