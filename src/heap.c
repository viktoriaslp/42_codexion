/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 18:17:10 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_priority(t_coder *a, t_coder *b)
{
	int	i;

	i = 0;
	if (a->config->scheduler == EDF)
	{
		if (a->request_deadline < b->request_deadline)
			i = 1;
	}
	if (a->config->scheduler == FIFO)
	{
		if (a->request_order < b->request_order)
			i = 1;
		else if (a->request_order == b->request_order
			&& a->request_deadline < b->request_deadline)
			i = 1;
	}
	return (i);
}

void	add_to_queue(t_dongle *dongle, t_coder *coder)
{
	t_coder	*tmp;

	if (dongle->queue_size >= 2)
		return ;
	dongle->queue[dongle->queue_size] = coder;
	dongle->queue_size++;
	if (dongle->queue_size == 2
		&& has_priority(dongle->queue[1], dongle->queue[0]))
	{
		tmp = dongle->queue[0];
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = tmp;
	}
}

void	remove_from_queue(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size == 0)
		return ;
	if (dongle->queue[0] == coder)
	{
		if (dongle->queue_size == 2)
			dongle->queue[0] = dongle->queue[1];
		dongle->queue_size--;
	}
	else if (dongle->queue_size == 2
		&& dongle->queue[1] == coder)
		dongle->queue_size--;
}
