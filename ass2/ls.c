#include <errno.h>
#include <fnmatch.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <regex.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

int simple_ls(char path[], int max_depth) {
    DIR * dir =  opendir(path);
    if (!dir || max_depth < 0) {
      return -1;
    }

    struct dirent* dEntry = readdir(dir);
    while(dEntry) {

      if (strcmp(dEntry -> d_name,".") && strcmp(dEntry -> d_name, "..")) {
        int newPathLen = strlen(path) + strlen(dEntry -> d_name) + 2;
        char newPath[newPathLen];
        snprintf(newPath, newPathLen, "%s/%s",path,dEntry->d_name);
        puts(newPath);

        simple_ls(newPath, max_depth-1);
      }
      dEntry = readdir(dir);
    }
    closedir(dir);

    return 0;
}


int main(int argc, char* argv[]) {
  if (argc == 1) {
    return simple_ls(".", 0);
  }

  if (argc == 2) {
    return simple_ls(argv[1], 10000);
  }


    char* path = argv[1];
    int max_depth = atoi(argv[2]);


    return simple_ls(path, max_depth);
}
