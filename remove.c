#define _DEFAULT_SOURCE
#include "trove.h"
#include <sys/wait.h>

// Removes paths and their corresponding words from the trove file.
// Essentially it builds a new trove file by skipping lines that are part of the filelist paths.
void remove_files(char **filelist, int fileNum, bool uflag)
{
  int thepipe[2];
  FILE *trove_output;
  FILE *rmFile;
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

    execl("/usr/bin/zcat", "zcat", filenm, NULL); // Outputs the information stored in trove file.
    perror("/usr/bin/zcat");

    exit(EXIT_FAILURE);
    break;

  default:               // parent process
    close(thepipe[1]);   // parent will never read from pipe
    dup2(thepipe[1], 1); // duplicate/clone the writing end's descriptor and stdout
    close(thepipe[1]);   // close the writing end's descriptor
    trove_output = fdopen(thepipe[0], "r");
    char *rmFileName = "rmfile";

    rmFile = fopen(rmFileName, "w");
    fclose(rmFile);

    rmFile = fopen(rmFileName, "a");

    bool pathTest = false;
    bool removeFile = false;
    char *newlineBuf = (char *)malloc(1);

    char *pathname = (char *)malloc(1);

    // Loops through output of zipepd trove file
    while (fgets(buf, sizeof buf, trove_output) != NULL)
    {
      newlineBuf = (char *)realloc(newlineBuf, sizeof buf);
      strcpy(newlineBuf, buf);
      buf[strcspn(buf, "\n")] = 0;
      // pathTest true when line is a string for a path.
      if (pathTest == true)
      {
        pathname = (char *)realloc(pathname, sizeof(buf));
        strcpy(pathname, buf);
        bool match = false; // Tracks if a match is found in our list of files.
        for (int i = 0; i < fileNum; i++)
        {
          if (strcmp(pathname, filelist[i]) == 0) // if the path in the trove file is found in our -r filelist then indicates it is to be removed.
          {
            removeFile = true;
            match = true; // We have found a match
          }
          else if (match == false)
          {
            removeFile = false;
          }
        }
      }
      // If we hit --- indicates that next line is a path.
      if (strcmp(buf, "---") == 0)
      {
        pathTest = true;
      }
      else
      {
        pathTest = false;
      }
      if (removeFile == false) // Writes to our new file all lines not to be removed.
      {
        fputs(newlineBuf, rmFile);
      }
    }
    fclose(rmFile);
    rmFile = fopen(rmFileName, "r");

    // For when we remove content from the trove file that is being rewritten from our -u call.
    if (uflag == true) // If uflag is true then we open our tmp update file and copy tmp rm into it.
    {

      FILE *ufp;
      ufp = fopen(updatefile, "a");
      while (fgets(buf, sizeof buf, rmFile) != NULL)
      {
        fputs(buf, ufp);
      }
      fclose(ufp);
      // Replaces trove file with remove file to get
      rename(rmFileName, filenm);
      // then replaces remove file with our updated trove file. (gets rid of our remove file).
      rename(updatefile, filenm);
      printf("Finished updating our trove!\n");
      compress(filenm);
      wait(&status);
      printf("Child exit status = %d\n", status);
      break;
    }

    // If no uflag, then just replaces the trove file with the new trove file lacking the removed elements.
    rename(rmFileName, filenm);

    // Then compresses this file.
    printf("Success files removed!\n");
    compress(filenm);

    wait(&status);
    printf("Child exit status = %d\n", status);
    break;
  }
}