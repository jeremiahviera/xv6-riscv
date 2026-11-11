#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// time1: runs a command and reports the elapsed (wall-clock) time in ticks
int
main(int argc, char *argv[])
{
 printf("argc = %d\n", argc);
  if (argc == 1){
	printf("Please specify a command to time\n");
	exit(1);
	}

  int start = uptime();

  int pid = fork();
  if (pid < 0) {
    printf("time1: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
	
    // CHILD
    // runs the command and exits if command exec failed
    exec(argv[1],argv + 1);
    printf("time1: exec %s failed\n", argv[1]);
    exit(1);
  }

  // PARENT
  // wait for child
  wait(0);
  int end = uptime();
// calculate time
  printf("elapsed time: %d ticks\n", end - start);
  exit(0);
}
