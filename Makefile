# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yongmipa <yongmipa@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/02 14:25:53 by yongmipa          #+#    #+#              #
#    Updated: 2023/03/10 17:46:48 by yongmipa         ###   ########seoul.kr   #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Wextra
RFLAGS		= -lreadline
LFLAGS		= -L${HOME}.brew/opt/readline/lib
IFLAGS		= -I${HOME}/.brew/opt/readline/include

AR			= ar
ARFLAGS		= -rcs
RM			= rm -f

INCS_DIR	= ./includes/
SRCS_DIR	= ./src/
BUILTINS_DIR= ./src/builtin/

LIB_DIR		= ./libft

LIB_NAME	= ./libft/libft.a

INCS		= -I includes

SRC			=	list_utils.c main.c init_shell.c path_utils.c tokenize.c delete_quote.c \
				init_envp.c shell_utils.c err_print.c dollar.c find_dollar.c
BUILTIN		=	ft_export.c ft_unset.c ft_builtin.c ft_cd.c ft_echo.c ft_env.c \
				ft_exit.c ft_pwd.c

SRCS		=	$(addprefix $(SRCS_DIR), $(SRC))
LIBS		=	$(addprefix $(LIB_DIR), $(LIB_NAME))
BUILTINS	=	$(addprefix $(BUILTINS_DIR), $(BUILTIN))
OBJS		=	$(SRCS:.c=.o) $(BUILTINS:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) $(RFLAGS) $(LFLAGS) $(IFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS)
	make -C $(LIB_DIR)
	$(CC) $(CFLAGS) $(RFLAGS) $(LFLAGS) $(IFLAGS) -o $(NAME) $(OBJS) $(LIB_NAME)

clean :
	make -C $(LIB_DIR) clean
	$(RM) $(OBJS)

fclean : clean
	make -C $(LIB_DIR) fclean
	$(RM) $(NAME)

re :
	make fclean
	make all

PHONY	: all bonus clean fclean re
