NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I include

SRC_DIR = src
OBJ_DIR = obj

SRC = main.c \
		parsing/pars.c \
		init/init.c \
		init/init_philo.c \
		routine/routine.c \
		monitor/monitor.c \
		utils/place.c \
		utils/print.c \
		utils/time.c
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c include/philo.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re