#include <stdio.h>    // IO such as println and puts
#include <stdlib.h>   // malloc, free and more
#include <pthread.h>  // POSIX threads and useful functions for them

typedef struct Thread_input{
    int lower_bound, upper_bound;


    long res;// Store the result of the computation in res
    long* arr;
} Thread_input;

/*
\brief The start routine for all our threads
@param thread_input void pointer
@return The start routine has to return a void pointer - just return NULL in this task :)
*/
void* thread_routine(void *thread_input){
  // Implement me!
  Thread_input* input = (Thread_input*) thread_input;

  int lower_bound = input->lower_bound;
  int upper_bound = input->upper_bound;
  long* arr = input->arr;
  long res = input -> res;

  printf("lower_bound: %d, upper_bound: %d\n", lower_bound, upper_bound);

  for(int i = lower_bound; i <= upper_bound; i++) {
    res += arr[i];


  }
  printf("res: %ld\n",res);


  return input;
}

/*
\brief Sums an array in a threaded fashion using a passed number of threads.
@param thread_num the number of threads
@param input_arr The array that has to be summed
@return the sum
*/
long threaded_sum(int thread_num, long input_arr[], size_t array_len){
    if (thread_num > array_len) {
      thread_num = array_len;
    }

    long* arr = calloc(array_len, sizeof(long));
    int lower_bound = 0;
    int upper_bound = 1;

    //find bounds
    int bound_thickness = array_len / thread_num;

    //use modulo to distribute remainding blocks
    int bound_rem = array_len % thread_num;
    //bound_rem many threads have +1 blocks to handle

    //printf("bound_thickness: %d, bound_rem: %d\n", bound_thickness, bound_rem);

    int bound_start = 0;
    pthread_t thread_ids[thread_num];
    for (int i = 0; i < thread_num; i++) {
      lower_bound = bound_start;

      if (i < bound_rem) { //bound_rem many threads have +1 blocks to handle
          upper_bound = bound_start + bound_thickness;
      } else {
        upper_bound = bound_start + bound_thickness - 1;
      }
      bound_start = upper_bound + 1;

      //printf("lower_bound: %d, upper_bound: %d\n", lower_bound, upper_bound);

      Thread_input thread_input = {lower_bound,upper_bound, 0, arr};
      pthread_create(&thread_ids[i],NULL,thread_routine, &thread_input);
    }


    //join the threads and sum all the results
    for (int i = 0; i < thread_num; i++) {
      pthread_join(thread_ids[i], NULL);
    }

    return 0;
}


int main(){
  int thread_num = 10;
  size_t arr_len = 100;
  long* arr = calloc(arr_len, sizeof(long));

  for (int i = 1; i <= arr_len; i++) {
    arr[i] = i;
  }

  threaded_sum(thread_num , arr, arr_len);
}
