/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:26:23 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/15 18:03:50 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_config(char **args, int count, t_config *data)
{
	if (parse_args(args, count, data) != 0)
	{
		print_usage();
		return (1);
	}
	data->coders = init_coders(data);
	if (!data->coders)
		return (1);
	data->dongles = init_dongles(data->number_of_coders);
	if (!data->dongles)
	{
		free(data->coders);
		return (1);
	}
	if (init_mnt_mtx(data) != 0)
	{
		free(data->coders);
		clean_dongles(data);
		return (1);
	}
	return (0);
}

t_coder	*init_coders(t_config *config)
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

// returns 0 in case of succes || returns 1 in case of failure
int	init_mnt_mtx(t_config *data)
{
	if (pthread_mutex_init(&data->burn_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->ncr_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->burn_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->burn_mutex);
		pthread_mutex_destroy(&data->ncr_mutex);
		return (1);
	}
	return (0);
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

