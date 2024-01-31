#include "Message.h"
#include <stdio.h>

void printChatMessage(chatMessage message){
    printf("Sender: %s\n", message.sender.username);
    printf("Content: %s\n", message.content);
    
    struct tm* ptr;
    ptr = localtime(&message.timestamp);
    printf("Timestamp: %s\n", asctime(ptr));
}