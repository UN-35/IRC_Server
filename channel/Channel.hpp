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
		// std::vector<std::string>		_kicked_users;
		std::vector<std::string>		_operators;
        std::vector<std::string>		_invited;
		std::vector<std::string>		_mode;
		std::string 					_name;
		// std::string						_operatorPassword;
		std::string						_topic;
		std::string						_channel_password;
		size_t							_limit;
	public:
		Channel(std::string const &name);
		~Channel();
        std::string                     _strLimit;

        /* Getters and Setters */
		std::string&					getName();
		std::string&					getTopic();
		std::vector<std::string>&		getMode();
		std::string&					getChannelPassword();
		size_t&							getCapacityLimit();
		std::vector<std::string>&		getOperators();
		// std::vector<std::string>&		getKickedUsers();
		std::map <std::string, Client>&	getClientList();
        std::vector<std::string>&		getInvitedUsers();
		void							setTopic(std::string& newTopic);
		void							setChannelPassword(std::string password);
		void							setCapacityLimit(int limit);
		bool							clientExist(std::string &clientName);
        // void                            sendToChannelUsers(std::string &message, std::string &sender);
		/* Clients */
		void							addClientToChannel(Client &client);
		void							removeClientFromChannel(std::string &clientName);
        void                            addInvited(std::string &invited_name);
        bool                            isInvited(std::string &invited_name);
		/* Client status */
		// void							addToKicked(std::string &banned_name);

		/* Operators */
		void							addOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		// void							addOperator(std::string operatorName);
		bool							isOperator(std::string &operatorName);
		/* Modes */
		void							addMode(char mode);
		void							removeMode(std::string const mode);
        bool                            validMode(char const mode);
        bool                            isModeSet(std::string const mode);
        void                            removeChannel();
		// void							removeChannelPassword();
};