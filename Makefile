# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/09 13:58:28 by yoelansa          #+#    #+#              #
#    Updated: 2024/05/18 18:12:12 by yoelansa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/zsh 

NAME = ircserv

SRC = main.cpp server/server.cpp client/client.cpp server/numReplies.cpp

RM = @rm -rf

CC = @c++

CPPFLAGS = -Wextra -Werror -Wall -std=c++98

CONTROL = @stty -echoctl



RESET := "\x1b[0m"
BOLD := "\x1b[1m"
RED := "\x1b[31m"
GREEN := "\x1b[32m"
YELLOW := "\x1b[33m"
BLUE := "\x1b[34m"
WHITE := "\x1b[37m"




all : $(NAME)


$(NAME) : ${SRC}
	${CC}  ${CPPFLAGS} $(SRC) -o $(NAME)
	@echo $(BOLD)$(GREEN)"\n\t✅ Mandatory Compiled\n\t" $(WHITE)"Program  "$(YELLOW)"./$(NAME)\n" $(RESET)

clean :
	${RM} *.o
	@echo $(BOLD)$(RED)"\n\t✅ Cleaning Files\n" $(RESET)

fclean :
	${RM} $(NAME)
	@echo $(BOLD)$(RED)"\n\t✅ Delete" $(WHITE)"$(NAME)\n" $(RESET)


re : fclean all


#./ircserv <port> <password>