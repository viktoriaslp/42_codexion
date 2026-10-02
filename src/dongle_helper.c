/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 18:22:15 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	register_request(t_coder *coder, t_dongle *left, t_dongle *right)
{
	coder->request_order = coder->config->request_counter++;
	coder->request_deadline = get_last_compile(coder)
		+ coder->config->time_to_burnout;
	add_to_queue(left, coder);
	add_to_queue(right, coder);
}

int	can_take_pair(t_coder *coder, t_dongle *left, t_dongle *right)
{
	long long	now;

	if (left->queue[0] != coder || right->queue[0] != coder)
		return (0);
	if (left->in_use || right->in_use)
		return (0);
	now = get_time_ms();
	if (now < left->available_at || now < right->available_at)
		return (0);
	return (1);
}

void	reserve_pair(t_coder *coder, t_dongle *first, t_dongle *second)
{
	first->in_use = 1;
	second->in_use = 1;
	remove_from_queue(first, coder);
	remove_from_queue(second, coder);
}

void	cancel_request(t_coder *coder, t_dongle *first, t_dongle *second)
{
	remove_from_queue(first, coder);
	remove_from_queue(second, coder);
	pthread_cond_broadcast(&coder->config->scheduler_cond);
}

long long	max_available_at(t_coder *coder)
{
	long long	first;
	long long	second;

	first = coder->config->dongles[coder->left_dongle].available_at;
	second = coder->config->dongles[coder->right_dongle].available_at;
	if (first > second)
		return (first);
	return (second);
}
