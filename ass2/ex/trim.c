#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int trimString(char* original, int n, char* resultPtr) {
    //resultPtr* = "";

    int strLen = strlen(original);

    if (strLen < n) {
      perror("The length of the original string\
        \n\ris shorter than n, which is the amount\
        \n\r of to be removed characters.");
      return 0;
    }


    for(int i = 0; i < strLen - n; i++) {
      resultPtr[i] = original[i];
    }

    return 1;
}

int main() {

    char testInput[] = "Lorem Ipsum";
    int trimLength = 12;

    char* result = calloc(1, sizeof(char));
    if(trimString(testInput, trimLength, result)) {
        printf("%s\n", result);
        free(result);
    }
    return 0;
}
