/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:58:51 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 17:08:47 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void  kick(Server *serv, t_msg msg, Client* client)
{
  Channel *channel;
  Client* target;
  std::string prefix = client->_getPrefix();

  if (msg.params.size() < 3)
  {
    client->reply(ERR_NEEDMOREPARAMS(client->_getNickname(), msg.command));
    return;
  }

  std::string reason = msg.params[2].substr(1).empty() ? "No reason" : msg.params[2].substr(1);
  channel = serv->_getChannel(msg.params[0]);
  if (!channel)
    client->reply(ERR_NOSUCHCHANNEL(client->_getNickname(), msg.params[0]));
  else if (!channel->is_operator(client))
    client->reply(ERR_CHANOPRIVSNEEDED(client->_getNickname(), channel->_getName()));
  else
  {
    target = serv->findClient(msg.params[1]);
    if (!target || !channel->deleteClient(target))
      client->reply(ERR_USERNOTINCHANNEL(client->_getNickname(), msg.params[1], channel->_getName()));
    else
    {
      channel->broadcast(RPL_KICK(prefix, channel->_getName(), target->_getNickname(), reason));
      target->reply(RPL_KICK(prefix, channel->_getName(), target->_getNickname(), reason));
      target->delete_channel(channel);
    }
  }
}
