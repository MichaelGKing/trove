#include "trove.h"

// Tests if char is an alphanumeric character, returns true if it is.
bool isAlphaNumeric(char x)
{
  int alpha = isalpha(x);
  int numeric = isdigit(x);
  if (alpha == 0 && numeric == 0)
  {
    return false;
  }
  return true;
}

// Finds words inside the given path and adds them to our trove file.
void find_words(char *path)
{
  printf("************************************************************\n");
  printf("Collecting words from: %s\n", path);
  build_trove(path, true);
  FILE *fp;
  bool alphaNum = false;
  int wordStart = 0;
  int wordEnd = 0;
  int wordCount = 0;

  fp = fopen(path, "r");
  if (fp == NULL)
  {
    exit(EXIT_FAILURE);
  }

  if (fp != NULL)
  {

    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));
    size_t got;
    while ((got = fread(buffer, 1, sizeof buffer, fp) > 0))
    {
      for (int i = 0; i < BUFSIZ; i++)
      {
        // Stores position of start of word if it finds it.
        if (alphaNum == false && isAlphaNumeric(buffer[i]) == true)
        {
          wordStart = i;
        }
        // stores position of end of word if it finds it.
        else if (alphaNum == true && isAlphaNumeric(buffer[i]) == false)
        {
          wordEnd = i;
        }
        alphaNum = isAlphaNumeric(buffer[i]);
        if (wordEnd == i && (wordEnd - wordStart >= wordlgth)) // Test that word is long enough
        {
          int m = 0;
          char *word;
          word = (char *)malloc(1);
          for (int j = wordStart; j < wordEnd; j++)
          {
            word[m] = buffer[j];
            m++;
            word = (char *)realloc(word, sizeof(word) + 1);
          }
          // Adds word to our trove file.
          build_trove(word, false);
          free(word);
          wordCount++;
        }
      }
    }
  }
  printf("Word count is: %d\n", wordCount);

  fclose(fp);
}