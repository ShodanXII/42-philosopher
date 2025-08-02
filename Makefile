NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = philo.c init_data.c \
       final_supper.c final_supper_helper.c \
       ft_atoi.c create_philosophers.c \
       ft_isdigit.c clean.c \
       utils.c special_case.c \
       actions.c \

OBJS = $(SRCS:.c=.o)

HEADERS = philo.h

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"

%.o: %.c $(HEADERS)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)✅ Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(RED)Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✅ Executable cleaned!$(RESET)"

re: fclean all

test: $(NAME)
	@echo "$(BLUE)Running basic test...$(RESET)"
	@./$(NAME) 4 800 200 200

test_meals: $(NAME)
	@echo "$(BLUE)Running test with meal count...$(RESET)"
	@./$(NAME) 3 800 200 200 2

test_single: $(NAME)
	@echo "$(BLUE)Running single philosopher test...$(RESET)"
	@./$(NAME) 1 800 200 200

test_death: $(NAME)
	@echo "$(BLUE)Running death test...$(RESET)"
	@./$(NAME) 4 310 200 100

help:
	@echo "$(GREEN)Available targets:$(RESET)"
	@echo "  $(YELLOW)all$(RESET)        - Build the program"
	@echo "  $(YELLOW)clean$(RESET)      - Remove object files"
	@echo "  $(YELLOW)fclean$(RESET)     - Remove object files and executable"
	@echo "  $(YELLOW)re$(RESET)         - Rebuild everything"
	@echo "  $(YELLOW)test$(RESET)       - Run basic test (4 philosophers)"
	@echo "  $(YELLOW)test_meals$(RESET) - Run test with meal limit"
	@echo "  $(YELLOW)test_single$(RESET)- Run single philosopher test"
	@echo "  $(YELLOW)test_death$(RESET) - Run death test"
	@echo "  $(YELLOW)help$(RESET)       - Show this help message"

.PHONY: all clean fclean re test test_meals test_single test_death help
