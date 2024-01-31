#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#define PORT 108
#define MAX_EVENTS 2

#include "client.h"
#include "User.h"
#include "protocol.h"
#include "IO.h"
#include "logs.h"
#include "Group.h"
#include "bool.h"

boolean isAuth = FALSE;
boolean inGroup = FALSE;

User *userClient;
Group *groupClient;

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    RequestMessage message;
    ResponseMessage response;

    groupClient = initGroup(groupClient, "", GROUP_DEFAULT, -1);
    userClient = initUser(userClient, "", ADMIN_DEFAULT, GROUP_DEFAULT);

    starterMenu();

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        print_bold_by_color("\n Socket creation error \n", RED);
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        print_bold_by_color("\nInvalid address / Address not supported\n", RED);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        print_bold_by_color("\nConnection Failed\n", RED);
        return -1;
    }

    // Set the socket to non-blocking
    set_non_blocking(sock);

    // Set up epoll
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN | EPOLLET; // Read operation and edge-triggered

    // Add the socket to epoll
    ev.data.fd = sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev) == -1)
    {
        perror("epoll_ctl: sock");
        exit(EXIT_FAILURE);
    }

    // Also monitor stdin (for user input)
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1)
    {
        perror("epoll_ctl: stdin");
        exit(EXIT_FAILURE);
    }

    while (TRUE)
    {
        // Wait for events
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == sock)
            {
                // Socket is ready to read (server sent something)
                handleServerResponse(sock, &response);
            }
            // Stdin is ready for reading (user input)
            else if (events[n].data.fd == STDIN_FILENO)
            {
                handleClientInput(sock, &message);
            }
        }
    }

    freeUser(userClient);
    freeGroup(groupClient);
    close(sock);
    return 0;
}

// Menu options printing

void starterMenu()
{
    print_bold_by_color("Welcome to the chat app!\n", CYAN);
    printf("1. Login\n");
    printf("2. Exit\n");
}

void authenticatedMenu()
{
    system("clear");
    print_bold_by_color("You can always select from these options here:\n", CYAN);
    printf("\t1. Show available groups \n");
    printf("\t2. Create a new group\n");
    printf("\t3. Join a group (by ID)\n");
    printf("\t4. Exit\n");
}

// Handler for client input

void handleClientInput(int sock, RequestMessage *message)
{
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Read the user input
    memset(buffer, 0, sizeof(buffer));
    memset(message, 0, sizeof(RequestMessage));
    int valread = read(STDIN_FILENO, buffer, sizeof(buffer));

    // Checks if valread variable is valid with data.
    if (valread > 0)
    {
        // Remove the newline character from the end of the string
        buffer[valread] = '\0'; // Null-terminate the received string
        if (isAuth == FALSE)
        {
            handleNotLoggedClientsMenu(sock, *message, buffer);
        }
        else if (isAuth == TRUE && inGroup == FALSE)
        {
            handleAuthenticatedMenu(sock, *message, buffer);
        }
        else if (inGroup == TRUE)
        {
            handleUserMessageInGroup(sock, *message, buffer);
        }
    }
    else if (valread == 0)
    {
        // Terminal has been closed
        // Ctrl-d was pressed
        printf("Exit\n");
        exit(EXIT_SUCCESS);
    }
    else if (valread == -1)
    {
        // Read error
        perror("read");
        print_bold_by_color("Exit\n", RED);
        exit(EXIT_FAILURE);
    }
}

// Handlers for menu options (User input)

void handleNotLoggedClientsMenu(int sock, RequestMessage message, char *buffer)
{
    buffer[strcspn(buffer, "\n")] = '\0';

    int choice = atoi(buffer);
    switch (choice)
    {
    case 1:
        printf("--- Login ---\n");
        message = (RequestMessage){
            .type = LOGIN,
        };
        printf("Enter username: ");
        fgets(message.request.login.username, MAX_USERNAME_LENGTH, stdin);

        message.request.login.username[strcspn(message.request.login.username, "\n")] = '\0';

        userClient->fd = sock;
        userClient->group_id = -1;

        strncpy(userClient->username, message.request.login.username, MAX_USERNAME_LENGTH);
        sendMessage(sock, &message);
        break;
    case 2:
        printf("Exit\n");
        exit(EXIT_SUCCESS);
        break;
    default:
        print_by_color("Invalid choice\n", PURPLE);
        exit(EXIT_FAILURE);
    }
}

void handleAuthenticatedMenu(int sock, RequestMessage message, char *buffer)
{
    buffer[strcspn(buffer, "\n")] = '\0';
    int choice;
    choice = atoi(buffer);

    switch (choice)
    {
    case 1:
        printf("--- Showing available groups ---\n");
        message = (RequestMessage){
            .type = SHOW_AVAILABLE_GROUPS,
        };
        sendMessage(sock, &message);
        break;
    case 2:
        printf("--- Creating a new group ---\n");
        message = (RequestMessage){
            .type = CREATE_GROUP,
        };
        printf("Enter group name: ");
        fgets(message.request.createGroup.groupName, MAX_GROUPNAME_LENGTH, stdin);
        message.request.createGroup.groupName[strcspn(message.request.createGroup.groupName, "\n")] = '\0';
        sendMessage(sock, &message);
        break;
    case 3:
        printf("--- Joining a group ---\n");
        printf("Enter group ID: ");
        message = (RequestMessage){
            .type = JOIN_GROUP,
        };
        scanf("%d", &message.request.joinGroup.groupID);
        sendMessage(sock, &message);
        break;
    case 4:
        print_bold_by_color("Exit\n", RED);
        message = (RequestMessage){
            .type = LOGOUT,
        };
        sendMessage(sock, &message);
        exit(EXIT_SUCCESS);
        break;
    default:
        authenticatedMenu();
        print_by_color("Invalid choice\n", RED);
    }
}

void handleUserMessageInGroup(int sock, RequestMessage message, char *buffer)
{
    buffer[strcspn(buffer, "\n")] = '\0';
    message = (RequestMessage){
        .type = SEND_MESSAGE,
    };
    strncpy(message.request.sendMessage.message.content, buffer, MAX_CONTENT_LENGTH);
    message.request.sendMessage.message.content[strcspn(message.request.sendMessage.message.content, "\n")] = '\0';

    if (strcmp(message.request.sendMessage.message.content, "") == 0)
    {
        print_bold_by_color("You can't send an empty message\n", RED);
        return;
    }
    if (strcmp(message.request.sendMessage.message.content, CLOSE_PROGRAM_COMMAND) == 0)
    {
        print_bold_by_color("Exit\n", RED);
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(message.request.sendMessage.message.content, LEAVE_GROUP_COMMAND) == 0)
    {
        print_bold_by_color("Leave\n", RED);
        message = (RequestMessage){
            .type = LEAVE_GROUP,
        };
        inGroup = FALSE;
    }
    else if (strcmp(message.request.sendMessage.message.content, HELP_BLACKLIST) == 0)
    {
        print_bold_by_color("Blacklist\n", RED);
        printf("To blacklist a list of words, type !blacklist_init\n");
        printf("To show the blacklist, type !show_blacklist\n");
        printf("To blacklist a word, type !blacklist {word}\n");
        return;
    }
    else if (strcmp(message.request.sendMessage.message.content, SET_BLACKLIST_COMMAND) == 0)
    {
        message = (RequestMessage){
            .type = SET_BLACKLIST,
        };
        printf("Enter words to blacklist (separated by spaces): ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, MAX_CONTENT_LENGTH, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(message.request.blacklist_words.words, buffer, MAX_CONTENT_LENGTH);
        // sendBlacklistMessage(sock, &message);
        sendMessage(sock, &message);
        return;
    }
    else if (strcmp(message.request.sendMessage.message.content, SHOW_BLACKLIST_COMMAND) == 0)
    {
        print_bold_by_color("Show blacklist\n", RED);
        message = (RequestMessage){
            .type = SHOW_BLACKLIST,
        };
        for (int j = 0; j < MAX_WORDS_BLACKLIST; j++)
        {
            if (groupClient->blacklist->blacklist[j] != NULL)
                print_bold_log_with_params_by_color(DEFAULT, "%s ", groupClient->blacklist->blacklist[j], GREEN);
        }
        return;
    }
    // else if (strcmp(message.request.sendMessage.message.content, ADD_WORD_BLACKLIST_COMMAND) == 0)
    // {
    //     print_bold_by_color("Show blacklist\n", RED);
    //     message = (RequestMessage){
    //         .type = ADD_WORD_BLACKLIST,
    //     };
    //     printf("Enter word to blacklist: ");
    //     memset(buffer, 0, sizeof(buffer));
    //     fgets(buffer, MAX_CONTENT_LENGTH, stdin);
    //     sendBlacklistMessage(sock, buffer);
    //     return;
    // }
    else if (strcmp(message.request.sendMessage.message.content, HELP_COMMAND) == 0)
    {
        print_bold_by_color("\t\t--- Help Menu ---\n", RED);
        printf("You can use the following commands:\n");
        print_bold_by_color("Every command should start with a exclamation mark to make the command run\n", CYAN);
        printf("\t- !help: Shows the help menu\n");
        printf("\t- !close_program: Closes the program\n");
        printf("\t- !leave: Leaves the group (currently admins can't leave their group)\n");
        printf("\t- !blacklist: censore certain words in group. (GROUP ADMIN COMMAND) [currently in development]\n");
        return;
    }
    sendMessage(sock, &message);
}

// Handler for server responses

void readServerResponse(int sock, ResponseMessage *response, char *buffer)
{
    // Size of the struct without the ResponseMessage description
    // which needs to be read separately using the buffer
    unsigned long int size = sizeof(ResponseMessage) - sizeof(char *);
    int valread = read(sock, response, size);
    if (valread > 0)
    {
        // Receive the actual message content
        int bytes_received = read(sock, buffer, MAX_BUFFER_SIZE);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0'; // Null-terminate the received string
        }
        // Assign the received content to the struct's pointer
        response->description = buffer;
    }
    else if (valread == 0)
    {
        // Server closed the connection
        print_bold_by_color("Server closed the connection\n", RED);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Read error
        perror("read");
        print_bold_by_color("Exit\n", RED);
        exit(EXIT_FAILURE);
    }
}

void handleServerResponse(int sock, ResponseMessage *response)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    memset(buffer, 0, sizeof(buffer));

    readServerResponse(sock, response, buffer);

    // Handle the received message
    MessageType received_type = response->type;
    ResponseStatus received_status = response->status;

    // Handle errors
    if (received_status == ERROR)
    {
        print_log_with_params_by_color(DEFAULT, "%s", response->description, RED);
        if (inGroup == FALSE)
            print_bold_log_by_color(DEFAULT, "Retry a command\n", RED);
        if (received_type == REMOVE_GROUP)
        {
            inGroup = FALSE;
            authenticatedMenu();
            print_log_with_params_by_color(DEFAULT, "%s", response->description, RED);
        }
        return;
    }
    
    switch (received_type)
    {
    case LOGIN:
        handleLoginSuccess();
        break;
    case LOGOUT:
        handleLogoutSuccess();
        break;
    case SEND_MESSAGE:
        print_log_with_params_by_color(DEFAULT, "%s\n", response->description, GREEN);
        break;
    case RECEIEVE_MESSAGE:
        print_bold_by_color("Received message\n", GREEN);
        printChatMessage(response->message);
        break;
    case CREATE_GROUP:
        strncpy(groupClient->name, response->description, MAX_GROUPNAME_LENGTH);
        handleJoinGroupSuccess(response);
        break;
    case JOIN_GROUP:
        strncpy(groupClient->name, response->description, MAX_GROUPNAME_LENGTH);
        handleJoinGroupSuccess(response);
        break;
    case LEAVE_GROUP:
        print_bold_by_color("Leave group successful\n", GREEN);
        authenticatedMenu();
        break;
    case REMOVE_GROUP:
        print_bold_by_color("Group removed\n", GREEN);
        authenticatedMenu();
        break;
    case SET_BLACKLIST:
        print_log_with_params_by_color(DEFAULT, "Your Blacklist is set:\n %s\n", response->description, GREEN);
        break;
    case SHOW_AVAILABLE_GROUPS:
        print_log_with_params_by_color(DEFAULT, "Groups:\n %s\n", response->description, GREEN);
        break;
    default:
        printf("Received unknown message type: %d\n", received_type);
        print_bold_by_color("The request you did was unusual, can't help you with that.\nplease restart the program\n", RED);
        exit(EXIT_FAILURE);
        break;
    }
}

// Handlers for success events

void handleJoinGroupSuccess(ResponseMessage *response)
{
    system("clear");
    print_bold_log_with_params_by_color(DEFAULT, "Join group successful\n", response->description, GREEN);
    sleep(CLIENT_SCREEN_PAUSES_SECONDS);
    system("clear");
    print_log_with_params_by_color(LOG_INFO, "Username: %s", userClient->username, CYAN);
    print_log_with_params_by_color(LOG_INFO, "Group name: %s \n", groupClient->name, CYAN);
    inGroup = TRUE;
}

void handleLoginSuccess()
{
    print_bold_by_color("Login Success\n", GREEN);
    print_bold_log_with_params_by_color(DEFAULT, "Your username is: %s", userClient->username, GREEN);
    sleep(CLIENT_SCREEN_PAUSES_SECONDS);
    system("clear");
    authenticatedMenu();
    isAuth = TRUE;
}

void handleLogoutSuccess()
{
    print_bold_by_color("Logout Success", GREEN);
    sleep(CLIENT_SCREEN_PAUSES_SECONDS);
    system("clear");
    starterMenu();
    isAuth = FALSE;
    inGroup = FALSE;
}
