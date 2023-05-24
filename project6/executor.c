/* 118035390 */

#include <stdio.h>
#include "command.h"
#include "executor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sysexits.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#define MODE 0666

/*static void print_tree(struct tree *t);*/
int execute_aux(struct tree *t, int fd_in, int fd_out);

int execute(struct tree *t) {
   execute_aux(t, STDIN_FILENO, STDOUT_FILENO);
   /*print_tree(t);*/

   return 0;
}

int execute_aux(struct tree *t, int fd_in, int fd_out) {
   int id, status, pipe_fd[2]; /*[0] read, [1] write*/

   /*cd, exit implemented internally, use fork for rest*/
   if (t->conjunction == NONE) { /* ======================================NONE*/
      if (strcmp(t->argv[0], "exit") == 0) {
            exit(0);
      } else if (strcmp(t->argv[0], "cd") == 0) {
         /* (getenv("HOME")) if given no arguments */
         if (t->argv[1] == NULL) {
            chdir(getenv("HOME"));
         } else {
            /* if chdir fails, use perror to print an error message */
            if (chdir(t->argv[1]) < 0) {
               perror("chdir fail");
               exit(EX_OSERR);
            }
         }
      }else { /* execute line */
         id = fork();

         if (id < 0) {
            perror("fork");
            exit(EX_OSERR);
         }

         if (id) { /* parent */
            wait(&status);
            return status;
         } else { /* child */
            /* redirection */
            if (t->input) {
               if ((fd_in = open(t->input, O_RDONLY, MODE)) < 0) {
                  perror("open input file fail\n");
                  exit(EX_OSERR);
               }
               if (dup2(fd_in, STDIN_FILENO) < 0) {
                  perror("stdin dup2 fail");
                  exit(EX_OSERR);
               }
               close(fd_in);
            }

            if (t->output) {
               if ((fd_out = open(t->output, O_CREAT | O_WRONLY | O_TRUNC, MODE)) < 0) {
                  perror("open output file fail\n");
                  exit(EX_OSERR);
               }
               if (dup2(fd_out, STDOUT_FILENO) < 0) {
                  perror("stdout dup2 fail");
                  exit(EX_OSERR);
               }
               close(fd_out);
            }

            execvp(t->argv[0], t->argv);
            fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
            exit(EX_OSERR);
         }   
      }
   } else if (t->conjunction == AND) { /* =================================AND*/
      /* Runs command1. If command1â€™s exit status is zero, then run command2.
         Otherwise, do not run command 2 and exit with command1â€™s status*/
      /* recursion, read from left to right (c1 then c2) */
      if (execute_aux(t->left, fd_in, fd_out) == 0) {
         execute_aux(t->right, fd_in, fd_out);
      }
   } else if (t->conjunction == PIPE) {
      /* Runs command1 and uses its output as input for command2. 
         In other words, run both commands and pipe command1 into command2*/
      /* When running a pipe, the shell must start all of the processes, but
         not return to print another prompt until all processes have exited*/
      /* print â€œAmbiguous input redirect.â€ or â€œAmbiguous output redirect.â€ if a
         user tries to provide a redirected file and a pipe at the same time.
         If both ambiguous input and output redirect take place, 
         only one message will be printed*/
      if (t->right->input) {
         printf("Ambiguous input redirect.\n");
      } else if (t->left->output || t->right->input) {
         printf("Ambiguous output redirect.\n");
      } else {
         if (pipe(pipe_fd) < 0) { 
            err(EX_OSERR, "PIPE pipe error"); 
         }
         if ((id = fork()) < 0) {
            err(EX_OSERR, "PIPE fork error");
         }
         
         if (id) { /* parent code */
            close(pipe_fd[1]);   /* closing read end */
            if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
               perror("PIPE dup2 fail\n");
               exit(EX_OSERR);
            }
            execute_aux(t->right, pipe_fd[0], fd_out);
            close(pipe_fd[0]);   /* closing write end */
            wait(NULL);  /* reaping */
         } else { /* child code */
            close(pipe_fd[0]);   /* closing write end */
            if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
               err(EX_OSERR, "PIPE dup2 error"); 
            }
            execute_aux(t->left, fd_in, pipe_fd[1]);
            close(pipe_fd[1]);
            exit(0);
         }
      } 
   }else if (t->conjunction == SUBSHELL) {
      /* Runs command1 in a subshell. The program will fork the 
         current process and call execute on the subshell */
      id = fork();

      if (id) {
         wait(&status);
         return status;
      } else {
         execute_aux(t->left, fd_in, fd_out);
         exit(-1);
      }

   }

   return 0;
}

/*static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}*/

