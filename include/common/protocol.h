#pragma once
#include "User.h"
#include "Group.h"
#include "Message.h"
#include "Blacklist.h"

/**
 * @brief Enumeration representing different types of messages.
 */
typedef enum request_type_t
{
    LOGIN,                 /* Login message type */
    LOGOUT,                /* Logout message type */
    SEND_MESSAGE,          /* Send message type */
    RECEIEVE_MESSAGE,      /* Receive message type */
    CREATE_GROUP,          /* Create group message type */
    JOIN_GROUP,            /* Join group message type */
    LEAVE_GROUP,           /* Leave group message type */
    REMOVE_GROUP,          /* Remove group message type */
    SET_BLACKLIST,         /* Set blacklist message type */
    ADD_WORD_BLACKLIST,    /* Add word to blacklist message type */
    SHOW_BLACKLIST,        /* Show blacklist message type */
    SHOW_AVAILABLE_GROUPS, /* Show available groups message type */
} MessageType;

/**
 * @brief Enumeration representing different types of responses.
 */
typedef enum
{
    SUCCESS, /* Success message type */
    ERROR    /* Error message type */
} ResponseStatus;

/**
 * @brief Struct representing a Response message.
 */
typedef struct
{
    MessageType type;
    ResponseStatus status;
    char *description;
    chatMessage message;
} ResponseMessage;

typedef struct protocol_message_t
{
    MessageType type;
    union request_t
    {
        struct login_t
        {
            char username[MAX_USERNAME_LENGTH];
        } login;

        struct logout_t
        {
            char username[MAX_USERNAME_LENGTH];
        } logout;

        struct send_message_t
        {
            chatMessage message;
        } sendMessage;

        struct create_group_t
        {
            char groupName[MAX_GROUPNAME_LENGTH];
        } createGroup;

        struct blacklist_req
        {
            char words[MAX_WORDS_BLACKLIST * MAX_WORD_LENGTH];
        } blacklist_words;
        struct
        {
            int groupID;
        } joinGroup;
    } request;
} RequestMessage;

/**
 * Sends a ProtocolMessage over a socket.
 *
 * @param sock The socket to send the message on.
 * @param message The ProtocolMessage to send.
 */
void sendMessage(int sock, RequestMessage *message);

/**
 * Sends a chat message over a socket.
 *
 * @param sock The socket to send the message on.
 * @param message The chat message to send.
 */
void sendChatMessage(int sock, chatMessage *message);

/**
 * sends a ResponseMessage from a socket.
 *
 * @param sock The socket to send the response to.
 * @param response The ResponseMessage to send.
 */
void sendResponse(int sock, ResponseMessage *response);

/**
 * @brief Prints the contents of a RequestMessage structure.
 *
 * This function takes a pointer to a RequestMessage structure and prints its contents.
 *
 * @param message A pointer to the RequestMessage structure to be printed.
 */
void printRequestMessage(RequestMessage *message);

/**
 * Initializes a RequestMessage with the specified MessageType.
 */
RequestMessage *initRequestMessage();

/**
 * @brief Initializes a ResponseMessage object.
 * 
 * @return A pointer to the initialized ResponseMessage object.
 */
ResponseMessage *initResponseMessage();