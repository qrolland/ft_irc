/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:57:28 by eorer             #+#    #+#             */
/*   Updated: 2024/02/02 17:12:38 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void  topic(Server *serv, t_msg msg, Client* client)
{
  Channel*  channel;
  std::string topic;

  if (msg.params.size() == 0)
  {
    client->reply(ERR_NEEDMOREPARAMS(client->_getPrefix(), msg.command));
    return ;
  }
  channel = serv->_getChannel(msg.params[0]);
  if (!channel)
  {
    client->reply(ERR_NOSUCHCHANNEL(client->_getPrefix(), msg.params[0]));
    return ;
  }
  if (msg.params.size() == 1 && channel->_getTopic().empty())
      client->reply(RPL_NOTOPIC(serv->_getHostname(), client->_getPrefix(), channel->_getName()));
  else if (msg.params.size() == 1 && !channel->_getTopic().empty())
      client->reply(RPL_TOPIC(serv->_getHostname(), client->_getPrefix(), channel->_getName(), channel->_getTopic()));
  else 
  {
    if (!channel->_getMode('t') && !channel->is_operator(client))
    {
      client->reply(ERR_CHANOPRIVSNEEDED(client->_getPrefix(), channel->_getName()));
      return;
    }
    topic = msg.params[1].substr(1);
    channel->_setTopic(topic);
    channel->broadcast(CMD_TOPIC(client->_getPrefix(), channel->_getName(), topic));
  }
}
