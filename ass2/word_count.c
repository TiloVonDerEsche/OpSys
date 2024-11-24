#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
#include <string.h>
#include <ctype.h>

// internal wc shall use this function to read a file into
// the string to which char **read_string points to
//Use POSIX read, open, and close
// Do not use fopen, fread, and fclose from stdin!
int read_file(char* path, char** readString) {
    struct stat fileInfo;

    int statErr = stat(path, &fileInfo);
    if (statErr == -1) {
        return -1;
    }
    if (statErr == 0) {
      //check if file isn't regular
      if(S_ISREG(fileInfo.st_mode != 1)) {
        return -1;
      }
    }


    //exit if file is empty
    int fileSize = fileInfo.st_size;
    if (fileSize < 0) {
        return -1;
    }
    printf("%d\n",S_ISREG(fileInfo.st_mode));


    int fd = open(path, O_RDONLY);
    if (fd == -1) {
      return -1;
    }



    char* fileString = calloc(1,fileSize + 1);

    int readErr = read(fd,  fileString, fileSize);
    if (readErr == -1) {
      return -1;
    }


    *readString = fileString;
    free(fileString);
    return 0;
}

struct Res{int lines, words, bytes;};

// This programme is meant to do the main workload and store the result in the result struct
int internal_wc(char* path, struct Res *result) {
    char* read_string = "";
    if(read_file(path, &read_string) == -1) {
        return -1;
    }

    int byte_count = strlen(read_string);

    int in_word = 0;
    int word_count = 0;
    int line_count = 0;
    char c = 'a';


    //TODO: Investigate this algorithm
    //An additional linebreak adds 1 or 2 lines,
    //with no discernible pattern
    //Bytecount is way off
    ///////////////////////////////////////////////
    for (int i = 0; i < byte_count; i++) {
      c = read_string[i];

      if (c == '\n') {
        line_count++;
      }
      if (isspace(c)) {
        in_word = 0;
      }
      if (!isspace(c) && !in_word) {
        in_word = 1;
        word_count++;
      }
    }
    ///////////////////////////////////////////////

    result->words = word_count;
    result->lines = line_count;
    result->bytes = byte_count;
    return 0;
}

//This function is meant to print Lines Words Bytes Filename
void prettify_wc(struct Res *result, char *path){
  printf("%d %d %d %s\n", result->lines, result->words, result->bytes, path);
}

// The path shall be passed as a command line arg
// If the programme receives no arg it should terminate with -1
// It might be a good idea to output a string
int main(int argc, char *argv[]) {
    if (argc < 2) {
      puts("The program simplewc expects a path to a file as argument!");
      return -1;
    }

    char* path = argv[1];

    struct Res res;
    if (internal_wc(path, &res) == 0) {
      prettify_wc(&res, path);
      return 0;
    }

    return -1;
}
