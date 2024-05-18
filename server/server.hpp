/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:17:38 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/18 17:04:35 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <sstream>

#include "../client/client.hpp"

// class Client;

class server {
    private:
        int port;
        std::string passwd;
        sockaddr_in server_addr;
        int sock_fd;
        std::string hostname;

        std::map<int, Client> Clients; // fds, cl
        std::vector<pollfd> poll_vec; // fds, events, revents
        std::map<int, std::string> errMsg;

        server();
    public:
        server( int _port, std::string _pass );
    
    // core funcs
        int multiplex();
        void newUser();
        void ClientRecv( int clientFileD );

    // utils funcs:
        std::vector<std::string> splitVec( std::string str, char c );
        std::string cleanLine( std::string line );
        int searchByNName( std::string NName);
    // for numeric replies:
        void errMsg_insert();
        void handleNumReps( int cl_fd, int errCode, std::string cmd );

        ~server();
};


#endif