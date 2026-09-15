#include "codexion.h"

void	print_coder_info(t_coder *coders, t_dongle *dongles, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		printf("coder_id: %d\n", coders[i].id);
		printf("	coder_compile_count: %d\n", coders[i].compile_count);
		printf("	coder_left_dongle: %d\n", coders[i].left_dongle);
		printf("	coder_right_dongle: %d\n", coders[i].right_dongle);
		i++;
	}
	i = 0;
	while (i < amount)
	{
		printf("dongle_id: %d\n", dongles[i].id);
		printf("	dongle_release_time: %lld\n", dongles[i].last_release_time);
		i++;
	}
}

void	print_config(t_config *data)
{
	printf("number_of_coders: %d\n", data->number_of_coders);
	printf("time_to_burnout: %d\n", data->time_to_burnout);
	printf("time_to_compile: %d\n", data->time_to_compile);
	printf("time_to_debug: %d\n", data->time_to_debug);
	printf("time_to_refactor: %d\n", data->time_to_refactor);
	printf("number_of_compiles_required: %d\n",
		data->number_of_compiles_required);
	printf("dongle_cooldown: %d\n", data->dongle_cooldown);
	if (data->scheduler == FIFO)
		printf("scheduler: FIFO\n");
	else if (data->scheduler == EDF)
		printf("scheduler: EDF\n");
}
