
#ifndef PROJECT_14_PART2READ_H
#define PROJECT_14_PART2READ_H

#include "part2_functions.h"

#define MAX_LINE 80
#define MAX_INT 16384
#define MIN_INT -16384
#define DATA_ARR_SIZE 4096
#define MAX_BITS 15

typedef struct swithc swithc;

char* build_new_file(char *string, char *endf);
char* remove_extra_spaces(char* str);
int sysnumber(char* operand);
char** two_operand(char* tmp, char* source, char* dest, char* bin_source, char* bin_dest, int sys_n_s, int sys_n_d, int* IC, char **code_arr);
void one_operand(char* tmp, char* dest, char* bin_dest, int sys_n_d, int* IC, char **code_arr);
char** get_sentence_id(char* first_field, char* sentence, int* DC, char** data_arr,int* test_d,int g);
char** get_command_id(char* first_field, char* sentence, int* IC, char** code_arr, int* len_op, char** s_label, char** d_label,int g);
char* binary(char* bin, int i, int count);
int bin_to_octal(char* bin);
int part2read(char* input_file,char** pre_arr,int *file_len,macro_table **macro_head);

#endif /* PROJECT_14_PART2READ_H */


