#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "protocol.h"
void sendMessage(int sock, RequestMessage *message)
{
    send(sock, message, sizeof(RequestMessage) - sizeof(char *), 0);
}

void sendChatMessage(int sock, chatMessage *message)
{
    ResponseMessage response;
    response.type = RECEIEVE_MESSAGE;
    response.status = SUCCESS;
    response.message = *message;

    send(sock, &response, sizeof(ResponseMessage) - sizeof(char *), 0);

    // Send the actual message content
    send(sock, message->content, strlen(message->content) + 1, 0); // +1 for null terminator
}

void sendResponse(int sock, ResponseMessage *response)
{
    // Send the struct without the message content
    send(sock, response, sizeof(ResponseMessage) - sizeof(char *), 0);

    // Send the actual message content
    send(sock, response->description, strlen(response->description) + 1, 0); // +1 for null terminator
}

void printRequestMessage(RequestMessage *message)
{
    switch (message->type)
    {
    case LOGIN:
        printf("LOGIN: %s\n", message->request.login.username);
        break;
    case LOGOUT:
        printf("LOGOUT: %s\n", message->request.logout.username);
        break;
    case SEND_MESSAGE:
        printf("SEND_MESSAGE: %s\n", message->request.sendMessage.message.content);
        break;
    case CREATE_GROUP:
        printf("CREATE_GROUP: %s\n", message->request.createGroup.groupName);
        break;
    case JOIN_GROUP:
        printf("JOIN_GROUP: %d\n", message->request.joinGroup.groupID);
        break;
    case SET_BLACKLIST:
        printf("BLACKLIST: ");
        for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
        {
            printf("%s ", message->request.blacklist_words.words);
        }
        printf("\n");
        break;
    default:
        printf("Unknown message type\n");
        break;
    }
}

RequestMessage *initRequestMessage()
{

    RequestMessage *message = (RequestMessage *)malloc(sizeof(RequestMessage));
    if (message == NULL)
    {
        printf("Error allocating memory for RequestMessage\n");
        exit(EXIT_FAILURE);
    }
    return message;
}

ResponseMessage *initResponseMessage()
{
    ResponseMessage *message = (ResponseMessage *)malloc(sizeof(ResponseMessage));
    if (message == NULL)
    {
        printf("Error allocating memory for ResponseMessage\n");
        exit(EXIT_FAILURE);
    }
    message->description = (char *)malloc(MAX_CONTENT_LENGTH);
    return message;
}