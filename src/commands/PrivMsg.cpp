/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:47:59 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 15:22:07 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void  privmsg(Server *serv, t_msg msg, Client* client)
{
  std::string target;
  std::string prefix = client->_getPrefix();
  std::string message;
  Channel *channel;

  if (msg.params.empty() || msg.params.size() < 2)
    client->reply(ERR_NEEDMOREPARAMS(client->_getNickname(), msg.command));
  else if (msg.params[0][0] != '&' && msg.params[0][0] != '#')
  {
    target = msg.params[0];
    Client*  dest = serv->findClient(target);
    if (!dest)
      client->reply(ERR_NOSUCHNICK(client->_getNickname(), target));
    else
      dest->reply(RPL_PRIVMSG(prefix, dest->_getNickname(), (msg.params[1][0] == ':' ? msg.params[1].substr(1) : msg.params[1])));
  }
  else
  {
    target = msg.params[0];
    channel = serv->_getChannel(target);
    if (!channel)
      client->reply(ERR_NOSUCHCHANNEL(client->_getNickname(), target));
    else if (!client->is_in_channel(channel))//(!client->_getChannel(channel->_getName()) || client->_getChannel()->_getName() != channel->_getName())
      client->reply(ERR_CANNOTSENDTOCHAN(client->_getNickname(), channel->_getName()));
    else
      channel->broadcast(RPL_PRIVMSG(prefix, channel->_getName(), (msg.params[1][0] == ':' ? msg.params[1].substr(1) : msg.params[1])), client);
  }
}
