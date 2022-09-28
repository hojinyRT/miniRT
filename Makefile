# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/24 17:01:08 by jinypark          #+#    #+#              #
#    Updated: 2022/09/28 12:24:39 by jinypark         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT

CC = cc
RM = rm -rvf

CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = libft

MLX_DIR = mlx

SRCS = minirt.c vector.c material.c utils.c object.c
BONUS = minirt.c vector.c

OBJS_SRCS = $(SRCS:.c=.o)

OBJS_BONUS = $(BONUS:.c=.o)

ifdef BONUS_FLAG
	OBJS = $(OBJS_BONUS)
else
	OBJS = $(OBJS_SRCS)
endif

all: $(NAME)

%.o : %.c
	$(CC) -c -o $@ $<

$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	# make -C $(MLX_DIR)
	$(CC) -o miniRT $(OBJS) $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx.a -framework openGL -framework AppKit -fsanitize=address

bonus:
	make BONUS_FLAG=1 all

clean:
	make clean -C $(LIBFT_DIR)
	# make clean -C $(MLX_DIR)
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@echo ">>>>>>deleted list<<<<<<<"
	$(RM) $(NAME)

re:
	make fclean
	make all

run:
	make all
	make clean
	./miniRT

.PHONY: bonus all clean fclean re
