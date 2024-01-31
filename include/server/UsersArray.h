#pragma once
#include "User.h"
#define MAX_USERS 10
#define USER_NOT_FOUND -1
typedef struct users_t
{
    User users_arr[MAX_USERS]; // All connected users
    int count;                 // Number of connected users
} Users;

// Add a new user to the users array
void addUser(Users *users, int fd, char name[MAX_USERNAME_LENGTH]);

// Get the username of a user by his fd
char *getUsername(Users *users, int fd);

/**
 * Retrieves a User object from the Users array based on the given file descriptor.
 *
 * @param users The Users array.
 * @param fd The file descriptor of the User to retrieve.
 * @return A pointer to the User object if found, or NULL if not found.
 */
User *getUser(Users *users, int fd);

/**
 * Removes a User object from the Users array based on the given file descriptor.
 *
 * @param users The Users array.
 * @param fd The file descriptor of the User to remove.
 * @return 0 if the User was removed successfully, or USER_NOT_FOUND (-1) if the User was not found.
 */
int removeUser(Users *users, int fd);

/**
 * @brief Updates the group ID of a user in the Users array.
 *
 * This function updates the group ID of a user identified by the given file descriptor (fd)
 * in the Users array. The new group ID is specified by the group_id parameter.
 *
 * @param users The Users array.
 * @param fd The file descriptor of the user.
 * @param group_id The new group ID.
 */
void updateUserGroupId(Users *users, int fd, int group_id);

/**
 * Checks if a username exists in the Users array.
 *
 * @param users The Users array to search in.
 * @param username The username to check for existence.
 * @return 1 if the username exists, USER_NOT_FOUND (-1) otherwise.
 */
int checkUsernameExists(Users *users, char *username);

/**
 * @brief Prints the array of users.
 * 
 * @param users The pointer to the Users object.
 */
void printUsers(Users *users);

/**
 * @brief Initializes the Users array.
 * 
 * This function initializes the Users array by allocating memory for it and setting all elements to default values.
 * 
 * @param users A pointer to the Users array.
 * @return A pointer to the initialized Users array.
 */
Users *initUsers(Users *users);

/**
 * @brief Frees the memory allocated for the Users object.
 * 
 * @param users The pointer to the Users object.
 */
void freeUsers(Users *users);