#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];
  int fd[2];

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }

  char line[MAXLINE];
  strncpy(line, user_id, MAX_PASSWORD);
  int loop = MAX_PASSWORD - strlen(line);
  for (int i=0; i < loop; i++){
      strcat(line, "\n");
  }
  strncat(line, password, MAX_PASSWORD);
  if (pipe(fd)) {
      perror("pipe");
      return -1;
  }
  switch(fork()) {
      case -1:
	  perror("fork");
	  return -1;
      case 0:
	  close(fd[1]);
	  dup2(fd[0], STDIN_FILENO);
	  close(fd[0]);
	  execl("./validate", "validate", NULL);
      default:
	  close(fd[0]);
	  write(fd[1], line, sizeof(line));
	  close(fd[1]);
	  int status;
	  wait(&status);
	  if (WIFEXITED(status)){
		if (WEXITSTATUS(status) == 0) {
			printf("%s", SUCCESS);
	  	} else if (WEXITSTATUS(status) == 2) {
			printf("%s", INVALID);
	  	} else if (WEXITSTATUS(status) == 3) {
			printf("%s", NO_USER);
	  	} else {
			printf("%s%d", line, WEXITSTATUS(status));
	  	}
	  }
  }
  return 0;
}
