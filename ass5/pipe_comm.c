#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

// Our pipe file descriptors
static int pipefd_ptc[2], pipefd_ctp[2];

sem_t parent_sem;
sem_t child_sem;

void static die(char *msg){
    perror("pipe");
    exit(EXIT_FAILURE);
}

/**
 * @brief Create twp pipes and call perror and exit, if the creation fails.
 */
void create_pipes() {

  if (pipe(pipefd_ptc) == -1){
      die("Parent pipe creation failed: ");}

  if (pipe(pipefd_ctp) == -1){
      die("Child pipe creation failed: ");}
}
/**
 * @brief The child process sends the parent Pong
 */
void tell_parent() {
  write(pipefd_ptc[1],"Pong!",6);


  printf("Child sent a message to parent!\n");
  //printf("Child: Pong!\n");//ToDo:Remove
  sem_post(&parent_sem);

}

/**
 * @brief Parent sends the child a message through the pipe
 *
 */
void tell_child() {
  write(pipefd_ctp[1],"Ping!",6);
  printf("Parent sent a message to child!\n");

  //printf("Parent: Ping!\n");//ToDo:Remove
  sem_post(&child_sem);
}

/**
 * @brief Child reads a message out of the paper that was sent by the parent
 */
void wait_parent() {
  char buf[6];

  sem_wait(&parent_sem);
  read(pipefd_ctp[0],buf,6);

  printf("Parent received %s from the child!\n", buf);
  //printf("Child received '%s' from the parent!\n", buf);//ToDo:Remove
}


/**
 * @brief Read the message out of the pipe that was sent by the child
 *
 */
void wait_child() {
  char buf[6];

  sem_wait(&child_sem);
  read(pipefd_ptc[0],buf,6);

  printf("Child received %s from the parent!\n", buf);
  //printf("Parent received '%s' from the child!\n", buf);//ToDo:Remove
}

int main(int argc, char* argv[]) {

  sem_init(&parent_sem, 0, 1);
  sem_init(&child_sem, 0, 1);

  create_pipes();

  //create child
  pid_t pid;
  pid = fork();
  if(pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }

  // Set a timer, the process will end after 10 seconds - sends an SIGALRM after the passage of this time.
  // The default disposition for SIGALRM is TERM(ination) ==> alarm kills our parent process
  // The OS will reap our zombie child (yeah admittdedly no clean solution)
  alarm(10);
  if (pid == 0) {
    // Parent process
    // TODO: You might need pipe handling here
    while (1) {
      sleep(1);
      tell_child();
      wait_child();
      //puts("");//ToDO:Remove
    }
  }
  else {
    // Child process
    // TODO: You might need pipe handling here
    while (1) {
      sleep(1);
      wait_parent();
      tell_parent();
      //puts("");//ToDo:Remove
    }
  }

  sem_destroy(&parent_sem);
  sem_destroy(&child_sem);
  return 0;
}
