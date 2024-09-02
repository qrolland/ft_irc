/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qrolland <qrolland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:36:52 by eorer             #+#    #+#             */
/*   Updated: 2024/02/02 14:49:58 by eorer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/irc.hpp"

static int is_valid_username( std::string username )
{
	const std::string rejected_chars = "@._";

	if (std::isdigit(username[0]))
		return (0);
	for (size_t i = 0; i < username.size(); i++)
	{
		if (!std::isalnum(username[i]) && rejected_chars.find(username[i]) != std::string::npos)
			return (0);
	}
	return (1);
}

void user(Server* serv, t_msg msg, Client *client)
{
    (void)serv;
    if (msg.params.size() != 4 || msg.params[3][0] != ':' || !is_valid_username(msg.params[0]))
        client->reply(ERR_NEEDMOREPARAMS(client->_getPrefix(), msg.command));
    else if (client->is_fully_registered())
        client->reply(ERR_ALREADYREGISTERED(client->_getPrefix()));
    else
    {
        client->set_username(msg.params[0]);
        client->set_mode(msg.params[1]);
        client->set_realname(msg.params[3].substr(1, msg.params[3].size()));
    }
}
