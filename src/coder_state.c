/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/26 14:59:27 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_last_compile(t_coder *coder)
{
	long long	value;

	pthread_mutex_lock(&coder->state_mutex);
	value = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	return (value);
}

void	set_last_compile(t_coder *coder, long long value)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = value;
	pthread_mutex_unlock(&coder->state_mutex);
}

int	get_compile_count(t_coder *coder)
{
	int	value;

	pthread_mutex_lock(&coder->state_mutex);
	value = coder->compile_count;
	pthread_mutex_unlock(&coder->state_mutex);
	return (value);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
}
