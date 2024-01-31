#pragma once
#include "Message.h"
#define MAX_MESSAGES_PER_CHAT 100
typedef struct messages_t
{
    chatMessage messages[MAX_MESSAGES_PER_CHAT]; // Array of messages
    int count;                                   // Number of messages
} Messages;