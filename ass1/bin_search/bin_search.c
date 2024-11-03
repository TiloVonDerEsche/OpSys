#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Fill the arrays with example data
void fill_array(int numbers[]) {
    numbers[0] = 4;
    numbers[1] = 10;
    numbers[2] = 22;
    numbers[3] = 23;
    numbers[4] = 26;
    numbers[5] = 31;
    numbers[6] = 44;
}

void print_arr(int m, int length, int* numbers) {
  printf("length: %d, numbers[m]: %d ", length, numbers[m]);

  printf("{");
  for (int i = 0; i < length; i++) {
    printf("%d",numbers[i]);
    if (i < length - 1) { printf(", ");}
  }
  printf("}\n");
}

// Binary search function
// Returns the number of search steps taken, or -1 if the element is not found
int binary_search(int numbers[], int length, int target_id) {
    int steps = 0;
    int m = 0;

    while (length > 0) {
      m = length / 2;

      print_arr(m, length, numbers);

      if (numbers[m] < target_id) {
        //add m to the numbers array pointer,
        //which effectivly discards all numbers to the left of m
        numbers += m;
        length -= m;
      }
      else if (numbers[m] > target_id) {
        length -= m;
      }
      else {
        return steps;
      }

      //prevent infinite loops, where m=0 and length-=m does nothing.
      if (m < 1) {
        length--;
      }

      print_arr(m, length, numbers);

      steps++;
    }

    return -1;
}


int main(int argc, char** argv) {


    if (argc != 2) {
      printf("Usage: bin_search <target_id>");
      return 1;
    }

    ////////////////////////////////////////////////////////////////////
    //------------Check if argv[1] only consists of digits------------//
    ////////////////////////////////////////////////////////////////////
    int isnumber = 1;
    int j = 0;
    char c = '\0';
    do {
      c = argv[1][j];

      if (!isdigit(c) && c != '\0') {
        isnumber = 0;}


      j++;
    } while(c != '\0');

    //if argv[1] has a character which isn't a digit: abort program
    if (isnumber == 0) {
      printf("target_id must only consist of digits!\nAborting program...");
      return 1;
    }


    ////////////////////////////////////////////////////////////////////
    //-----------------------Using Binary Search----------------------//
    ////////////////////////////////////////////////////////////////////



    int target_id = atoi(argv[1]); //atoi -> ascii to integer

    int array_length = 7;
    int numbers_array[array_length];

    fill_array(numbers_array);

    int steps = binary_search(numbers_array, array_length, target_id);


    ////////////////////////////////////////////////////////////////////
    //-------------------------Result Handling------------------------//
    ////////////////////////////////////////////////////////////////////
    if (steps == -1) {
      printf("The number %d is not in the array.",target_id);
    }
    else {
      printf("It took %d steps to find the number %d in the array.", steps, target_id);
    }
    ////////////////////////////////////////////////////////////////////

    return 0;
}
