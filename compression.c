#include "trove.h"

// Compresses the given filename using gzip.
void compress(char *file)
{
  int thepipe[2];

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
  case 0:                // new child process
    close(thepipe[1]);   // child will never write to pipe
    dup2(thepipe[0], 0); // duplicate/clone the reading end's descriptor and stdin
    close(thepipe[0]);   // close the reading end's descriptor

    // child may now read from its stdin (fd=0)

    execl("/usr/bin/gzip", "/usr/bin/gzip", "-f", file, NULL); // execute a new (filter) program
    perror("/usr/bin/gzip");

    exit(EXIT_FAILURE);
    break;

  default:               // parent process
    close(thepipe[0]);   // parent will never read from pipe
    dup2(thepipe[1], 1); // duplicate/clone the writing end's descriptor and stdout
    close(thepipe[1]);   // close the writing end's descriptor

    // parent may now write to its stdout (fd=1)
    break;
  }
}