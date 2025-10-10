# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/06 10:19:24 by vinpache          #+#    #+#              #
#    Updated: 2025/10/10 12:31:29 by vinpache         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET		= \033[0m
GREEN_BOLD	= \033[1;32m
YELLOW_BOLD	= \033[1;33m
RED_BOLD	= \033[1;31m

NAME		= 	philo
CC			= 	cc
CFLAGS		= 	-Wall -Wextra -Werror -O3 -g
INC			= 	includes
RM			= 	rm -f

SRC			=	src/main.c \
				src/initializers.c \
				src/routine.c \
				src/utils.c \
				src/monitor.c

OBJ			= 	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -I$(INC) -o $(NAME) -lpthread
	@echo "$(GREEN_BOLD)✅ $(NAME) compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INC) -c $< -o $@
	@echo "$(YELLOW_BOLD)Compiling:$(RESET) $<"

clean:
	@$(RM) $(OBJ)
	@echo "$(RED_BOLD)🧼 Object files removed!$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED_BOLD)🧼 Project fully cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
