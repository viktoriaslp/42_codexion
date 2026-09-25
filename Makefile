NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I.
RM = rm -f

SRCS = main.c \
parser.c \
config_init.c \
monitor.c \
simulation.c \
heap.c \
mutex_helpers.c

HEADER = codexion.h
OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean 
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
