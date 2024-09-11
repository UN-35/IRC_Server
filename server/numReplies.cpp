/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhtab <aakhtab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:57:39 by yoelansa          #+#    #+#             */
/*   Updated: 2024/09/11 15:22:49 by aakhtab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::errMsg_insert() {
    errMsg.insert( std::make_pair( 331, "\033[1;31m :No topic is set\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 401, "\033[1;31m :No such nick/channel\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 403, "\033[1;31m :No such channel\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 404, "\033[1;31m :Cannot send to channel\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 405, "\033[1;31m :You have joined too many channels\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 407, "\033[1;31m :Too many targets\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 411, "\033[1;31m :No recipient given\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 421, "\033[1;31m :Unknown command\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 433, "\033[1;31m :Nickname is already in use\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 442, "\033[1;31m :You're not on that channel\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 443, "\033[1;31m :is already on channel\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 451, "\033[1;31m :You have not registered\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 461, "\033[1;31m :Not enough parameters\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 462, "\033[1;31m :You may not reregister\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 464, "\033[1;31m :Password incorrect\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 471, "\033[1;31m :Cannot join channel (+l)\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 472, "\033[1;31m :is unknown mode char to me\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 473, "\033[1;31m :Cannot join channel (+i)\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 475, "\033[1;31m :Cannot join channel (+k)\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 481, "\033[1;31m :Permission Denied- You're not an IRC operator\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 482, "\033[1;31m :You're not channel operator\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 441, "\033[1;31m :They aren't on that channel\033[0m\033[1m\r\n") );
    errMsg.insert( std::make_pair( 501, "\033[1;31m :Unknown MODE flag\033[0m\033[1m\r\n" ) );
    errMsg.insert( std::make_pair( 502, "\033[1;31m :MODE is already set\033[0m\033[1m\r\n"));
    errMsg.insert( std::make_pair( 503, "\033[1;31m :Channel name too long\033[0m\033[1m\r\n"));
    errMsg.insert( std::make_pair( 504, "\033[1;31m :Too many channels\033[0m\033[1m\r\n"));
    errMsg.insert( std::make_pair( 432, " :Erroneus nickname\r\n"));
}

void server::handleNumReps( int cl_fd, int replyCode, std::string cmd ) {
    std::string err;
    std::string client;
    std::stringstream ss;
    
    ss << cl_fd;
    ss >> client;

    std::string nickname = Clients[cl_fd].getNickName();


    if ( replyCode == 432 ) // ERR_ERRONEUSNICKNAME
        err = ":" + hostname + " 432 " + nickname + errMsg[replyCode];
    else if ( replyCode == 401 ) // ERR_NOSUCHNICK
        err = ":" + hostname + " 401 " + nickname + errMsg[replyCode];
    else if ( replyCode == 403 ) // ERR_NOSUCHCHANNEL
        err = ":" + hostname + " 403 " + nickname + errMsg[replyCode];
    else if ( replyCode == 404 ) // ERR_CANNOTSENDTOCHAN
        err = ":" + hostname + " 404 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 405 ) // ERR_TOOMANYCHANNELS
        err = ":" + hostname + " 405 " + nickname + errMsg[replyCode];
    else if ( replyCode == 407 ) // ERR_TOOMANYTARGETS
        err = client + errMsg[replyCode];
    else if ( replyCode == 411 ) // ERR_NORECIPIENT
        err = ":" + hostname + " 411 " + nickname + errMsg[replyCode] + cmd;
    else if ( replyCode == 421 ) // ERR_UNKNOWNCOMMAND
        err = ":" + hostname + " 421 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 433 ) // ERR_NICKNAMEINUSE
        err = ": " + hostname + " 433 * " + cmd + errMsg[replyCode];
    else if ( replyCode == 442 ) // ERR_NOTONCHANNEL
        err = ":" + hostname + " 442 " + nickname + errMsg[replyCode];
    else if ( replyCode == 443 ) // ERR_USERONCHANNEL
        err = ":" + hostname + " 443 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 451 ) // ERR_NOTREGISTERED
        err =  ":" + hostname + " 451 " + cmd + errMsg[replyCode];
    else if ( replyCode == 461 ) // ERR_NEEDMOREPARAMS
        err = ":" + hostname + " 461 " + nickname + errMsg[replyCode];
    else if ( replyCode == 462 ) // ERR_ALREADYREGISTERED
        err = client + errMsg[replyCode];
    else if ( replyCode == 464 ) // ERR_PASSWDMISMATCH
        err = ":" + hostname + " 464 " + nickname + errMsg[replyCode];
    else if ( replyCode == 471 ) // ERR_CHANNELISFULL
        err = ":" + hostname + " 471 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 472 ) // ERR_UNKNOWNMODE
        err = ":" + hostname + " 472 " + nickname + errMsg[replyCode];
    else if ( replyCode == 473 ) // ERR_INVITEONLYCHAN
        err = ":" + hostname + " 473 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 475 ) // ERR_BADCHANNELKEY
        err = ":" + hostname + " 475 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 481 ) // ERR_NOPRIVILEGES
        err = ":" + hostname + " 481 " + nickname + errMsg[replyCode];
    else if ( replyCode == 482 ) // ERR_CHANOPRIVSNEEDED
        err = ":" + hostname + " 482 " + nickname + errMsg[replyCode];
    else if ( replyCode == 441 )
        err = ":" + hostname + " 441 " + nickname + errMsg[replyCode];
    else if ( replyCode == 501 ) // ERR_UMODEUNKNOWNFLAG
        err = client + errMsg[replyCode];
    else if ( replyCode == 331 )
        err = ":" + hostname + " 331 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 503 ) // ERR_CHANNELNAMELONG
        err = ":" + hostname + " 503 " + nickname + errMsg[replyCode];
    else if ( replyCode == 502 ) // ERR_MODEALREADYSET
        err = ":" + hostname + " 502 " + nickname + " " + cmd + errMsg[replyCode];
    else if ( replyCode == 504 ) // ERR_TOOMANYCHANN
        err = ":" + hostname + " 504 " + nickname + errMsg[replyCode];

    send( cl_fd, err.c_str(), err.size(), 0 );
}