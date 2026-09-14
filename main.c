/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/14 11:33:05 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	data;
	t_coder		*coders;
	t_dongle	*dongles;

	if (argc != 9)
		return (print_usage());
	if (parse_args(argv, argc, &data) != 0)
		return (print_usage());
	print_config(&data);
	if (data.number_of_coders == 0)
		return (0);
	coders = init_coders(data.number_of_coders);
	dongles = init_dongles(data.number_of_coders);
	print_coder_info(coders, dongles, data.number_of_coders);
	free(coders);
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

void	print_coder_info(t_coder *coders, t_dongle *dongles, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		printf("coder_id: %d\n", coders[i].id);
		printf("	coder_compile_count: %d\n", coders[i].compile_count);
		printf("	coder_left_dongle: %d\n", coders[i].left_dongle);
		printf("	coder_right_dongle: %d\n", coders[i].right_dongle);
		i++;
	}
	i = 0;
	while (i < amount)
	{
		printf("dongle_id: %d\n", dongles[i].id);
		printf("	dongle_release_time: %d\n", dongles[i].last_release_time);
		i++;
	}
}

void	print_config(t_config *data)
{
	printf("number_of_coders: %d\n", data->number_of_coders);
	printf("time_to_burnout: %d\n", data->time_to_burnout);
	printf("time_to_compile: %d\n", data->time_to_compile);
	printf("time_to_debug: %d\n", data->time_to_debug);
	printf("time_to_refactor: %d\n", data->time_to_refactor);
	printf("number_of_compiles_required: %d\n",
		data->number_of_compiles_required);
	printf("dongle_cooldown: %d\n", data->dongle_cooldown);
	if (data->scheduler == FIFO)
		printf("scheduler: FIFO\n");
	else if (data->scheduler == EDF)
		printf("scheduler: EDF\n");
}
