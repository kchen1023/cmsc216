#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"

/*initializes doc with name and 0 paragraphs*/
int init_document(Document *doc, const char *name) {
    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);

    return SUCCESS;
}

/*set doc's num paraghaph to 0*/
int reset_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;

    return SUCCESS;
}

/*prints document name, num paragraphs, and paragraphs (that are separated by
a blank line)*/
int print_document(Document *doc) {
    int paragraph = 0, line = 0;

    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
    for (paragraph = 0; paragraph < doc->number_of_paragraphs; paragraph++) {
        if (doc->paragraphs[paragraph].number_of_lines != 0) {
            /*don't print anything if paragraph is empty*/
            for (line = 0; line < doc->paragraphs[paragraph].number_of_lines; line++) {
                printf("%s\n", doc->paragraphs[paragraph].lines[line]);
            }
            /*don't print new line if it's the last paragraph*/
            if (paragraph != doc->number_of_paragraphs - 1) {
                printf("\n");
            }
        }  
    }
    return SUCCESS;
}

/*add paragraph after paragraph number (aka setting paragraph num lines = 0)
if paragraph num = 0, insert paragraph at the beginning*/
int add_paragraph_after(Document *doc, int paragraph_number) {
    int i = 0;

    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS || 
        paragraph_number > doc->number_of_paragraphs || paragraph_number > MAX_PARAGRAPHS ||
        paragraph_number < 0) {
            return FAILURE;
    }

    /*if empty doc and paragraph_number = 0*/
    if (doc->number_of_paragraphs == 0 && paragraph_number == 0) {
        doc->paragraphs[0].number_of_lines = 0;
    } else if (doc->number_of_paragraphs != 0 && paragraph_number < doc->number_of_paragraphs) {
        /*adds in b/t if there num paragraph > 0 and paragraph is not added at the end*/
        for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {
            doc->paragraphs[i] = doc->paragraphs[i - 1];
        }
        /*resetting paragraph*/
        doc->paragraphs[i].number_of_lines = 0;
    } else if (doc->number_of_paragraphs != 0 && paragraph_number == doc->number_of_paragraphs) {
        /*add paragraph at end of doc*/
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    } 


    doc->number_of_paragraphs++;

    return SUCCESS;
}

/*adds new line after line number of specified paragraph. if line num = 0, 
place new line at the beginning of paragraph*/
int add_line_after(Document *doc, int paragraph_number, int line_number,  const char *new_line) {
    int i = 0, paragraph = paragraph_number -  1;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        doc->paragraphs[paragraph].number_of_lines == MAX_PARAGRAPH_LINES ||
        line_number > doc->paragraphs[paragraph].number_of_lines ||
        new_line == NULL || strlen(new_line) > MAX_STR_SIZE - 1 ||
        paragraph > MAX_PARAGRAPHS || line_number > MAX_PARAGRAPH_LINES ||
        paragraph_number <= 0 || line_number < 0) {
            return FAILURE;
    }

    /*if paragraph is empty*/
    if (doc->paragraphs[paragraph].number_of_lines == 0) {
        strcpy(doc->paragraphs[paragraph].lines[0], new_line);  
    } else if (doc->paragraphs[paragraph].number_of_lines > 0 &&
            doc->paragraphs[paragraph].number_of_lines != line_number) { 
        /*if paragraph num lines > 0 and not append to end*/
        for (i = doc->paragraphs[paragraph].number_of_lines; i > line_number; i--) {
            strcpy(doc->paragraphs[paragraph].lines[i], doc->paragraphs[paragraph].lines[i - 1]);
        }
        strcpy(doc->paragraphs[paragraph].lines[i], new_line);
    } else if (doc->paragraphs[paragraph].number_of_lines > 0 &&
            doc->paragraphs[paragraph].number_of_lines == line_number) {
        /*append to end*/
        strcpy(doc->paragraphs[paragraph].lines[line_number], new_line);
    }

    doc->paragraphs[paragraph].number_of_lines++;

    return SUCCESS;
} /* if strlen is longer than max*/

/*appends line to specified paragraph*/
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int paragraph = paragraph_number - 1, end_of_paragraph  = 0;
 
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        doc->paragraphs[paragraph].number_of_lines == MAX_PARAGRAPH_LINES ||
        new_line == NULL || strlen(new_line) > MAX_STR_SIZE ||
        paragraph_number > MAX_PARAGRAPHS || paragraph_number <= 0) {
            return FAILURE;
    }

    end_of_paragraph = doc->paragraphs[paragraph].number_of_lines;
    add_line_after(doc, paragraph_number, end_of_paragraph, new_line);
    /*strcpy(doc->paragraphs[paragraph].lines[end_of_paragraph], new_line);
    doc->paragraphs[paragraph].number_of_lines++;*/

    return SUCCESS;
}

/*returns num of lines in out parameter (number_of_lines)*/
int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {
    if (doc == NULL || number_of_lines == NULL || paragraph_number > doc->number_of_paragraphs ||
        paragraph_number > MAX_PARAGRAPHS || paragraph_number <= 0) {
            return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    return SUCCESS;
}

/*removes specified line from paragraph*/
int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i = 0, paragraph = paragraph_number - 1;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        line_number > doc->paragraphs[paragraph].number_of_lines ||
        paragraph_number > MAX_PARAGRAPHS || paragraph_number <= 0 ||
        line_number > MAX_PARAGRAPH_LINES || line_number <= 0) {
        return FAILURE;
    }

    /*shift all the lines down by one*/
    for (i = line_number - 1 ; i < doc->paragraphs[paragraph].number_of_lines - 1; i++) {
        strcpy(doc->paragraphs[paragraph].lines[i], doc->paragraphs[paragraph].lines[i + 1]);   
    }

    /*remove last line*/
    strcpy(doc->paragraphs[paragraph].lines[i], "\0");

    doc->paragraphs[paragraph].number_of_lines--;
    
    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {
    int line = 0;/*, paragraph_number = 1;*/

    if (doc == NULL || data == NULL || data_lines <= 0 || data_lines <= 0) {
        return FAILURE;
    }

    /*creating first paragraph*/
    add_paragraph_after(doc, 0);
    
    while (line < data_lines) {
        /*if line is empty string, add new paragraph to the end*/
        if (strlen(data[line]) == 0) {
            add_paragraph_after(doc, doc->number_of_paragraphs);
            /*add_paragraph_after(doc, paragraph_number);
            paragraph_number++;*/
        } else {
            /*add new line to last paragraph*/
            append_line(doc, doc->number_of_paragraphs, data[line]);
        }
        line++;
    }

    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i = 0, j = 0, para = 0, num_para = doc->number_of_paragraphs, line = 0, 
        nums_line = 0, line_len = 0, beginning_len  = 0 , target_len = strlen(target),
        rep_len = strlen(replacement);    
    char temp[MAX_STR_SIZE + 1] = "", *found = NULL;

    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }
    
    for (para = 0; para < num_para; para++) {
        nums_line = doc->paragraphs[para].number_of_lines;
        for (line = 0; line < nums_line; line++) {
            if (strstr(doc->paragraphs[para].lines[line], target)) {
                found = strstr(doc->paragraphs[para].lines[line], target);
                beginning_len = strlen(doc->paragraphs[para].lines[line]) - strlen(found);
                /*adds beginning*/
                for (i = 0; i < beginning_len; i++) {
                    temp[i] = doc->paragraphs[para].lines[line][i];
                }
                j = i;
                /*adds replacement*/
                line_len = strlen(doc->paragraphs[para].lines[line]);
                for (i = 0; i < rep_len; i++) {
                    temp[j] = replacement[i];
                    j++;
                }
                /*adds the rest*/
                for (i = beginning_len + target_len; i < line_len + 1; i++) {
                    temp[j] = doc->paragraphs[para].lines[line][i];
                    j++;
                }
                /*clear line*/
                for (i = 0; i < MAX_STR_SIZE; i++) {
                    doc->paragraphs[para].lines[line][i] = '\0';
                }
                strcpy(doc->paragraphs[para].lines[line], temp);
            }
            for (i = 0; i < MAX_STR_SIZE; i++) {
                temp[i] = '\0';
            }
        }
    }

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    char highlighted[MAX_STR_SIZE] = "";

    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    strcat(highlighted, HIGHLIGHT_START_STR);
    strcat(highlighted, target);
    strcat(highlighted, HIGHLIGHT_END_STR);

    replace_text(doc, target, highlighted);

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {
    int i = 0, j = 0, para = 0, num_para = doc->number_of_paragraphs, line = 0,
        nums_line = 0, line_len = 0, beginning_len = 0, target_len = strlen(target);    
    char temp[MAX_STR_SIZE + 1] = "", *found = NULL;

    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    
    for (para = 0; para < num_para; para++) {
        nums_line = doc->paragraphs[para].number_of_lines;
        for (line = 0; line < nums_line;) {
            if (strstr(doc->paragraphs[para].lines[line], target)) {
                found = strstr(doc->paragraphs[para].lines[line], target);
                beginning_len = strlen(doc->paragraphs[para].lines[line]) - strlen(found);
                /*adds beginning*/
                for (i = 0; i < beginning_len; i++) {
                    temp[i] = doc->paragraphs[para].lines[line][i];
                }
                j = i;
                /*adds everything after target*/
                line_len = strlen(doc->paragraphs[para].lines[line]);
                for (i = beginning_len + target_len; i < line_len; i++) {
                    temp[j] = doc->paragraphs[para].lines[line][i];
                    j++;
                }
                strcpy(doc->paragraphs[para].lines[line], temp);
            } else {
                line++;
            }
            for (i = 0; i < MAX_STR_SIZE; i++) {
                temp[i] = '\0';
            }
        }
    }

    return SUCCESS;
}

int load_file(Document *doc, const char *filename) {
    FILE *input;
    char line[MAX_STR_SIZE + 1] = "";
    int paragraph_num = 1, line_len = 0;

    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }

    /*create first paragraoh*/
    add_paragraph_after(doc, 0);
    /*reading filename passed into parameter*/
    input = fopen(filename, "r");
    /*checks if file opened successfully*/
    if (input == NULL) {
        return FAILURE;
    }

    while (fgets(line, MAX_STR_SIZE, input)) {
        line_len = strlen(line);
        /*deletes \n from line*/
        line[line_len - 1] = '\0';
        /*function only adds paragraph if max limit has not been reached*/
        if (paragraph_num <= MAX_PARAGRAPHS) {
            /*if line is empty, add new paragraph*/
            if (isspace(line[0]) || line[0] == '\0') {
            /*if (strcmp(line, "\n") == 0 || isspace(line[0]) || line[0] == '\0') {*/
                add_paragraph_after(doc, paragraph_num++);
            } else {
                append_line(doc, paragraph_num, line);
            }
        } else {
            /*return failure when number of paragraphs read exceeds max limit*/
            return FAILURE;
        }
    }

    fclose(input);

    return SUCCESS;
}

int save_document(Document *doc, const char *filename) {
    FILE *output;
    int para = 0, line = 0;

    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }

    output = fopen(filename, "w");
    if (output == NULL) {
        return FAILURE;
    }
    
    for (para = 0; para < doc->number_of_paragraphs; para++) {
        for (line = 0; line < doc->paragraphs[para].number_of_lines; line++) {
            fputs(doc->paragraphs[para].lines[line], output);   
            fputs("\n", output);
        }
        fputs("\n", output);
    }

    fclose(output);

    return SUCCESS;
}
