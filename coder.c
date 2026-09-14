/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:26:23 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/14 16:52:40 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	*init_coders(int amount)
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

void	ft_programing(t_coder *coder, t_config *config)
{
	long long	time_ms;
	//TOTO: take each of the dongles.
	time_ms = get_timestamp_ms() - config->start_time;
	printf("%lld %d has taken a dongle\n", time_ms, coder->id);
	time_ms = get_timestamp_ms() - config->start_time;
	printf("%lld %d has taken a dongle\n", time_ms, coder->id);
	time_ms = get_timestamp_ms() - config->start_time;
	ft_fase("is compiling\n", config->time_to_compile, coder->id, time_ms);
	//TODO: leave each of the dongles and set the cooldown time
	coder->compile_count++;
	time_ms = get_timestamp_ms() - config->start_time;
	ft_fase("is debugging\n", config->time_to_debug, coder->id, time_ms);
	time_ms = get_timestamp_ms() - config->start_time;
	ft_fase("is refactoring\n", config->time_to_refactor, coder->id, time_ms);
}

void	ft_fase(char *state, long long exc_tm, int id, int time)
{
	printf("%d %d %s", time, id, state);
	usleep(exc_tm * 1000);
}
