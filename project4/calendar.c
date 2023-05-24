#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calendar.h"
#include "event.h"

/*initalizes calendar, malloc for calendar, name, and event*/
int init_calendar(const char *name, int days, int (*comp_func) (const void *ptr1,
    const void *ptr2), void (*free_info_func) (void *ptr), Calendar ** calendar) {
    /*return fail if calendar or name are null, if days < 1, or if malloc fails*/
    if (calendar && name && days >= 1) {
        /*calendar struct*/
        (*calendar) = malloc(sizeof(Calendar));

        if (!calendar) {
            return FAILURE;
        } else{
            /*name*/
            (*calendar)->name = malloc(strlen(name) + 1);
            strcpy((*calendar)->name, name);
            /*events*/
            (*calendar)->events = calloc(days, sizeof(Event));
            /*check malloc*/
            if (!(*calendar)->name || !(*calendar)->events) {
                return FAILURE;
            }
            /*total num days set to 0*/
            (*calendar)->total_events = 0;
            /*others*/
            (*calendar)->comp_func = comp_func;
            (*calendar)->free_info_func = free_info_func;
            (*calendar)->days = days;
        }
        return SUCCESS;
    }
    
    return FAILURE;
}

/*prints to output stream the calendar's name, days, and total num events if print_all is true*/
/*information abt each event (name, start time, duration) is printed regardless of print_all*/
/*returns fail if calendar or output stream are NULL*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int day = 0;
    Event *curr = NULL;

    /*if calendar or output_stream are null*/
    if (!calendar || !output_stream) {
        return FAILURE;
    } else {
        if (print_all) {
            fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
            fprintf(output_stream, "Days: %d\n", calendar->days);
            fprintf(output_stream, "Total Events: %d\n\n", calendar->total_events);
        }
    }

    /*"**** Events ****" is always printed*/
    printf("**** Events ****\n");
    /*if not empty, print events of each day*/
    if (calendar->total_events > 0) {
        for (day = 0; day < calendar->days; day++) {
            printf("Day %d\n", day + 1);
            /*curr pointer iterates thru list. -1 bc arr idx starts at 0*/
            curr = calendar->events[day];
            /*if event on that day exists*/
            while (curr) {
                printf("Event's Name: \"%s\"", curr->name);
                printf(", Start_time: %d", curr->start_time);
                printf(", Duration: %d\n", curr->duration_minutes);
                /*curr moves to next event*/
                curr = curr->next;
            }
        }
    }

    return SUCCESS;
}

/*adds specified event to list associated with day param, added in increasing sorting order
using calendar's comp_func*/
/*malloc for new event and event name, others are initialized normally*/
/*return fail if calendar or name are NULL, start time is not bt 0-2400,
ducation_minutes <= 0, day < 1 or > calendar num days, if even alr exist, malloc fails*/
int add_event(Calendar *calendar, const char *name, int start_time,
    int duration_minutes, void *info, int day) {
    Event *current = NULL, *prev = NULL, *new_event = NULL, *alr_exist = NULL;

    if (!calendar || !name || start_time < 0 || start_time > 2400 ||
        duration_minutes <= 0 || day < 1 || day > calendar->days ||
        find_event(calendar, name, &alr_exist) == SUCCESS) {
        return FAILURE;
    } else {
        new_event = malloc(sizeof(Event));
        if (!new_event) {
            return FAILURE;
        }
        new_event->name = malloc(strlen(name) + 1);
        if (!new_event->name) {
            return FAILURE;
        }

        strcpy(new_event->name, name);
        new_event->duration_minutes = duration_minutes;
        new_event->info = info;
        new_event->start_time = start_time;

        /*add events to this day - 1*/
        /*curr = head*/
        current = calendar->events[day - 1];

        /*if empty, sets curr to new event, new_event->next = NULL*/
        if (!current) {
            /*head = new_item*/
            calendar->events[day - 1] = new_event;
            new_event->next = NULL;
        } else {
            while (current && calendar->comp_func(new_event, current) > 0) {
                prev = current;
                current = current->next;
            }
            new_event->next = current;

            if (prev == NULL) {
                calendar->events[day - 1] = new_event;
            } else {
                prev->next = new_event;
            }
        }
        calendar->total_events++;
    }
           
    return SUCCESS;
}


/*returns a pointer (using out parameter event) to the event with specified name
if event exists */
/*if event param == NULL, nothing is returned*/
/*return fail if calendar or name are NULL*/
/*return success if event was found, otherwise fail (fail on outside)*/
int find_event(Calendar *calendar, const char *name, Event **event) {
    Event *curr = NULL;
    int day = 0;

    if (calendar && name) {
        /*loop thru every day of calendar*/
        for (day = 0; day <= calendar->days; day++) {
            curr = calendar->events[day];

            while (curr) {
                /*if found*/
                if (strcmp(curr->name, name) == 0 && event) {
                    (*event) = curr;

                    return SUCCESS;
                }    
                curr = curr->next;
            }
        }
    }

    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event) {
    Event *curr = NULL;

    if (calendar && name && day >= 1 && day <= calendar->days) {
        /*loop thru every day of calendar*/
        curr = calendar->events[day - 1];

        while (curr) {
            /*if found*/
            if (strcmp(curr->name, name) == 0 && event) {
                (*event) = curr;

                return SUCCESS;
            }    
            curr = curr->next;
        }
    }

    return FAILURE;
}

/*removes and returns any malloc*/
/*if  info != NULL && free_info_func != NULL, call func on info*/
/*adjust num of events*/
/*return fail if calendar or name is NULL or if event not found*/
int remove_event(Calendar *calendar, const char *name) {
    Event *alr_exist = NULL, *prev = NULL, *curr = NULL;
    int day = 0;

    if (!calendar || !name || find_event(calendar, name, &alr_exist) == FAILURE) {
        return FAILURE;
    }

    for (day = 0; day < calendar->days; day++) {
        curr = calendar->events[day];

        while (curr) {
            /*if found*/
            if (strcmp(curr->name, name) == 0) {
                if (prev == NULL) { 
                    calendar->events[day] = curr->next; /*del first item*/
                } else {
                    prev->next = curr->next;
                }

                if (curr->info != NULL && calendar->free_info_func != NULL) {
                    calendar->free_info_func(curr->info);
                }
                free(curr->name);
                free(curr);
                calendar->total_events--;
                return SUCCESS;
            }    
            prev = curr;
            curr = curr->next;
        }
    }

    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
    Event *curr = NULL;

    if (find_event(calendar, name, &curr) == FAILURE) {
        return NULL;
    } else {
        return curr->info;
    }
}

/*removes all events in calendar and sets them to empty list*/
/*total num events == 0*/
/*if info != NULL, and free_info_func exist, call on info*/
/*return fail if calendar == NULL*/
int clear_calendar(Calendar *calendar) {
    int day = 0;

    if (!calendar) {
        return FAILURE;
    }
    
    for (day = 1; day <= calendar->days; day++) {
        clear_day(calendar, day);
    }

    calendar->total_events = 0;

    return SUCCESS;
}

/*removes all event from day, set list to empty, total event is adjusted*/
/*if info != NULL, and free_info_func exist, call on info*/
/*return fail if calendar == NULL*/
int clear_day(Calendar *calendar, int day) {
    Event *curr = NULL, *prev = NULL;

    if (!calendar || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    curr = calendar->events[day - 1];
    while (curr) {
        prev = curr;
        curr = curr->next;
        if (prev->info != NULL && calendar->free_info_func != NULL) {
            calendar->free_info_func(prev->info);
        }
        free(prev->name);
        free(prev);
        calendar->total_events--;
        calendar->events[day - 1] = NULL;
    }

    return SUCCESS;
}

/*frees memory that was dynamically-allocated for the calendar and individual event*/
int destroy_calendar(Calendar *calendar) {
    if (!calendar) {
        return FAILURE;
    }

    clear_calendar(calendar);
    free(calendar->name);
    free(calendar->events);
    free(calendar);

    return SUCCESS;
}
