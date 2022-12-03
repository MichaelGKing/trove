
#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <ctype.h>
#include <stdint.h>

// DECLARE GLOBAL PREPROCESSOR CONSTANTS
#define PATHLENGTH 1024
#define NH 100000
#define MAXUPATHS 100

// DECLARE GLOBAL FUNCTIONS
extern void print_path(char **, int);
extern void find_words(char *);
typedef struct _list
{
  char *string;
  struct _list *next;
} LIST;
extern LIST *list_new(void);
extern LIST *list_add(LIST *, char *);
extern void list_print(LIST *);
extern uint32_t hash_string(char *); // converts string to a hash.
extern void hashtable_add(char *, char *);
extern void hashtable_print(void);
extern void build_trove(char *, bool);
extern void compress_file(char *);
extern void compress(char *);
extern void find_word(char *);
extern LIST *hash_word_lookup(char *);
extern void remove_files(char **, int, bool);
extern void store_paths(char **, int);
extern int dir_or_file(char *);

// DECLARE GLOBAL VARIABLES
extern char absolute_path[];
extern LIST *hashtable[];

// extern bool verbose; // declarations do not provide initializations
extern int wordlgth;
extern char *filenm;
extern int updpathsnum;
extern char *updatefile;

// Special array to store -u pathnames so I can use it in my recursive function
char updatepaths[PATHLENGTH][MAXUPATHS];