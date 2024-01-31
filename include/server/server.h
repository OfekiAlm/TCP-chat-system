#pragma once
#include "protocol.h"

/**
 * Accepts a new connection on the given server socket.
 *
 * @param server_fd The file descriptor of the server socket.
 * @param address   A pointer to a sockaddr structure that will store the client address information.
 * 
 * @return The file descriptor of the accepted connection.
 */
int acceptNewConnection(int server_fd, struct sockaddr *address);

/**
 * Sends a message to a specific group.
 *
 * @param sender The ID of the sender.
 * @param group_id The ID of the group.
 * @param message The message to be sent.
 * @return Returns 0 on success, -1 on failure.
 */
int sendMessageToGroup(int sender, int group_id, chatMessage message);

/**
 * Handles the login success for a client.
 *
 * @param current_fd The file descriptor of the client.
 * @param username The username of the client.
 * @param response The response message to be sent to the client.
 */
void handleLoginSuccess(int current_fd, char *username, ResponseMessage response);

/**
 * Handles the join group request from a client.
 *
 * @param current_fd The file descriptor of the client.
 * @param username The username of the client.
 * @param message The request message received from the client.
 * @param response The response message to be sent back to the client.
 */
void handleJoinGroup(int current_fd, char *username, RequestMessage message, ResponseMessage response);

/**
 * Handles the creation of a group.
 *
 * @param current_fd The file descriptor of the current connection.
 * @param username The username of the client requesting the group creation.
 * @param message The request message containing the group creation details.
 * @param response The response message to be sent back to the client.
 */
void handleGroupCreation(int current_fd, char *username, RequestMessage message, ResponseMessage response);

/**
 * @brief Handles sending a message to a group.
 *
 * This function is responsible for sending a message to other member of the client's group.
 * 
 * @param current_fd The file descriptor of the client.
 * @param username The username of the client.
 * @param message The message to be sent.
 * @param response The response message.
 */
void handleSendMessage(int current_fd, char *username, RequestMessage message, ResponseMessage response);

/**
 * @brief Handles the "Show Blacklist" command received from a client.
 * 
 * @param current_fd The file descriptor of the client connection.
 * @param username The username of the client.
 * @param response A pointer to a ResponseMessage structure to store the response.
 */
void handleShowBlacklist(int current_fd, char *username, ResponseMessage *response);

/**
 * Handles the "Set Blacklist" request from a client.
 *
 * @param current_fd The file descriptor of the client connection.
 * @param username The username of the client.
 * @param message The request message received from the client.
 * @param response A pointer to the response message to be sent back to the client.
 */
void handleSetBlacklist(int current_fd, char *username, RequestMessage message, ResponseMessage *response);

/**
 * @brief Handles the request to show available groups.
 *
 * @param current_fd The file descriptor of the client.
 * @param username The username of the client.
 * @param response A pointer to the response message structure.
 */
void handleShowAvailableGroups(int current_fd, char *username, ResponseMessage *response);

/**
 * @brief Handles the leave group operation for a client.
 *
 * @param current_fd The file descriptor of the client.
 * @param username The username of the client.
 * @param response A pointer to the response message structure.
 */
void handleLeaveGroup(int current_fd, char *username, ResponseMessage *response);

/**
 * @brief Handles the logout process for a client.
 *
 * @param current_fd The file descriptor of the client to handle logout.
 */
void handleLogout(int current_fd);

/**
 * @brief Handles the request to add a word to the blacklist.
 *
 * @param current_fd The file descriptor of the current connection.
 * @param username The username of the client making the request.
 * @param message The request message containing the word to be added to the blacklist.
 * @param response A pointer to the response message that will be updated by the function.
 */
void handleAddWordToBlacklist(int current_fd, char *username, RequestMessage message, ResponseMessage *response);


/**
 * @brief Handles the input from a client.
 *
 * This function is responsible for processing the input received from a client.
 *
 * @param current_fd The current file descriptor.
 * @param new_socket The new socket for the client.
 * @param count The count of clients.
 * @param message The request message received from the client.
 */
void handleClientInput(int current_fd, int new_socket, int count, RequestMessage message);

/**
 * @brief Handles the input from the client.
 *
 * This function is responsible for processing the input received from the client.
 * It takes the current file descriptor, the new socket, a pointer to the RequestMessage structure,
 * and the username as parameters.
 *
 * @param current_fd The current file descriptor.
 * @param new_socket The new socket.
 * @param message A RequestMessage structure.
 * @param username The username of the client.
 */
void handleClientRequest(int current_fd, int new_socket, RequestMessage message, char *username);

/**
 * @brief Handles the disconnection of a client.
 *
 * This function is called when a client disconnects from the server.
 * 
 * @param current_fd The file descriptor of the disconnected client.
 */
void handleDisconnect(int current_fd);
