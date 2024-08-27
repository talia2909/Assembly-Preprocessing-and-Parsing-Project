
#ifndef PRE_ASSEMBLER_FUNCTIONS_H
#define PRE_ASSEMBLER_FUNCTIONS_H

#define MAX_MACRO_LINES 80



int isNotRegister(const char *str);
int isNotOperation(const char *str);
macro_table* create_macro_node(macro_table* prev, char* str);
macro_table * pre_assembler(FILE *ifp,char **pre_arr,int *file_len);
char* cut_start_whitespace(char *str);

#endif /* PRE_ASSEMBLER_FUNCTIONS_H */


