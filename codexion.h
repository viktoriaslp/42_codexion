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

#include <pthread.h>   // pthread_create, pthread_join,
                       // pthread_mutex_init, pthread_mutex_lock,
                       // pthread_mutex_unlock, pthread_mutex_destroy,
                       // pthread_cond_init, pthread_cond_wait,
                       // pthread_cond_timedwait, pthread_cond_broadcast,
                       // pthread_cond_destroy

#include <sys/time.h>  // gettimeofday
#include <unistd.h>    // usleep, write
#include <stdlib.h>    // malloc, free, atoi
#include <stdio.h>     // printf, fprintf
#include <string.h>    // strcmp, strlen, memset


// structs
typedef struct s_args
{
	// only => 0 or integers.
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	char	*scheduler; // only fifo or edf
}	t_args;

// Lists of functions
void parse_args(int count, char **args, t_args *data);


#endif
