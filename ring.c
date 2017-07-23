#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 1024

int main(int argc, char** argv) {
  // Parse the number of processes.
  if (argc < 2) {
    printf("The number of processes must be passed!\n");
    return 1;
  }
  int nprocs = atoi(argv[1]);

  fprintf(stderr, "This is process %d with parent %d.\n", getpid(), getppid());
  int first_process_pid = getpid();

  // Create pipe for the first process in the ring to receive from the last process.
  int final_fd[2];
  pipe(final_fd);
  dup2(final_fd[0], 0);

  // Write the initial message to buffer.
  char* buffer = malloc(sizeof(char) * BUFFSIZE);
  sprintf(buffer, "Hello World");
  int i;
  for (i = 1; i < nprocs; i++) {
    int fd[2];
    pipe(fd);
    int childpid = fork();
    if (childpid) {
      dup2(fd[1],1);
      // This is the parent process. Send message to child.
      printf("%s\n", buffer);
      fflush(stdout);
      fprintf(stderr, "Process#%d sent \"%s\" to process#%d.\n", getpid(), buffer, childpid);
      close(fd[1]);
    } else {
      fprintf(stderr, "This is process %d with parent %d.\n", getpid(), getppid());
      dup2(fd[0],0);
      // This is the child process. Read message from pipe and store in buffer.
      buffer = malloc(sizeof(char) * BUFFSIZE);
      size_t size;
      getline(&buffer, &size, stdin);
      buffer[strlen(buffer) - 1] = '\0'; // Remove newline char from the end.
      fprintf(stderr, "Process#%d received \"%s\".\n", getpid(), buffer);
      close(fd[0]);
    }
    
    if (childpid) break;
  }

  // If this is the first process, wait for the final message.
  if (getpid() == first_process_pid) {
    buffer = malloc(sizeof(char) * BUFFSIZE);
    size_t size;
    getline(&buffer, &size, stdin);
    buffer[strlen(buffer) - 1] = '\0';
    fprintf(stderr, "Process#%d received \"%s\". Message passed around ring successfully.\n", getpid(), buffer);
    close(final_fd[0]);
  }
  
  // If this is the last process in the ring, send message to first process.
  if (i == nprocs) {
    dup2(final_fd[1], 1);
    printf("%s\n", buffer);
    fflush(stdout);
    fprintf(stderr, "Process#%d sent \"%s\" to process#%d.\n", getpid(), buffer, first_process_pid);
    close(final_fd[1]);
  }

  // Wait for child to die.
  int wstatus;
  wait(&wstatus);
  return 0;
}
