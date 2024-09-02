/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:18:21 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 15:00:42 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define C_ITERATOR std::vector<Client*>::iterator
# define CH_ITERATOR std::map<std::string, Channel*>::iterator

class Server 
{
  public:
    Server();
    Server(int port, std::string pwd);
    ~Server();

    std::string           _getHostname();
    std::string           _getPwd();
    std::vector<Client*>  _getClients();
    Channel*              _getChannel(std::string name);

    void                  run();
    void                  handleCommunication(Client* client);
    void                  handleNewConnection();
    void                  handleDeconnection(int socket);

    int                   parseMessage(std::string message, t_msg &msg);
    void                  executeCommand(Client* client, std::string message);
    std::string           readRequest(Client* client);
    Client*               findClient(int socket);
    Client*               findClient(std::string nickname);
    Channel*              createChannel(std::string name);
    bool                  nickname_in_use(std::string nickname);

  private:
    
    void  _initializeCommands();

    struct                                  sockaddr_in _address;
    int                                     _epfd;
    int                                     _port;
    int                                     _socket;
    std::string                             _pwd;
    std::string                             _hostname;
    std::vector<Client*>                    _clients;
    std::map<std::string, Channel*>         _channels;
    std::map<std::string, commandFunction>  _commands;
};

#endif
