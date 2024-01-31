#pragma once
#include "User.h"
#include "Group.h"
#include "bool.h"
#define MAX_GROUPS 10
#define GROUP_NOT_FOUND -1
typedef struct groups_t
{
    Group groups_arr[MAX_GROUPS]; // All groups
    int count;                    // Number of groups
} Groups;

/**
 * @brief Initializes a Groups array.
 * 
 * This function initializes a Groups array by allocating memory for it and setting all elements to NULL.
 * 
 * @param groups A pointer to the Groups array.
 * @return A pointer to the initialized Groups array.
 */
Groups *initGroups(Groups *groups);

/**
 * @brief Frees the memory allocated for a Groups object.
 * 
 * This function frees the memory allocated for a Groups object, including
 * all the memory allocated for the group names and the client lists.
 * 
 * @param groups The Groups object to be freed.
 */
void freeGroups(Groups *groups);

/**
 * @brief Adds a new group with the given name to the groups array.
 *
 * This function adds a new group to the groups array, using the provided name and admin file descriptor.
 * This function also add the admin user to the group.
 * 
 * @param groups The groups array to add the new group to.
 * @param name The name of the new group.
 * @param admin_fd The file descriptor of the admin user for the new group.
 */
void addNewGroup(Groups *groups, char name[MAX_GROUPNAME_LENGTH], int admin_fd);

/**
 * @brief Removes a group from the groups array.
 *
 * This function removes a group with the specified group_id from the groups array.
 *
 * @param groups The groups array.
 * @param group_id The ID of the group to be removed.
 */
void removeGroup(Groups *groups, int group_id);

/**
 * Adds a user to a group.
 *
 * @param groups The groups array.
 * @param user   The user to be added.
 */
void addUserToGroup(Groups groups, User *user);

/**
 * Retrieves the group name by its ID.
 *
 * @param groups The Groups structure containing the group information.
 * @param id The ID of the group.
 * @return A pointer to the group name, or NULL if the group is not found.
 */
char *getGroupName(Groups *groups, int id);

/**
 * Finds a group by its name in the given array of groups.
 *
 * @param groups The array of groups to search in.
 * @param name The name of the group to find.
 * @return The index of the found group, or GROUP_NOT_FOUND (-1) if the group was not found.
 */
int findGroupByName(Groups *groups, char *name);

/**
 * @brief Finds a group in the array by its ID.
 *
 * This function searches for a group with the specified ID in the given array of groups.
 *
 * @param groups The array of groups to search in.
 * @param id The ID of the group to find.
 * @return The index of the group in the array if found, or GROUP_NOT_FOUND (-1) if not found.
 */
int findGroupByID(Groups *groups, int id);

/**
 * @brief Show all groups.
 *
 * This function displays all the groups in the given 'groups' array.
 *
 * @param groups The pointer to array of groups.
 * @return A pointer to a char array containing the names of all the groups.
 */
char *showAllGroupsToClient(Groups *groups);

/**
 * @brief Prints the information of all groups.
 * 
 * @param groups The pointer to the Groups object.
 */
void printGroups(Groups *groups);