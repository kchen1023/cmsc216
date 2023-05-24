#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
    Document doc;
    const char *doc_name = "Loading Document ";
    int data_lines = 8;
    char data[20][MAX_STR_SIZE + 1] = {
    "abc cmsc abc cmsc",
    "cmsc131.",
    "followed by cmsc132 (which is also cmsc132).",
    "",
    "The next cmsc course you will take is cmsc216.",
    "This course relies on C.",
    "",
    "in cmsc330"
    };

    init_document(&doc, doc_name);
    load_document(&doc, data, data_lines);
    replace_text(&doc, "cmsc", "COMPUTER");
    print_document(&doc);

    /*Document doc;
    init_document(&doc, "Testing Document");
    add_paragraph_after(&doc, 0);
    append_line(&doc, 1, "first paragraph 1");
    append_line(&doc, 1, "first paragraph 2");
    add_paragraph_after(&doc, 1);
    append_line(&doc, 2, "second paragraph 1");
    append_line(&doc, 2, "second paragraph 2");
    add_paragraph_after(&doc, 2);
    append_line(&doc, 3, "third paragraph 1");
    append_line(&doc, 3, "third paragraph 2");
    add_paragraph_after(&doc, 1);
    append_line(&doc, 2, "new paragraph appended after paragraph 1");
    add_line_after(&doc, 1, 0, "inserted line at beginning");
    add_line_after(&doc, 1, 2, "inserted line at middle");
    add_line_after(&doc, 1, 4, "inserted line at end");
    remove_line(&doc, 1, 1);
    remove_line(&doc, 1, 2);
    remove_line(&doc, 1, 3);
    reset_document(&doc);
    print_document(&doc);*/

    /*add_line_after(&doc, 1, 0, "para 1, line 1");
    add_line_after(&doc, 1, 1, "para 1, line 2");
    add_line_after(&doc, 1, 2, "para 1, line 3");
    add_line_after(&doc, 1, 2, "add after line 2");
    add_line_after(&doc, 1, 0, "add at beginning");
    append_line(&doc, 1, "append line 1");
    append_line(&doc, 1, "append line 2");
    print_document(&doc);
    printf("Paragraph 1 number of lines: %d\n\n", doc.paragraphs[0].number_of_lines);
    printf("After removing \"para 1, line 2\"\n");
    remove_line(&doc, 1, 3);
    print_document(&doc);
    printf("Paragraph 1 number of lines: %d\n\n", doc.paragraphs[0].number_of_lines);
    printf("After removing every line\n");
    remove_line(&doc, 1, 1);
    remove_line(&doc, 1, 1);
    remove_line(&doc, 1, 1);
    remove_line(&doc, 1, 1);
    remove_line(&doc, 1, 1);
    remove_line(&doc, 1, 1);
    print_document(&doc);
    printf("Paragraph 1 number of lines: %d\n\n", doc.paragraphs[0].number_of_lines);
    add_paragraph_after(&doc, 1);
    print_document(&doc);
    printf("Paragraph 2 number of lines: %d\n\n", doc.paragraphs[1].number_of_lines);*/


    return 0;
}
