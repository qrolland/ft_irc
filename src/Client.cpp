/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:20:51 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 15:36:24 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/irc.hpp"

/*************** Constructors and destructor ****************/
Client::Client() : _socket(-1), _hostname(""), _nickname("*"), _username("*"),  _realname("*"), _mode("*"), _fully_registered(false), _entered_valid_password(false), _welcome(false)
{
 // CGREEN("A client has been created");
}

Client::Client(int socket, std::string hostname) : _socket(socket), _hostname(hostname), _nickname("*"), _username("*"),  _realname("*"), _mode("*"), _fully_registered(false), _entered_valid_password(false), _welcome(false)
{
//  CGREEN("A client has been created");
}

Client::~Client()
{
//  CGREEN("Client destroyed");
}

Client::Client(const Client& cpy)
{
  _nickname = cpy._nickname;
  _hostname = cpy._hostname;
  _username = cpy._username;
  _socket = cpy._socket;
  _mode = cpy._mode;
  _welcome = cpy._welcome;
  _entered_valid_password = cpy._entered_valid_password;
}

/*************** Accessors ***************/
	//Getters
Channel*	Client::_getChannel(std::string name)
{
  if (_channels.empty())
    return (NULL);
  for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
  {
    if ((*it)->_getName() == name)
      return (*it);
  }
  return (NULL);
}

std::vector<Channel*>    Client::_getChannels()
{
  return (_channels);
}

int Client::_getSocket()
{
  return (_socket);
}

std::string Client::_getHostname() const
{
  return (_hostname);
}

std::string Client::_getNickname()
{
  return (_nickname);
}

std::string Client::_getUsername() const
{
  return (_username);
}

std::string Client::_getPrefix()
{
  std::string prefix;

  prefix = _nickname + "!" + _username + "@" + _hostname;
  return (prefix);
}

	//Setters
void Client::set_username(std::string username)
{
	_username = username;
}

void Client::set_nickname(std::string nickname)
{
	_nickname = nickname;
}

void Client::set_hostname(std::string hostname)
{
    _hostname = hostname;
}

void Client::set_realname(std::string realname)
{
    _realname = realname;
}

void Client::set_mode(std::string mode)
{
  _mode = mode;
}

void Client::set_channel(Channel* channel)
{
	_channels.push_back(channel);
}

void  Client::delete_channel(Channel* channel)
{
  if (_channels.empty())
    return ;
  for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
  {
    if ((*it)->_getName() == channel->_getName())
    {
      _channels.erase(it);
      return ;
    }
  }
}

void Client::set_fully_registered(bool fully_registered)
{
    _fully_registered = fully_registered;
}

void Client::set_entered_valid_password(bool entered_valid_password)
{
    _entered_valid_password = entered_valid_password;
}

/*************** Public Functions ***************/
bool Client::is_fully_registered()
{
    if (_entered_valid_password && _username != "*" && _nickname != "*")
        return (true);
    else
	    return (false);
}

void  Client::closeSocket()
{
  if (close(_socket) == -1)
    throw ("Error: could not close client socket");
}

void  Client::reply(std::string reply)
{
	reply += "\r\n";
	if (send(_socket, reply.c_str(), strlen(reply.c_str()), 0) == -1)
		throw ("Error: sending response for conenction did not work");
	CMAGENTA("sent -> " + reply);
}

void	Client::welcome()
{
	if (!_welcome)
	{
        reply(RPL_WELCOME(std::string("localhost"), _nickname, _username, _hostname));
		_welcome = true;
	}
}

bool  Client::is_in_channel(Channel *channel)
{
  if (_channels.empty())
    return (false);
  for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
  {
    if ((*it)->_getName() == channel->_getName())
      return (true);
  }
  return (false);
}
