# 💬 ft_irc - Internet Relay Chat Server

A complete IRC (Internet Relay Chat) server implementation written in C++98, supporting all core IRC functionalities including channels, private messaging, operator privileges, and channel modes.

## 📋 Project Overview

This project is a fully functional IRC server that allows multiple clients to connect, communicate in channels, and send private messages. It implements the core features of the IRC protocol while maintaining a clean, efficient codebase.

### 🌟 Key Features

- **Multi-client Support:** Handle multiple client connections simultaneously using non-blocking I/O
- **Authentication:** Password-protected server access
- **Nickname & Username Registration:** Unique identity for each user 
- **Channel Management:** Create, join, and manage IRC channels
- **Private Messaging:** Direct communication between users
- **Channel Operators:** Administrative privileges for channel management
- **Channel Modes:** Various modes to customize channel behavior
- **Signal Handling:** Proper shutdown with SIGINT and SIGQUIT
- **Bot Integration:** Utility bot with helpful commands

## 🛠️ Technical Implementation

- **Language:** C++98 standard compliant
- **I/O Handling:** Non-blocking socket operations via `poll()`
- **Socket Programming:** TCP/IP implementation (IPv4)
- **Error Handling:** Comprehensive error reporting and numeric replies
- **Project Structure:** Modular design with clear separation of concerns

## 🚀 Getting Started

### Prerequisites

- GCC/G++ compiler with C++98 support
- UNIX-based system (Linux or macOS)
- Make utility

### Building the Project

Clone the repository and compile using the provided Makefile:

```bash
git clone https://github.com/your-username/ft_irc.git
cd ft_irc
make
```

### Running the Server

```bash
./ircserv <port> <password>
```

- `port`: Port number on which the server will listen for incoming connections
- `password`: Password required for clients to connect to the server

Example:
```bash
./ircserv 6667 secretpassword
```

## 📝 Supported Commands

The server supports the following IRC commands:

| Command | Description |
|---------|-------------|
| PASS | Set connection password |
| NICK | Set or change nickname |
| USER | Set username and real name |
| JOIN | Join or create a channel |
| PRIVMSG | Send private message to user or channel |
| QUIT | Disconnect from server |
| KICK | Remove a user from a channel |
| INVITE | Invite a user to a channel |
| TOPIC | Change or view the channel topic |
| MODE | Change channel modes |
| PING | Check server connection |
| BOT | Access utility bot commands |

### Channel Modes

| Mode | Description |
|------|-------------|
| i | Set/remove Invite-only channel |
| t | Restrict TOPIC command to channel operators |
| k | Set/remove channel password |
| o | Give/take channel operator privileges |
| l | Set/remove user limit for channel |

## 🤖 Bot Commands

The server includes a utility bot that can be accessed with the `BOT` command:

```
BOT -h                  # Display help
BOT -c                  # List all available channels
BOT -cu <channel>       # List all users in a specific channel
BOT -cm <channel>       # List all modes of a specific channel
BOT -i                  # List all channels you are invited to
BOT -d                  # Display your connection duration
BOT -cc                 # Display the number of channels you've joined
```

## 🔌 Connecting with IRC Clients

This server is compatible with standard IRC clients. Here are some examples:

### Using netcat
```bash
nc localhost 6667
PASS secretpassword
NICK mynickname
USER myusername * * :My Real Name
```

### Using irssi
```bash
irssi -c localhost -p 6667 -w secretpassword -n mynickname
```

## 🧩 Project Structure

- **client/**: Client connection handling
- **server/**: Core server functionalities
- **channel/**: Channel management implementation
- **Makefile**: Project compilation
- **main.cpp**: Program entry point

## 👥 Contributors

This project was developed by:
- [yoelansa](https://github.com/UN-35)
- [aakhtab](https://github.com/essamad22)

## 📜 License

This project is part of the curriculum at [1337 School](https://1337.ma/) (part of the [42 Network](https://42.fr/)) and is released under the MIT License.

## 📚 Additional Resources

- [IRC RFC 1459](https://www.rfc-editor.org/rfc/rfc1459) - The original IRC protocol specification
- [Modern IRC Client Protocol](https://modern.ircdocs.horse/) - Updated IRC client protocol documentation
