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

typedef struct s_coder
{
	int				id;
	int				compile_count;
	int				left_dongle;
	int				right_dongle;
	pthread_t		thread;
	long long		last_compile_start;
	long long		request_order;
	long long		request_deadline;
	pthread_mutex_t	state_mutex;
	struct s_config	*config;
}	t_coder;

typedef struct s_dongle
{
	int				in_use;
	long long		available_at;
	pthread_mutex_t	mutex;
	t_coder			*queue[2];
	int				queue_size;
}	t_dongle;

typedef struct s_config
{
	// only => 0 or integers.
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler; // only fifo or edf
	long long		start_time;
	long long		request_counter;

	t_coder			*coders;
	t_dongle		*dongles;

	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	scheduler_mutex;
	pthread_cond_t	scheduler_cond;
	int				end; // 0: no, n: burn, -1: finish, -2: startup/error stop
	pthread_t		monitor_thread;

}	t_config;

/* Initialization and cleanup */
int			init_config(char **args, int count, t_config *data);
void		clean_up(t_config *data);
void		clean_coders(t_config *data);
void		destroy_shared_mutexes(t_config *data);

/* Simulation and monitor */
int			start_simulation(t_config *data);
void		join_coders(t_config *data, int count);
void		stop_created_coders(t_config *data, int created);
void		*coder_routine(void *args);
void		*monitor_routine(void *arg);
void		wake_all_coders(t_config *data);

/* Dongles and scheduling */
int			take_two_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
void		add_to_queue(t_dongle *dongle, t_coder *coder);
void		remove_from_queue(t_dongle *dongle, t_coder *coder);
void		register_request(t_coder *coder, t_dongle *left, t_dongle *right);
int			can_take_pair(t_coder *coder, t_dongle *left, t_dongle *right);
void		reserve_pair(t_coder *coder, t_dongle *first, t_dongle *second);
void		cancel_request(t_coder *coder, t_dongle *first, t_dongle *second);
long long	max_available_at(t_coder *coder);

/* Shared state */
int			get_end(t_config *data);
void		set_end(t_config *data, int value);
long long	get_last_compile(t_coder *coder);
void		set_last_compile(t_coder *coder, long long value);
int			get_compile_count(t_coder *coder);
void		increment_compile_count(t_coder *coder);

/* Utilities and parsing */
int			parse_args(char **args, int count, t_config *data);
int			print_usage(void);
void		log_event(t_coder *coder, const char *message);
long long	get_time_ms(void);
void		ms_to_timespec(long long ms, struct timespec *ts);
int			wait_ms(t_config *data, int duration);

#endif
