/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:26:23 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/15 00:24:01 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	*init_coders(int amount, t_config *config)
{
	t_coder	*arr_coders;
	int		i;

	arr_coders = ft_calloc(amount, sizeof(t_coder));
	if (!arr_coders)
		return (NULL);
	i = 0;
	while (i < amount)
	{
		arr_coders[i].id = i + 1;
		arr_coders[i].left_dongle = i;
		arr_coders[i].right_dongle = (i + 1) % amount;
        arr_coders[i].config = config;
		i++;
	}
	return (arr_coders);
}

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

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memalloc;

	memalloc = (void *) malloc(nmemb * size);
	if (!memalloc)
		return (NULL);
	memset(memalloc, 0, (nmemb * size));
	return (memalloc);
}

void	ft_programing(t_coder *coder)
{
    while (coder->compile_count < coder->config->number_of_compiles_required)
    {
        //TOTO: take each of the dongles.
        log_event(coder, "has taken a dongle");
        log_event(coder, "has taken a dongle");
        log_event(coder, "is compiling");
        usleep(coder->config->time_to_compile * 1000);
        //TODO: leave each of the dongles and set the cooldown time
        coder->compile_count++;
        log_event(coder, "is debugging");
        usleep(coder->config->time_to_debug * 1000);
        log_event(coder, "is refactoring");
        usleep(coder->config->time_to_refactor * 1000);
    }
}

void    log_event(t_coder *coder, const char *message)
{
    long long elapsed;

    elapsed = get_timestamp_ms() - coder->config->start_time;
    printf("%lld %d %s\n", elapsed, coder->id, message);
}