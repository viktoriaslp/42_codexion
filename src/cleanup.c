/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   004_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 18:23:14 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	clean_dongles(t_config *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	free(data->dongles);
}

void	destroy_shared_mutexes(t_config *data)
{
	pthread_mutex_destroy(&data->end_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->scheduler_mutex);
	pthread_cond_destroy(&data->scheduler_cond);
}

void	join_coders(t_config *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
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

void	clean_up(t_config *data)
{
	if (data->dongles)
		clean_dongles(data);
	if (data->coders)
		clean_coders(data);
	destroy_shared_mutexes(data);
}
