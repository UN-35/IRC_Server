#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../client/client.hpp"
#include "../header.hpp"

class Channel
{
	private:
		std::map<std::string, Client>	_clientList;
		std::vector<std::string>		_kicked_users;
		std::vector<std::string>		_operators;
		std::string 					_name;
		std::string						_operatorPassword;
		std::string						_topic;
		std::string						_mode;
		std::string						_channel_password;
		int								_limit;
	public:
		Channel(std::string const &name);
		~Channel();

        /* Getters and Setters */
		std::string&					getName();
		std::string&					getTopic();
		std::string&					getMode();
		std::string&					getChannelPassword();
		int&							getCapacityLimit();
		std::vector<std::string>&		getOperators();
		std::vector<std::string>&		getKickedUsers();
		std::map <std::string, Client>&	getClientList();
		void							setTopic(std::string& newTopic);
		void							setChannelPassword(std::string password);
		void							setCapacityLimit(int limit);
		bool							clientExist(std::string &clientName);
        // void                            sendToChannelUsers(std::string &message, std::string &sender);
		/* Clients */
		void							addClientToChannel(Client &client);
		void							removeClientFromChannel(std::string &clientName);
		/* Client status */
		void							addToKicked(std::string &banned_name);

		/* Operators */
		void							addFirstOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		void							addOperator(std::string operatorName);
		bool							isOperator(std::string &operatorName);
		/* Modes */
		void							addMode(std::string const mode);
		void							removeMode(std::string const mode);
		void							removeChannelPassword();
};