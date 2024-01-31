#pragma once
#include "User.h"
#include <time.h>
#define MAX_CONTENT_LENGTH 100
#define MAX_WORD_LENGTH 20
/**
 * @brief Struct representing a chat message.
 *
 * This struct contains information about a chat message, including the sender, content, and timestamp.
 */
typedef struct chat_message_t
{
    User sender;                      /* The sender of the message. */
    char content[MAX_CONTENT_LENGTH]; /* The content of the message. */
    time_t timestamp;                 /* The timestamp of the message. */
} chatMessage;

/**
 * @brief Prints a chat message.
 * 
 * This function takes a chatMessage structure as input and prints its contents.
 * 
 * @param message The chat message to be printed.
 */
void printChatMessage(chatMessage message);