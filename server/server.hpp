/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:17:38 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/12 20:57:07 by yoelansa         ###   ########.fr       */
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
#include <poll.h>

class server {
    private:
        int port;
        std::string passwd;
        sockaddr_in server_addr;
        int sock_fd;
        std::string hostname;

        std::vector<pollfd> poll_vec;

        server();
    public:
        server( int _port, std::string _pass );
        ~server();
};


#endif