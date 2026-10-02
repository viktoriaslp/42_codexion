NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -Isrc. -g
RM = rm -f
ARGS ?= 5 10000 200 200 200 3 50 fifo

SRCS = src/main.c \
	src/parser.c \
	src/config_init.c \
	src/cleanup.c \
	src/simulation.c \
	src/monitor.c \
	src/coder.c \
	src/coder_state.c \
	src/dongles.c \
	src/dongle_helper.c \
	src/heap.c

HEADER = src/codexion.h
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

run: $(NAME)
	./$(NAME) $(ARGS)

valgrind: $(NAME)
	@command -v valgrind >/dev/null 2>&1 || \
		{ echo "Error: Valgrind is not installed. Run this check on Linux with Valgrind installed."; exit 1; }
	valgrind --tool=memcheck --leak-check=full \
		--show-leak-kinds=all --errors-for-leak-kinds=all \
		--track-origins=yes --error-exitcode=1 \
		./$(NAME) $(ARGS)

helgrind: $(NAME)
	@command -v valgrind >/dev/null 2>&1 || \
		{ echo "Error: Valgrind is not installed. Run this check on Linux with Valgrind installed."; exit 1; }
	valgrind --tool=helgrind --error-exitcode=1 \
		./$(NAME) $(ARGS)

.PHONY: all clean fclean re run valgrind helgrind
