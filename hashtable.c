#include "trove.h"

// Functions for generating list.
LIST *list_new(void)
{
  return NULL;
}

LIST *list_add(LIST *oldlist, char *string)
{
  LIST *newlist = malloc(sizeof(LIST));
  // should always check if malloc works, in case we run out of memory.

  newlist->string = strdup(string);
  // this allocates memory and can fail also.

  newlist->next = oldlist; // this points to the element of our old list.

  return newlist;
}

void list_print(LIST *list)
{
  while (list != NULL)
  {
    printf("%s\n", list->string);
    list = list->next;
  }
}

// hashtable is an array of elements (20), each othose elements are pointers (due to *) and points to a data type called a list.

// Functions for generating hash.
uint32_t hash_string(char *string) // converts string to a hash.
{
  uint32_t hash = 0;

  while (*string != '\0')
  {
    hash = hash * 33 + *string;
    ++string;
  }
  return hash;
}

void hashtable_add(char *path, char *word)
{
  int h = hash_string(word) % NH;
  int same = 1;
  if (hashtable[h] != NULL)
  {
    same = strcmp(hashtable[h]->string, path);
  }

  if (same != 0)
  {
    hashtable[h] = list_add(hashtable[h], path);
  }
}

void hashtable_print(void)
{
  for (int i = 0; i < NH; i++)
  {
    printf("\n--- [%i] ---\n", i);
    list_print(hashtable[i]);
  }
  printf("nh is %d\n", NH);
}

// Returns a pointer to the hash of the given string.
LIST *hash_word_lookup(char *string)
{

  int h = hash_string(string) % NH;

  if (hashtable[h] != NULL)
  {
    return hashtable[h];
  }
  return hashtable[h];

  exit(EXIT_FAILURE);
}