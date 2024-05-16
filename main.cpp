/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:58:53 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/15 13:25:53 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

int g_ToBreak;
void sigHandler( int signum ) {
    ( void )signum;
    std::cout << "Server Shutted DOWN!!" << std::endl;
    g_ToBreak = 0;
}

int main(int ac, const char** av) {

    if ( ac != 3 ) {

        return 1;
    }
    int port = atoi( av[1] );
    std::string passwd = av[2] ;

    g_ToBreak = 1;
    signal(SIGINT, sigHandler);

    server irc_serv( port, passwd );

    int servStatus = 0;
    while ( g_ToBreak && !servStatus ) {

	    signal(SIGINT, sigHandler); /////// reThink about it !!;

        servStatus = irc_serv.multiplex();
    }
    
    return 0;
}