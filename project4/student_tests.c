#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test checks ...  */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}
/*tests initialization, printing of calendar *WITH NO* events, and destroying*/
static int test2() {
   int days = 31;
   Calendar *calendar;
   
   printf("TEST 2==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*tests basic initialization and printing of calendar *WITH* events and
w/ and w/o print_all, test if repeat*/
static int test3() {
   int days = 10;
   Calendar *calendar;

   printf("TEST 3==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "first event 1", 450, 80, NULL, 1);
      add_event(calendar, "second event 1", 1400, 20, NULL, 1);
      add_event(calendar, "third event 1", 2230, 10, NULL, 1);
      add_event(calendar, "first event 5", 2359, 60, NULL, 5);
      add_event(calendar, "first event 9", 100, 120, NULL, 9);
      if (add_event(calendar, "first event 9", 100, 120, NULL, 9) == FAILURE) {
         printf("ADDED rEPEAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      }

      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*tests finding events*/
static int test4() {
   int days = 5;
   Calendar *calendar;
   Event *event;

   printf("TEST 4==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "first event 1", 450, 80, NULL, 1);
      add_event(calendar, "second event 1", 1400, 20, NULL, 1);
      add_event(calendar, "third event 1", 2230, 10, NULL, 1);
      add_event(calendar, "first event 2", 2359, 60, NULL, 2);
      add_event(calendar, "first event 4", 100, 120, NULL, 4);

      if (find_event(calendar, "first event 2", &event) == SUCCESS) {
         printf("FOUND!!!!!!!!!!!!!!\n");
      } else {
         printf("NOT FOUND!!!!!!!!!!\n");
      }

      if (find_event_in_day(calendar, "second evet 1", 1, &event) == SUCCESS) {
         printf("FOUND!!!!!!!!!!!!!!\n");
      } else {
         printf("NOT FOUND!!!!!!!!!!\n");
      }

      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*tests finding events in a day*/
static int test5() {
   int days = 5;
   Calendar *calendar;
   Event *event;

   printf("TEST 5==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "first event 1", 450, 80, NULL, 1);
      add_event(calendar, "second event 1", 1400, 20, NULL, 1);
      add_event(calendar, "third event 1", 2230, 10, NULL, 1);
      add_event(calendar, "first event 2", 2359, 60, NULL, 2);
      add_event(calendar, "first event 4", 100, 120, NULL, 4);

      if (find_event_in_day(calendar, "second evet 1", 1, &event) == SUCCESS) {
         printf("FOUND!!!!!!!!!!!!!!\n");
      } else {
         printf("NOT FOUND!!!!!!!!!!\n");
      }

      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*tests remove event*/
static int test6() {
   int days = 5;
   Calendar *calendar;

   printf("TEST 6==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "first event 1", 450, 80, NULL, 1);
      add_event(calendar, "second event 1", 1400, 20, NULL, 1);
      add_event(calendar, "meeting", 2230, 10, NULL, 1);
      add_event(calendar, "first event 3", 2359, 60, NULL, 3);
      add_event(calendar, "first event 5", 100, 120, NULL, 5);

      if (remove_event(calendar, "meeting") == SUCCESS) {
         printf("REMOVED!!!!!!!!!!!!!\n");
      } else {
         printf("NOT REMOVED!!!!!!!!!\n");
      }

      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*tests get info ================== ================== copy from pub05*/
typedef struct task_info {
  int num;
  char *str;
} Task_info;

static Task_info *create_task_info(int num, const char *str) {
   Task_info *task_info = malloc(sizeof(Task_info));

   if (task_info) {
      task_info->str = malloc(strlen(str) + 1);
      if (task_info->str) {
         task_info->num = num;
         strcpy(task_info->str, str);
         return task_info;
      }
   }

   return NULL;
}

static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *)ptr;

   free(task_info->str);
   free(task_info);
}

static int test7() {
   Calendar *calendar;
   Task_info *info;

   printf("TEST 7==========================================================\n");
   init_calendar("Winter Calendar", 8, comp_minutes, free_task_info, &calendar);

   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", 800, 300, 
             info, 1);

   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1); 

   print_calendar(calendar, stdout, 1);

   info = get_event_info(calendar, "debugging"); 
    
   if (info != NULL) {
      printf("%d %s\n", info->num, info->str);
      return destroy_calendar(calendar);
   } else {
      return FAILURE;
   }
}

/*tests clear calendar*/
static int test8() {
   int days = 5;
   Calendar *calendar;

   printf("TEST 8==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "first event 1", 450, 80, NULL, 1);
      add_event(calendar, "second event 1", 1400, 20, NULL, 1);
      add_event(calendar, "meeting", 2230, 10, NULL, 1);
      add_event(calendar, "first event 3", 2359, 60, NULL, 3);
      add_event(calendar, "first event 5", 100, 120, NULL, 5);

      clear_calendar(calendar);

      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*tests clear day*/
static int test9() {
   int days = 5;
   Calendar *calendar;

   printf("TEST 9==========================================================\n");
   if (init_calendar("Calendah!", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "first event 1", 450, 80, NULL, 1);
      add_event(calendar, "second event 1", 1400, 20, NULL, 1);
      add_event(calendar, "meeting", 2230, 10, NULL, 1);
      add_event(calendar, "first event 3", 2359, 60, NULL, 3);
      add_event(calendar, "first event 5", 100, 120, NULL, 5);

      clear_day(calendar, 1);

      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   /*start_memory_check();*/
   /***** Starting memory checking *****/
   if (test1() == FAILURE) {
      printf("test 1 fail\n");
      result = FAILURE;
   }
   if (test2() == FAILURE) {
      printf("test 2 fail\n");
      result = FAILURE;
   }
   if (test3() == FAILURE) {
      printf("test 3 fail\n");
      result = FAILURE;
   }
   if (test4() == FAILURE) {
      printf("test 4 fail\n");
      result = FAILURE;
   }
   if (test5() == FAILURE) {
      printf("test 5 fail\n");
      result = FAILURE;
   }
   if (test6() == FAILURE) {
      printf("test 6 fail\n");
      result = FAILURE;
   }
   if (test7() == FAILURE) {
      printf("test 7 fail\n");
      result = FAILURE;
   }
   if (test8() == FAILURE) {
      printf("test 8 fail\n");
      result = FAILURE;
   }
   if (test9() == FAILURE) {
      printf("test 9 fail\n");
      result = FAILURE;
   }

   /****** Gathering memory checking info *****/
   /*stop_memory_check();*/
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      printf("FAILED=======================================================\n");
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
