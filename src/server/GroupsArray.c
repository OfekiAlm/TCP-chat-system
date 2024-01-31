#include "GroupsArray.h"
#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "Group.h"

Groups *initGroups(Groups *groups)
{
    groups = (Groups *)malloc(sizeof(Groups));
    for (int i = 0; i < MAX_GROUPS; i++)
    {
        initGroup(&groups->groups_arr[i], "Uncreated", -1, -1);
    }
    return groups;
}

void freeGroups(Groups *groups)
{
    for (int i = 0; i < groups->count; ++i)
    {
        freeGroup(&groups->groups_arr[i]);
    }
    free(groups);
}

void addNewGroup(Groups *groups, char name[MAX_GROUPNAME_LENGTH], int admin_fd)
{
    int groupIndex = groups->count;
    groups->groups_arr[groupIndex] = (Group){
        .name = "",
        .id = groupIndex,
        .admin = admin_fd,
        .member_count = 1,
        .member_fds = {admin_fd}};                                            // Update the first index to admin_fd
    strncpy(groups->groups_arr[groupIndex].name, name, MAX_GROUPNAME_LENGTH); // Copy the group name
    groups->count++;
}

void removeGroup(Groups *groups, int group_id){
    for (int i = 0; i < groups->count; i++)
    {
        if (groups->groups_arr[i].id == group_id)
        {
            // Remove the group from the array
            for (int j = i; j < groups->count - 1; j++)
            {
                groups->groups_arr[j] = groups->groups_arr[j + 1];
            }
            groups->count--;
            return;
        }
    }
}

char *getGroupName(Groups *groups, int id)
{
    for (int i = 0; i < groups->count; ++i)
    {
        if (groups->groups_arr[i].id == id)
        {
            return groups->groups_arr[i].name;
        }
    }
    return NULL;
}

char *showAllGroupsToClient(Groups *groups)
{
    int resultSize = groups->count * MAX_GROUPS; // Calculate the required memory size based on the number of groups
    char *result = malloc(resultSize);           // Allocate memory for the result
    for (int i = 0; i < groups->count; ++i)
    {
        sprintf(result + strlen(result), "\t%d. %s\n", groups->groups_arr[i].id, groups->groups_arr[i].name);
    }
    return result;
}

int findGroupByID(Groups *groups, int id)
{
    for (int i = 0; i < groups->count; ++i)
    {
        if (groups->groups_arr[i].id == id)
        {
            return i;
        }
    }
    return GROUP_NOT_FOUND;
}

int findGroupByName(Groups *groups, char *name)
{
    for (int i = 0; i < groups->count; ++i)
    {
        if (strcmp(groups->groups_arr[i].name, name) == 0)
        {
            return i;
        }
    }
    return GROUP_NOT_FOUND;
}

void printGroups(Groups *groups)
{
    printf("Going to print groups\n");
    printf("Groups:\n");
    printf("Groups count: %d\n", groups->count);
    for (int i = 0; i < groups->count; ++i)
    {
        printf("id: %d.name: %s\nmember count: %d\n", groups->groups_arr[i].id, groups->groups_arr[i].name, groups->groups_arr[i].member_count);
        printf("Group member fds: ");
        for (int j = 0; j < groups->groups_arr[i].member_count; j++)
        {
            printf("%d ", groups->groups_arr[i].member_fds[j]);
        }
        printf("\n");
    }
    printf("Done printing groups\n");
}