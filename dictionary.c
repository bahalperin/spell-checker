/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 *
 * Functions/definitions in this program are used in speller.c to help create a 
 * spellchecker.  Functions include load, unload, check, and size.  Load runs 
 * through a text file full of words and loads them into memory as a hash table
 * (dictionary).  Unload frees the memory the hash table is using after it is 
 * no longer needed.  Check performs a lookup on the hash table to determine 
 * if a word is in the dictionary (if not it's mispelled).  Size counts the number
 * of words in the dictionary.
 *
 * I implemented dictionary.c myself, but take no credit for speller.c as it was
 * already implemented as a part this assignment for edx cs50.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Number of linked lists in hash table.
#define HASH_SIZE 2000

// Each element in linked list is a node.  Node contains
// a word and a pointer to the next node.
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// Hash table is an array of linked lists.
node* hashtable[HASH_SIZE];

int hash_function(const char* word);

// Tracks number of words in dictionary.
int number_of_words = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int word_length = strlen(word);
    char lower_word[LENGTH+1];
    
    // Convert word to lowercase to accurately compare to hash table.
    for (int i = 0; i < word_length; i++)
    {
        // If character is uppercase, make it lowercase.
        if(isupper(word[i]))
        {
            lower_word[i] = tolower(word[i]) ;
        }
        // Otherwise it's already lowercase or it's not a letter.
        else
        {
            lower_word[i] = word[i];
        }
    }    
    // Add null character to end of char array.
    lower_word[word_length] = '\0';
    // Use hash function to find correct "bucket" to place word.
    int bucket = hash_function(lower_word);
    // Set cursor to first node in bucket.
    node* cursor = hashtable[bucket];   
    // Until the end of the linked list is reached (cursor == NULL),
    // compare each word stored in each node to lower_word.  If they're
    // the same, then the word is in the dictionary and is not mispelled.
    // Otherwise, it is spelled incorrectly.
    while (cursor != NULL)
    {
        if (strcmp(lower_word, cursor->word) == 0)
        {
            // If lowercase'd word is the same as another in the bucket,
            // it's a match and return true.
            return true;
        }
        cursor = cursor->next;
    }
    
    return false;
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Initialize each value in hash table to NULL.
    for(int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }   
    
    // Open the dictionary text file.
    FILE* the_dictionary;    
    the_dictionary = fopen(dictionary, "r"); 
      
    // Check if dictionary opened correctly.
    if (the_dictionary == NULL)
    {
        printf("Failed to load dictionary");
        return false;
    }  
    
    char buffer[LENGTH+1];
    // Loop through file until end of file is reached.
    // Save each word in buffer.
    while (fscanf(the_dictionary, "%s", buffer) > 0)
    {
        // Allocate memory for a new node.
        node* new_node = malloc(sizeof(node));
        // Set node's next pointer to NULL.
        new_node->next = NULL;
        // Set node's word to value stored in buffer.
        strcpy(new_node->word, buffer);
        // Run word through hash function to set bucket in hash table.
        int bucket = hash_function(new_node->word);
        // If it's the first node being added to the bucket, replace 
        // the NULL value with the new node.
        if (hashtable[bucket] == NULL)
        {
            hashtable[bucket] = new_node;
        }
        // Otherwise set new node's pointer to the first node in the linked list.
        // Then set new node to be the first node in the linked list.
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
        // Track number of words in dictionary.
        number_of_words++;
    }
    // Done with text file.  Time to close it.
    fclose(the_dictionary);
    // Everything seems to have gone well, return true.
    return true;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return number_of_words;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // Iterate over all linked lists in hash table.  Set
    // cursor to point at each one's location in memory.
    for (int i = 0; i < HASH_SIZE;i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            // Set temporary pointer to point at cursor's
            // location in memory.  Move cursor to the next node
            // so we don't lose track of it before freeing
            // the current node's (temp's) memory.
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    } 
    return true;
}

// Maps a word to an integer value to place it in the hash table.
// Sum the value of each character in the word, then find the 
// remainder after dividing by the size of the hash table.
int hash_function(const char* word)
{
    int sum = 0;
    int word_length = strlen(word);

    for (int i = 0; i < word_length; i++)
    {
        sum += word[i];
    }
    
    int bucket = sum % HASH_SIZE;
    return bucket;
}

