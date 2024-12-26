#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <limits.h>
#include "triangle.h"
#include <ctype.h>

// The shared state in our concurrent scenario
struct state {
  int boundary, interior;

  int active, finished;
} state;

sem_t worker_semaphore;
sem_t callback_semaphore;
sem_t printer_semaphore;

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


  if (isNumber == 0) {
    fprintf(stderr,"String: %s must only consist of digits!\n",str);
    return 0;
  }

  return 1;
}

void print_triangle(struct triangle* tri) {
  printf("Triangle read:\n");
  for (int i = 0; i < 3; i++) {
      printf("Point %d: (%d, %d)\n", i + 1, tri->point[i].x, tri->point[i].y);
  }
  puts("");
}

/**
 * @brief Callback function for the countPoints() function of triangle.h.
 * This function should increment the number of found points by the given amount and signal the
 * output thread to update the progress report on stdout.
 * @param boundary Found points on the boundary of the triangle
 * @param interior Found points in the interior of the triangle
 */
static void calc_finished_cb(int boundary, int interior) {

  sem_wait(&callback_semaphore);
    state.interior += interior;
    state.boundary += boundary;
  sem_post(&callback_semaphore);

}


/**
*\brief Start routine of our worker threads of this problem.
* Remember the threads are m<eant to run detached.
* This has the advantage that no join is necessary but
* you need to bookkeep yourself if a thread has finished its workload.
* @param param the param of our worker threads
*/
static void* worker(void* param) {
  struct triangle* tri = (struct triangle*) param;

  sem_wait(&worker_semaphore);
  state.active++;
  sem_post(&worker_semaphore);

  //get points
  countPoints(tri, calc_finished_cb);

  sem_wait(&worker_semaphore);
  state.active--;
  state.finished++;
  sem_post(&worker_semaphore);

  sem_post(&printer_semaphore); //trigger the printer thread
}

/**
*\brief Start routine of the thread that is meant to present the results.
*@param param the param of our thread
**/
static void* printer(void* param) {

  while(1) {
    sem_wait(&printer_semaphore);

    printf("Found %d boundary and %d interior points, %d active threads, %d finished threads\n",
          state.boundary,
          state.interior,
          state.active,
          state.finished
          );

    sleep(0.01);
  }

}


void scan_tri() {
  int x1, x2, x3, y1, y2, y3;

  if(scanf("(%d,%d),(%d,%d),(%d,%d)", &x1, &y1, &x2, &y2, &x3, &y3) == 6) {

    struct triangle* tri = malloc(sizeof(struct triangle));
    if (tri == NULL) {
        fprintf(stderr,"Malloc failed while trying to allocate memory for a triangle!");
    }

    tri->point[0] = (struct coordinate){x1, y1};
    tri->point[1] = (struct coordinate){x2, y2};
    tri->point[2] = (struct coordinate){x3, y3};

    //print_triangle(tri);

    pthread_t tid;
    pthread_create(&tid, NULL, worker, (void*)tri);
  }
  else {
    fprintf(stderr, "stdin input is expected in format: '(int,int),(int,int),(int,int)\\n'\n");
  }

  //go to the next line
  //remove the remaining chars from stdin
  while(1) {
    char c = getchar();
    if (c == EOF) {
      sem_destroy(&worker_semaphore);
      sem_destroy(&callback_semaphore);
      sem_destroy(&printer_semaphore);
      exit(0);
    }
    if(c == '\n') {
      break;
    }
  }


}

int main(int argc, char * argv[]) {
  if (!(argc == 2)) {
    fprintf(stderr, "Usage: patric <num of worker threads>\n");
    return 1;
  }
  if (!isNumber(argv[1])) {
    fprintf(stderr,"Warning: <num of worker threads> must be an integer!\n");
    return 1;
  }
  int thread_num = atoi(argv[1]);

  sem_init(&worker_semaphore, 0, 1);
  sem_init(&callback_semaphore, 0, 1);
  sem_init(&printer_semaphore, 0, 0);

  pthread_t tid;
  pthread_create(&tid, NULL, printer, NULL);

  while(1) {
    //ensure that thread_num isn't exceeded
    if (state.active < thread_num) {
        scan_tri();
    }
    else {
      sleep(0.1);
    }
  }


  return 0;
}
