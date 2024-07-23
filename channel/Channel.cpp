#include "Channel.hpp"

Channel::Channel(std::string const &name) : _name(name), _limit(-1) {
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

/* Operators */
void Channel::addFirstOperator(std::string operatorName)
{
    _operators.push_back(operatorName);
    std::cout << GREEN << operatorName << " is now an operator in the channel " << getName() << RESET << std::endl;
}

bool Channel::isOperator(std::string &operatorName)
{
    std::vector<std::string>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == operatorName)
        {
            // std::cout << operatorName << " is an operator in the channel " << getName() << std::endl
            return (true);
        }
    }
    // std::cout << operatorName << " is not an operator in the channel " << getName() << std::endl;
    return (false);
}