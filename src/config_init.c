/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   003_config_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:21:51 by vslyunko          #+#    #+#             */
/*   Updated: 2026/10/01 16:32:50 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memalloc;

	memalloc = (void *) malloc(nmemb * size);
	if (!memalloc)
		return (NULL);
	memset(memalloc, 0, (nmemb * size));
	return (memalloc);
}

static int	init_shared_mutexes(t_config *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->end_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->scheduler_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->end_mutex);
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	if (pthread_cond_init(&data->scheduler_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&data->end_mutex);
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->scheduler_mutex);
		return (1);
	}
	return (0);
}

static t_coder	*init_coders(t_config *config)
{
	t_coder	*arr_coders;
	int		i;

	arr_coders = ft_calloc(config->number_of_coders, sizeof(t_coder));
	if (!arr_coders)
		return (NULL);
	i = 0;
	while (i < config->number_of_coders)
	{
		arr_coders[i].id = i + 1;
		arr_coders[i].left_dongle = i;
		arr_coders[i].right_dongle = (i + 1) % config->number_of_coders;
		arr_coders[i].config = config;
		if (pthread_mutex_init(&arr_coders[i].state_mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&arr_coders[i].state_mutex);
			free(arr_coders);
			return (NULL);
		}
		i++;
	}
	return (arr_coders);
}

static t_dongle	*init_dongles(int amount)
{
	t_dongle	*arr_dongles;
	int			i;

	arr_dongles = ft_calloc(amount, sizeof(t_dongle));
	if (!arr_dongles)
		return (NULL);
	i = 0;
	while (i < amount)
	{
		if (pthread_mutex_init(&arr_dongles[i].mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&arr_dongles[i].mutex);
			free(arr_dongles);
			return (NULL);
		}
		i++;
	}
	return (arr_dongles);
}

int	init_config(char **args, int count, t_config *data)
{
	if (parse_args(args, count, data) != 0)
	{
		print_usage();
		return (1);
	}
	data->end = 0;
	data->request_counter = 0;
	if (init_shared_mutexes(data) != 0)
		return (1);
	data->coders = init_coders(data);
	if (!data->coders)
	{
		destroy_shared_mutexes(data);
		return (1);
	}
	data->dongles = init_dongles(data->number_of_coders);
	if (!data->dongles)
	{
		destroy_shared_mutexes(data);
		clean_coders(data);
		return (1);
	}
	return (0);
}
