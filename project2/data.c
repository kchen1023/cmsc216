#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Loading Document";
   int data_lines = 8;
   char data[20][MAX_STR_SIZE + 1] = {
        "The first course you need to take",
	"is cmsc131.  This course will be",
	"followed by cmsc132 (which is also based on Java).",
	"",
	"The next course you will take is cmsc216.",
	"This course relies on C.",
	"",
	"Ruby and Ocaml will be covered in cmsc330"
   };

   /*char data[20][MAX_STR_SIZE + 1] = {
        "The first course you need to cochella cochella cochella",
	"is cmsc131.  This course course will be ttcottcoco",
	"followed by cmsc132 (which is also based on Java).",
	"",
	"The next course you will take is cmsc216.",
	"This course relies on C.",
	"",
	"Ruby and Ocaml will be covered in cmsc330",
   "hiiiiiiiiiiiiiiiiii conilla cones",
   "",
   "stank11",
   "",
   "stank13",
   "stank",
   "stank",
   "stank16",
   "stank17"
   };*/
   
   /*char data[20][MAX_STR_SIZE + 1] = {
      "cochella cochella cochella",
      "ttcottcoco",
      "",
      "cococo",
      "",
      "hi hi hi cochella hi",
      "stank",
      "",
      "stank",
      "",
      "stank",
      "",
      "stank",
      "stank",
      "stank",
      "",
      "stank17",
      "stank",
      "stank",
      "stank20"
   };*/

   init_document(&doc, doc_name);
   load_document(&doc, data, data_lines);
   print_document(&doc);

   /*printf("After remove line: ================================\n");
   remove_line(&doc, 1, 2);
   print_document(&doc);*/

   /*printf("After removing: co ================================\n");
   remove_text(&doc, "co");
   print_document(&doc);*/

   /*printf("After replace; co with ***============================\n");
   replace_text(&doc, "co", "co");
   print_document(&doc);*/

   /*printf("After replace; co with COURSE============================\n");
   replace_text(&doc, "course", "COURSE");
   print_document(&doc);*/

   printf("After highlight: co ====================================\n");
   highlight_text(&doc, "cou");
   print_document(&doc);

   return 0;
}
