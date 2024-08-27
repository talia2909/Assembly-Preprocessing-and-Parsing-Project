
#ifndef PART2_FUNCTIONS_H
#define PART2_FUNCTIONS_H
#define MAX_LINE 80

int is_valid_number(const char *str);
char** get_command_id(char* first_field, char* sentence, int* IC, char** code_arr, int* len_op, char** s_label, char** d_label,int g);
void one_operand(char* tmp, char* dest, char* bin_dest, int sys_n_d, int* IC, char **code_arr);
char** two_operand(char* tmp, char* source, char* dest, char* bin_source, char* bin_dest, int sys_n_s, int sys_n_d, int* IC, char **code_arr);
char** get_sentence_id(char* first_field, char* sentence, int* DC, char** data_arr,int* test_d,int g);
int sysnumber(char* operand);
int bin_to_octal(char* bin);

struct order {
    char* name;
    char* code;
};

struct data_image {
    char* name;
    char* val[MAX_LINE];
    struct data_image* next;
    int line_num;
    char* location; /* .code or .data */
    char* ext_or_relo;
};

struct signed_table {
    char* name;
    char val[31];
    struct signed_table* next;
    int line_num;
    char* location; /* .code or .data */
    char* ext_or_relo;
};
#endif /* PART2_FUNCTIONS_H */




