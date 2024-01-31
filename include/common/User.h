#pragma once
#define ADMIN_DEFAULT -1
#define FILE_DESCRIPTOR_DEFAULT -1
#define MAX_USERNAME_LENGTH 20
// User structure
typedef struct user_t
{
    char username[MAX_USERNAME_LENGTH]; // Name of the user
    int fd;                             // File descriptor for user's socket
    int group_id;                       // Group ID the user is part of (-1 if not part of any group)
} User;

/**
 * Initializes a User object with the given parameters.
 *
 * @param user      Pointer to the User object to be initialized.
 * @param username  The username of the user.
 * @param fd        The file descriptor associated with the user's connection.
 * @param group_id  The ID of the group the user belongs to.
 * @return          The initialized User object.
 */
User *initUser(User *user, char *username, int fd, int group_id);

/**
 * @brief Frees the memory allocated for a User object.
 *
 * This function frees the memory allocated for a User object.
 * @param user The User object to be freed.
 */
void freeUser(User *user);