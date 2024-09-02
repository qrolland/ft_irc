# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/23 11:51:01 by eorer             #+#    #+#              #
#    Updated: 2024/02/05 16:13:13 by eorer            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++

CFLAGS = -Wall -Wextra -Werror -g3 -std=c++98

SRC = $(addprefix src/, main.cpp Server.cpp Client.cpp Channel.cpp signal.cpp)

SRC += $(addprefix src/commands/, Ping.cpp Join.cpp PrivMsg.cpp Kick.cpp Part.cpp Quit.cpp Topic.cpp User.cpp Nick.cpp Pass.cpp Mode.cpp Invite.cpp Notice.cpp)

OBJ = $(SRC:.cpp=.o)

NAME = ircserv


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
