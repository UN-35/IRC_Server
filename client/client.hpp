/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:42:13 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/18 17:46:18 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include "../server/server.hpp"
#include <iostream>

class Client {
    private:
        std::string nickName;
        std::string userName;
        std::string buffer;
        
        int fd_client;
        std::string clPasswd;

        bool operat;
        bool auth;
        bool regist;
        
    public:
        Client();
        Client( int _fd, std::string _passwd );
        ~Client();
// set
        void setNickName( std::string nick );
        void setUserName( std::string uName );
        void setBuffer( std::string buff );
        void setOperator( bool is_operator );
        void setAuth( bool Authentic );
        void setRegistration( bool is_regist );
// reset/erase buffer
        void erase_buff();
// get
        std::string getNickName();
        std::string getUserName();
        std::string getBuffer();
        bool getOperator();
        bool getAuth();
        bool getRegistration();
        
};


#endif