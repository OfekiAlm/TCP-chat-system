#pragma once

#define MAX_MEMBERS 10
#define MAX_GROUPNAME_LENGTH 50
#define BLACKLIST_DELIMITER " "
#define GROUP_DEFAULT -1
#include "bool.h"
#include "Blacklist.h"

// Group structure
typedef struct group_t
{
    char name[MAX_GROUPNAME_LENGTH]; // Name of the group
    int id;                          // Group id
    int member_fds[MAX_MEMBERS];     // Array of user fds
    int member_count;                // Number of members in the group
    int admin;                       // Admin of the group, user's fd
    Blacklist *blacklist;            // Blacklist of words
} Group;

/**
 * Initializes a Group object with the specified name, id, and admin.
 *
 * @param group The Group object to initialize.
 * @param name The name of the group.
 * @param id The ID of the group.
 * @param admin The admin flag indicating if the user is an admin of the group (1) or not (0).
 */
Group *initGroup(Group *group, char *name, int id, int admin);

/**
 * @brief Frees the memory allocated for the group.
 *
 * This function frees the memory allocated for the group and its members.
 *
 * @param group The group to free.
 */
void freeGroup(Group *group);

/**
 * Checks if the given file descriptor belongs to an admin in the specified group.
 *
 * @param group The group to check.
 * @param fd The file descriptor to check.
 * @return 1 if the file descriptor belongs to an admin, 0 otherwise.
 */
int isAdmin(Group *group, int fd);

/**
 * Adds a member to the group.
 *
 * @param group The group to add the member to.
 * @param fd The file descriptor of the member to add.
 */
void addMemberToGroup(Group *group, int fd);

/**
 * @brief Removes a user from a group.
 *
 * This function removes a user with the specified file descriptor (fd) from the given group.
 *
 * @param group The group from which to remove the user.
 * @param fd The file descriptor of the user to be removed.
 */
void removeUserFromGroup(Group *group, int fd);

/**
 * Finds a member in the group based on the given file descriptor.
 *
 * @param group The group to search in.
 * @param fd The file descriptor of the member to find.
 * @return The index of the member in the group if found, -1 otherwise.
 */
int findMemberInGroup(Group group, int fd);

/**
 * @brief Removes a specified file descriptor from the group.
 *
 * This function removes all users from group and notifies them about the action.
 * the admin gets a notification that he removed the group.
 *
 * @param group The group from which to remove the file descriptor.
 * @param fd The admin file descriptor.
 * @return Returns 0 on success, or a negative value on failure.
 */
boolean removeEveryoneFromGroup(Group *group, int adminFD);