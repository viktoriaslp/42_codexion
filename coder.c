/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:26:23 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/10 00:37:29 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder *init_coders(int amount)
{
    t_coder *arr_coders;
    int i;

    arr_coders = ft_calloc(amount, sizeof(t_coder));
    if (!arr_coders)
        return (NULL);

    i = 0;
    while (i < amount)
    {
        arr_coders[i].id = i + 1;
        arr_coders[i].compile_count = 0;
        arr_coders[i].left_dongle = (i + 1) % amount;
        arr_coders[i].right_dongle = (i - 1 + amount) % amount;
        i++;
    }
    return (arr_coders);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memalloc;

	memalloc = (void *) malloc(nmemb * size);
	if (!memalloc)
		return (NULL);
	memset(memalloc, 0, (nmemb * size));
	return (memalloc);
}