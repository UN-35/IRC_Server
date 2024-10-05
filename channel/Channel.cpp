#include "Channel.hpp"

Channel::Channel(std::string const &name) : _name(name), _channel_password(""),_limit(-1){
    _clientList.clear();
    _kicked_users.clear();
    _invited.clear();
    _operators.clear();
    _mode.clear();
}

Channel::~Channel() {}
/* Getters and Setters */
std::string& Channel::getName() {return _name;}
std::string& Channel::getTopic() {return _topic;}
std::string& Channel::getChannelPassword() {return _channel_password;}
size_t& Channel::getCapacityLimit() {return _limit;}
std::vector<std::string>& Channel::getMode() {return _mode;}
std::vector<std::string>& Channel::getOperators() {return _operators;}
std::map <std::string, Client>& Channel::getClientList() {return _clientList;}
std::vector<std::string>& Channel::getKickedUsers() {return _kicked_users;}
std::vector<std::string>& Channel::getInvitedUsers() {return _invited;}

void Channel::setTopic(std::string& newTopic) {_topic = newTopic;}
void Channel::setChannelPassword(std::string password) {_channel_password = password;}
void Channel::setCapacityLimit(int limit) {
    // std::cout << "limit set to: " << limit << std::endl;
    _limit = limit;
}
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
			// std::cout << kicked_name << " is already kicked from the channel " << getName() << std::endl;
			return ;
		}
	}
	_kicked_users.push_back(kicked_name);
	// std::cout << RED << kicked_name << " is now kicked from the channel " << getName() << RESET << std::endl;
}

void    Channel::addInvited(std::string &invited_name)
{
    std::vector<std::string>::iterator it;
    for (it = _invited.begin(); it != _invited.end(); it++)
    {
        if (*it == invited_name)
        {
            // std::cout << invited_name << " is already invited to the channel " << getName() << std::endl;
            return ;
        }
    }
    _invited.push_back(invited_name);
    // std::cout << GREEN << invited_name << " is now invited to the channel " << getName() << RESET << std::endl;
}

bool    Channel::isInvited(std::string &invited_name)
{
    std::vector<std::string>::iterator it;
    for (it = _invited.begin(); it != _invited.end(); it++)
    {
        if (*it == invited_name)
        {
            // std::cout << invited_name << " is invited to the channel " << getName() << std::endl;
            return (true);
        }
    }
    // std::cout << invited_name << " is not invited to the channel " << getName() << std::endl;
    return (false);
}

/* Operators */
void Channel::addFirstOperator(std::string operatorName)
{
    _operators.push_back(operatorName);
    // std::cout << GREEN << operatorName << " is now an operator in the channel " << getName() << RESET << std::endl;
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

void Channel::removeOperator(std::string operatoName){
    std::vector<std::string>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == operatoName)
        {
            _operators.erase(it);
            // std::cout << RED << operatoName << " is no longer an operator in the channel " << getName() << RESET << std::endl;
            return ;
        }
    }
    // std::cout << operatoName << " is not an operator in the channel " << getName() << std::endl;
}

void Channel::addOperator(std::string operatorName)
{
    std::vector<std::string>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == operatorName)
        {
            // std::cout << operatorName << " is already an operator in the channel " << getName() << std::endl;
            return ;
        }
    }
    _operators.push_back(operatorName);
    // std::cout << GREEN << operatorName << " is now an operator in the channel " << getName() << RESET << std::endl;
}

/* Modes */

void Channel::addMode( char mode)
{
    std::string app;
    if (mode == 'i')
        app = "i";
    else if (mode == 'o')
        app = "o";
    else if (mode == 'l')
        app = "l";
    else if (mode == 'k')
        app = "k";
    else if (mode == 't')
        app = "t";
    std::string tmp = "" + app;
    // std::cout << "mode added: " << tmp << std::endl;
    _mode.push_back(tmp);
}

bool Channel::validMode( char const mode )
{
    std::string allmodes = "itkol";
    // std::cout << "heelo" << std::endl;
    if (allmodes.find(mode) != std::string::npos)
        return true;
    return false;
}

void Channel::removeMode( std::string const mode )
{
    std::vector<std::string>::iterator it;
    for (it = _mode.begin(); it != _mode.end(); it++)
    {
        if (*it == mode)
        {
            _mode.erase(it);
            return ;
        }
    }
}

bool Channel::isModeSet( std::string const mode )
{
    std::vector<std::string>::iterator it;
    for (it = _mode.begin(); it != _mode.end(); it++)
    {
        if (*it == mode){
            // std::cout << "check mode yes" << std::endl;
            return true;
        }
    }
    // std::cout << "check mode false" << std::endl;
    return false;
}

void Channel::removeChannel()
{
    _clientList.clear();
    _kicked_users.clear();
    _invited.clear();
    _operators.clear();
    _mode.clear();
    _name.clear();
    _operatorPassword.clear();
    _topic.clear();
    _channel_password.clear();
    _limit = -1;
}