# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aakhtab <aakhtab@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/09 13:58:28 by yoelansa          #+#    #+#              #
#    Updated: 2024/07/18 09:31:13 by aakhtab          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

AUTHOR = yoelansa && aakhtab

SHELL := /bin/zsh 

NAME = ircserv

SRC = main.cpp server/server.cpp client/client.cpp server/numReplies.cpp	\
		channel/Channel.cpp 

RM = @rm -rf

CC = @c++

CPPFLAGS = -Wextra -Werror -Wall -std=c++98
OBJ = $(SRC:.cpp=.o)
CONTROL = @stty -echoctl

RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m
GREEN = \033[0;32m
CYAN = \033[0;36m
BOLD = \033[1m
GREY = \033[1;30m


all : $(NAME)


$(NAME) : ${SRC}
	@printf "$(YELLOW)Compiling...$(RESET)\n"
	${CC}  ${CPPFLAGS} $(SRC) -o $(NAME)
	@printf  "$(GREEN)Author\t\t: $(CYAN)$(AUTHOR)\n"
	@printf  "$(GREEN)CC    \t\t: $(YELLOW)$(CC)\n\033[m"
	@printf  "$(GREEN)FLAGS \t\t: $(YELLOW)$(CPPFLAGS)\n\033[m"
	@printf  "$(GREEN)Program  \t: $(YELLOW)$(NAME)\n\033[m"
	@printf "$(YELLOW)\nUSAGE: \n\t$(BOLD)$(RESET)./ircserv <port> <password>$(RESET)\n"

clean :
	${RM} $(OBJ)
	@printf "$(RED)$(NAME) deleted !$(RESET)\n"
	

fclean :
	${RM} $(NAME)
	@printf "$(RED)$(NAME) deleted !$(RESET)\n"


re : fclean all


#./ircserv <port> <password>