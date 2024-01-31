#pragma once
#define MAX_WORDS_BLACKLIST 10
#include "bool.h"
#include "protocol.h"
#include "Message.h"

/**
 * @brief Represents a blacklist of words.
 *
 * The Blacklist class is responsible for storing a collection of words that are considered
 * blacklisted. It provides functionality to create and manage a blacklist.
 */
typedef struct blacklist_t
{
    char *blacklist[MAX_WORDS_BLACKLIST]; // Array of blacklisted words
} Blacklist;

/**
 * @brief Initializes a Blacklist object.
 *
 * This function initializes a Blacklist object by allocating memory and setting initial values.
 *
 * @param blacklist A pointer to the Blacklist object to be initialized.
 * @return A pointer to the initialized Blacklist object.
 */
Blacklist *initBlacklist(Blacklist *blacklist);

/**
 * @brief Displays the contents of the blacklist.
 *
 * This function takes a pointer to a Blacklist structure and displays its contents.
 *
 * @param blacklist A pointer to the Blacklist structure.
 * @return A pointer to a string containing the contents of the blacklist.
 */
char *showBlacklist(Blacklist *blacklist);
/**
 * @brief Adds a word to the blacklist.
 *
 * This function adds a word to the blacklist.
 *
 * @param blacklist The Blacklist object to add the word to.
 * @param word The word to add to the blacklist.
 * @return A pointer to the Blacklist object.
 */
Blacklist *addWordtoBlacklist(Blacklist *blacklist, char word[MAX_WORD_LENGTH]);

/**
 * @brief Checks if the given text contains a blacklisted word.
 *
 * This function checks if the given text contains a blacklisted word.
 *
 * @param blacklist The Blacklist object to check against.
 * @param text The text to check.
 * @return True if the text contains a blacklisted word, false otherwise.
 */
boolean containsBlacklistedWord(const Blacklist *blacklist, const char *text);

/**
 * Parses a buffer into a Blacklist object.
 *
 * @param buffer The buffer containing the data to parse.
 * @return A pointer to the parsed Blacklist object.
 */
Blacklist *parseBufferToBlacklist(char *buffer);

/**
 * @brief Frees a Blacklist object.
 *
 * @param blacklist The Blacklist object to be freed.
 */
void freeBlacklist(Blacklist *blacklist);