# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/23 18:47:22 by mshpakov          #+#    #+#              #
#    Updated: 2018/10/23 18:47:24 by mshpakov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = ft_strlen.c ft_strcmp.c ft_isalpha.c ft_isupper.c ft_islower.c  \
ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_atoi.c ft_isspace.c \
ft_strncmp.c ft_tolower.c ft_toupper.c ft_strcat.c ft_strlcat.c ft_strdup.c \
ft_strcpy.c ft_strncpy.c ft_strncat.c ft_strchr.c ft_strrchr.c ft_strstr.c \
ft_strnstr.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c \
ft_memchr.c ft_memcmp.c ft_putchar.c ft_putstr.c ft_putendl.c ft_putnbr.c \
ft_strnew.c ft_memalloc.c ft_strclr.c ft_striter.c ft_striteri.c ft_strmap.c \
ft_strmapi.c ft_strequ.c ft_strnequ.c ft_strsub.c ft_memdel.c ft_strdel.c \
ft_strjoin.c ft_strtrim.c ft_strsplit.c ft_count_words.c ft_putchar_fd.c \
ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_itoa.c ft_abs_cut.c \
ft_lstnew.c ft_strulen.c ft_iswhspace.c ft_lstdelone.c ft_lstdel.c \
ft_lstadd.c ft_lstiter.c ft_lstmap.c ft_isxdigit.c ft_digittoint.c \
ft_strcasecmp.c
OBJ = $(SRC:.c=.o)
NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	@$(CC) -c $(CFLAGS) $(SRC)
	@ar rc $(NAME) $(OBJ)
clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
