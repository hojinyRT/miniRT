# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/24 17:01:08 by jinypark          #+#    #+#              #
#    Updated: 2022/09/29 13:45:16 by jinypark         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	   ft_memchr.c ft_memcmp.c ft_strlen.c ft_strlcpy.c \
	   ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strnstr.c \
	   ft_strncmp.c ft_atoi.c ft_isalpha.c ft_isdigit.c \
	   ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c \
	   ft_tolower.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
	   ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	   ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	   get_next_line.c get_next_line_utils.c ft_atoi_base.c

BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
		ft_lstmap.c

OBJS_SRCS = $(SRCS:.c=.o)

OBJS_BONUS = $(BONUS:.c=.o)

ifdef BONUS_FLAG
	OBJS = $(OBJS_SRCS:.c=.o) $(BONUS:.c=.o)
else
	OBJS = $(OBJS_SRCS:.c=.o)
endif

all: $(NAME)

%.o : %.c
	@echo "[$<] compiling ..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	@echo "[$@] making library ..."
	@ar rcs $@ $^


bonus:
	make BONUS_FLAG=1 all

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: bonus all clean fclean re
