#ifndef NUMERICAL_REPLIES_HPP
#define NUMERICAL_REPLIES_HPP

  //Numeric errors
# define ERR_ALREADYREGISTERED(client) (std::string(":localhost 462 ") + client + " :You may not reregister.")
# define ERR_NICKNAMEINUSE(client, nickname) (std::string(":localhost 433 ") + client + " " + nickname + " :Nickname is already in use.")
# define ERR_ERRONEUSNICKNAME(client, nickname) (std::string(":localhost 432 ") + client + " " + nickname + " :Erroneus nickname")
# define ERR_NONICKNAMEGIVEN(client) (std::string(":localhost 431 ") + client + " :There is no nickname.")
# define ERR_UNKNOWNCOMMAND(nick, command) ":localhost 421 " + nick + " " + command + " :Unknown command"
# define ERR_NEEDMOREPARAMS(nick, command) ":localhost 461 " + nick + " " + command + " :Not enough parameters"
# define ERR_NOSUCHNICK(nick, target) ":localhost 401 " + nick + " " + target + " :No such nick/channel"
# define ERR_CHANOPRIVSNEEDED(nick, channel) ":localhost 482 " + nick + " " + channel + " :You are not channel operator"
# define ERR_NOSUCHCHANNEL(nick, channel) ":localhost 403 " + nick + " " + channel + " :No such channel"
# define ERR_USERNOTINCHANNEL(nick, target, channel) ":localhost 441 " + nick + " " + target + " " + channel + " :They are not in the channel"
# define ERR_CANNOTSENDTOCHAN(nick, channel) ":localhost 404 " + nick + " " + channel + " :Cannot send to channel"
# define ERR_NOTONCHANNEL(nick, channel) ":localhost 442 " + nick + " " + channel + " :You are not on the channel"
# define ERR_NOORIGIN ":localhost 409 :No origin specified"
# define ERR_PASSWDMISMATCH(nick) ":localhost 464 " + nick + " :Password incorrect"
# define ERR_NOTREGISTERED(nick) ":localhost 451 " + nick + " :You need to register"
# define ERR_UNKNOWNMODE(nick, mode, channel) ":localhost 472 " + nick + " " + mode + " :is unknown mode char to me for channel " + channel
# define ERR_BADCHANNELKEY(nick, channel) ":localhost 475 " + nick + " " + channel + " :Wrong channel key"
# define ERR_CHANNELISFULL(nick, channel) ":localhost 471 " + nick + " " + channel + " :Channel is full"
# define ERR_INVITEONLYCHAN(nick, channel) ":localhost 473 " + nick + " " + channel + " :You must get an invitation"

  //Numeric replies
# define RPL_WELCOME(server, client, user, host) ":" + server + " 001 " + client + " :Welcome on irc Server <3 " + client + "!" + user + "@" + host
# define RPL_WELCOME_PWD(server, client) ":" + server + " 001 " + client + " :Password is good"
# define RPL_TOPIC(server, client, channel, topic) ":" + server + " 332 " + client + " " + channel + " :" + topic
# define RPL_NOTOPIC(server, client, channel) ":" + server + " 331 " + client + " " + channel + " :No topic set"
# define RPL_NAMREPLY(server, client, channel, clients) ":" + server + " 353 " + client + " = " + channel + " :" + clients
# define RPL_ENDOFNAMES(server, client, channel) ":" + server + " 366 " + client + " " + channel + " :End of /NAMES list"
# define RPL_CHANNELMODEIS(server, client, channel, mode_string, mode_parameters) ":" + server + " 324 " + client + " " + channel + " " + mode_string + " " + mode_parameters
# define RPL_INVITING(server, client, target, channel) ":" + server + " 342 " + client + " " + target + " " + channel

  //Command replies
# define RPL_PONG(prefix, dest) ":" + prefix + " PONG " + dest
# define RPL_JOIN(prefix, channel) ":" + prefix + " JOIN " + channel
# define RPL_PRIVMSG(prefix, target, message) ":" + prefix + " PRIVMSG " + target + " :" + message
# define RPL_NOTICE(prefix, target, message) ":" + prefix + " NOTICE " + target + " :" + message
# define RPL_KICK(prefix, channel, user, reason) ":" + prefix + " KICK " + channel + " " + user + " :" + reason
# define RPL_PART(prefix, channel, reason) ":" + prefix + " PART " + channel + " :" + reason
# define RPL_ERROR(prefix, reason) ":" + prefix + " ERROR :" + reason
# define CMD_TOPIC(prefix, channel, topic) ":" + prefix + " TOPIC " + channel + " :" + topic
# define RPL_NICK(prefix, new_nick) ":" + prefix + " NICK " + new_nick
# define RPL_INVITE(prefix, target, channel) ":" + prefix + " INVITE " + target + " " + channel
# define RPL_MODE(prefix, channel, mode_string, mode_parameters) ":" + prefix + " MODE " + channel + " " + mode_string + " " + mode_parameters

#endif
