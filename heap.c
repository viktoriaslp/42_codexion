/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 15:40:58 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/25 21:45:14 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	add_to_queue(t_dongle *dongle, t_coder *coder)
{
	t_coder	*tmp;

	if (dongle->queue_size == 0)
	{
		dongle->queue[0] = coder;
		dongle->queue_size++;
	}
	else if (dongle->queue_size == 1)
	{
		dongle->queue[1] = coder;
		dongle->queue_size++;
		if (coder->config->scheduler == EDF)
		{
			if (get_last_compile(dongle->queue[0]) > get_last_compile(dongle->queue[1]))
			{
				tmp = dongle->queue[0];
				dongle->queue[0] = dongle->queue[1];
				dongle->queue[1] = tmp;
			}
		}
	}
}

t_coder	*pop_from_queue(t_dongle *dongle)
{
	t_coder	*tmp;

	if (dongle->queue_size == 1)
	{
		dongle->queue_size--;
		tmp = dongle->queue[0];
	}
	else if (dongle->queue_size == 2)
	{
		tmp = dongle->queue[0];
		dongle->queue[0] = dongle->queue[1];
		dongle->queue_size--;
	}
	else
		tmp = NULL;
	return (tmp);
}
