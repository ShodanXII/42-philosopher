NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = philo.c init_data.c final_supper.c final_supper_helper.c \
       ft_atoi.c create_philosophers.c ft_isdigit.c clean.c \
       utils.c special_case.c actions.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re