/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/25 21:41:42 by vslyunko         ###   ########.fr       */
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
	if (data->dongles)
		clean_dongles(data);
	if (data->coders)
		clean_coders(data);
	pthread_mutex_destroy(&data->end_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	return (status);
}

void	clean_dongles(t_config *data)
{
	c_m_destroy(data->number_of_coders, data->dongles);
	free(data->dongles);
}

void	clean_coders(t_config *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->coders[i].state_mutex);
		i++;
	}
	free(data->coders);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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

void	stop_created_coders(t_config *data, int created)
{
	int	i;

	set_end(data, -2);
	wake_all_coders(data);
	i = 0;
	while (i < created)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
}
