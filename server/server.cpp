/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:23:34 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/12 21:00:58 by yoelansa         ###   ########.fr       */
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
    //handle init msg err;
}
