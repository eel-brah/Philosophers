CC := cc
CFLAGS := -Wall -Wextra -Werror

PARN_DIR := ./philo
SRC_DIR := $(PARN_DIR)/src
INCLUDE_DIR := $(PARN_DIR)/include
BUILD_DIR := $(PARN_DIR)/build

INCLUDE := philo.h
INCLUDE := $(addprefix $(INCLUDE_DIR)/,$(INCLUDE))

SRC := philo.c acts.c monitor.c init.c utils_1.c utils_2.c print.c
OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)

NAME := ppphilo

all: pre $(NAME)

$(NAME): $(OBJ) $(INCLUDE)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[1;34m$(NAME) \033[0;34mhas been compiled"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE)
	@$(CC) $(CFLAGS) -c $< -o $@

pre:
	@[ -d "$(BUILD_DIR)" ] || mkdir "$(BUILD_DIR)"

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -rf $(NAME) $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re pre