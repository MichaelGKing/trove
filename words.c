#include "trove.h"
#include <sys/wait.h>

// Uses hash table by assigning a hash to every word, then linking it to the pathnames it is found in.
void find_word(char *string)
{
  int thepipe[2];
  FILE *cmd_output;
  char buf[1024];
  int status;

  if (pipe(thepipe) != 0)
  {
    perror("cannot create pipe");
    exit(EXIT_FAILURE);
  }

  // fork()ing THE PROCESS WILL DUPLICATE ITS DATA, INCLUDING THE pipe'S TWO FILE-DESCRIPTORS

  switch (fork())
  {
  case -1:
    printf("fork() failed\n"); // process creation failed
    exit(EXIT_FAILURE);
    break;

    //  CHILD PROCESS RUNS sort, READING ITS stdin FROM THE PIPE
  case 0:                            // new child process
    close(thepipe[0]);               // close the reading end's descriptor
    dup2(thepipe[1], STDOUT_FILENO); // duplicate/clone the reading end's descriptor and stdin

    close(thepipe[1]); // child will never write to pipe

    // child may now read from its stdin (fd=0)
    execl("/usr/bin/zcat", "zcat", filenm, NULL); // execute a new (filter) program
    perror("/usr/bin/zcat");

    exit(EXIT_FAILURE);
    break;

  default:             // parent process
    close(thepipe[1]); // parent will never read from pipe
    // dup2(thepipe[1], 1); // duplicate/clone the writing end's descriptor and stdout
    // close(thepipe[1]);   // close the writing end's descriptor
    cmd_output = fdopen(thepipe[0], "r");
    bool pathTest = false;

    char *pathname = (char *)malloc(1);
    char *wordalone = (char *)malloc(1);
    // Goes through trove file and allocates path to all the words hashes in that path.
    while (fgets(buf, sizeof buf, cmd_output) != NULL)
    {
      buf[strcspn(buf, "\n")] = 0;
      if (pathTest == true)
      {
        pathname = (char *)realloc(pathname, sizeof(buf));
        strcpy(pathname, buf);
      }
      if (strcmp(buf, "---") == 0)
      {
        pathTest = true;
      }
      else
      {
        pathTest = false;
      }

      if (pathTest == false && strcmp(buf, pathname) != 0)
      {
        wordalone = (char *)realloc(wordalone, sizeof(buf));
        strcpy(wordalone, buf);
        hashtable_add(pathname, wordalone);
      }
    }

    LIST *listPointer = hash_word_lookup(string);

    // If hash_word_lookup returns a non null value indicates words are in that path and returns them.
    if (listPointer != NULL)
    {
      printf("Paths are:\n");
      list_print(listPointer);
      exit(EXIT_SUCCESS);
    }
    else
    {
      printf("No paths found for that word =(\n");
      exit(EXIT_FAILURE);
    }

    wait(&status);
    printf("Child exit status = %d\n", status);

    break;
  }
}