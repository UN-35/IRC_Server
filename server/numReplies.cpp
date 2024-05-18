/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:57:39 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/18 13:43:32 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::errMsg_insert() {
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
    // errMsg.insert( std::make_pair( 501, " :Unknown MODE flag\r\n" ) );
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
    // else if ( errCode == 501 ) // ERR_UMODEUNKNOWNFLAG
    //     err = client + errMsg[errCode];

    send( cl_fd, err.c_str(), err.size(), 0 );
}