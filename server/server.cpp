/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:23:34 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/18 18:19:01 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


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
    // std::cout << "The Client [ " << fdClient << " ] connected!" << std::endl;
}

void server::ClientRecv( int clientFileD ) {
    std::vector<char> buffer(5000);
    ssize_t index = read( clientFileD, buffer.data(), buffer.size() );
    if ( !index ) { //means that the client disconnected
        std::cerr << "Client [" << Clients[clientFileD].getNickName() << "] Disconnected" << std::endl;
        //
        // ATTENTION... you might Need to add a call for /QUIT command Func in here;
        // 
        exit(1); // maybe u'll never gonna need this exit after adding the /QUIT command ;
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
        if ( sp == std::string::npos || sp == 0 || line[sp + 1] == '\0' ) {
            handleNumReps( clientFileD, 461, line ); // ERR_NEEDMOREPARAMS
            return ;
        }
        
        std::string cmd = line.substr( 0, sp );
        
        // if ( cmd == "CAP" )
        //     continue;

        if ( Clients[clientFileD].getAuth() == false ) {
            if ( cmd != "PASS" && cmd != "pass" ) {
                handleNumReps( clientFileD, 451, line ); // ERR_NOTREGISTERED
            } else {
                std::string pass( line.substr( sp + 1 ));
                if ( pass != passwd ){
                    handleNumReps( clientFileD, 464 , line ); // ERR_PASSWDMISMATCH
                } else
                    Clients[clientFileD].setAuth( true );
            }
        } else {
            if ( Clients[clientFileD].getNickName().empty() || Clients[clientFileD].getUserName().empty() ) {
                if ( cmd == "NICK" || cmd == "nick" ) {
                    std::vector<std::string> NName = splitVec( line.substr( sp + 1 ), 1 );
                    if ( NName.size() != 1 ) {
                        handleNumReps( clientFileD, 461, line ); //ERR_NEEDMOREPARAMS
                    } else if ( searchByNName(NName[0]) != -1 ) {
                        handleNumReps( clientFileD, 433, NName[0] ); //ERR_NICKNAMEINUSE
                    } else {
                        if ( Clients[clientFileD].getNickName().empty() ) {
                            Clients[clientFileD].setNickName( NName[0] );
                            std::string errM = " :" + Clients[clientFileD].getNickName() + " NICK " + Clients[clientFileD].getNickName() + "\r\n";
                            send( clientFileD, errM.c_str(), errM.size(), 0 );
                        } else {
                            handleNumReps( clientFileD, 433, line ); //ERR_NICKNAMEINUSE
                        }
                    }
                } else if ( cmd == "USER" || cmd == "user" ) {
                    std::vector<std::string> username = splitVec( line.substr( sp + 1 ), ' ' );
                    if ( Clients[clientFileD].getUserName().empty() ) {
                        Clients[clientFileD].setUserName( username[0] );
                    } else {
                        handleNumReps( clientFileD, 462, line ); //ERR_ALREADYREGISTERED
                    }
                } else {
                    handleNumReps( clientFileD, 451, line); //ERR_NOTREGISTERED
                }
                if ( !Clients[clientFileD].getUserName().empty() && !Clients[clientFileD].getNickName().empty() ) {
                    std::string n = Clients[clientFileD].getNickName();
                    std::string u = Clients[clientFileD].getUserName();
                    std::string welcome = ":" + hostname + " 001 " + n + " :Welcome to the IRC Network, " + n + "!" + u + "@" + hostname + "\r\n";
                    send( clientFileD, welcome.c_str(), welcome.size(), 0 );
                    std::cout << "Client [" << n << "] Joined the CLUUB!" << std::endl;
                }
            } else {
                ;/// here we start handle the commaaands---->>>>>>>>>
            }
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