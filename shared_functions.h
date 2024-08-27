
#ifndef SHARED_FUNCTIONS_H
#define SHARED_FUNCTIONS_H


typedef struct macro_table macro_table;

struct macro_table {
    char *name;
    int flag;
    char **text;
    int text_size;
    struct macro_table *next;
};

char* build_new_file(char *string, char *endf);
char* remove_extra_spaces(char* str);
char* binary(char* bin, int j, int count);
char* cut_start_whitespace(char *str);

#endif /* SHARED_FUNCTIONS_H */

