/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoelansa <yoelansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:58:53 by yoelansa          #+#    #+#             */
/*   Updated: 2024/05/12 16:49:14 by yoelansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"


int main(int ac, const char** av) {

    if ( ac != 3 ) {

        return 1;
    }
    int port = atoi( av[1] );
    std::string passwd = av[2] ;

    server irc_serv( port, passwd );

    std::cout << port << "," << passwd << std::endl;
    
    return 0;
}