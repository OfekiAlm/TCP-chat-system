#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logs.h"
#include "Message.h"
#include "bool.h"
#include "protocol.h"
#include "Group.h"
#include "Blacklist.h"

Group *initGroup(Group *group, char *name, int id, int admin)
{
    group = (Group *)malloc(sizeof(Group));
    if (group == NULL)
    {
        print_log_by_color(LOG_ERROR, "Error allocating memory for group\n", RED);
        perror("Error:");

        exit(EXIT_FAILURE);
    }
    strncpy(group->name, name, MAX_GROUPNAME_LENGTH);
    group->id = id;
    group->admin = admin;
    group->member_count = 0;

    // Blacklist isn't initialized here,
    // it's initialized when the admin sends the blacklist

    return group;
}

// void allocateBlacklist(char ***blacklist)
// {
//     for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
//     {
//         (*blacklist)[i] = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
//     }
// }

void allocateBlacklistWords(char *(*blacklist_word)[MAX_WORDS_BLACKLIST])
{
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        (*blacklist_word)[i] = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));

        if ((*blacklist_word)[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free((*blacklist_word)[j]);
            }
            return;
        }
    }
}
void freeGroup(Group *group)
{
    freeBlacklist(group->blacklist);
    free(group);
    group = NULL;
}

int isAdmin(Group *group, int fd)
{
    return group->admin == fd;
}

void addMemberToGroup(Group *group, int fd)
{
    group->member_fds[group->member_count] = fd;
    group->member_count++;
}

void removeUserFromGroup(Group *group, int fd)
{
    for (int i = 0; i < group->member_count; ++i)
    {
        if (group->member_fds[i] == fd)
        {
            // Remove the user from the array
            for (int j = i; j < group->member_count - 1; ++j)
            {
                group->member_fds[j] = group->member_fds[j + 1];
            }
            group->member_count--;
            return;
        }
    }
}

boolean isWordBlacklisted(char *word, Group *group)
{
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        if (strcmp(group->blacklist->blacklist[i], word) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

boolean isBlacklistConfigured(Group *group)
{
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        if (strcmp(group->blacklist->blacklist[i], "") != 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

boolean removeEveryoneFromGroup(Group *group, int adminFD)
{
    ResponseMessage *response = initRequestMessage();
    response->type = REMOVE_GROUP;
    response->status = ERROR;
    response->description = "Group removed by admin";
    for (int i = 0; i < group->member_count; i++)
    {
        if (group->member_fds[i] == adminFD)
        {
            response->status = SUCCESS;
            response->description = "You removed the group";
            response->type = LEAVE_GROUP;
            sendResponse(group->member_fds[i], response);
        }
        else
        {
            sendResponse(group->member_fds[i], response);
        }
        removeUserFromGroup(group, group->member_fds[i]);
    }
    free(response);
    return TRUE;
}