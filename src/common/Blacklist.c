#include "protocol.h"
#include "Blacklist.h"
#include "bool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Blacklist *initBlacklist(Blacklist *blacklist)
{
    if (blacklist == NULL)
    {
        blacklist = (Blacklist *)malloc(sizeof(Blacklist));
    }
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        blacklist->blacklist[i] = NULL;
    }
    return blacklist;
}

char *showBlacklist(Blacklist *blacklist)
{
    char *blacklistString = (char *)malloc(MAX_WORDS_BLACKLIST * MAX_WORD_LENGTH);
    strcpy(blacklistString, "blacklist of words:");
    if (blacklist == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        if (blacklist->blacklist[i] != NULL)
        {
            strcat(blacklistString, blacklist->blacklist[i]);
            strcat(blacklistString, "\n");
        }
    }
    return blacklistString;
}

Blacklist *addWordtoBlacklist(Blacklist *blacklist, char word[MAX_WORD_LENGTH])
{
    if (blacklist == NULL || word == NULL)
    {
        return blacklist;
    }
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        if (blacklist->blacklist[i] == NULL)
        {
            blacklist->blacklist[i] = (char *)malloc(strlen(word) + 1);
            strcpy(blacklist->blacklist[i], word);
            break;
        }
    }
    return blacklist;
}

boolean containsBlacklistedWord(const Blacklist *blacklist, const char *text)
{
    if (blacklist == NULL || text == NULL)
    {
        return FALSE;
    }
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        if (blacklist->blacklist[i] != NULL && strstr(text, blacklist->blacklist[i]) != NULL)
        {
            return TRUE;
        }
    }
    return FALSE;
}

Blacklist *parseBufferToBlacklist(char *buffer)
{
    Blacklist *blacklist = initBlacklist(blacklist);
    // Tokenize the input string and store each word in an array
    char *token;
    int i = 0;
    char *blacklistWordsSeperated[MAX_WORDS_BLACKLIST];
    token = strtok(buffer, BLACKLIST_DELIMITER);
    while (token != NULL && i < MAX_WORDS_BLACKLIST)
    {
        blacklistWordsSeperated[i++] = token;
        token = strtok(NULL, BLACKLIST_DELIMITER);
    }
    // Add each word to the blacklist
    for (int j = 0; j < i; j++)
    {
        addWordtoBlacklist(blacklist, blacklistWordsSeperated[j]);
    }
    return blacklist;
}

void freeBlacklist(Blacklist *blacklist)
{
    if (blacklist == NULL)
    {
        return;
    }
    for (int i = 0; i < MAX_WORDS_BLACKLIST; i++)
    {
        if (blacklist->blacklist[i] != NULL)
        {
            free(blacklist->blacklist[i]);
        }
    }
    free(blacklist);
}
