#include "trove.h"

// DEFINES GLOBAL VARIABLES
char absolute_path[PATHLENGTH];
LIST *hashtable[NH];

int wordlgth = 4;            // Default word length
char *filenm = "/tmp/trove"; // Default trove file name.

// Variables for updating our trove file.
int updpathsnum = 0;
char *updatefile = "a";
