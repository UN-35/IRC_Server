#include "Channel.hpp"

Channel::Channel(std::string const &name) : _name(name), _limit(-1) {
    _banned_users.clear();
    _clientList.clear();
    _kicked_users.clear();
}

Channel::~Channel() {}
/* Getters and Setters */
std::string& Channel::getName() {return _name;}
std::string& Channel::getTopic() {return _topic;}
std::string& Channel::getMode() {return _mode;}
std::string& Channel::getChannelPassword() {return _channel_password;}
int& Channel::getCapacityLimit() {return _limit;}
std::vector<std::string>& Channel::getOperators() {return _operators;}
std::map <std::string, Client>& Channel::getClientList() {return _clientList;}
std::vector<std::string>& Channel::getKickedUsers() {return _kicked_users;}
std::vector<std::string>& Channel::getBannedUsers() {return _banned_users;}
std::vector<std::string>& Channel::getVoicedUsers() {return _voiced_users;}

void Channel::setTopic(std::string& newTopic) {_topic = newTopic;}
void Channel::setChannelPassword(std::string password) {_channel_password = password;}
void Channel::setCapacityLimit(int limit) {_limit = limit;}
bool Channel::clientExist(std::string &clientName) {
    if (_clientList.size() == 0)
        return false;
    std::map<std::string, Client>::iterator it = _clientList.find(clientName);
    if (it == _clientList.end())
        return false;
    return true;
}

/* Clients */
void	Channel::addClientToChannel(Client &client)
{
	_clientList.insert(std::pair<std::string, Client>(client.getNickName(), client));
}

void	Channel::removeClientFromChannel(std::string &clientName)
{
    _clientList.erase(clientName);
}

// void    Channel::sendToChannelUsers(std::string &message, std::string &sender)
// {
//     std::map<std::string, Client>::iterator it;
//     for (it = _clientList.begin(); it != _clientList.end(); it++)
//     {
//         if (it->first != sender)
//             send(it->second., message.c_str(), message.size(), 0);
//     }
// }
/* Clients status */

void	Channel::addToKicked(std::string &kicked_name)
{
	std::vector<std::string>::iterator it;
	for (it = _kicked_users.begin(); it != _kicked_users.end(); it++)
	{
		if (*it == kicked_name)
		{
			std::cout << kicked_name << " is already kicked from the channel " << getName() << std::endl;
			return ;
		}
	}
	_kicked_users.push_back(kicked_name);
	std::cout << RED << kicked_name << " is now kicked from the channel " << getName() << RESET << std::endl;
}

void	Channel::addToBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator it;
	for (it = _banned_users.begin(); it != _banned_users.end(); it++)
	{
		if (*it == banned_name)
		{
			std::cout << banned_name << " is already banned from the channel " << getName() << std::endl;
			return ;
		}
	}
	_banned_users.push_back(banned_name);
	std::cout << RED << banned_name << " is now banned from the channel " << getName() << RESET << std::endl;
}

void	Channel::removeFromBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator user;
	for (user = _banned_users.begin(); user != _banned_users.end(); user++)
	{
		if (*user == banned_name)
		{
			_banned_users.erase(user);
			std::cout << banned_name << " is removed From Banned List of " << getName() << std::endl;
			return ;
		}
	}
	std::cout << banned_name << " is not Banned from" << getName() << std::endl;
}

bool	Channel::isBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator user;
	if (_banned_users.empty() == true)
		return (false);
	for (user = _banned_users.begin(); user != _banned_users.end(); user++)
	{
		if (*user == banned_name)
			return (true);
	}
	return (false);	
}

void Channel::addToVoiced(std::string &voiced_name)
{
    std::vector<std::string>::iterator it;
    for (it = _voiced_users.begin(); it != _voiced_users.end(); it++)
    {
        if (*it == voiced_name)
        {
            std::cout << voiced_name << " is already voiced in the channel " << getName() << std::endl;
            return ;
        }
    }
    _voiced_users.push_back(voiced_name);
    std::cout << GREEN << voiced_name << " is now voiced in the channel " << getName() << RESET << std::endl;
}

bool Channel::isVoiced(std::string &voiced_name)
{
    std::vector<std::string>::iterator it;
    for (it = _voiced_users.begin(); it != _voiced_users.end(); it++)
    {
        if (*it == voiced_name)
        {
            std::cout << voiced_name << " is voiced in the channel " << getName() << std::endl;
            return (true);
        }
    }
    std::cout << voiced_name << " is not voiced in the channel " << getName() << std::endl;
    return (false);
}

void Channel::removeFromVoiced(std::string &voiced_name)
{
    std::vector<std::string>::iterator it;
    for (it = _voiced_users.begin(); it != _voiced_users.end(); it++)
    {
        if (*it == voiced_name)
        {
            _voiced_users.erase(it);
            std::cout << voiced_name << " is removed from the voiced list of " << getName() << std::endl;
            return ;
        }
    }
    std::cout << voiced_name << " is not voiced in the channel " << getName() << std::endl;
}

/* Operators */
void Channel::addFirstOperator(std::string operatorName)
{
    _operators.push_back(operatorName);
    std::cout << GREEN << operatorName << " is now an operator in the channel " << getName() << RESET << std::endl;
}