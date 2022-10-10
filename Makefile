# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/24 17:01:08 by jinypark          #+#    #+#              #
#    Updated: 2022/10/10 11:47:53 by minsuki2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 			=	cc
NAME		=	miniRT
RM 			=	rm -rvf
CFLAGS 		=	-Wall -Wextra -Werror # -g3 -fsanitize=address
LIBFT_DIR 	=	libft
LIBFT 		=	libft.a
MLX_DIR 	=	mlx2
MLX 		=	libmlx.dylib
# MLX 	=	libmlx.a

MANDA_SRCS 	=	minirt.c vector.c material.c utils.c object.c
BONUS_SRCS 	=	minirt.c vector.c

OBJS_MANDA 	=	$(MANDA_SRCS:.c=.o)
OBJS_BONUS	=	$(BONUS_SRCS:.c=.o)

ifdef BONUS_FLAG
	OBJS = $(OBJS_BONUS)
else
	OBJS = $(OBJS_MANDA)
endif

all: | $(LIBFT_DIR)/$(LIBFT) $(MLX) $(NAME)

$(NAME): $(OBJS)
	@echo "[$@] linking ..."
	@$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lmlx -L$(LIBFT_DIR) -lft -framework openGL -framework AppKit
# @$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lmlx $(LIBFT_DIR)/libft.a -framework openGL -framework AppKit
#	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx.a -framework openGL -framework AppKit 
#

$(MLX):
	@make -C $(MLX_DIR)
	@echo "[$@] making ..."
	@mv $(MLX_DIR)/$(MLX) .

$(LIBFT_DIR)/$(LIBFT):
	@make -C $(LIBFT_DIR)/
	@echo "[$@] making ..."


%.o : %.c
	@echo [$<] compiling ...
	@$(CC) $(CFLAGS) -c -o $@ $<

bonus:
	make BONUS_FLAG=1 all

clean:
	@echo ">>>>>>clean deleted list<<<<<<<"
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(MLX_DIR)
	@$(RM) $(OBJS) $(OBJS_BONUS)

fclean: clean
	@echo ">>>>>>fclean deleted list<<<<<<<"
	@$(RM) $(MLX)
	@$(RM) $(LIBFT_DIR)/$(LIBFT)
	@$(RM) $(NAME)

re:
	@make fclean
	@make all

run:
	@make all
	@make clean
	./miniRT

.PHONY: all clean fclean re bonus
