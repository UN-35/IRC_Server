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

/* Clients status */

void	Channel::addToKicked(std::string &kicked_name)
{
	std::vector<std::string>::iterator it;
	for (it = _kicked_users.begin(); it != _kicked_users.end(); it++)
	{
		if (*it == kicked_name)
			return ;
	}
	_kicked_users.push_back(kicked_name);
}

void    Channel::addInvited(std::string &invited_name)
{
    std::vector<std::string>::iterator it;
    for (it = _invited.begin(); it != _invited.end(); it++)
    {
        if (*it == invited_name)
            return ;
    }
    _invited.push_back(invited_name);
}

bool    Channel::isInvited(std::string &invited_name)
{
    std::vector<std::string>::iterator it;
    for (it = _invited.begin(); it != _invited.end(); it++)
    {
        if (*it == invited_name)
            return (true);
    }
    return (false);
}

/* Operators */
void Channel::addFirstOperator(std::string operatorName)
{
    _operators.push_back(operatorName);
}

bool Channel::isOperator(std::string &operatorName)
{
    std::vector<std::string>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == operatorName)
            return (true);
    }
    return (false);
}

void Channel::removeOperator(std::string operatoName){
    std::vector<std::string>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == operatoName){
            _operators.erase(it);
            return ;
        }
    }
}

void Channel::addOperator(std::string operatorName)
{
    std::vector<std::string>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++){
        if (*it == operatorName)
            return ;
    }
    _operators.push_back(operatorName);
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
    _mode.push_back(tmp);
}

bool Channel::validMode( char const mode )
{
    std::string allmodes = "itkol";
    if (allmodes.find(mode) != std::string::npos)
        return true;
    return false;
}

void Channel::removeMode( std::string const mode )
{
    std::vector<std::string>::iterator it;
    for (it = _mode.begin(); it != _mode.end(); it++) {
        if (*it == mode) {
            _mode.erase(it);
            return ;
        }
    }
}

bool Channel::isModeSet( std::string const mode )
{
    std::vector<std::string>::iterator it;
    for (it = _mode.begin(); it != _mode.end(); it++) {
        if (*it == mode)
            return true;
    }
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