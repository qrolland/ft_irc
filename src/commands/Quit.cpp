/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eorer <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:05:38 by eorer             #+#    #+#             */
/*   Updated: 2024/02/02 14:51:00 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void  quit(Server *serv, t_msg msg, Client* client)
{
  std::string reason = msg.params.size() > 0 ? msg.params[0].substr(1) : "No reason";

  client->reply(RPL_ERROR(client->_getPrefix(), reason));
  serv->handleDeconnection(client->_getSocket());
}
