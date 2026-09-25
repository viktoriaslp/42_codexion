/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 15:40:58 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/25 21:49:55 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	data->end = value;
	pthread_mutex_unlock(&data->end_mutex);
}

long long	get_last_compile(t_coder *coder)
{
	long long	value;

	pthread_mutex_lock(&coder->state_mutex);
	value = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	return (value);
}

int	get_compile_count(t_coder *coder)
{
	int	value;

	pthread_mutex_lock(&coder->state_mutex);
	value = coder->compile_count;
	pthread_mutex_unlock(&coder->state_mutex);
	return (value);
}

void	set_last_compile(t_coder *coder, long long value)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = value;
	pthread_mutex_unlock(&coder->state_mutex);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
}
