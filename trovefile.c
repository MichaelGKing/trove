#include "trove.h"

void build_trove(char *string, bool newPath) // Receives a string and information on if the string is a path.
{
  // Checks if a file doesnt exist for the given trove file name
  if (access(filenm, F_OK) != 0)
  {
    FILE *trovew = fopen(filenm, "w");
    fclose(trovew);
  }

  // Appends information to file as it comes through the function
  FILE *trovea = fopen(filenm, "a");
  if (trovea != NULL)
  {
    if (newPath == true)
    {
      fputs("---\n", trovea); // Indicates if next line is a path.
    }
    fputs(string, trovea);
    fputs("\n", trovea);
  }
  fclose(trovea);
}