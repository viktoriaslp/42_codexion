/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/02 11:40:05 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	validate_nonnegative_int(const char *nptr, int *nbr)
{
	int		i;
	long	nb;

	nb = 0;
	i = 0;
	if (nptr[0] == '-')
		return (1);
	if (nptr[0] == '+' && (nptr[1] >= '0' && nptr[1] <= '9'))
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		// if (nb > (2147483647 - (nptr[i] - '0')) / 10)
		// 	return (1);
		nb = nb * 10 + (nptr[i] - '0');
		if (nb > 2147483647)
			return (1);
		i++;
	}
	if (nptr[i] != '\0' || nptr[0] == '\0')
	{
		return (1);
	}
	*nbr = (int)nb;
	return (0);
}

static int	validate_numeric_args(char **args, t_config *data)
{
	if (validate_nonnegative_int(args[1], &data->number_of_coders)
		|| validate_nonnegative_int(args[2], &data->time_to_burnout)
		|| validate_nonnegative_int(args[3], &data->time_to_compile)
		|| validate_nonnegative_int(args[4], &data->time_to_debug)
		|| validate_nonnegative_int(args[5], &data->time_to_refactor)
		|| validate_nonnegative_int(args[6],
			&data->number_of_compiles_required)
		|| validate_nonnegative_int(args[7], &data->dongle_cooldown))
		return (1);
	return (0);
}

static int	parse_scheduler(const char *ptr, t_config *data)
{
	if (strcmp(ptr, "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(ptr, "edf") == 0)
		data->scheduler = EDF;
	else
		return (1);
	return (0);
}

int	parse_args(char **args, int count, t_config *data)
{
	if (validate_numeric_args(args, data) == 1)
		return (1);
	if (parse_scheduler(args[count - 1], data) == 1)
		return (1);
	if (data->number_of_coders == 0)
		return (1);
	return (0);
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
