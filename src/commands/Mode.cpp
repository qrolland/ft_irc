/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:19:44 by eorer             #+#    #+#             */
/*   Updated: 2024/02/16 16:00:53 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

std::string get_mode_string(Channel *channel)
{
  std::string str;// = "+";
  std::string mode_set = "itkl";

  for (std::string::iterator it = mode_set.begin(); it != mode_set.end(); ++it)
  {
    if (channel->_getMode(*it))
      str += *it;
  }
  return (str);
}

std::string get_mode_parameters(Channel *channel)
{
  if (channel->_getMode('l'))
  {
    std::stringstream ss;
    ss << channel->_getLimit();
    return (ss.str());
  }
  return ("");
}

void  mode(Server *serv, t_msg msg, Client* client)
{
  Channel *channel;
  std::string mode_string;
  std::string mode_parameters;

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
  else if (msg.params.size() == 1 || (msg.params[1][0] != '+' && msg.params[1][0] != '-'))
  {
    mode_string = get_mode_string(channel);
    mode_parameters = get_mode_parameters(channel);
    client->reply(RPL_CHANNELMODEIS(serv->_getHostname(), client->_getPrefix(), channel->_getName(), mode_string, mode_parameters));
    return ;
  }
  else if (!channel->is_operator(client))
  {
    client->reply(ERR_CHANOPRIVSNEEDED(client->_getPrefix(), channel->_getName()));
    return ;
  }

  bool  oper;
  int   arg = 2;
  mode_string = msg.params[1];
  for (std::string::iterator c = mode_string.begin(); c != mode_string.end(); ++c)
  {
    if (*c == '+' || *c == '-')
    {
      oper = *c == '+' ? true : false;
      continue;
    }
    switch (*c)
    {
      case 'k':
      {
        if (static_cast<unsigned long>(arg) >= msg.params.size())
          break;
        channel->_setKey(oper ? msg.params[arg] : "");
        channel->_setMode('k', oper ? true : false);
        channel->broadcast(RPL_MODE(client->_getPrefix(), channel->_getName(), (oper ? "+k" : "-k"), (oper ? msg.params[arg] : "")));
        arg += (oper ? 1 : 0);
        break;
      }
      case 'l':
      {
        if (oper && static_cast<unsigned long>(arg) >= msg.params.size())
          break;
        channel->_setLimit(oper ? std::atoi(msg.params[arg].c_str()) : 0);
        channel->_setMode('l', oper ? true : false);
        channel->broadcast(RPL_MODE(client->_getPrefix(), channel->_getName(), (oper ? "+l" : "-l"), (oper ? msg.params[arg] : "")));
        arg += (oper ? 1 : 0);
        break;
      }
      case 'o':
      {
        if (static_cast<unsigned long>(arg) >= msg.params.size())
          break;
        Client* target = serv->findClient(msg.params[arg]);
        if (!target || !target->is_in_channel(channel))//target->_getChannel()->_getName() != channel->_getName())
          client->reply(ERR_USERNOTINCHANNEL(client->_getNickname(), msg.params[arg], channel->_getName()));
        else if (oper)
        {
          channel->addOperator(target);
          channel->broadcast(RPL_MODE(client->_getPrefix(), channel->_getName(), "+o", target->_getNickname()));
        }
        else
        {
          channel->deleteOperator(target);
          channel->broadcast(RPL_MODE(client->_getPrefix(), channel->_getName(), "-o", target->_getNickname()));
        }
        arg++;
        break;
      }
      case 'i':
      {
        channel->_setMode('i', oper ? true: false);
        channel->broadcast(RPL_MODE(client->_getPrefix(), channel->_getName(), (oper ? "+i" : "-i"), ""));
        break;
      }
      case 't':
      {
        channel->_setMode('t', oper ? true: false);
        channel->broadcast(RPL_MODE(client->_getPrefix(), channel->_getName(), (oper ? "+t" : "-t"), ""));
        break;
      }
      default:
        client->reply(ERR_UNKNOWNMODE(client->_getPrefix(), *c, channel->_getName()));
    }
  }
}
