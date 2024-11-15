#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Parameters {
  char* name;
  int greeting; //1 for "Hello", 2 for "Hi", 3 for "Greetings"
  int season; //flag: 0 Nothing 1 Easter, 2 Halloween, 3 Christmas
};

int isNumber(char* str) {
  char c = '\0';
  int isNumber = 1;
  int i = 0;

  do {
    c = str[i];

    if (!isdigit(c) && c != '\0') {
      isNumber = 0;}

    i++;
  } while(c != '\0');

  //if argv[1] has a character which isn't a digit: abort program
  if (isNumber == 0) {
    printf("String: %s must only consist of digits!\nAborting program...",str);
    return 0;
  }

  return 1;
}

// Parse the parameters and put them into the params struct.
// Returns 0 on success, -1 on failure.
int parseParameters(int argc, char* argv[], struct Parameters* params) {
    /*
    Usage: welcome <name> <greeting: int in range:(1-3)> [-s <0-3>]
    */
    if (argc < 3 || argc > 5) {
      printf("Usage: welcome <name> <greeting: int in range:(1-3)> [-s <season (0-3)>]\n");
      return -1;
    }


    params->name = argv[1];

    ///////////////////////////////////////////////////////////////////////////
    //----------------------------greeting-----------------------------------//
    ///////////////////////////////////////////////////////////////////////////
    char * greeting = argv[2];

    //check for only digits
    if (!isNumber(greeting)) {
      return -1;
    }

    params->greeting = atoi(argv[2]);
    ///////////////////////////////////////////////////////////////////////////

    if (argc == 5) {
      //printf("Is argv[3]=%s equal to '-s'? : %d\n",argv[3] ,strcmp(argv[3], "-s"));
      if ( strcmp(argv[3], "-s") != 0 || !isNumber(argv[4])) {
        printf("Usage: welcome <name> <greeting: int in range:(1-3)> [-s <season (0-3)>]\n");
        return -1;
      }
      params->season = atoi(argv[4]);
    }
    else {
      params->season = 0;
    }


    return 0;
}

// Print a formatted greeting message.
void printMessage(struct Parameters* params) {
    char* returnStr = calloc(100, sizeof(char));

    //1 for "Hello", 2 for "Hi", 3 for "Greetings"
    switch(params->greeting) {
      case 1: strcat(returnStr, "Hello "); break;
      case 2: strcat(returnStr, "Hi "); break;
      case 3: strcat(returnStr, "Greetings "); break;
      default: perror("Greeting parameter must be an integer from 1 to 3!"); exit(-1);
    }

    strcat(returnStr, params->name);

    //flag: 0 Nothing 1 Easter, 2 Halloween, 3 Christmas
    switch(params->season) {
      case 0: strcat(returnStr, "!"); break;
      case 1: strcat(returnStr, ", Happy Easter!"); break;
      case 2: strcat(returnStr, ", Happy Halloween!"); break;
      case 3: strcat(returnStr, ", Merry Christmas!"); break;
      default: perror("Season flag must be an integer from 1 to 3, if specified!");
               exit(-1);
    }

    puts(returnStr);
}

int main(int argc, char *argv[]) {
    struct Parameters* params;
    parseParameters(argc, argv, params);
    printMessage(params);

    return 0;
}
