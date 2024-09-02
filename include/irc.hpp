/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:47:46 by eorer             #+#    #+#             */
/*   Updated: 2024/02/05 16:13:03 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/epoll.h>
# include <netdb.h>
# include <cstring>
# include <cerrno>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <vector>
# include <map>
# include <iostream>
# include <sstream>
# include <cstdlib>
# include <csignal>
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "color.h"
# include "Numeric_replies.hpp"

# define COUT(x) std::cout << x << std::endl;
# define CYELLOW(x) std::cout << YELLOW << x << DEFAULT << std::endl;
# define CRED(x) std::cout << RED << x << DEFAULT << std::endl;
# define CGREEN(x) std::cout << GREEN << x << DEFAULT << std::endl;
# define CMAGENTA(x) std::cout << MAGENTA << x << DEFAULT << std::endl;
# define CBLUE(x) std::cout << BLUE << x << DEFAULT << std::endl;
# define CERR(x) std::cerr << x << std::endl;

class Server;

typedef struct s_msg
{
	std::string	prefix;
	std::string	command;
    std::vector<std::string> params;
	 ~s_msg() {
        // Libérer la mémoire pour chaque vecteur dans params
        for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it) {}}
} t_msg;

typedef void (*commandFunction)(Server*, t_msg, Client*);

void  ping(Server *serv, t_msg msg, Client* client);
void  join(Server *serv, t_msg msg, Client* client);
void  privmsg(Server *serv, t_msg msg, Client* client);
void  user(Server *serv, t_msg msg, Client* client);
void  kick(Server *serv, t_msg msg, Client* client);
void  part(Server *serv, t_msg msg, Client* client);
void  quit(Server *serv, t_msg msg, Client* client);
void  topic(Server *serv, t_msg msg, Client* client);
void  user(Server* serv, t_msg msg, Client *client);
void  nick(Server* serv, t_msg msg, Client *client);
void  pass(Server *serv, t_msg msg, Client *client);
void  mode(Server *serv, t_msg msg, Client *client);
void  invite(Server *serv, t_msg msg, Client *client);
void  notice(Server *serv, t_msg msg, Client *client);

extern bool stopServer;
void signalHandler(int signum);

#endif
