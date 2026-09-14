/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vslyunko <vslyunko@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 15:52:36 by vslyunko          #+#    #+#             */
/*   Updated: 2026/09/26 15:57:56 by vslyunko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>   // pthread_create, pthread_join,
// pthread_mutex_init, pthread_mutex_lock,
// pthread_mutex_unlock, pthread_mutex_destroy,
// pthread_cond_init, pthread_cond_wait,
// pthread_cond_timedwait, pthread_cond_broadcast,
// pthread_cond_destroy

# include <sys/time.h>  // gettimeofday
# include <unistd.h>    // usleep, write
# include <stdlib.h>    // malloc, free, atoi
# include <stdio.h>     // printf, fprintf
# include <string.h>    // strcmp, strlen, memset

// structs
typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_config
{
	// only => 0 or integers.
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler; // only fifo or edf
}	t_config;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	int			left_dongle;
	int			right_dongle;
	// pthread_t	thread;
	// int			last_compile_start;
}	t_coder;

typedef struct s_dongle
{
	int	id;
	int	last_release_time;
}	t_dongle;

// Lists of functions
int			print_usage(void);
void		*ft_calloc(size_t nmemb, size_t size);
t_coder		*init_coders(int amount);
t_dongle	*init_dongles(int amount);


void		print_config(t_config *data); // TODO: only testing purpose
void	print_coder_info(t_coder *coders, t_dongle *dongles, int amount);

int			parse_args(char **args, int count, t_config *data);
int			int_checker(const char *nptr);
int			int_args_checker(char **args, int count);
int			scheduler_check(const char *ptr, t_config *data);

#endif
