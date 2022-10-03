// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
//const unsigned int N = 26;
const unsigned int N = 26;
unsigned int number_ofwords;
unsigned int hashindex;
// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //create a variable that is a cursor that will traverse the linked list
    int index = 0;
    index = hash(word);
    node *cursor = table[index]; //no need for malloc becuase its only a variable pointer
    while (cursor != NULL) //loop through the linked list until it returns NULL or 0
    {
        if (strcasecmp(word, cursor->word) == 0) //compare the lowercased word if true or not
        {
            return true;
        }
        else
        {
            cursor = cursor->next; //if not then go to the next node
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash += tolower(word[i]);
    }
    return hash % N;
    //return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //load the string/ or disctionary file you will need to read from
    //load it into your hashtable
    //its gonna return a boolean - true if load successful/ false if theres a memory error like no available memory

    //hash table is an array of linked list
    //every element of a hash table can be called a bucket and can store words
    //hash function returns what index of the element/bucket that a word was put into in the array called hash table

    //create a hash table array with each index or bucket can contain the words that will be entered (maximum amount of letters in a word is LENGTH/45)

    FILE *dic = fopen(dictionary, "r");
    char words[LENGTH + 1];
    if (dic == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }
    //read through each word inside the opened file and read it into the word string array. end loop if reached EndOfFile(EOF)
    while (fscanf(dic, "%s", words) != EOF)
    {
        fscanf(dic, "%s", words); //scan the file, it is string so %s is used, store the read string onto an array
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, words);
        hashindex = hash(words);

        n->next = table[hashindex];
        table[hashindex] = n;
        fclose(dic);
        number_ofwords++;
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (number_ofwords > 0)
    {
        return number_ofwords;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    //create a cursor that will temporarily point to a node
    //create a tmp that will also temporarily point to the same node as cursor
    //first iterate through all linked lists on the hash table then free individual nodes inside
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
