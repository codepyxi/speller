/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "dictionary.h"

// Function enum that states if it is the word or a word before or a word after
// I want to be able to return: word before, word after, and word


char **dict_load = NULL;   // global variable
char **marque_page = NULL;   // global variable. It is a pointer to the word I want to compare.
int lettre = 0;   // global variable to indicate the index of the letter. At first equal to zero.
int dict_size = 0;   // Dictionary size


/**
 * New function that compares two words
 */
enum gps compare(char* word1, const char* word2)   // NOTE: word2 is the word I am looking for. word1 is the dictionary word
    {
        int i = 0;
        int j = 0;
        char *w1 = strdup(word1);   // I duplicate word1
        char *w2 = strdup(word2);    // I duplicate word2
        
        while (w1[j] != '\0')
        {
            w1[j] = tolower(w1[j]);   // This is to lower each letter in w1
            j++;
        }
        
        j = 0;
        while (w2[j] != '\0')
        {
            w2[j] = tolower(w2[j]);   // This is to lower each letter in w2
            j++;
        }


        while (w1[i] != '\0' && w2[i] != '\0')   // Until we are not at the end of the words (word1 and word2)
        {
            if (w1[i] != w2[i])   // if letters in word1 and word2 are not the same, break
                break;
            i++;
        }
        if (w1[i] > w2[i])
        {
            free(w1);
            free(w2);
            return(AFTER);
        }

        else if(w1[i] < w2[i])
        {
            free(w1);
            free(w2);
            return(BEFORE);
        }

        free(w1);
        free(w2);
        return(EQUAL);
    }


/**
 * Returns true if word is in dictionary, else false.
 */
bool check(const char *word)
{
    // TODO
    int i = 0;
    
    //printf("Check letter %d in dict %s for %s\n", lettre, marque_page[0], word);
    while (i < dict_size && compare(dict_load[i], word) == BEFORE)   // Recall: word est une char* et marque_page est un char**
        {
            i++;   // marque_page-0 is where I am, marque-page-1 is the next word
        }

        if (i < dict_size && compare(dict_load[i], word) == EQUAL)
            return(true);
        else
            return(false);
}



/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    // fread my dictionary  and store it in a char** or in a linked list
    FILE *fd;   // fd is my dictionary file.
    fd = fopen (dictionary, "r");   // fopen returns an FILE * 
    if (fd == NULL)
        printf("Noooooonn. I cannot open the file!\n");
    
    
    char word[47] = "";  // The longest word plus 1
    int i = 0;
    while (fscanf(fd, "%s\n", word) != EOF)   // Keep readin until the end of the file
    {
        dict_load = realloc(dict_load, sizeof(char*) * (i+1));     // Once I found my last word, I want to set the malloc 
            // i + 1: it is to take into consideration the end of the word -> '\0'

        dict_load[i] = strdup(word);   // Function that takes a copy of string and stores a malloc-copy.
        i++;
    }
    
    dict_size = i;
    //my_recursif_load(fd, 0);
    fclose(fd);
    marque_page = dict_load;   // On met le marque page au debut du dictionnaire
    
    return (true);
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    unsigned int i = 0;   // an unsigned int is expected by my function size
    while (dict_load[i] != NULL)   // Count until there are no more words to count in my dictionary "dict_load"
    {
        i++;
    }
    
    return (i);
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 * This is where I free my mallocs: dict_load and all the strdup created dict_load[i]
 */
bool unload(void)
{
    // TODO
    
    int i = size() - 1;   // The end of my dictionary (dict_load) is NULL. Thus, size() - 1
    while (i >= 0)
    {
        free(dict_load[i]);
        i--;
    }
    
    free(dict_load);   // First, I free word by word: free(dict_load[i]) then I free the dictionary (dict_load)
                        // Avoid the situation: "La charrue avant le boeufs"
    return true;
}
