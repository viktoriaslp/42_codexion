/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/15 00:20:28 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	data;

	if (argc != 9)
		return (print_usage());
	if (parse_args(argv, argc, &data) != 0)
		return (print_usage());
	if (data.number_of_coders == 0)
		return (0);
	compleate_init(&data);
	if (!data.coders || !data.dongles)
		return (free_struct(1, &data));
	
	print_config(&data);
	print_coder_info(data.coders, data.dongles, data.number_of_coders);
	ft_programing(&data.coders[0]);
	free_struct(0, &data);
}

int	free_struct(int bool, t_config *data)
{
	if (data->coders)
		free(data->coders);
	if (data->dongles)
		free(data->dongles);
	return (bool);
}	
void	compleate_init(t_config *data)
{
	data->start_time = get_timestamp_ms();
	printf("Start time: %lld\n", data->start_time);
	
	data->coders = init_coders(data->number_of_coders, data);
	data->dongles = init_dongles(data->number_of_coders);
}

long long get_timestamp_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
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
