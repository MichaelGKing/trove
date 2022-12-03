#define _DEFAULT_SOURCE
#include "trove.h"

// This file is basically a copy of the findfiles.c file but has been adjusted to store paths found in a global variable.

// Recursively searches directory/s for files.
void dir_path_generator(char *dirname)
{
  DIR *dirp;
  struct dirent *dp;
  dirp = opendir(dirname);

  if (dirp == NULL)
  {
    printf("Could not open directory!\n");
    exit(EXIT_FAILURE);
  }
  while ((dp = readdir(dirp)) != NULL)
  {
    if (dp->d_type == DT_DIR)
    {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
        char *new_path = (char *)malloc(strlen(dirname) + strlen(dp->d_name) + 1);
        sprintf(new_path, "%s/%s", dirname, dp->d_name);
        dir_path_generator(new_path);
        free(new_path);
      }
    }
    else
    {
      char *path_name = (char *)malloc(strlen(dirname) + strlen(dp->d_name) + 1);
      sprintf(path_name, "%s/%s", dirname, dp->d_name);
      char *abspath = realpath(path_name, absolute_path);
      for (int i = 0; i < strlen(abspath); i++) // Stores path in our global variable.
      {
        updatepaths[updpathsnum][i] = abspath[i];
      }
      updpathsnum++;
    }
  }
  closedir(dirp);
}

// Stores the path of any files and calls the dir_path_generator for finding paths for inside any directories given.
void store_paths(char **filelist, int filelistLength)
{
  memset(updatepaths, 0, sizeof updatepaths);
  // array to store paths
  int fileOrName;

  printf("\n****************************************************************\n");
  printf("Testing if all filelist values are valid!\n");
  for (int i = 0; i < filelistLength; i++)
  {
    fileOrName = dir_or_file(filelist[i]);
    if (fileOrName == 1)
    {
      printf("'%s' is a regular file!\n", filelist[i]);
    }
    else if (fileOrName == 2)
    {
      printf("'%s' is a directory!\n", filelist[i]);
    }
  }
  printf("Success all files/directories are valid!\n");

  printf("\n***********************************************************\n");
  printf("Finding all the paths...\n");

  // Once successful will print out the paths of each file.
  for (int i = 0; i < filelistLength; i++)
  {
    fileOrName = dir_or_file(filelist[i]);
    char *abspath = realpath(filelist[i], absolute_path);
    if (fileOrName == 1)
    {
      for (int i = 0; i < strlen(abspath); i++) // Stores path in our global variable.
      {
        updatepaths[updpathsnum][i] = abspath[i];
      }
      updpathsnum++;
      if (updpathsnum == MAXUPATHS) // Global variable limits it to 100. Can increase if need be.
      {
        printf("Apologies too many paths, keep it under %d, or change the code XD!\n", MAXUPATHS);
        exit(EXIT_FAILURE);
      }
    }
    else if (fileOrName == 2)
    {
      dir_path_generator(filelist[i]);
    }
  }
}
