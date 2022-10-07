# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/24 17:01:08 by jinypark          #+#    #+#              #
#    Updated: 2022/10/07 09:55:02 by jinypark         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 			=	cc
NAME		=	miniRT
RM 			=	rm -rvf
CFLAGS 		=	-Wall -Wextra -Werror -g3 -fsanitize=address
LIBFT_DIR 	=	libft
MLX_DIR 	=	mlx

MANDA_SRCS 	=	minirt.c vector.c material.c utils.c object.c put.c remove.c
BONUS_SRCS 	=	minirt.c vector.c

OBJS_MANDA 	=	$(MANDA_SRCS:.c=.o)
OBJS_BONUS	=	$(BONUS_SRCS:.c=.o)

ifdef BONUS_FLAG
	OBJS = $(OBJS_BONUS)
else
	OBJS = $(OBJS_MANDA)
endif

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	@echo "[$@] linking ..."
#	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx.a -framework openGL -framework AppKit 
	@$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lmlx $(LIBFT_DIR)/libft.a -framework openGL -framework AppKit 
# make -C $(MLX_DIR)

%.o : %.c
	@echo [$<] compiling ...
	@$(CC) $(CFLAGS) -c -o $@ $<

bonus:
	make BONUS_FLAG=1 all

clean:
	make clean -C $(LIBFT_DIR)
	@echo ">>>>>>deleted list<<<<<<<"
	@$(RM) $(OBJS) $(OBJS_BONUS)
# make clean -C $(MLX_DIR)

fclean: clean
	@echo ">>>>>>deleted list<<<<<<<"
	@make fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME)

re:
	@make fclean
	@make all

run:
	@make all
	@make clean
	./miniRT

.PHONY: all clean fclean re bonus
