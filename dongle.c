/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 11:24:00 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/14 11:30:30 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*init_dongles(int amount)
{
	t_dongle *arr_dongles;
	int		i;
	
	arr_dongles = ft_calloc(amount, sizeof(t_dongle));
	if (!arr_dongles)
		return (NULL);
	i = 0;
	while (i < amount)
	{
		arr_dongles[i].id = i + 1;
		i++;
	}
	return (arr_dongles);
}