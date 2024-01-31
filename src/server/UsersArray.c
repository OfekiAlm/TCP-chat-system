#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UsersArray.h"
#include "Group.h"

void addUser(Users *users, int fd, char name[MAX_USERNAME_LENGTH])
{
    users->users_arr[users->count] = (User){
        .fd = fd,                  // Set the socket file descriptor
        .group_id = GROUP_DEFAULT, // Initially, user is not part of any group
        .username = ""             // Allocate memory for the string
    };
    strncpy(users->users_arr[users->count].username, name, MAX_USERNAME_LENGTH);
    users->count++;
}

char *getUsername(Users *users, int fd)
{
    for (int i = 0; i < users->count; i++)
    {
        if (users->users_arr[i].fd == fd)
        {
            return users->users_arr[i].username;
        }
    }
    return NULL;
}

User *getUser(Users *users, int fd)
{
    for (int i = 0; i < users->count; i++)
    {
        if (users->users_arr[i].fd == fd)
        {
            return &users->users_arr[i];
        }
    }
    return NULL;
}

int removeUser(Users *users, int fd)
{
    for (int i = 0; i < users->count; i++)
    {
        if (users->users_arr[i].fd == fd)
        {
            // Remove the user from the array
            for (int j = i; j < users->count - 1; j++)
            {
                users->users_arr[j] = users->users_arr[j + 1];
            }
            users->count--;
            return 0;
        }
    }
    return USER_NOT_FOUND;
}

void updateUserGroupId(Users *users, int fd, int group_id)
{
    for (int i = 0; i < users->count; i++)
    {
        if (users->users_arr[i].fd == fd)
        {
            users->users_arr[i].group_id = group_id;
            return;
        }
    }
}

int checkUsernameExists(Users *users, char *username)
{
    for (int i = 0; i < users->count; i++)
    {
        if (strcmp(users->users_arr[i].username, username) == 0)
        {
            return 1;
        }
    }
    return USER_NOT_FOUND;
}

void printUsers(Users *users)
{
    printf("Going to print users\n");
    printf("Users:\n");
    printf("Users count: %d\n", users->count);
    for (int i = 0; i < users->count; ++i)
    {
        printf("\tfd: %d.username: %s group-id: %d\n", users->users_arr[i].fd, users->users_arr[i].username, users->users_arr[i].group_id);
    }
    printf("Done printing users\n");
}

Users *initUsers(Users *users)
{
    users = (Users *)malloc(sizeof(Users) * MAX_USERS);
    if (users == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_USERS; i++)
    {
        initUser(&users->users_arr[i], "", FILE_DESCRIPTOR_DEFAULT, GROUP_DEFAULT);
    }
    users->count = 0;
    return users;
}

void freeUsers(Users *users)
{
    for (int i = 0; i < MAX_USERS; i++)
    {
        freeUser(&users->users_arr[i]);
    }
    free(users);
}