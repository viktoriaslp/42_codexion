/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/26 15:00:06 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	data;

	if (argc != 9)
		return (print_usage());
	if (init_config(argv, argc, &data) != 0)
		return (1);
	if (start_simulation(&data) == 1)
	{
		clean_up(&data);
		return (1);
	}
	join_coders(&data, data.number_of_coders);
	pthread_join(data.monitor_thread, NULL);
	clean_up(&data);
	return (0);
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
