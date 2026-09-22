/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/22 21:38:07 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_args(char **args, int count, t_config *data)
{
	if (int_args_checker(args, count) == 1)
		return (1);
	else
	{
		data->number_of_coders = atoi(args[1]);
		data->time_to_burnout = atoi(args[2]);
		data->time_to_compile = atoi(args[3]);
		data->time_to_debug = atoi(args[4]);
		data->time_to_refactor = atoi(args[5]);
		data->number_of_compiles_required = atoi(args[6]);
		data->dongle_cooldown = atoi(args[7]);
	}
	if (scheduler_check(args[count - 1], data) == 1)
		return (1);
	if (data->number_of_coders == 0)
		return (1);
	return (0);
}

int	scheduler_check(const char *ptr, t_config *data)
{
	if (strcmp(ptr, "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(ptr, "edf") == 0)
		data->scheduler = EDF;
	else
		return (1);
	return (0);
}

int	int_args_checker(char **args, int count)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	while (i < count - 1 && check == 0)
	{
		check = int_checker(args[i]);
		i++;
	}
	return (check);
}

// returns 0 in case of succes || returns 1 in case of failure
int	int_checker(const char *nptr)
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
		nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	if ((nptr[i] != '\0' || nptr[0] == '\0')
		|| (nb < -2147483648 || nb > 2147483647))
	{
		return (1);
	}
	return (0);
}
