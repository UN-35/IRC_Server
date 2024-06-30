/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhtab <aakhtab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:42:16 by yoelansa          #+#    #+#             */
/*   Updated: 2024/06/30 13:34:14 by aakhtab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"


Client::Client() {}

Client::Client( int _fd, std::string _passwd ): fd_client( _fd ), clPasswd( _passwd ) {

    nickName = "";
    userName = "";
    auth = false;
    operat = false;
    regist = false;
}
Client::~Client() {}


void Client::setNickName( std::string nick ) { this->nickName = nick; }
void Client::setUserName( std::string uName ) { this->userName = uName; }
void Client::setBuffer( std::string buff ) { this->buffer += buff; }
void Client::setOperator( bool is_operator ) { this->operat = is_operator; }
void Client::setAuth( bool Authentic ) { this->auth = Authentic; }
void Client::setRegistration( bool is_regist ) { this->regist = is_regist; }

void Client::erase_buff() { this->buffer.erase(); }

std::string Client::getNickName() { return this->nickName; }
std::string Client::getUserName() { return this->userName; }
std::string Client::getBuffer() { return this->buffer; }
bool Client::getOperator() { return this->operat; }
bool Client::getAuth() { return this->auth; }
bool Client::getRegistration() { return this->regist; }
