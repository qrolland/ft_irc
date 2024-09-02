/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:28:30 by eorer             #+#    #+#             */
/*   Updated: 2024/02/12 13:32:58 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

void pass(Server *serv, t_msg msg, Client *client)
{
    if (msg.params.empty())
        client->reply(ERR_NEEDMOREPARAMS(client->_getPrefix(), msg.command));
    else if (client->is_fully_registered())
        client->reply(ERR_ALREADYREGISTERED(client->_getPrefix()));
    else 
    {
        if (msg.params.size() != 1 || msg.params[0] != serv->_getPwd())
            client->reply(ERR_PASSWDMISMATCH(client->_getPrefix()));
        else
            client->set_entered_valid_password(true);
    }
}
