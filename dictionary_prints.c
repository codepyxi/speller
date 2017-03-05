/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dictionary.h"

// Function enum that states if it is the word or a word before or a word after
// I want to be able to return: word before, word after, and word


char **dict_load = NULL;   // global variable
char **marque_page = NULL;   // global variable. It is a pointer to the word I want to compare.
int lettre = 0;   // global variable to indicate the index of the letter. Au debut, il est egale a zero

/**
 * New function that compares two words
 */
enum gps compare(char* word1, const char* word2)   // NOTE: word2 is the word I am looking for. word1 is the dictionary word
    {
        int i = 0;
        
        if (marque_page[0] == NULL)
            return (AFTER);
        
        while (word1[i] != '\0' && word2[i] != '\0')   // Until we are not at the end of the words (word1 and word2)
        {
            if (word1[i] != word2[i])   // if letters in word1 and word2 are not the same, break
                break;
            i++;
        }
        if (word1[i] > word2[i])
            return(AFTER);
        else if(word1[i] < word2[i])
            return(BEFORE);
        return(EQUAL);
    }


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    enum gps resultat;
    
    printf("Check letter %d in dict %s for %s\n", lettre, marque_page[0], word);
    
    resultat = compare(marque_page[0], word); // Je vais comparer le mot de mon marque page et le mot qu'on m'a donne en parametre
    if (resultat == EQUAL)
        {
            printf("EQUAL\n");
            marque_page = dict_load;   // Pour le prochain mot il va falloir retourner le marque-page au debut
            lettre = 0;
            return(true);   // Si mon resultat est EQUAL alors cela veut dire que le mot est bien ecrit   
        }
    else if (resultat == AFTER)
        {
            printf("AFTER\n");
            marque_page = dict_load;   // Pour le prochain mot il va falloir retourner le marque-page au debut
            lettre = 0;
            return(false);
        }
        
    // sinon il faut remettre notre marque-page et rappeler la fonction
    else
    {
        printf("BEFORE\n");
       // int i = lettre;
        
        while (marque_page[0] != NULL && compare(marque_page[0], word) == BEFORE)   // Recall: word est une char* et marque_page est un char**
        {
            marque_page = &marque_page[1];   // marque_page-0 est le mot ou je suis, marque-page-1 c'est le prochain mot
        }
        lettre = lettre + 1;   // C'est la seule partie que va changer une fois que j'ai verifier chaque lettre
        bool res = check(word);
        return(res);
    }
}


/**
 * New function: my_recursif_load
 * I only want to know how big is malloc at the end of my_recusifload
 */

void my_recursif_load(FILE *fd, int i)
    {
        char word[67] = "";
        
        // fscanf returns the number of items found
        // if fscanf returns 0 number of items found, then 
        if (fscanf(fd, "%s\n", word) == EOF)
        {
            dict_load = malloc(sizeof(char *) * (i+1));   // Once I found my last word, I want to set the malloc 
            // i + 1: it is to take into consideration the end of the word -> '\0'
            
            dict_load[i] = NULL;   // Since it is char** it is not zero or '\0', it is NULL to notify the end of the word 
        }
        else
        {
            printf("%s\n", word);
            // si scanf equals 1. It means that I have found a word
            my_recursif_load(fd, i+1);   // my_recursif_load has to store the word in dict_load
            dict_load[i] = strdup(word);   // Function that takes a copy of string and stores a malloc-copy.
            printf("Dict[%i] = %s\n", i, word);
            // Thus, it returns a pointer to that copy.
            // NOTE: Inside strdup malloc is used. Thus, free it at the end.
        }
        
    }



/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    // fread my dictionary  and store it in a char** or in a linked list
    FILE *fd;   // fd is my dictionary file.
    fd = fopen (dictionary, "r");   // fopen returns an FILE * et ce ca que 
    if (fd == NULL)
        printf("Noooooonn\n");
    my_recursif_load(fd, 0);
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
