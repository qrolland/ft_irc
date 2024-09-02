/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:04:55 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 16:26:43 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void  notice(Server *serv, t_msg msg, Client* client)
{
  std::string target = msg.params[0];
  std::string prefix = client->_getPrefix();
  std::string message;
  Channel *channel;

  if (msg.params.empty())
    return ;
  else if (target[0] != '&' && target[0] != '#')
  {
    Client*  dest = serv->findClient(target);

    if (dest)
      dest->reply(RPL_NOTICE(prefix, dest->_getNickname(), (msg.params[1][0] == ':' ? msg.params[1].substr(1) : msg.params[1])));
  }
  else
  {
    channel = serv->_getChannel(target);
    if (channel && client->is_in_channel(channel))//client->_getChannel() && client->_getChannel()->_getName() == channel->_getName())
      channel->broadcast(RPL_NOTICE(prefix, channel->_getName(), msg.params[1].substr(1)), client);
  }
}
