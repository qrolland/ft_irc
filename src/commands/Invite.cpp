/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:56:51 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 15:22:43 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void  invite(Server *serv, t_msg msg, Client* client)
{
  Channel *channel;
  Client  *target;

  if (msg.params.size() != 2)
  {
    client->reply(ERR_NEEDMOREPARAMS(client->_getNickname(), msg.command));
    return ;
  }

  channel = serv->_getChannel(msg.params[1]);
  target = serv->findClient(msg.params[0]);
  if (!channel)
    client->reply(ERR_NOSUCHCHANNEL(client->_getNickname(), msg.params[1]));
  else if (!channel->is_operator(client))
    client->reply(ERR_CHANOPRIVSNEEDED(client->_getNickname(), channel->_getName()));
  else if (!target || !client->is_in_channel(channel))// || client->_getChannel()->_getName() != channel->_getName())
    client->reply(ERR_NOTONCHANNEL(client->_getNickname(), channel->_getName()));
  else
  {
    channel->addGuest(target);
    client->reply(RPL_INVITING(serv->_getHostname(), client->_getNickname(), target->_getNickname(), channel->_getName()));
    target->reply(RPL_INVITE(client->_getPrefix(), target->_getNickname(), channel->_getName()));
  }

}
