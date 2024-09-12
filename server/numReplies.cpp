/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhtab <aakhtab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:57:39 by yoelansa          #+#    #+#             */
/*   Updated: 2024/09/12 23:24:45 by aakhtab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::errMsg_insert() {
    errMsg.insert( std::make_pair( 331, " :No topic is set\r\n" ) );
    errMsg.insert( std::make_pair( 401, " :No such nick/channel\r\n" ) );
    errMsg.insert( std::make_pair( 403, " :No such channel\r\n" ) );
    errMsg.insert( std::make_pair( 404, " :Cannot send to channel\r\n" ) );
    errMsg.insert( std::make_pair( 405, " :You have joined too many channels\r\n" ) );
    errMsg.insert( std::make_pair( 407, " :Too many targets\r\n" ) );
    errMsg.insert( std::make_pair( 411, " :No recipient given\r\n" ) );
    errMsg.insert( std::make_pair( 421, " :Unknown command\r\n" ) );
    errMsg.insert( std::make_pair( 433, " :Nickname is already in use\r\n" ) );
    errMsg.insert( std::make_pair( 442, " :You're not on that channel\r\n" ) );
    errMsg.insert( std::make_pair( 443, " :is already on channel\r\n" ) );
    errMsg.insert( std::make_pair( 451, " :You have not registered\r\n" ) );
    errMsg.insert( std::make_pair( 461, " :Not enough parameters\r\n" ) );
    errMsg.insert( std::make_pair( 462, " :You may not reregister\r\n" ) );
    errMsg.insert( std::make_pair( 464, " :Password incorrect\r\n" ) );
    errMsg.insert( std::make_pair( 471, " :Cannot join channel (+l)\r\n" ) );
    errMsg.insert( std::make_pair( 472, " :is unknown mode char to me\r\n" ) );
    errMsg.insert( std::make_pair( 473, " :Cannot join channel (+i)\r\n" ) );
    errMsg.insert( std::make_pair( 475, " :Cannot join channel (+k)\r\n" ) );
    errMsg.insert( std::make_pair( 481, " :Permission Denied- You're not an IRC operator\r\n" ) );
    errMsg.insert( std::make_pair( 482, " :You're not channel operator\r\n" ) );
    errMsg.insert( std::make_pair( 441, " :They aren't on that channel\r\n") );
    errMsg.insert( std::make_pair( 501, " :Unknown MODE flag\r\n" ) );
    errMsg.insert( std::make_pair( 502, " :MODE is already set\r\n"));
    errMsg.insert( std::make_pair( 503, " :Channel name too long\r\n"));
    errMsg.insert( std::make_pair( 504, " :Too many channels\r\n"));
    errMsg.insert( std::make_pair( 505, " :Wrong number of parameters\r\n"));
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