/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:33:25 by eorer             #+#    #+#             */
/*   Updated: 2024/02/05 18:43:30 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <fcntl.h>

/*************** Constructors and destructor ****************/
Server::Server()
{
  COUT("Mets des parametres fdp");
}

Server::Server(int port, std::string pwd) : _port(port), _pwd(pwd)
{
  int opt_value = 1;
  char hostname[100];

  if (port < 0 || port > 65535)
    throw ("Error: port number wrong");
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket == -1)
    throw ("Error: Failed to create socket");
  memset(&_address, 0, sizeof(sockaddr_in));
  _address.sin_family = AF_INET;
  _address.sin_port = htons(_port);
  _address.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (_address.sin_addr.s_addr == INADDR_NONE)
    throw ("Error: IP address conversion failed");
  if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value)) == -1)
    throw ("Error: setsockopt failed");
  if (bind(_socket, reinterpret_cast<struct sockaddr*>(&_address), sizeof(_address)) == -1)
    throw ("Error: failed to bind socket to address");
  if (listen(_socket, 32) == -1)
    throw ("Error: failed to set socket socket as listen");
  if (getnameinfo(reinterpret_cast<struct sockaddr*>(&_address), sizeof(_address), hostname, 100, NULL, 0, NI_NAMEREQD))
    throw ("Error: getnameinfo");
  signal(SIGINT, signalHandler);
  _hostname = std::string(hostname);

  _initializeCommands();
  CGREEN("*********** Server created ***********")
}

void  Server::_initializeCommands()
{
  _commands["USER"] = &user;
  _commands["NICK"] = &nick;
  _commands["PASS"] = &pass;
  _commands["PING"] = &ping;
  _commands["JOIN"] = &join;
  _commands["PRIVMSG"] = &privmsg;
  _commands["KICK"] = &kick;
  _commands["PART"] = &part;
  _commands["QUIT"] = &quit;
  _commands["TOPIC"] = &topic;
  _commands["MODE"] = &mode;
  _commands["INVITE"] = &invite;
  _commands["NOTICE"] = &notice;
}

Server::~Server()
{
  close(_socket);
  for (C_ITERATOR it = _clients.begin(); it != _clients.end(); ++it)
  {
    (*it)->closeSocket();
    delete *it;
  }
  for (CH_ITERATOR it = _channels.begin(); it != _channels.end(); ++it)
  {
    delete it->second;
  }
  close(_epfd);
  CRED("\n*********** Server destructed ***********")
}


/*************** Accessors ****************/
std::string Server::_getHostname()
{
  return (_hostname);
}

Channel*  Server::_getChannel(std::string name)
{
  for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
  {
    if (it->second->_getName() == name)
      return (it->second);
  }
  return (NULL);
}

std::vector<Client *>  Server::_getClients()
{
  return (_clients);
}

std::string Server::_getPwd()
{
  return (_pwd);
}

/*************** Public Functions ****************/
void  Server::run(void)
{
  const int maxEvent = 20;
  struct  epoll_event e_recv[maxEvent];
  struct  epoll_event event;

        //Creation d'une instance epoll
  _epfd = epoll_create(1);
  if (_epfd == -1)
    throw ("Error: failed ot create epoll instance");
  event.events = EPOLLIN | EPOLLHUP | EPOLLERR;
  event.data.fd = _socket;
  if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _socket, &event) == -1)
    throw ("Error: failed to add socketServer to epoll interest list");
  
        //Main loop
  while (!stopServer)
  {
    int waitfds;

    CGREEN("Waiting...");
    waitfds = epoll_wait(_epfd, e_recv, maxEvent, -1);
    if (waitfds == -1)
      throw("Error: waiting events on epoll interest list");
    for (int i = 0; i < waitfds; i++)
    {
      if (e_recv[i].events & EPOLLERR || e_recv[i].events & EPOLLHUP)
        handleDeconnection(e_recv[i].data.fd);
      else if (e_recv[i].data.fd == _socket)
        handleNewConnection();
      else
      {
        Client* client = findClient(e_recv[i].data.fd);
        if (client == NULL)
          throw ("Error: couldn't find client in database");
        handleCommunication(client);
      }
    }
  }
}

void  Server::handleNewConnection()
{
  int           fd;
  std::string   answer;
  char          hostname[100];
  struct        epoll_event event;
  struct        sockaddr_in address;
  socklen_t     addrLen = sizeof(struct sockaddr_in);
  Client*       newClient;
  
  fd = accept(_socket, reinterpret_cast<struct sockaddr*>(&address), &addrLen);
  if (fd == -1)
    throw ("Error: failed to accept socket");
  if (getnameinfo(reinterpret_cast<struct sockaddr*>(&address), addrLen, hostname, 100, NULL, 0, NI_NAMEREQD))
    throw ("Error: getnameinfo");
  event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
  event.data.fd = fd;
  if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event) == -1)
    throw ("Error: failed to add client socket to epoll interest list");
  newClient = new Client(fd, std::string(hostname));
  _clients.push_back(newClient);

  std::cout << "Connection established with client " << _clients.back()->_getHostname() << ". Socket is : " << _clients.back()->_getSocket() << std::endl;
}

void  Server::handleCommunication(Client* client)
{
  std::string message;
  std::string line;

  try
  {
    message = readRequest(client);
    if (message.empty())
      return ;

    std::stringstream ss(message);
    while (std::getline(ss, line))
    {
      executeCommand(client, line);
    }
  }
  catch (char const * str)
  {
    throw(str);
  }
}

void  Server::handleDeconnection(int socket)
{
  Client*  client;

  client = findClient(socket);
  if (client == NULL)
    throw ("Could not find client in database");
  if (epoll_ctl(_epfd, EPOLL_CTL_DEL, client->_getSocket(), NULL) == -1)
    throw ("Error: failed to del client socket to epoll interest list");
  client->closeSocket();
  for (CH_ITERATOR it = _channels.begin(); it != _channels.end(); it++)
  {
    it->second->deleteClient(client);
    it->second->deleteOperator(client);
  }
  for (C_ITERATOR it = _clients.begin(); it != _clients.end(); ++it)
  {
    if (*it == client)
    {
      _clients.erase(it);
      delete client;
      break;
    }
  }
  std::cout << "Connection with a client has been lost" << std::endl;
}

std::string Server::readRequest(Client* client)
{
  int         bytesRead;
  char        buffer[512];
  std::string message;

  memset(&buffer, 0, sizeof(buffer));
  while (!std::strchr(buffer, '\n'))
  {
    memset(&buffer, 0, sizeof(buffer));
    bytesRead = recv(client->_getSocket(), buffer, sizeof(buffer), 0);//MSG_DONTWAIT
    if (bytesRead == 0)
    {
      handleDeconnection(client->_getSocket());
      return ("");
    }
    else if (bytesRead == -1)
        throw ("Error: could not read what the client sent");
    else
    {
      std::cout << client->_getSocket() << " Read -> " + std::string(buffer) << std::endl;
      message.append(buffer);
    }
  }
  return (message);
}

int Server::parseMessage(std::string message, t_msg &msg)
{
    std::string tmp = message;
    std::string command;

    if (message.empty())
        return (0);
    if (message[0] == ':') //prefixe
	{
		if (message.find(" ") != std::string::npos)
        {
            msg.prefix = message.substr(0, message.find(' '));
			tmp.erase(0, tmp.find_first_of(' ') + 1);
        }
	}
	if (tmp.find(" ") != std::string::npos) //command
	{
		for (size_t i = 0; i < tmp.find(" "); ++i) {
            msg.command += std::toupper(tmp[i]);}
        tmp.erase(0, tmp.find(" ") + 1);
    }
    else
    {
        for (size_t i = 0; i < tmp.size(); ++i) {
            msg.command += std::toupper(tmp[i]);}
        tmp.clear();
    }
    
    if (tmp.size() > 0) //params
    {
        std::stringstream ss(tmp);
        std::string arg;
        std::string sub;

        while (ss >> arg)
        {
          if (arg[0] == ':')
          {
            arg += " ";
            while (ss >> sub)
              arg += sub + " ";
            msg.params.push_back(arg);
            break;
          }
          msg.params.push_back(arg);
        }
    }
    return (1);
}

void Server::executeCommand(Client *client, std::string message)
{
  commandFunction f;
  t_msg msg;
  
  if (!parseMessage(message, msg))
    return;
  f = _commands[msg.command];
  if (f == 0)
    client->reply(ERR_UNKNOWNCOMMAND(client->_getNickname(), msg.command));
  else if (!client->is_fully_registered())
  {
    if (!(msg.command == "PASS" || msg.command == "USER" || msg.command == "NICK" || msg.command == "QUIT"))
      client->reply(ERR_NOTREGISTERED(client->_getNickname()));
    if (msg.command == "PASS")
      pass(this, msg, client);
    if (msg.command == "USER")
      user(this, msg, client);
    if (msg.command == "NICK")
      nick(this, msg, client);
    if (client->is_fully_registered())
      client->reply(RPL_WELCOME(_hostname, client->_getNickname(), client->_getUsername(), this->_getHostname()));
    if (msg.command == "QUIT")
      quit(this, msg, client);
  }
  else
    f(this, msg, client);
}

Client*  Server::findClient(int socket)
{
  C_ITERATOR client;

  for (client = _clients.begin(); client != _clients.end(); ++client)
  {
    if ((*client)->_getSocket() == socket)
      return (*client);
  }
  return (NULL);
}

Client*  Server::findClient(std::string nickname)
{
  C_ITERATOR client;

  for (client = _clients.begin(); client != _clients.end(); ++client)
  {
    if ((*client)->_getNickname() == nickname)
      return (*client);
  }
  return (NULL);
}

bool  Server::nickname_in_use(std::string nickname)
{
    C_ITERATOR it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
      if ((*it)->_getNickname() == nickname)
        return (true);
    }
    return (false);
}

Channel*  Server::createChannel(std::string name)
{
  Channel* newChannel = new Channel(name);

  _channels[name] = newChannel;
  return (newChannel);
}
