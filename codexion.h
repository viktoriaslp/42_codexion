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
	pthread_mutex_t	state_mutex;
	struct s_config	*config;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	int				in_use;
	long long		available_at;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
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
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
	int				end; // 0: no, n: burn, -1: finish, -2: startup/error stop
	pthread_t		monitor_thread;

}	t_config;

// initializing data structures
int			print_usage(void);
int			init_config(char **args, int count, t_config *data);
int			parse_args(char **args, int count, t_config *data);
t_coder		*init_coders(t_config *config);
t_dongle	*init_dongles(int amount);
long long	get_time_ms(void);
void		ms_to_timespec(long long ms, struct timespec *ts);
int			init_data_mtx(t_config *data);

// auxiliary functions
int			int_checker(const char *nptr);
int			int_args_checker(char **args, int count);
int			scheduler_check(const char *ptr, t_config *data);
void		*ft_calloc(size_t nmemb, size_t size);

// simulation data
int			start_simulation(t_config *data);
void		*coder_routine(void *args);
void		log_event(t_coder *coder, const char *message);
int			take_two_dongles(t_coder *coder);
int			take_dongle(t_coder *coder, t_dongle *dongle);
void		release_dongle(t_coder *coder, t_dongle *dongle);
void		return_dongles(t_coder *coder);
void		add_to_queue(t_dongle *dongle, t_coder *coder);
t_coder		*pop_from_queue(t_dongle *dongle);
int			begin_compile(t_coder *coder);
int			wait_ms(t_config *data, int duration);

// memory up
int			clean_up(int status, t_config *data);
void		clean_dongles(t_config *data);
void		clean_coders(t_config *data);
void		c_m_destroy(int i, t_dongle *dongles);
void		stop_created_coders(t_config *data, int created);

// monitor
void		*monitor(void *arg);
void		check_done(t_config *data);
void		check_burn(t_config *data);
int			sim_running(t_config *data);
void		wake_all_coders(t_config *data);
int			get_end(t_config *data);
void		set_end(t_config *data, int value);
long long	get_last_compile(t_coder *coder);
int			get_compile_count(t_coder *coder);
void		set_last_compile(t_coder *coder, long long value);
void		increment_compile_count(t_coder *coder);

#endif
