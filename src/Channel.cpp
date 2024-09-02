/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:59:39 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 15:55:17 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/Channel.hpp"
#include "../include/irc.hpp"
#include <limits.h>

/*************** Constructors and destructor ****************/
Channel::Channel(){}

Channel::Channel(std::string name) : _name(name), _key(""), _limit(0)
{
  _modes['i'] = false;
  _modes['t'] = false;
  _modes['k'] = false;
  _modes['l'] = false;
}

Channel::~Channel(){}


/*************** Accessors ****************/
  //Getters
std::string Channel::_getName() const
{
  return (this->_name);
}

std::string Channel::_getTopic() const
{
  return (this->_topic);
}

std::string Channel::_getNicknames()
{
  std::string nicknames;

  for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
  {
    if (is_operator(*it))
      nicknames.append("@");
    nicknames.append((*it)->_getNickname() + " ");
  }
  return (nicknames);
}

std::string Channel::_getKey()
{
  return (_key);
}

int Channel::_getLimit()
{
  return (_limit);
}

int Channel::_getMode(char mode)
{
  bool  value;
  std::string mode_set = "itkl";

  if (mode_set.find(mode) == std::string::npos)
    return (-1);
  value = _modes[mode];
  return (value ? 1 : 0);
}

std::vector<Client *> Channel::_getMembers()
{
  return (_members);
}

  //Setters
int  Channel::_setMode(char mode, bool value)
{
  std::string mode_set = "itkl";

  if (mode_set.find(mode) == std::string::npos)
    return (-1);
  _modes[mode] = value;
  return (0);
}

void  Channel::_setTopic(std::string topic)
{
  _topic = topic;
}

void  Channel::_setKey(std::string pwd)
{
  _key = pwd;
}

void  Channel::_setLimit(double limit)
{
  if (limit < 0 || limit > INT_MAX)
    return ;
  _limit = limit;
}


/*************** Public Functions ****************/
void  Channel::broadcast(std::string message)
{
  message += "\r\n";
  for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
  {
    if (send((*it)->_getSocket(), message.c_str(), strlen(message.c_str()), 0) == -1)
      throw ("Error: pb with broadcast all");
    CMAGENTA("broadcast -> " + message);
  }
}

void  Channel::broadcast(std::string message, Client* client)
{
  message += "\r\n";
  for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
  {
    if ((*it)->_getSocket() == client->_getSocket())
      continue;
    if (send((*it)->_getSocket(), message.c_str(), strlen(message.c_str()), 0) == -1)
      throw ("Error: pb with broadcasti from a client");
    CMAGENTA("msgchannel -> " + message);
  }
}

void  Channel::addClient(Client* newClient)
{
  _members.push_back(newClient);
}

void  Channel::addOperator(Client* client)
{
  _operators.push_back(client);
}

void  Channel::addGuest(Client* client)
{
  _guests.push_back(client);
}

bool  Channel::deleteClient(Client* client)
{
  for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
  {
    if (client->_getSocket() == (*it)->_getSocket())
    {
      deleteGuest(client);
      _members.erase(it);
      return (true);
    }
  }
  return (false);
}

bool  Channel::deleteOperator(Client* client)
{
  for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); ++it)
  {
    if (client->_getSocket() == (*it)->_getSocket())
    {
      _operators.erase(it);
      return (true);
    }
  }
  return (false);
}

bool  Channel::deleteGuest(Client* client)
{
  for (std::vector<Client *>::iterator it = _guests.begin(); it != _guests.end(); ++it)
  {
    if (client->_getSocket() == (*it)->_getSocket())
    {
      _guests.erase(it);
      return (true);
    }
  }
  return (false);
}

bool  Channel::is_operator(Client* client)
{
  std::vector<Client *>::iterator  it;

  for (it = _operators.begin(); it != _operators.end(); ++it)
  {
    if ((*it)->_getNickname() == client->_getNickname())
      return (true);
  }
  return (false);
}

bool  Channel::is_guest(Client* client)
{
  std::vector<Client *>::iterator  it;

  for (it = _guests.begin(); it != _guests.end(); ++it)
  {
    if ((*it)->_getNickname() == client->_getNickname())
      return (true);
  }
  return (false);
}

bool  Channel::is_member(Client *client)
{
  std::vector<Client *>::iterator  it;

  for (it = _members.begin(); it != _members.end(); ++it)
  {
    if ((*it)->_getNickname() == client->_getNickname())
      return (true);
  }
  return (false);
}
