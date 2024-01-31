#include <stdlib.h>
#include "User.h"
#include "logs.h"
#include <string.h>

User *initUser(User *user, char *username, int fd, int group_id)
{
    user = (User *)malloc(sizeof(User));
    if (user == NULL)
    {
        print_log_by_color(LOG_ERROR, "Error allocating memory for userClient\n", RED);
        exit(EXIT_FAILURE);
    }
    strncpy(user->username, username, MAX_USERNAME_LENGTH);
    user->fd = fd;
    user->group_id = group_id;
    return user;
}

void freeUser(User *user)
{
    free(user);
    user = NULL;
}