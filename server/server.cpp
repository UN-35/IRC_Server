/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:23:34 by yoelansa          #+#    #+#             */
/*   Updated: 2024/09/25 22:25:00 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../client/client.hpp"
#include "../channel/Channel.hpp"


server::server() {}
server::~server() {}

server::server( int _port, std::string _pass ) : port( _port ), passwd( _pass ) {
    
    if ( port <= 0 || port > 65535 ) {
        std::cerr << "Error: Invalid Port Number!" << std::endl;
        exit( EXIT_FAILURE );
    }
    if ( passwd.empty() ) {
        std::cerr << "Error: It is Necessary to Set a Password!" << std::endl;
        exit( EXIT_FAILURE );
    }

    memset( &server_addr, 0, sizeof( server_addr ) ); //to ensure that all fields in the struct are initialized, which can prevent potential issues caused by uninitialized memory.

    sock_fd = socket( AF_INET, SOCK_STREAM, 0 ); //Socket creation.
    if ( sock_fd < 0 ) {
        std::cerr << "Error: Failed to Create Socket!!" << std::endl;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    server_addr.sin_port = htons( port );

    int optname;
    if ( setsockopt( sock_fd, SOL_SOCKET, SO_REUSEADDR, &optname, sizeof( optname ) ) < 0 ) {
        std::cerr << "Error: Failed to ReUse the address!" << std::endl;
        close( sock_fd );
        exit( EXIT_FAILURE );
    }
    if ( bind( sock_fd, ( struct sockaddr* )&server_addr, sizeof( server_addr ) ) < 0 ) {
        std::cerr << "Error: Failed to Bind!" << std::endl;
        close( sock_fd );
        exit( EXIT_FAILURE );
    }
    if ( listen( sock_fd, 100 ) < 0 ) {
        std::cerr << "Error: Failed to Listen on The Given Socket!" << std::endl;
        close( sock_fd );
        exit( EXIT_FAILURE );
    }

    std::cout << "Server is REAADY!" << std::endl;

    char Hname[1024];
    gethostname( Hname,1024 );
    hostname = Hname;
    
    pollfd serv_poll;
    memset( &serv_poll, 0, sizeof( serv_poll ) );
    serv_poll.fd = sock_fd;
    serv_poll.events = POLLIN;

    poll_vec.push_back( serv_poll );
    errMsg_insert();  //initialize the numeric replies errors;
}


//////   SEEEEERV !WoW!
int server::multiplex() {
    if ( poll( poll_vec.data(), poll_vec.size(), 0 ) < 0 ) {
        std::cerr << "Error: Failed to Connect With Multiple Clients" << std::endl;
        exit (EXIT_FAILURE);
    }
    for ( unsigned long i = 0; i < poll_vec.size(); i++ ) {
        pollfd &curr = poll_vec[i];
        if ( curr.revents && POLLIN ) {
            if ( curr.fd == sock_fd )
                newUser();
            else
                ClientRecv( curr.fd );
        }
    }
    return 0;
}

void server::newUser() {
    struct pollfd pollClient;
    struct sockaddr_in addrClient;
    socklen_t len = sizeof( addrClient );

    int fdClient = accept( sock_fd, ( struct sockaddr* )&addrClient, &len );
    fcntl( fdClient, F_SETFL, O_NONBLOCK );
    if ( fdClient < 0 ) {
        std::cerr << "Error: Failed to Accept Clients" << std::endl;
        close( fdClient );
        exit( EXIT_FAILURE );
    }

    pollClient.fd = fdClient;
    pollClient.events = POLLIN;
    poll_vec.push_back( pollClient );
    Client _client( fdClient, passwd );
    Clients.insert( std::make_pair( fdClient, _client ) );
    std::cout << "The Client [ " << fdClient << " ] connected!" << std::endl;
}

void server::ClientRecv( int clientFileD ) {
    std::vector<char> buffer(5000);

    time_t startChronoBot;
    
    ssize_t index = read( clientFileD, buffer.data(), buffer.size() );
    if ( !index ) { //means that the client disconnected
        std::string quit = ": " + Clients[clientFileD].getNickName() + " QUIT  " + "\r\n";
                    for ( std::map<int, Client>::iterator it = Clients.begin(); it != Clients.end(); it++ ) {
                        if ( it->first != clientFileD )
                            send( it->first, quit.c_str(), quit.size(), 0 );
                    }
        std::cout << "Client [" << Clients[clientFileD].getNickName() << "] Left the CLUUB!" << std::endl;
        close( clientFileD );
        Clients.erase( clientFileD );
    }
    if ( buffer[index - 1] != '\n' ) {
        Clients[clientFileD].setBuffer( buffer.data() );
        return ;
    } else
        Clients[clientFileD].setBuffer( buffer.data() );
    

    // get the line from buffer and parse it with deleting /r /n (for numeric replies);
    std::vector<std::string> splitted = splitVec( Clients[clientFileD].getBuffer(), '\n' );
    Clients[clientFileD].erase_buff();

    for ( std::vector<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++ ) {
        std::string line = cleanLine( *it );

        if ( line.empty() )
            return ;

        size_t sp = line.find(" ");


        std::string nick = Clients[clientFileD].getNickName();
        std::string cmd = line.substr( 0, sp );

        // if ( cmd == "CAP" )
        //     continue;

        if ( Clients[clientFileD].auth[2] == false ) {
            if ( cmd == "PASS" ) {
                std::string pass( line.substr( sp + 1 ));
                if ( pass != passwd ) {
                    handleNumReps( clientFileD, 464 , line ); // ERR_PASSWDMISMATCH
                    return ;
                }
                else
                    Clients[clientFileD].auth[0] = true;
            } else if (cmd == "NICK" ) {
                if ( Clients[clientFileD].auth[0] == false ) {
                    std::string needpass = "You need to set a Password first.\r\n";
                    send( clientFileD, needpass.c_str(), needpass.size(), 0 );
                    return ;
                }
                std::vector<std::string> NName = splitVec( line.substr( sp + 1 ), ' ' );
                if ( NName.size() != 1 ) {
                    handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    return ;
                } else if ( searchByNName(NName[0]) != -1 ) {
                    handleNumReps( clientFileD, 433, NName[0] ); //ERR_NICKNAMEINUSE
                    return ;
                } 
                else if ( NName[0].at(0) == '#' || NName[0].at(0) == ':' ) {
                    // handleNumReps( clientFileD, 432, NName[0] ); //ERR_ERRONUSENICKNAME
                    // this 432 error disconnect the client from the server, but it works fine with just return();
                    return ;
                }
                else {
                    Clients[clientFileD].setNickName( NName[0] );
                    std::string rep = ":" + nick + "!" + Clients[clientFileD].getUserName() + "@localhost NICK " +  Clients[clientFileD].getNickName() + "\r\n";
                    send( clientFileD, rep.c_str(), rep.size(), 0 );
                    Clients[clientFileD].auth[1] = true;
                }
            } else if ( cmd == "USER" ) {
                if ( Clients[clientFileD].auth[1] == false ) {
                    std::string neednick = "You need to set Password and NickName both first.\r\n";
                    send( clientFileD, neednick.c_str(), neednick.size(), 0 );
                    return ;
                }
                std::vector<std::string> username = splitVec( line.substr( sp + 1 ), ' ' );
                if ( Clients[clientFileD].getUserName().empty() ) {
                    Clients[clientFileD].setUserName( username[0] );

                    std::string rep1 = ":" + hostname + " 001 " + Clients[clientFileD].getNickName() + " :Welcome to IRC server\r\n";
                    std::string rep2 = ":" + hostname + " 002 " + Clients[clientFileD].getNickName() + " :Your host is Running on v1.0\r\n";
                    std::string rep3 = ":" + hostname + " 003 " + Clients[clientFileD].getNickName() + " :This server was created now\r\n";
                    std::string rep4 = ":" + hostname + " 004 " + Clients[clientFileD].getNickName() + " :FT_irc v1.0\r\n";

                    send( clientFileD, rep1.c_str(), rep1.size(), 0 );
                    send( clientFileD, rep2.c_str(), rep2.size(), 0 );
                    send( clientFileD, rep3.c_str(), rep3.size(), 0 );
                    send( clientFileD, rep4.c_str(), rep4.size(), 0 );

                    Clients[clientFileD].auth[2] = true;
                }

            }
            if ( !Clients[clientFileD].getUserName().empty() && !Clients[clientFileD].getNickName().empty() ) {
                std::string n = Clients[clientFileD].getNickName();
                std::string u = Clients[clientFileD].getUserName();
                // std::string welcome = "\033[1;34m :" + hostname + " 001 " + "" + n + "" + " :Welcome to the IRC Network, " + n + "!" + u + "@" + hostname + "  \r\n";
                // send( clientFileD, welcome.c_str(), welcome.size(), 0 );
                std::cout << "Client [" << n << "] Joined the CLUUB!" << std::endl;
                //
                //
                //
                startChronoBot = time(NULL);
                //
                //
            }
        } else {
            std::cout << "line = " << line << std::endl;
            if ( Clients[clientFileD].auth[2] == false ) {
                handleNumReps( clientFileD, 451, line ); // ERR_NOTREGISTERED
                return ;
            }
            if (cmd == "PRIVMSG") {
                std::vector<std::string> msg = splitVec( line.substr( sp + 1 ), ' ' );
                std::cout << msg[0] << std::endl;
                if ( msg.size() < 2 ) {
                    handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                } else {
                    size_t i = msg[0].length() + 2;
                    std::string message = line.substr( sp + i );
                    std::string channel_name = msg[0];
                    if ( channel_name[0] == '#')
                    {
                        Channel* channel = searchChannel( msg[0]);
                        if ( channel == NULL ) {
                            send( clientFileD, "HER1\r\n", 6, 0 );
                            handleNumReps( clientFileD, 403, msg[0] ); //ERR_NOSUCHCHANNEL
                        }
                        else if ( channel->clientExist( nick ) == false ){
                            handleNumReps( clientFileD, 442, nick ); //ERR_NOTONCHANNEL
                        }
                        else {
                            std::string msgToSend = ":" + nick + " PRIVMSG " + channel_name + " :" + message + "\r\n";
                            std::map <std::string, Client> clients_list = channel->getClientList();
                            for ( std::map<std::string, Client>::iterator it = clients_list.begin(); it != clients_list.end(); it++ ) {
                                if ( it->first != nick ) {
                                    send( it->second.getFd(), msgToSend.c_str(), msgToSend.size(), 0 );
                                }
                            }
                        }
                    }
                    else {
                        int fd = searchByNName( msg[0] );
                        if ( fd == -1 ) {
                            handleNumReps( clientFileD, 401, msg[0] ); //ERR_NOSUCHNICK
                        } else {
                            std::string msgToSend = ": " + nick + "  PRIVMSG " + channel_name + " " + message + "\r\n";
                            send( fd, msgToSend.c_str(), msgToSend.size(), 0 );
                        }
                    }
                }
            }
            else if (cmd == "NICK" ) {
                if ( Clients[clientFileD].auth[0] == false ) {
                    std::string needpass = "You need to set a Password first.\r\n";
                    send( clientFileD, needpass.c_str(), needpass.size(), 0 );
                    return ;
                }
                std::vector<std::string> NName = splitVec( line.substr( sp + 1 ), ' ' );
                if ( NName.size() != 1 ) {
                    handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    return ;
                } else if ( searchByNName(NName[0]) != -1 ) {
                    handleNumReps( clientFileD, 433, NName[0] ); //ERR_NICKNAMEINUSE
                    return ;
                } 
                else if ( NName[0].at(0) == '#' || NName[0].at(0) == ':' ) {
                    // handleNumReps( clientFileD, 432, NName[0] ); //ERR_ERRONUSENICKNAME
                    // this 432 error disconnect the client from the server, but it works fine with just return();
                    return ;
                }
                else {
                    Clients[clientFileD].setNickName( NName[0] );
                    std::string rep = ":" + nick + "!" + Clients[clientFileD].getUserName() + "@localhost NICK " +  Clients[clientFileD].getNickName() + "\r\n";
                    nick = Clients[clientFileD].getNickName();
                    send( clientFileD, rep.c_str(), rep.size(), 0 );
                    Clients[clientFileD].auth[1] = true;
                }
            }
            else if( cmd == "PING" ) {
                std::string pong = "PONG " + line.substr( sp + 1 ) + "\r\n";
                if ( line.substr( sp + 1 ) == hostname){
                    send( clientFileD, pong.c_str(), pong.size(), 0 );
                }
                send( clientFileD, pong.c_str(), pong.size(), 0 );
            }
            else if ( cmd == "QUIT") {
                    std::string quitMsg = line.substr( sp + 1 );
                    std::string quit = ": " + nick + " QUIT "+ quitMsg + "\r\n";
                    for ( std::map<int, Client>::iterator it = Clients.begin(); it != Clients.end(); it++ ) {
                        if ( it->first != clientFileD )
                            send( it->first, quit.c_str(), quit.size(), 0 );
                    }
                    std::cout << "Client [ " << Clients[clientFileD].getNickName() << " ] Left the CLUUB!" << std::endl;
                    close( clientFileD );
                    Clients.erase( clientFileD );
                    Clients[clientFileD].setNickName("");
                    Clients[clientFileD].setUserName("");
                    Clients[clientFileD].setChanLimit(0);
                    //
                    //
                    //
                } // JOIN COMMMAND
                else if ( cmd == "JOIN"){
                    std::vector<std::string> msg = splitVec( line.substr( sp + 1 ), ' ' );
                    if (msg.size() < 1 || msg.size() > 2)
                        handleNumReps( clientFileD, 505, line ); //ERR_NEEDMOREPARAMS
                    else {
                        std::string channel_name;// = msg[0];
                        std::string nick = Clients[clientFileD].getNickName();
                        std::vector<std::string> channels_name = splitVec( msg[0], ',');
                        std::vector<std::string> keys = splitVec( "", ',');
                        if (msg.size() == 2)
                            keys = splitVec( msg[1], ',');
                        // std::cout << "keys.size() = " << keys.size() << std::endl;
                        // std::cout << "channels_name.size() = " << channels_name.size() << std::endl;
                        for (size_t i = 0; i < channels_name.size(); i++){
                            channel_name = channels_name[i];
                            if ( channel_name[0] != '#' ) {
                                // send( clientFileD, "HER2\r\n", 6, 0 );
                                handleNumReps( clientFileD, 403, channel_name ); //ERR_NOSUCHCHANNEL
                            } else if ( channel_name[0] == '#' && Clients[clientFileD].getChanLimit() > 9 ) {
                                handleNumReps( clientFileD, 405, channel_name ); //ERR_TOOMANYCHANNELS
                            } else if (!keys.empty() && channels_name.size() != keys.size()) {
                                handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                                break;
                            }else if (channels_name.size() > 5) {
                                handleNumReps( clientFileD, 504, channel_name ); //ERR_TOOMANYCHANN
                                break;
                            }else if (channel_name.size() > 20) {
                                handleNumReps( clientFileD, 503, channel_name ); //ERR_CHANNELNAMETOOLONG
                            }else {
                                Channel* channel = searchChannel( channel_name );
                                if ( channel == NULL) {
                                    // if (msg.size() != 1)
                                    //     handleNumReps(clientFileD, 475, channel_name );
                                    // else {
                                        Channel newChannel( channel_name );
                                        Channels.push_back( newChannel );
                                        channel = &Channels.back();
                                        Clients[clientFileD].setOperator( true );
                                        channel->addClientToChannel( Clients[clientFileD] );
                                        Clients[clientFileD].addJoindChan();
                                        // std::string join = ":" + nick + " JOIN " + channel_name + "\r\n";
                                        if (Clients[clientFileD].getOperator() == true) {
                                            channel->addFirstOperator( nick );
                                        // }
                                    }
                                }
                                else if (channel->isModeSet("k") && ( keys.empty() || keys[i] != channel->getChannelPassword()))
                                    handleNumReps( clientFileD, 475, channel_name ); //ERR_BADCHANNELKEY
                                else if (channel->isModeSet("k") == false && !keys[i].empty())
                                    handleNumReps( clientFileD, 475, channel_name ); //ERR_BADCHANNELKEY
                                else if (channel->isModeSet("l") && channel->getCapacityLimit() == channel->getClientList().size()) {
                                    handleNumReps( clientFileD, 471, channel_name ); //ERR_CHANNELISFULL
                                } else if ( channel->isModeSet("i") && channel->isInvited( nick ) == false ) {
                                    handleNumReps( clientFileD, 473, channel_name ); //ERR_INVITEONLYCHAN
                                // } else if ( channel->isModeSet("k") && channel->getChannelPassword() != msg[1] && msg.size() != 2) { // #TODO: check if the args are more than 4
                                //     handleNumReps( clientFileD, 475, channel_name ); //ERR_BADCHANNELKEY
                                } else if ( channel->clientExist(nick) ){
                                    handleNumReps(clientFileD, 443, channel_name ); //ERR_ALREADYONCHANNEL
                                }
                                else {
                                    channel->addClientToChannel( Clients[clientFileD] );
                                    Clients[clientFileD].addJoindChan();
                                    std::string join = ":" + nick + " JOIN " + channel_name + "\r\n";
                                    if (!channel->getTopic().empty()){
                                        std::string topicMessage = ":server_name 332 " + nick + " " + channel_name + " :" + channel->getTopic() + "\r\n";
                                        send(clientFileD, topicMessage.c_str(), topicMessage.size(), 0);
                                    }
                                    std::map <std::string, Client> clients_list = channel->getClientList();
                                    for ( std::map<std::string, Client>::iterator it = clients_list.begin(); it != clients_list.end(); it++ ) {
                                        if ( it->first != nick ) {
                                            send( it->second.getFd(), join.c_str(), join.size(), 0 );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }// KICK command 
                else if (cmd == "KICK"){
                    std::vector<std::string> msg = splitVec( line.substr( sp + 1 ), ' ' );
                    std::string nick = Clients[clientFileD].getNickName();
                    std::string kick;
                    if ( msg.size() < 2 ) {
                        handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    } else {
                        std::string channel_name = msg[0];
                        Channel* channel = searchChannel( channel_name );
                        if ( channel == NULL ) {
                            // send( clientFileD, "HER3\r\n", 6, 0 );
                            handleNumReps( clientFileD, 403, channel_name ); //ERR_NOSUCHCHANNEL
                        } else if ( channel->clientExist( nick ) == false ) {
                            handleNumReps( clientFileD, 442, nick ); //ERR_NOTONCHANNEL
                        } else if ( nick != msg[1] ){
                            if ( channel->isOperator( nick ) == false ) {
                                handleNumReps( clientFileD, 482, nick ); //ERR_CHANOPRIVSNEEDED
                            } else {
                                std::string kicked = msg[1];
                                if ( channel->clientExist( kicked ) == false ) {
                                    handleNumReps( clientFileD, 441, kicked ); //ERR_USERNOTINCHANNEL
                                } else {
                                    channel->removeClientFromChannel( kicked );
                                    Clients[clientFileD].delJoindChan();
                                    std::map <std::string, Client> clients_list = channel->getClientList();
                                    if ( msg.size() > 2){
                                        std::string reason = line.substr( sp + 1 + msg[0].length() + msg[1].length() + 2 );
                                        kick = ":" + nick + " KICK " + channel_name + " " + kicked + " :" + reason + "\r\n";
                                        
                                    }else {
                                        kick = ":" + nick + " KICK " + channel_name + " " + kicked + " :" + "\r\n";
                                    }
                                    for ( std::map<std::string, Client>::iterator it = clients_list.begin(); it != clients_list.end(); it++ ) {
                                        send( it->second.getFd(), kick.c_str(), kick.size(), 0 );
                                    }
                                    send( server::searchByNName( kicked ), kick.c_str(), kick.size(), 0 );
                                }
                            }
                        }
                    }
                } // INVITE command
                else if (cmd == "INVITE")
                {
                    std::vector<std::string> msg = splitVec( line.substr( sp + 1 ), ' ' );
                    std::string nick = Clients[clientFileD].getNickName();
                    std::string invite;
                    std::string invited;
                    if ( msg.size() < 2)
                        handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    else {
                        std::string channel_name = msg[1];
                        Channel* channel = searchChannel( channel_name );
                        invited = msg[0];
                        if ( channel == NULL ){
                            // send( clientFileD, "HER4\r\n", 6, 0 );
                            handleNumReps( clientFileD, 403, channel_name ); //ERR_NOSUCHCHANNEL
                        } else if ( channel->clientExist( nick ) == false )
                            handleNumReps( clientFileD, 442, nick ); //ERR_NOTONCHANNEL
                        else if ( server::searchByNName( invited ) == -1 )
                            handleNumReps( clientFileD, 401, invited ); //ERR_NOSUCHNICK
                        else {
                            channel->addInvited( invited );
                            invite = ": " + nick + " INVITE " + invited + " " + channel_name + "  \r\n";
                            send( server::searchByNName( invited ), invite.c_str(), invite.size(), 0 );
                        }
                    }
                } // TOPIC command
                else if (cmd == "TOPIC") {
                    std::vector<std::string> msg = splitVec( line.substr( sp + 1 ), ' ' );
                    std::string topic;
                    std::string channel_name;
                    if (msg.size() < 1)
                        handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    else {
                        channel_name = msg[0];
                        Channel* channel = searchChannel( channel_name );
                        if (channel == NULL) {
                            handleNumReps(clientFileD, 403, channel_name); // ERR_NOSUCHCHANNEL
                        } else if (!channel->clientExist(nick)) {
                            handleNumReps(clientFileD, 442, nick); // ERR_NOTONCHANNEL
                        } else if (channel->isModeSet("t") && !channel->isOperator(nick) && msg.size() > 1) {
                            handleNumReps(clientFileD, 482, nick); // ERR_CHANOPRIVSNEEDED
                        } else if (msg.size() > 1) { // The client is setting a new topic
                            topic = line.substr(sp + 1 + msg[0].length() + 1) + "\r\n";
                            channel->setTopic(topic);

                            // Notify the channel about the new topic (RPL_TOPIC)
                            std::string topicMessage = ":server_name 332 " + nick + " " + channel_name + " :" + topic;
                            for (std::map<std::string, Client>::iterator it = channel->getClientList().begin(); it != channel->getClientList().end(); it++) {
                                send(it->second.getFd(), topicMessage.c_str(), topicMessage.size(), 0);
                            }

                        } else if (msg.size() == 1) { // The client is requesting the current topic
                            if (channel->getTopic().empty()) {
                                // No topic is set, send RPL_NOTOPIC (331)
                                std::string noTopicMessage = ":server_name 331 " + nick + " " + channel_name + " :No topic is set\r\n";
                                send(clientFileD, noTopicMessage.c_str(), noTopicMessage.size(), 0);
                            } else {
                                // A topic is set, send RPL_TOPIC (332)
                                std::string topicMessage = ":server_name 332 " + nick + " " + channel_name + " :" + channel->getTopic() + "\r\n";
                                send(clientFileD, topicMessage.c_str(), topicMessage.size(), 0);

                                // Optionally, send the topic setter information (RPL_TOPICWHOTIME 333)
                                // std::string topicInfoMessage = ":server_name 333 " + nick + " " + channel_name + " " + channel->getTopicSetter() + " " + std::to_string(channel->getTopicTimestamp()) + "\r\n";
                                // send(clientFileD, topicInfoMessage.c_str(), topicInfoMessage.size(), 0);
                            }
                        }
                    }
                } // MODE command
                else if (cmd == "MODE") {
                    std::vector<std::string> msg = splitVec( line.substr( sp + 1 ), ' ');
                    std::string modes;
                    std::string channel_name;
                    if (msg.size() < 2)
                        handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    else if (msg[0] == nick ){
                        // Do nothing ---------------
                    }
                    else {
                        channel_name = msg[0];
                        Channel* channel = searchChannel( channel_name );
                        modes = msg[1];
                        if ( channel == NULL ){
                            // send( clientFileD, "HER6\r\n", 6, 0 );
                            handleNumReps( clientFileD, 403, channel_name ); //ERR_NOSUCHCHANNEL
                        }
                        else {
                            if (modes.length() != 2)
                                handleNumReps( clientFileD, 501, line ); //ERR_UMODEUNKNOWNFLAG
                            else if (modes[1] != 'i' && modes[1] != 't' && msg.size() == 2)
                                handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                            else if (modes[0] != '+' && modes[0] != '-')
                                handleNumReps( clientFileD, 501, line ); //ERR_UMODEUNKNOWNFLAG
                            else if (channel->validMode(modes[1]) == false)
                                handleNumReps( clientFileD, 501, line ); //ERR_UMODEUNKNOWNFLAG
                            else if (channel->isOperator(nick) == false)
                                handleNumReps( clientFileD, 482, line ); //ERR_CHANOPRIVSNEEDED
                            else if (modes[0] == '+' && modes[1] == 'i' && channel->isModeSet("i") == false)
                                channel->addMode('i');
                            else if (modes[0] == '-' && modes[1] == 'i' && channel->isModeSet("i") == true)
                                channel->removeMode("i");
                            else if (modes[0] == '+' && modes[1] == 'o')
                            {
                                if (channel->clientExist(msg[2]) == false)
                                    handleNumReps(clientFileD, 441, msg[2]); //ERR_USERNOTINCHANNEL
                                else if (channel->isOperator(msg[2]) == true)
                                    handleNumReps(clientFileD, 482, msg[2]); //ERR_CHANOPRIVSNEEDED
                                else {
                                    channel->addOperator(msg[2]);
                                }
                            }
                            else if (modes[0] == '-' && modes[1] == 'o')
                            {
                                if (channel->clientExist(msg[2]) == false)
                                    handleNumReps(clientFileD, 441, msg[2]); //ERR_USERNOTINCHANNEL
                                else if (channel->isOperator(msg[2]) == false)
                                    handleNumReps(clientFileD, 482, msg[2]); //ERR_CHANOPRIVSNEEDED
                                else
                                    channel->removeOperator(msg[2]);
                            }
                            else if (modes[0] == '+' && modes[1] == 'k')
                            {
                                if (msg.size() < 3)
                                    handleNumReps(clientFileD, 461, line); //ERR_NEEDMOREPARAMS
                                else{
                                    channel->addMode('k');
                                    channel->setChannelPassword(msg[2]);
                                }
                            }
                            else if (modes[0] == '-' && modes[1] == 'k')
                            {
                                if (msg.size() < 3)
                                    handleNumReps(clientFileD, 461, line); //ERR_NEEDMOREPARAMS
                                else if (channel->getChannelPassword() != msg[2])
                                    handleNumReps(clientFileD, 475, channel_name); //ERR_BADCHANNELKEY
                                else{
                                    channel->removeMode("k");
                                    channel->setChannelPassword("");
                                }
                            }
                            else if (modes[0] == '+' && modes[1] == 'l')
                            {
                                if (msg.size() < 3)
                                    handleNumReps(clientFileD, 461, line); //ERR_NEEDMOREPARAMS
                                else if (channel->isModeSet("l") == true)
                                    handleNumReps(clientFileD, 502, line); //ERR_KEYSET
                                else{
                                    channel->addMode('l');
                                    channel->_strLimit = msg[2];
                                    channel->setCapacityLimit(atoi(msg[2].c_str()));
                                }
                            }
                            else if (modes[0] == '-' && modes[1] == 'l')
                            {
                                if (msg.size() < 2)
                                    handleNumReps(clientFileD, 461, line); //ERR_NEEDMOREPARAMS
                                else if (channel->isModeSet("l") == false)
                                    handleNumReps(clientFileD, 502, line); //ERR_KEYSET
                                else{
                                    channel->removeMode("l");
                                    channel->setCapacityLimit(-1);
                                }
                            }
                            else if (modes[0] == '+' && modes [1] == 't')
                            {
                                if (channel->isModeSet("t") == true)
                                    handleNumReps(clientFileD, 502, line); //ERR_KEYSET
                                else
                                    channel->addMode('t');
                            }
                            else if (modes[0] == '-' && modes [1] == 't')
                            {
                                if (channel->isModeSet("t") == false)
                                    handleNumReps(clientFileD, 502, line); //ERR_KEYSET
                                else
                                    channel->removeMode("t");
                            }
                            else
                                handleNumReps(clientFileD, 501, line); //ERR_UMODEUNKNOWNFLAG
                        }
                    }
                } else if (cmd == "/EMET"){
                    std::string emet_help = " Usage: /EMET [OPTION] [ARGUMENT]\n"
                        " Options:\n"
                        "   -h, --help\t\tDisplay this help and exit\n"
                        "   -c, --channels list\t\tList all channels available\n"
                        "   -u, --users list\t\tList all users connected\n"
                        "   -cu, --list-users [CHANNEL]\t\tList all users in a specific channel\n"
                        "   -cm, --channel-mode [CHANNEL]\t\tList all modes of a specific channel\n"
                        "   -i, --invited \t\tAll channels you are invited to\r\n";
                    std::vector<std::string> option = splitVec( line.substr( sp + 1 ), ' ' );
                    std::string msg;
                    if (option[0] == "-h" && option.size() < 2){
                        send( clientFileD, emet_help.c_str(), emet_help.size(), 0 );
                    } else if (option[0] == "-c" && option.size() < 2) {
                        msg = "Channels Available: \r\n";
                        send( clientFileD, msg.c_str(), msg.size(), 0 );
                        for (std::vector<Channel>::iterator it = Channels.begin(); it != Channels.end(); it++){
                            msg = "- " + it->getName() + "\r\n";
                            send( clientFileD, msg.c_str(), msg.size(), 0 );
                        }
                    // } else if (option[0] == "-u" && option.size() < 2) {
                    //     msg = "Users Connected: \r\n";
                    //     send( clientFileD, msg.c_str(), msg.size(), 0 );
                    //     for (std::map<int, Client>::iterator it = Clients.begin(); it != Clients.end(); it++){
                    //         if ( it->first. ){
                    //             msg = "- " + it->second.getNickName() + "\r\n";
                    //             send( clientFileD, msg.c_str(), msg.size(), 0 );
                    //         }
                    //     }
                    } else if (option[0] == "-cu" && option.size() == 2) {
                        Channel* channel = searchChannel( option[1] );
                        if (channel == NULL) {
                            // send( clientFileD, "HER7\r\n", 6, 0 );
                            handleNumReps( clientFileD, 403, option[1] ); //ERR_NOSUCHCHANNEL
                        } else if (channel->clientExist(nick) == false){
                            msg = "(Permession denied!)\n"
                                "You are not in the channel\r\n";
                            send( clientFileD, msg.c_str(), msg.size(), 0 );
                        }else {
                            std::map<std::string, Client> clients = channel->getClientList();
                            msg = ""+ option[1] + " Users: \r\n";
                            send( clientFileD, msg.c_str(), msg.size(), 0 );
                            for (std::map<std::string, Client>::iterator it = clients.begin(); it != clients.end(); it++){
                                msg = "- " + it->first + "\r\n";
                                send( clientFileD, msg.c_str(), msg.size(), 0 );
                            }
                        }
                    } else if (option[0] == "-cm" && option.size() == 2) {
                        Channel* channel = searchChannel( option[1] );
                        if (channel == NULL) {
                            // send( clientFileD, "HER8\r\n", 6, 0 );
                            handleNumReps( clientFileD, 403, option[1] ); //ERR_NOSUCHCHANNEL
                        } else if (channel->clientExist(nick) == false){
                            msg = "(Permession denied!)\n"
                                "You are not in the channel\r\n";
                            send( clientFileD, msg.c_str(), msg.size(), 0 );
                        }else {
                            msg = ""+ option[1] + " Modes: \r\n";
                            send( clientFileD, msg.c_str(), msg.size(), 0 );
                            msg = "- ";
                            if (channel->isModeSet("i")){
                                msg += "- Mode (i) : Invite Only\r\n";
                            } if (channel->isModeSet("t")){
                                msg += "- Mode (t) : Topic Set\r\n";
                            } if (channel->isModeSet("o"))
                            {
                                msg += "- Mode (o) : Operators [";
                                for (std::vector<std::string>::iterator it = channel->getOperators().begin(); it != channel->getOperators().end(); it++){
                                    msg += *it + " ";
                                }
                                msg += "]\r\n";
                            } if (channel->isModeSet("k")) {
                                msg += "- Mode (k) : Password Set\r\n";
                            } if (channel->isModeSet("l")) {
                                msg += "- Mode (l) : Capacity Limit (" + channel->_strLimit + ")\r\n";
                            }
                            msg += "\r\n";
                            send( clientFileD, msg.c_str(), msg.size(), 0 );
                        }
                    } else if (option[0] == "-i" && option.size() < 2) {
                        msg = "Channels You are Invited to: \r\n";
                        send( clientFileD, msg.c_str(), msg.size(), 0 );
                        bool check = false;
                        for (std::vector<Channel>::iterator it = Channels.begin(); it != Channels.end(); it++){
                            if (it->isInvited(nick)){
                                check = true;
                                msg = "- " + it->getName() + "\r\n";
                                send( clientFileD, msg.c_str(), msg.size(), 0 );
                            }
                            if (!check){
                                msg = "- No Channels Invited to\r\n";
                                send( clientFileD, msg.c_str(), msg.size(), 0 );
                            }
                        }
                        
                    } else {
                        send( clientFileD, emet_help.c_str(), emet_help.size(), 0 );
                    }
                }
                else if ( cmd == "BOT" ) {
                    // std::string bot = "I am Thaaa BOT\r\n";
                    // send( clientFileD, bot.c_str(), bot.size(), 0);
                    std::vector<std::string> option = splitVec( line.substr( sp + 1 ), ' ');

                    if ( option[0] == "--help" || option[0] == "-h" ) {
                        std::string help = "NAME :\r\n    BOT\r\nSYNOPSIS :\r\n    BOT [Option]\r\nVALID OPTIONS :\r\n    -h, --help : shows informations about the Bot.\r\n    -d : shows the duration you have been online on the our IRCserver\r\n    -cc : this is the channel counter, it tells you how many channels you are currenly a member of\r\n";
                        send( clientFileD, help.c_str(), help.size(), 0 );
                    } else if ( option[0] == "-d" ) {
                        // Record the end time
                        time_t endChrono = time(NULL);
                    
                        // Calculate the duration
                        double duration = difftime(endChrono, startChronoBot);
                        int minutes = static_cast<int>(duration) / 60;
                        int remainingSeconds = static_cast<int>(duration) % 60;
                        
                        std::ostringstream oss;
                        if (minutes > 0) {
                            oss << minutes << "min";
                        }
                        if (remainingSeconds > 0 || minutes == 0) {
                            if (minutes > 0) oss << " ";
                            oss << remainingSeconds << "s";
                        }
        
                        std::string ConnectionChrono =  "You have been online on the server for : " + oss.str() + " now.\r\n";
                        send( clientFileD, ConnectionChrono.c_str(), ConnectionChrono.size(), 0 );
                    }
                    else if ( option[0] == "-cc" ) { //for channel counter
                        int count = Clients[clientFileD].getChanLimit();
                        std::ostringstream oss_count;
                        oss_count << count;
                        std::string counter;
                        if ( count <= 1 )
                            counter = "You are currently a member in " + oss_count.str() + " channel\r\n";
                        else
                            counter = "You are currently a member in " + oss_count.str() + " channels\r\n";
                        send( clientFileD, counter.c_str(), counter.size(), 0 );
                        
                    }
                    
                    else {
                        std::string noOption = "Try BOT -h/--help to show all the valid options IRCbot can do.\r\n";
                        send( clientFileD, noOption.c_str(), noOption.size(), 0 );
                        return ;
                    }
                }
                // else {
                //     std::string msg = "Invalid Command!\r\n";
                //     send( clientFileD, msg.c_str(), msg.size(), 0 );
                // }
        }
    }
}



// some utilssssss
std::vector<std::string> server::splitVec( std::string str, char c ) {
    std::vector<std::string> splitted;
    std::string tmp;
    
    for ( size_t i = 0; i < str.size(); i++ ) {
        if ( str[i] == c ) {
            splitted.push_back( tmp );
            tmp.clear();
        } else
            tmp += str[i];
    }
    splitted.push_back( tmp );
    return splitted;
}


std::string server::cleanLine( std::string line ) {
    for ( size_t i = 0; i < line.size(); i++ ) {
        if ( line[i] == '\n' || line[i] == '\r' )
            line.erase( i, 1 );
    }
    return line;
}


int server::searchByNName( std::string NName ) {
    for ( std::map<int, Client>::iterator it = Clients.begin(); it != Clients.end(); it++ ) {
        if ( it->second.getNickName() == NName )
            return it->first;
    }
    return -1;
}

Channel* server::searchChannel( std::string channelName ) {
    for ( std::vector<Channel>::iterator it = Channels.begin(); it != Channels.end(); it++ ) {
        if ( it->getName() == channelName )
            return &(*it);
    }
    return NULL;
}

bool server::isDup(std::string modes)
{
    int i = 0;
    int t = 0;
    int o = 0;
    int k = 0;
    int l = 0;
    for (size_t j = 0; j < modes.size(); j++){
        if (modes[j] == 'i')
            i++;
        if (modes[j] == 't')
            t++;
        if (modes[j] == 'o')
            o++;
        if (modes[j] == 'k')
            k++;
        if (modes[j] == 'l')
            l++;
    }
    std::cout << i << " " << t << " "<< o<< " " << l << " "<< k << std::endl;
    if (i > 1 || t > 1 || o > 1 || k > 1 || l > 1)
        return true;
    return false;
}

bool server::parseMode(std::string modes)
{
    std::string valid = "ioklt";
    std::cout << "modes: " << modes << std::endl;
    
    if (isDup(modes))
        return false;
    for (size_t i = 0; i < modes.size(); i++){
        if (modes[i] == '+'){
            for (size_t j = i + 1; j < modes.size(); j++){
                if (modes[modes.size() - 1] == '+' || modes[modes.size() - 1] == '-')
                    return false;
                else if ((modes[j] == '-' || modes[j] == '+') && j == (i + 1) )
                    return false;
                else if (valid.find(modes[j]) != std::string::npos)
                    continue ;
                else if (modes[j] == '-' || modes[j] == '+')
                    break ;
                else 
                    return false;
            }
        } else if (modes[i] == '-') {
            for (size_t j = i + 1; j < modes.size(); j++){
                if (modes[modes.size() - 1] == '+' || modes[modes.size() - 1] == '-')
                    return false;
                else if ((modes[j] == '-' || modes[j] == '+') && j == (i + 1) )
                    return false;
                else if (valid.find(modes[j]) != std::string::npos)
                    continue ;
                else if (modes[j] == '-' || modes[j] == '+')
                    break ;
                else 
                    return false;
            }
        }
    }
    return true;
}