#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include "trove.h"
// Functions for checking if files exist, and finding files when given a directory.

// Recursively files inside the directory, and sends them to a function which extracts the words from them.
void dir_file_finder(char *dirname)
{
  printf("Opening directory %s...\n", dirname);
  DIR *dirp;
  struct dirent *dp;
  dirp = opendir(dirname);

  if (dirp == NULL)
  {
    printf("Could not open directory!\n");
    return;
  }
  while ((dp = readdir(dirp)) != NULL)
  {
    if (dp->d_type == DT_DIR) // Checks if we have a directory and re-executes program to continue its search
    {

      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
        printf("dp_>d_name is %s\n", dp->d_name);
        char *new_path = (char *)malloc(strlen(dirname) + strlen(dp->d_name) + 1);
        sprintf(new_path, "%s/%s", dirname, dp->d_name);
        dir_file_finder(new_path);
        free(new_path);
      }
    }
    else // Executes when we find a file.
    {
      char *path_name = (char *)malloc(strlen(dirname) + strlen(dp->d_name) + 1);
      sprintf(path_name, "%s/%s", dirname, dp->d_name); // Creates a path out of the file
      char *abspath = realpath(path_name, absolute_path);
      // Extracts words from the files.
      find_words(abspath);
    }
  }
  closedir(dirp);
}

char *progname;

int dir_or_file(char *name)
{
  int status;
  struct stat st_buf;

  status = stat(name, &st_buf);

  if (status != 0)
  {
    printf("'%s' is neither a directory or a file.\n", name);
    exit(EXIT_FAILURE);
  }

  // Tell us what it is then exit.

  if (S_ISREG(st_buf.st_mode))
  {
    // printf("%s is a regular file.\n", name);
    return 1;
  }
  if (S_ISDIR(st_buf.st_mode))
  {
    // printf("%s is a directory.\n", name);
    return 2;
  }
  return 0;
}

void print_path(char **filelist, int filelistLength)
{
  int fileOrName;

  // Go through the filelist and test they exist as directory or normal file.
  printf("\n****************************************************************\n");
  printf("Testing if all filelist values are valid!\n");
  for (int i = 0; i < filelistLength; i++)
  {
    fileOrName = dir_or_file(filelist[i]);
    if (fileOrName == 1)
    {
      // realpath(filelist[i], absolute_path);
      printf("'%s' is a regular file!\n", filelist[i]);
    }
    else if (fileOrName == 2)
    {
      printf("'%s' is a directory!\n", filelist[i]);
    }
  }
  printf("Success all files are valid!\n");

  printf("\n***********************************************************\n");
  printf("Printing file names...\n");

  // Once successful will print out the paths of each file.
  for (int i = 0; i < filelistLength; i++)
  {
    fileOrName = dir_or_file(filelist[i]);
    if (fileOrName == 1)
    {
      // realpath(filelist[i], absolute_path);
      // printf("%s\n", filelist[i]);
      char *abspath = realpath(filelist[i], absolute_path);
      printf("Path is: %s\n", abspath);
      find_words(abspath);
    }
    else if (fileOrName == 2)
    {
      dir_file_finder(filelist[i]);
    }
  }
  // realpath("trovee-testing", absolute_path);
  // printf("%s\n", absolute_path);
}
