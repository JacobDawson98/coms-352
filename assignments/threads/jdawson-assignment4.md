1. List reasons why a mode switch between threads may be cheaper
   than a mode switch between processes. __List two disadvantages
   of ULTs compared to KLTs.__
   * A Mode switch between threads may be cheaper than a mode switch between processes because
     the process does not have to switch between user mode to kernel mode, then back to user mode.
     Also, scheduling can be application specific. Allowing a tailored scheduling algorithm to be
     used.
   * Disadvantages of ULTs compared to KLTs
   1. When a ULT executes a system call, the thread is blocked and the threads within that process
      are blocked.
   2. A multithreaded application can not take advanatage of multiprocessing.


2. Consider an environment in which there is a one-to-one mapping between
   user-level threads and kernel-level threads that allows one or more threads within
   a process to issue blocking system calls while other threads continue to run.
   Explain why this model can make multithreaded programs run faster than their
   single-threaded counterparts on a uniprocessor computer
   * 1:1 mapping between user-level threads and kernel-level threads can make multithreaded
     programs run faster than single-threaded programs because the programmer of the application
     can adjust the number of KLTs for a particular application and processor for optimum speed.

3. If a process exits and there are still threads of that process running,
   will they continue to run?
   * The threads will not continue to run. The process is the execution environment, without it,
     the threads can not continue to execute.

4. Consider the following code segment:
  ```c
  pid_t pid;
  pid = fork();
  if (pid == 0) {
    /* child process */
    fork();
    thread create(...);
  }
  fork();
  ```
  a. How many unique processes are created?
  * 6
  b. How many unique threads are created?
  * 2

5. Convert the following program to use threads.

   The following restrictions apply:
   1) One thread will print "hello ", one thread will print "world" and another
   thread will print the exclamation mark “!”, and the main function will print
   the trailing `"\n"`, using just `pthread_create()`, `pthread_exit()`,
   `pthread_yield()`, and `pthread_join()`.

   Hints & Tips:
   1) You must use a synchronization method to ensure that the "world" thread runs after the
      "hello" thread.
   2) You must use a synchronization method to assure that the main thread does not
      execute until after the "world" and “exclamation” threads.

```c
/* Include Files*/
# include <stdio.h>
# include <pthread.h>

/* External References*/
extern int world(void);
extern int hello(void);
extern int exclamation(void);

int main(int argc, char *argv[]) {
  world();
  hello();
  exclamation();
  printf("\n");

  return(0);
}

/* world - print the "world" part.*/
int world(void) {
  printf( "world" );

  return 0 ;
}

/* hello -print the "hello" part.*/
int hello(void) {
  printf("hello");

  return 0 ;
}

/* exclamation –print “!”.*/
int exclamation() {
  printf("!");

  return 0;
}
```
