/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhtab <aakhtab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:57:39 by yoelansa          #+#    #+#             */
/*   Updated: 2024/07/26 13:14:33 by aakhtab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::errMsg_insert() {
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
}

void server::handleNumReps( int cl_fd, int errCode, std::string cmd ) {
    std::string err;
    std::string client;
    std::stringstream ss;
    
    ss << cl_fd;
    ss >> client;

    std::string nickname = Clients[cl_fd].getNickName();
    
    if ( errCode == 401 ) // ERR_NOSUCHNICK
        err = ":" + hostname + " 401 " + nickname + errMsg[errCode];
    else if ( errCode == 403 ) // ERR_NOSUCHCHANNEL
        err = ":" + hostname + " 403 " + nickname + errMsg[errCode];
    else if ( errCode == 404 ) // ERR_CANNOTSENDTOCHAN
        err = ":" + hostname + " 404 " + nickname + " " + cmd + errMsg[errCode];
    else if ( errCode == 405 ) // ERR_TOOMANYCHANNELS
        err = client + " " + cmd + errMsg[errCode];
    else if ( errCode == 407 ) // ERR_TOOMANYTARGETS
        err = client + errMsg[errCode];
    else if ( errCode == 411 ) // ERR_NORECIPIENT
        err = ":" + hostname + " 411 " + nickname + errMsg[errCode] + cmd;
    else if ( errCode == 421 ) // ERR_UNKNOWNCOMMAND
        err = ":" + hostname + " 421 " + nickname + " " + cmd + errMsg[errCode];
    else if ( errCode == 433 ) // ERR_NICKNAMEINUSE
        err = ": " + hostname + " 433 * " + cmd + errMsg[errCode];
    else if ( errCode == 442 ) // ERR_NOTONCHANNEL
        err = ":" + hostname + " 442 " + nickname + errMsg[errCode];
    else if ( errCode == 443 ) // ERR_USERONCHANNEL
        err = nickname + errMsg[errCode];
    else if ( errCode == 451 ) // ERR_NOTREGISTERED
        err =  ":" + hostname + " 451 " + cmd + errMsg[errCode];
    else if ( errCode == 461 ) // ERR_NEEDMOREPARAMS
        err = ":" + hostname + " 461 " + nickname + errMsg[errCode];
    else if ( errCode == 462 ) // ERR_ALREADYREGISTERED
        err = client + errMsg[errCode];
    else if ( errCode == 464 ) // ERR_PASSWDMISMATCH
        err = ":" + hostname + " 464 " + nickname + errMsg[errCode];
    else if ( errCode == 471 ) // ERR_CHANNELISFULL
        err = ":" + hostname + " 471 " + nickname + " " + cmd + errMsg[errCode];
    else if ( errCode == 472 ) // ERR_UNKNOWNMODE
        err = ":" + hostname + " 472 " + nickname + errMsg[errCode];
    else if ( errCode == 473 ) // ERR_INVITEONLYCHAN
        err = ":" + hostname + " 473 " + nickname + " " + cmd + errMsg[errCode];
    else if ( errCode == 475 ) // ERR_BADCHANNELKEY
        err = ":" + hostname + " 475 " + nickname + " " + cmd + errMsg[errCode];
    else if ( errCode == 481 ) // ERR_NOPRIVILEGES
        err = ":" + hostname + " 481 " + nickname + errMsg[errCode];
    else if ( errCode == 482 ) // ERR_CHANOPRIVSNEEDED
        err = ":" + hostname + " 482 " + nickname + errMsg[errCode];
    else if ( errCode == 441 )
        err = ":" + hostname + " 441 " + nickname + errMsg[errCode];
    // else if ( errCode == 501 ) // ERR_UMODEUNKNOWNFLAG
    //     err = client + errMsg[errCode];

    send( cl_fd, err.c_str(), err.size(), 0 );
}