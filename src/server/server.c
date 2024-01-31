#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include "server.h"

#include "User.h"
#include "Group.h"
#include "UsersArray.h"
#include "GroupsArray.h"

#include "protocol.h"
#include "logs.h"
#include "bool.h"
#include "IO.h"

#define PORT 108
#define MAX_EVENTS 100

Users *users;
Groups *groups;

int main()
{

    print_bold_log_with_params_by_color(DEFAULT, "Server Started!, PORT: %d\n", PORT, RED);

    // Ignore SIGPIPE signal for not terminating when writing to a closed socket
    // if user exits the client program
    signal(SIGPIPE, SIG_IGN);

    int server_fd, new_socket;
    struct sockaddr_in address;
    RequestMessage message;

    users = initUsers(users);
    groups = initGroups(groups);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    set_non_blocking(server_fd);

    // Set socket options
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Allow socket to be reused
    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval)) < 0)
    {
        perror("Error setting SO_REUSEADDR option");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1)
    {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    while (TRUE)
    {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == server_fd)
            {
                // Accept new connections
                new_socket = acceptNewConnection(server_fd, (struct sockaddr *)&address);

                // TODO: Move code to IO.h and IO.c
                set_non_blocking(new_socket);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &ev) == -1)
                {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                ssize_t count;
                memset(&message, 0, sizeof(message));
                count = read(events[n].data.fd, &message, sizeof(message));
                int current_fd = events[n].data.fd;

                handleClientInput(current_fd, new_socket, count, message);
            }
        }
    }
    freeGroups(groups);
    freeUsers(users);
    close(server_fd);
    return 0;
}

void handleLoginSuccess(int current_fd, char *username, ResponseMessage response)
{
    if (username != NULL && checkUsernameExists(users, username) == USER_NOT_FOUND)
    {
        print_log_with_params_by_color(LOG_INFO, "Login request from username: %s\n", username, GREEN);
        addUser(users, current_fd, username);
        response.status = SUCCESS;
        response.type = LOGIN;
        response.description = "Login successful\n";
        sendResponse(current_fd, &response);
    }
    else
    {
        response.status = ERROR;
        response.type = LOGIN;
        response.description = "Username already exists\n";
        sendResponse(current_fd, &response);
    }
}

void handleLogout(int current_fd)
{
    User *user = getUser(users, current_fd); // if getUser is NULL, the user wasn't logged in
    if (user != NULL)
    {
        int userGroupID = user->group_id;

        print_log_with_params_by_color(LOG_INFO, "Removed client with file descriptor: %d\n", current_fd, RED);
        if (userGroupID != GROUP_NOT_FOUND)
        {
            if (isAdmin(&groups->groups_arr[userGroupID], current_fd) == TRUE)
            {
                removeEveryoneFromGroup(&groups->groups_arr[userGroupID], current_fd);
            }
            else
            {
                removeUserFromGroup(&groups->groups_arr[userGroupID], current_fd);
            }
            print_log_with_params_by_color(LOG_INFO, "Removed from group: %s\n", getGroupName(groups, groups->groups_arr[userGroupID].id), RED);
            removeGroup(groups, userGroupID);
        }
        removeUser(users, current_fd);
    }
    else
    {
        print_log_with_params_by_color(LOG_INFO, "Client with fd: %d wasn't logged in\n", current_fd, RED);
    }
    // close client socket
    close(current_fd);
}

void handleLeaveGroup(int current_fd, char *username, ResponseMessage *response)
{
    // Handle leave group
    printf("Leave group request from %s\n", username);
    User *user = getUser(users, current_fd);
    int groupID = user->group_id;
    if (user != NULL)
    {
        if (isAdmin(&groups->groups_arr[groupID], current_fd) == TRUE)
        {
            printf("Admin left the group\n");
            // if the admin leaves the group, the group needs to be deleted
            removeEveryoneFromGroup(&groups->groups_arr[groupID], current_fd);
        }
        else
        {
            removeUserFromGroup(&groups->groups_arr[groupID], current_fd);
            response->status = SUCCESS;
            response->type = LEAVE_GROUP;
            response->description = "Leave group successful\n";
        }
    }
    else
    {
        response->status = ERROR;
        response->type = LEAVE_GROUP;
        response->description = "You are not in a group\n";
    }

    sendResponse(current_fd, response);
}

void handleShowAvailableGroups(int current_fd, char *username, ResponseMessage *response)
{
    print_log_with_params_by_color(LOG_INFO, "Show available groups request from %s\n", username, GREEN);
    // Handle show available groups
    response->status = SUCCESS;
    response->type = SHOW_AVAILABLE_GROUPS;

    if (showAllGroupsToClient(groups) == NULL || strlen(showAllGroupsToClient(groups)) == 0)
    {
        response->description = "\t\tNo groups available\n";
    }
    else
    {
        response->description = showAllGroupsToClient(groups);
    }
    sendResponse(current_fd, response);
}

void handleSetBlacklist(int current_fd, char *username, RequestMessage message, ResponseMessage *response)
{
    // Handle set blacklist
    printf("Set blacklist request from %s:\n", username);

    printf("Blacklist words: %s\n", message.request.blacklist_words.words);
    Blacklist *list = parseBufferToBlacklist(message.request.blacklist_words.words);
    showBlacklist(list);
    response->status = SUCCESS;
    response->type = SET_BLACKLIST;
    response->description = "Set blacklist successful\n";
    sendResponse(current_fd, response);
}

void handleAddWordToBlacklist(int current_fd, char *username, RequestMessage message, ResponseMessage *response)
{
    // Handle add word blacklist
    // Not functional yet
    printf("Add word blacklist request from %s\n", username);
    response->status = SUCCESS;
    response->type = ADD_WORD_BLACKLIST;
    response->description = "Add word blacklist successful\n";
    sendResponse(current_fd, response);
}

void handleShowBlacklist(int current_fd, char *username, ResponseMessage *response)
{
    printf("Show blacklist request from %s\n", username);
    // Handle show blacklist
    response->status = SUCCESS;
    response->type = SHOW_BLACKLIST;
    response->description = showBlacklist(groups->groups_arr[getUser(users, current_fd)->group_id].blacklist);
    sendResponse(current_fd, response);
}

void handleClientRequest(int current_fd, int new_socket, RequestMessage message, char *username)
{
    ResponseMessage response;
    switch (message.type)
    {
    case LOGIN:
        // Handle login
        char *input_username = (char *)malloc(MAX_USERNAME_LENGTH * sizeof(char));
        strncpy(input_username, message.request.login.username, MAX_USERNAME_LENGTH);
        handleLoginSuccess(current_fd, input_username, response);
        break;
    case LOGOUT:
        // Handle logout
        if (username != NULL)
        {
            print_log_with_params_by_color(LOG_INFO, "Logout request from username: %s\n", username, GREEN);
        }
        handleLogout(current_fd);
        break;
    case SEND_MESSAGE:
        // Handle send message
        handleSendMessage(current_fd, username, message, response);
        break;
    case CREATE_GROUP:
        // Handle create group
        handleGroupCreation(current_fd, username, message, response);
        break;
    case JOIN_GROUP:
        // Handle join group
        handleJoinGroup(current_fd, username, message, response);
        break;
    case LEAVE_GROUP:
        handleLeaveGroup(current_fd, username, &response);
        break;
    case SHOW_AVAILABLE_GROUPS:
        handleShowAvailableGroups(current_fd, username, &response);
        // WORKS
        break;
    case SET_BLACKLIST:
        handleSetBlacklist(current_fd, username, message, &response);
        break;
    case ADD_WORD_BLACKLIST:
        handleAddWordToBlacklist(current_fd, username, message, &response);
        break;
    case SHOW_BLACKLIST:
        handleShowBlacklist(current_fd, username, &response);
        break;
    default:
        response.status = ERROR;
        print_bold_log_by_color(LOG_ERROR, "Unknown message type received\n", RED);
    }
}

void handleDisconnect(int current_fd)
{
    if (getUsername(users, current_fd) != NULL)
    {
        print_log_with_params_by_color(LOG_INFO, "Client disconnected: %s\n", getUsername(users, current_fd), RED);
        handleLogout(current_fd);
        return;
    }
    print_bold_log_by_color(LOG_INFO, "Client has disconnected\n", RED);
    handleLogout(current_fd);
}

int acceptNewConnection(int server_fd, struct sockaddr *address)
{
    int addrlen = sizeof(address);
    int new_socket;
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    print_bold_log_by_color(LOG_INFO, "New connection\n", GREEN);

    if (new_socket == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return new_socket;
}

void handleSendMessage(int current_fd, char *username, RequestMessage message, ResponseMessage response)
{
    printf("Send message request from %s\nContents: %s\n", username, message.request.sendMessage.message.content);

    chatMessage chat_message;

    strncpy(chat_message.content, message.request.sendMessage.message.content, MAX_CONTENT_LENGTH);
    chat_message.sender = *(getUser(users, current_fd));
    chat_message.timestamp = time(NULL);

    int group_id = chat_message.sender.group_id;
    sendMessageToGroup(current_fd, chat_message.sender.group_id, chat_message);

    response.status = SUCCESS;
    response.type = SEND_MESSAGE;
    response.description = "Message sent successfully\n";
    sendResponse(current_fd, &response);
}

void handleGroupCreation(int current_fd, char *username, RequestMessage message, ResponseMessage response)
{
    print_log_with_params_by_color(LOG_INFO, "Create group request from %s", username, GREEN);
    if (strcmp(message.request.createGroup.groupName, "") == 0)
    {
        print_bold_log_by_color(LOG_ERROR, "Client entered a invalid group name\n", RED);
        response.status = ERROR;
        response.type = CREATE_GROUP;
        response.description = "Group name cannot be empty\n";
    }
    else if (findGroupByName(groups, message.request.createGroup.groupName) != GROUP_NOT_FOUND)
    {
        // Group with the same name already exists
        print_log_with_params_by_color(LOG_INFO, "Group name: %s\n", message.request.createGroup.groupName, GREEN);
        print_bold_log_by_color(LOG_ERROR, "Group with the same name already exists\n", RED);
        response.status = ERROR;
        response.type = CREATE_GROUP;
        response.description = "Group with the same name already exists\n";
    }
    else
    {
        int groupCount = groups->count;
        print_log_with_params_by_color(LOG_INFO, "GroupID: %d\n", groups->count, GREEN);
        addNewGroup(groups, message.request.createGroup.groupName, current_fd);
        updateUserGroupId(users, current_fd, groupCount);

        response.status = SUCCESS;
        response.type = CREATE_GROUP;
        response.description = message.request.createGroup.groupName;
    }
    sendResponse(current_fd, &response);
}

void handleJoinGroup(int current_fd, char *username, RequestMessage message, ResponseMessage response)
{
    // Handle join group
    int groupID = message.request.joinGroup.groupID;
    print_log_with_params_by_color(LOG_INFO, "Join group request from: %s ", username, GREEN);
    // Check if group already opened
    if (findGroupByID(groups, groupID) == GROUP_NOT_FOUND)
    {
        print_bold_log_by_color(LOG_ERROR, "Group doesn't exist\n", RED);
        response.status = ERROR;
        response.type = JOIN_GROUP;
        response.description = "Group doesn't exist\n";
    }
    else if (groups->groups_arr[groupID].member_count == MAX_MEMBERS)
    {
        print_bold_log_by_color(LOG_ERROR, "Group is full\n", RED);
        response.status = ERROR;
        response.type = JOIN_GROUP;
        response.description = "Group is full\n";
    }
    else
    {
        print_bold_log_with_params_by_color(LOG_INFO, "Group name: %s\n", getGroupName(groups, groupID), GREEN);
        print_log_with_params_by_color(LOG_INFO, "Group-id: %d\n", groupID, GREEN);
        response.status = SUCCESS;
        response.type = JOIN_GROUP;
        response.description = getGroupName(groups, groupID);
        updateUserGroupId(users, current_fd, groupID);
        addMemberToGroup(&groups->groups_arr[groupID], current_fd);
    }
    sendResponse(current_fd, &response);
}

void handleClientInput(int current_fd, int new_socket, int count, RequestMessage message)
{
    char *username = (char *)malloc(MAX_USERNAME_LENGTH * sizeof(char));
    if (username == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (users->count > 0)
    {
        if (getUsername(users, current_fd) != NULL)
            strncpy(username, getUsername(users, current_fd), MAX_USERNAME_LENGTH);
    }
    else
    {
        strncpy(username, "", MAX_USERNAME_LENGTH);
    }

    if (count == 0)
    {
        handleDisconnect(current_fd);
    }
    else if (count == -1)
    {
        perror("read");
        print_bold_log_by_color(LOG_INFO, "Exit program cause of read()\n", RED);
        exit(EXIT_FAILURE);
    }
    else
    {
        handleClientRequest(current_fd, new_socket, message, username);
    }
}

// Needs to be moved to protocol.c
int sendMessageToGroup(int sender, int group_id, chatMessage message)
{
    int member_count = groups->groups_arr[group_id].member_count;
    int *member_fds = groups->groups_arr[group_id].member_fds;

    for (int i = 0; i < member_count; ++i)
    {
        if (member_fds[i] != sender)
        {
            sendChatMessage(member_fds[i], &message);
        }
    }
    return 0;
}
