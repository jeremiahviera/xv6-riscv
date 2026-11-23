#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"   // full definition of struct rusage
#include "user/user.h"

// time: runs a command and reports its elapsed time, cpu time,
// and the percentage of the CPU it used, all in clock ticks.
// usage: time command [args...]
int
main(int argc, char *argv[])
{
  // a command to time is required
  if (argc < 2) {
    printf("usage: time command [args...]\n");
    exit(1);
  }

  // record the time before the command starts
  int start = uptime();

  int pid = fork();
  if (pid < 0) {
    printf("time: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // child: replace this process with the requested command.
    // argv + 1 is the command's own argv list (already 0-terminated).
    exec(argv[1], argv + 1);

    // exec only returns if it failed
    printf("time: exec %s failed\n", argv[1]);
    exit(1);
  }

  // parent: wait for the child, collecting its status and cpu time
  int status;
  struct rusage ru;
  wait2(&status, &ru);

  // record the time after the command finishes
  int end = uptime();
  int elapsed = end - start;

  // percent of CPU the child used while it existed.
  // multiply before dividing to avoid integer truncation,
  // and guard against dividing by zero for very short commands.
  int cpu_percent = 0;
  if (elapsed > 0)
    cpu_percent = ru.cputime * 100 / elapsed;

  printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",
         elapsed, ru.cputime, cpu_percent);
  exit(0);
}
