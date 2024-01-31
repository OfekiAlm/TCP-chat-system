#pragma once
#define CLIENT_SCREEN_PAUSES_SECONDS 0
#define MAX_BUFFER_SIZE 1024

#include "protocol.h"

#define SET_BLACKLIST_COMMAND "!init"
#define ADD_WORD_BLACKLIST_COMMAND "!add_blacklist"
#define SHOW_BLACKLIST_COMMAND "!show_blacklist"
#define HELP_BLACKLIST "!blacklist"

#define CLOSE_PROGRAM_COMMAND "!close_program"
#define LEAVE_GROUP_COMMAND "!leave_group"
#define HELP_COMMAND "!help"

/**
 * @brief Displays the starter menu for the client.
 *
 * This function is responsible for displaying the starter menu
 * options for the client. It allows the user to choose different
 * actions to perform.
 */
void starterMenu();

/**
 * @brief This function handles the menu options for a client who is not logged in.
 *
 * @param sock: The socket descriptor for the client connection.
 * @param message: The request message received from the client.
 * @param type: Pointer to the request type.
 * @param buffer: Pointer to the buffer for receiving and sending data.
 */
void handleNotLoggedClientsMenu(int sock, RequestMessage message, char *buffer);

/**
 * @brief Displays the authenticated menu for a logged-in user.
 *
 * This function is responsible for displaying the authenticated menu
 * options for the user. It allows the user to choose different
 * actions to perform.
 */
void authenticatedMenu();

/**
 * @brief This function handles the menu options for a client who is logged in.
 *
 * @param sock: The socket descriptor for the client connection.
 * @param message: The request message received from the client.
 * @param lastType: Pointer to the last request type.
 * @param buffer: Pointer to the buffer for receiving and sending data.
 */
void handleAuthenticatedMenu(int sock, RequestMessage message, char *buffer);

/**
 * @brief This function handles the chat for a client who is in a group.
 *
 * @param sock: The socket descriptor for the client connection.
 * @param message: The request message received from the client.
 * @param type: Pointer to the request type.
 * @param buffer: Pointer to the buffer for receiving and sending data.
 */
void handleUserMessageInGroup(int sock, RequestMessage message, char *buffer);

/**
 * @brief Handles the response received from the server.
 *
 * This function takes a pointer to a ResponseMessage structure as input and
 * processes the server response accordingly.
 *
 * @param sock The socket descriptor for the client connection.
 * @param response Pointer to the ResponseMessage structure containing the server response.
 */
void handleServerResponse(int sock, ResponseMessage *response);

/**
 * @brief Reads the server response from the given socket and stores it in the response structure.
 *
 * This function reads the server response from the specified socket and stores it in the response structure.
 *
 * @param sock The socket descriptor to read from.
 * @param response Pointer to the ResponseMessage structure to store the server response.
 * @param buffer Pointer to the buffer to store the response description.
 */
void readServerResponse(int sock, ResponseMessage *response, char *buffer);

/**
 * @brief Handles the input from the client.
 *
 * This function is responsible for processing the input received from the client.
 * The function performs the necessary operations based on the input received.
 *
 * @param sock The socket descriptor for the client connection.
 * @param message A pointer to the RequestMessage structure containing the client's request.
 */
void handleClientInput(int sock, RequestMessage *message);

/**
 * @brief Handles the login success event.
 *
 * This function is called when the login process is successful.
 */
void handleLoginSuccess();

/**
 * @brief Handles the success of creating a group.
 *
 * This function is called when a group creation operation is successful.
 */
void handleCreateGroupSuccess();

/**
 * @brief Handles the successful joining of a group.
 *
 * This function is called when the client successfully joins a group.
 * @param response Pointer to the ResponseMessage structure containing the server response.
 */
void handleJoinGroupSuccess(ResponseMessage *response);

/**
 * @brief Handles the successful logout of the client.
 *
 * This function is called when the client successfully logs out from the server.
 */
void handleLogoutSuccess();