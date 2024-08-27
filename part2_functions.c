
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "part2_functions.h"
#include "shared_functions.h"
#include <ctype.h>
#include <math.h>


/*
 * Overview of Important Functions and Key Variables
 *
 * Functions:
 *
 * 1. get_command_id:
 *    - This function identifies the command type from the assembly code and processes it accordingly.
 *    - It handles instructions with both source and destination operands.
 *    -Found and prints error.
 *    - Key variables:
 *      - `IC`: Instruction counter.
 *      - `code_arr`: Array that holds the binary representation of instructions.
 *      - `len_op`: Length of the operation (number of words).
 *      - `s_label` and `d_label`: Source and destination labels for operands.
 *
 * 2. two_operand:
 *    - This function handles operations with two operands, converting the operands to their binary form.
 *    - Key variables:
 *      - `source` and `dest`: Source and destination operands.
 *      - `sys_n_s` and `sys_n_d`: System numbers for source and destination operands.
 *      - `IC`: Instruction counter.
 *      - `code_arr`: Array that holds the binary representation of instructions.
 *
 * 3. one_operand:
 *    - This function handles operations with one operand, converting the operand to its binary form.
 *    - Key variables:
 *      - `dest`: Destination operand.
 *      - `sys_n_d`: System number for the destination operand.
 *      - `IC`: Instruction counter.
 *      - `code_arr`: Array that holds the binary representation of instructions.
 *
 * 4. get_sentence_id:
 *    - This function processes data and string directives, converting them to their binary representations.
 *    - Key variables:
 *      - `DC`: Data counter.
 *      - `data_arr`: Array that holds the binary representation of data or strings.
 *      - `test_d`: Test flag to indicate errors in data processing.
 *
 * 5. sysnumber:
 *    - This function identifies the system type (immediate, direct, etc.) of an operand.
 *    - Returns an integer that represents the type of the operand (e.g., 0 for immediate).
 *
 * 6. bin_to_octal:
 *    - Converts a binary string to its octal equivalent.
 *
 * Key Variables:
 * - `DATA_ARR_SIZE`: Defines the size of the data array.
 * - `code_arr`: An array used to store binary representations of assembly instructions.
 * - `data_arr`: An array used to store binary representations of data or strings.
 * - `IC`: Instruction Counter, used to track the current position in the instruction array.
 * - `DC`: Data Counter, used to track the current position in the data array.
 */
#define DATA_ARR_SIZE 4096
char** get_command_id(char* first_field, char* sentence, int* IC, char** code_arr, int* len_op, char** s_label, char** d_label,int g) {
    char *source,*dest,*tmp;
    int  sys_n_s,sys_n_d;
    sys_n_s=-1;
    sys_n_d=-1;
    tmp = (char*)malloc(sizeof(char) * 16); /*opcode  */
    if (tmp == NULL) {
        printf("Memory allocation error for bin- in order %s\n",first_field);
        exit(1);
    }
    memset(tmp, '0', 15);
    tmp[15] = '\0';
    tmp[12]='1';

    if (strcmp(first_field, "mov") == 0) {
        char *bin_source,*bin_dest;
        source = strtok(sentence, ",");
        if(source==NULL){
            printf("line %d -mov order with no source",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        bin_source = (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_source == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_source, '0', 15);
        bin_source[15] = '\0';

        dest = strtok(NULL, "\0");
        if(dest==NULL){
            printf("line %d - mov order with no destination",g);
            *len_op=-1;
            free(bin_source);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(bin_source);
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        sys_n_s=sysnumber(source);
        sys_n_d=sysnumber(dest);

        if((sys_n_s==0 || sys_n_s==1|| sys_n_s==2 || sys_n_s==3) && ( sys_n_d==1|| sys_n_d==2 || sys_n_d==3)){
            code_arr=two_operand(tmp,source,dest, bin_source,bin_dest,sys_n_s,sys_n_d,IC,code_arr);
            *len_op=3;
            if(sys_n_s==1)
                *s_label=source;
            else if(sys_n_s!=1)
                *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -mov there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_source);
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "cmp") == 0) {
        char *bin_source,*bin_dest;
        source = strtok(sentence, ",");
        if(source==NULL){
            printf("line %d - cmp order with no source\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        bin_source = (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_source == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_source, '0', 15);
        bin_source[15] = '\0';

        dest = strtok(NULL, "\0");
        if(dest==NULL){
            printf("line %d - cmp order with no destination\n",g);
            *len_op=-1;
            free(bin_source);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(bin_source);
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[3] = '1';

        sys_n_s=sysnumber(source);
        sys_n_d=sysnumber(dest);

        if((sys_n_s==0 || sys_n_s==1|| sys_n_s==2 || sys_n_s==3) && ( sys_n_d==0||sys_n_d==1|| sys_n_d==2 || sys_n_d==3)){
            code_arr=two_operand(tmp,source,dest, bin_source,bin_dest,sys_n_s,sys_n_d,IC,code_arr);
            *len_op=3;
            if(sys_n_s==1)
                *s_label=source;
            else if(sys_n_s!=1)
                *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else  {
            printf("line %d -cmp- there's an error in the source or destination for this order\n",g);
            *len_op=-1;

        }
        /* the data words : */
        free(bin_source);
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "add") == 0) {
        char *bin_source,*bin_dest;
        source = strtok(sentence, ",");
        if(source==NULL){
            printf("line %d -add order with no source\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        bin_source = (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_source == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_source, '0', 15);
        bin_source[15] = '\0';

        dest = strtok(NULL, "\0");
        if(dest==NULL){
            printf("line %d -add order with no destination\n",g);
            *len_op=-1;
            free(bin_source);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(bin_source);
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[2] = '1';

        sys_n_s=sysnumber(source);
        sys_n_d=sysnumber(dest);

        if((sys_n_s==0 || sys_n_s==1|| sys_n_s==2 || sys_n_s==3) && ( sys_n_d==1|| sys_n_d==2 || sys_n_d==3)){
            code_arr=two_operand(tmp,source,dest, bin_source,bin_dest,sys_n_s,sys_n_d,IC,code_arr);
            *len_op=3;
            if(sys_n_s==1)
                *s_label=source;
            else if(sys_n_s!=1)
                *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -add - there's an error in the source or destination for this order\n",g);
            *len_op=-1; /* invalid line */
        }
        /* the data words : */
        free(bin_source);
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "sub") == 0) {
        char *bin_source,*bin_dest;
        source = strtok(sentence, ",");
        if(source==NULL){
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        bin_source = (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_source == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_source, '0', 15);
        bin_source[15] = '\0';

        dest = strtok(NULL, "\0");
        if(dest==NULL){
            printf("line %d - sub order with no destination\n",g);
            *len_op=-1;
            free(bin_source);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(bin_source);
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[2] = '1';
        tmp[3] = '1';

        sys_n_s=sysnumber(source);
        sys_n_d=sysnumber(dest);

        if((sys_n_s==0 || sys_n_s==1|| sys_n_s==2 || sys_n_s==3) && ( sys_n_d==1|| sys_n_d==2 || sys_n_d==3)){
            code_arr=two_operand(tmp,source,dest, bin_source,bin_dest,sys_n_s,sys_n_d,IC,code_arr);
            if(sys_n_s==1)
                *s_label=source;
            else if(sys_n_s!=1)
                *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else
            printf("line %d -sub- there's an error in the source or destination for this order\n",g);

        /* the data words : */
        free(bin_source);
        free(bin_dest);
        free(tmp);
        *len_op=3;
        return code_arr;

    }
    if (strcmp(first_field, "lea") == 0) {
        char *bin_source,*bin_dest;
        source = strtok(sentence, ",");
        if(source==NULL){
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        bin_source = (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_source == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_source, '0', 15);
        bin_source[15] = '\0';

        dest = strtok(NULL, "\0");
        if(dest==NULL){
            printf("line %d -lea order with no destination\n",g);
            *len_op=-1;
            free(bin_source);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(bin_source);
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[1] = '1';

        sys_n_s=sysnumber(source);
        sys_n_d=sysnumber(dest);

        if((sys_n_s==1) && ( sys_n_d==1|| sys_n_d==2 || sys_n_d==3)){
            two_operand(tmp,source,dest, bin_source,bin_dest,sys_n_s,sys_n_d,IC,code_arr);
            *len_op=3;

            if(sys_n_s==1)
                *s_label=source;
            else if(sys_n_s!=1)
                *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -lea there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_source);
        free(bin_dest);
        free(tmp);
        return code_arr;

    }
    if (strcmp(first_field, "clr") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            *len_op=-1;
            printf("line %d -clr order with no destination\n",g);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[1] = '1';
        tmp[3] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 || sys_n_d==3){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;

        }
        else {
            printf("line %d clr-there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "not") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            *len_op=-1;
            printf("line %d - not order with no destination\n",g);
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[1] = '1';
        tmp[2] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 || sys_n_d==3){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d - not -there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "inc") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            printf("line %d -inc no destination\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[1] = '1';
        tmp[2] = '1';
        tmp[3] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 || sys_n_d==3){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d - inc-there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "dec") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            printf("line %d -dec order with no destination\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[0] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 || sys_n_d==3){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -dec-there's an error in the source or destination for this order\n",g);
            *len_op=-1;

        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "jmp") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            printf("line %d -jmp order with no destination\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[0] = '1';
        tmp[3] = '1';

        sys_n_d=sysnumber(dest);

        if(sys_n_d==1|| sys_n_d==2 ){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -jmp-there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "bne") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            printf("line %d -bne with no destination\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[0] = '1';
        tmp[2] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 ){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -bne-there's an error in the source or destination for this order\n",g);
            *len_op=-1;

        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "red") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            printf("line %d -red order with no destination\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[0] = '1';
        tmp[2] = '1';
        tmp[3] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 || sys_n_d==3){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -red there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "prn") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL){
            printf("line %d prn order with no destination\n",g);
            *len_op=-1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[0] = '1';
        tmp[1] = '1';

        sys_n_d=sysnumber(dest);

        if(sys_n_d==0 || sys_n_d==1|| sys_n_d==2 || sys_n_d==3){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -prn-there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "jsr") == 0) {
        char *bin_dest;
        dest = strtok(sentence, "\0");
        if(dest==NULL) {
            printf("line %d jsr order with no destination\n",g);
            *len_op = -1;
            free(tmp);
            return code_arr;
        }
        while((*dest)==' '){
            dest++;
        }

        bin_dest= (char*)malloc(sizeof(char) * 16); /*opcode  */
        if (bin_dest == NULL) {
            printf("Memory allocation error for bin\n");
            free(tmp);
            exit(1);
        }
        memset(bin_dest, '0', 15);
        bin_dest[15] = '\0';

        tmp[0] = '1';
        tmp[1] = '1';
        tmp[3] = '1';

        sys_n_d=sysnumber(dest);

        if( sys_n_d==1|| sys_n_d==2 ){
            one_operand(tmp,dest,bin_dest,sys_n_d,IC,code_arr);
            *len_op=2;
            *s_label=NULL;
            if(sys_n_d==1)
                *d_label=dest;
            else if(sys_n_d!=1)
                *d_label=NULL;
        }
        else {
            printf("line %d -jsr-there's an error in the source or destination for this order\n",g);
            *len_op=-1;
        }
        /* the data words : */
        free(bin_dest);
        free(tmp);
        return code_arr;
    }
    if (strcmp(first_field, "rts") == 0) {
        tmp[0] = '1';
        tmp[1] = '1';
        tmp[2] = '1';
        strcpy(code_arr[*IC], tmp); /* העתקת המחרוזת הבינארית למערך */
        (*IC)++;
        free(tmp);
        *len_op=1;
        *d_label=NULL;
        return code_arr;
    }
    if (strcmp(first_field, "stop") == 0) {
        tmp[0] = '1';
        tmp[1] = '1';
        tmp[2] = '1';
        tmp[3] = '1';
        strcpy(code_arr[*IC], tmp); /* העתקת המחרוזת הבינארית למערך */
        (*IC)++;
        free(tmp);
        *len_op=1;
        *d_label=NULL;
        return code_arr;
    }
    free(tmp);
    *len_op=-1;
    return code_arr;
}

char** two_operand(char* tmp, char* source, char* dest, char* bin_source, char* bin_dest, int sys_n_s, int sys_n_d, int* IC, char **code_arr) {
    int num, flag = 0; /* flag=1 if source is system 2 or 3 */
    if (sys_n_s == 0) {
        source++;
        num = atoi(source);
        tmp[7] = '1';
        bin_source = binary(bin_source, num, 11);
        bin_source[14] = '0';
        bin_source[13] = '0';
        bin_source[12] = '1';
        if (sys_n_d == 0) {
            ++dest;
            num = atoi(dest);
            tmp[11] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        } else if (sys_n_d == 1) {
            num = atoi(dest);
            tmp[10] = '1';
        } else if (sys_n_d == 2) {
            dest = dest + 2;
            num = atoi(dest);
            tmp[9] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        } else if (sys_n_d == 3) {
            ++dest;
            num = atoi(dest);
            tmp[8] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        }

    } else if (sys_n_s == 1) {
        num = atoi(source);
        tmp[6] = '1';
        bin_source = binary(bin_source, num, 11);
        bin_source[14] = '0';
        bin_source[13] = '0';
        bin_source[12] = '0';
        if (sys_n_d == 0) {
            ++dest;
            num = atoi(dest);
            tmp[11] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        } else if (sys_n_d == 1) {
            num = atoi(dest);
            tmp[10] = '1';
        } else if (sys_n_d == 2) {
            dest = dest + 2;
            num = atoi(dest);
            tmp[9] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        } else if (sys_n_d == 3) {
            ++dest;
            num = atoi(dest);
            tmp[8] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        }

    } else if (sys_n_s == 2) {
        source = source + 2;
        num = atoi(source);
        tmp[5] = '1';
        bin_source = binary(bin_source, num, 8);
        bin_source[14] = '0';
        bin_source[13] = '0';
        bin_source[12] = '1';
        if (sys_n_d == 0) {
            ++dest;
            num = atoi(dest);
            tmp[11] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        } else if (sys_n_d == 1) {
            num = atoi(dest);
            tmp[10] = '1';
        } else if (sys_n_d == 2) {
            flag = 1;
            dest = dest + 2;
            num = atoi(dest);
            tmp[9] = '1';
            if (num == 1)
                bin_source[11] = '1';
            else if (num == 2)
                bin_source[10] = '1';
            else if (num == 3) {
                bin_source[11] = '1';
                bin_source[10] = '1';
            } else if (num == 4)
                bin_source[9] = '1';
            else if (num == 5){
                bin_source[9] = '1';
                bin_source[11] = '1';}
            else if (num == 6){
                bin_source[9] = '1';
                bin_source[10] = '1';}
            else if (num == 7){
                bin_source[9] = '1';
                bin_source[10] = '1';
                bin_source[11] = '1';}


        } else if (sys_n_d == 3) {
            flag = 1;
            ++dest;
            num = atoi(dest);
            tmp[8] = '1';
            if (num == 1)
                bin_source[11] = '1';
            else if (num == 2)
                bin_source[10] = '1';
            else if (num == 3) {
                bin_source[11] = '1';
                bin_source[10] = '1';
            } else if (num == 4)
                bin_source[9] = '1';
            else if (num == 5){
                bin_source[9] = '1';
                bin_source[11] = '1';}
            else if (num == 6){
                bin_source[9] = '1';
                bin_source[10] = '1';}
            else if (num == 7){
                bin_source[9] = '1';
                bin_source[10] = '1';
                bin_source[11] = '1';}

        }
    } else if (sys_n_s == 3) {
        source = source + 1;
        num = atoi(source);
        tmp[4] = '1';
        bin_source = binary(bin_source, num, 8);
        bin_source[14] = '0';
        bin_source[13] = '0';
        bin_source[12] = '1';
        if (sys_n_d == 0) {
            ++dest;
            num = atoi(dest);
            tmp[11] = '1';
            bin_dest = binary(bin_dest, num, 11);
            bin_dest[14] = '0';
            bin_dest[13] = '0';
            bin_dest[12] = '1';
        } else if (sys_n_d == 1) {
            num = atoi(dest);
            tmp[10] = '1';
        } else if (sys_n_d == 2) {
            flag = 1;
            dest = dest + 2;
            num = atoi(dest);
            tmp[9] = '1';
            if (num == 1)
                bin_source[11] = '1';
            else if (num == 2)
                bin_source[10] = '1';
            else if (num == 3) {
                bin_source[11] = '1';
                bin_source[10] = '1';
            } else if (num == 4)
                bin_source[9] = '1';
            else if (num == 5){
                bin_source[9] = '1';
                bin_source[11] = '1';}
            else if (num == 6){
                bin_source[9] = '1';
                bin_source[10] = '1';}
            else if (num == 7){
                bin_source[9] = '1';
                bin_source[10] = '1';
                bin_source[11] = '1';}


        } else if (sys_n_d == 3) {
            flag = 1;
            ++dest;
            num = atoi(dest);
            tmp[8] = '1';
            if (num == 1)
                bin_source[11] = '1';
            else if (num == 2)
                bin_source[10] = '1';
            else if (num == 3) {
                bin_source[11] = '1';
                bin_source[10] = '1';
            } else if (num == 4)
                bin_source[9] = '1';
            else if (num == 5){
                bin_source[9] = '1';
                bin_source[11] = '1';}
            else if (num == 6){
                bin_source[9] = '1';
                bin_source[10] = '1';}
            else if (num == 7){
                bin_source[9] = '1';
                bin_source[10] = '1';
                bin_source[11] = '1';}

        }
    }

    strcpy(code_arr[*IC], tmp); /* העתקת המחרוזת הבינארית למערך */
    (*IC)++;
    if (flag == 0) {
        strcpy(code_arr[*IC], bin_source); /* העתקת המחרוזת הבינארית למערך */
        (*IC)++;
        strcpy(code_arr[*IC], bin_dest); /* העתקת המחרוזת הבינארית למערך */
        (*IC)++;
    } else {
        strcpy(code_arr[*IC], bin_source); /* העתקת המחרוזת הבינארית למערך */
        (*IC)++;
    }
    return code_arr;
}

void one_operand(char* tmp, char* dest, char* bin_dest, int sys_n_d, int* IC, char **code_arr){
    int num; /*  flag=1 if source is system 2 or 3 */

    if(sys_n_d==0){
        /*starts with #  */
        ++dest;
        num= atoi(dest);
        tmp[11]='1';
        bin_dest= binary(bin_dest,num,11);
        bin_dest[14]='0';
        bin_dest[13]='0';
        bin_dest[12]='1';
    }
    else if(sys_n_d==1){
        /* this we would know after going throw all the file first  */
        num= atoi(dest);
        tmp[10]='1';
    }
    else if(sys_n_d==2){
        /*starts with *r  */
        dest=dest+2;/*  the dest operand also direct*/
        num= atoi(dest);
        tmp[9]='1';
        bin_dest= binary(bin_dest,num,11);
        bin_dest[14]='0';
        bin_dest[13]='0';
        bin_dest[12]='1';
    }
    else if(sys_n_d==3){
        /*starts with r  */
        ++dest;
        num= atoi(dest);
        tmp[8]='1';
        bin_dest= binary(bin_dest,num,11);
        bin_dest[14]='0';
        bin_dest[13]='0';
        bin_dest[12]='1';
    }

    strcpy(code_arr[*IC], tmp); /* העתקת המחרוזת הבינארית למערך */
    (*IC)++;
    strcpy(code_arr[*IC], bin_dest); /* העתקת המחרוזת הבינארית למערך */
    (*IC)++;
}


char** get_sentence_id(char* first_field, char* sentence, int* DC, char** data_arr,int* test_d,int g) {
    int val = 0;
    char *num,*bin_val;
    char* bin = (char*)malloc(sizeof(char) * 16);
    if (bin == NULL) {
        printf("Memory allocation error for bin\n");
        exit(1);
    }
    memset(bin, '0', 15);
    bin[15] = '\0';

    if (strcmp(first_field, ".data") == 0) {
        num = strtok(sentence, ",");
        while (num != NULL) {
            num=cut_start_whitespace(num);
            if((num[0]=='-')||(num[0]=='+')) {
                if (is_valid_number(num)) {
                    val = atoi(num);
                    if ((val > 16383) || (val < -16384)) {
                        printf("line %d - data error number sign overflow\n", g);
                        (*test_d) = -1;
                        return data_arr;
                    }
            }
            else{
                printf("line %d - undefiled char for data order\n",g);
                    (*test_d) = -1;
                    return data_arr;
            }
            }
            else if(is_valid_number(num)) {
                val = atoi(num);
                if ((val > 32767) || (val < 0)) {
                    printf("line %d - data error number unsign overflow\n", g);
                    (*test_d) = -1;
                    return data_arr;
                }
            }
            else{
                printf("line %d - undefiled char for data order\n",g);
                (*test_d) = -1;
                return data_arr;
            }
            bin_val = binary(bin, val,14); /* קבלת המחרוזת הבינארית */
            if (*DC < DATA_ARR_SIZE) {
                strcpy(data_arr[*DC], bin_val); /* העתקת המחרוזת הבינארית למערך */
                (*DC)++;
            } else {
                printf("Data array out of bounds\n");
                free(bin);
                return data_arr;
            }
            num = strtok(NULL, ",");
        }
        free(bin);
        return data_arr;
    }

    if (strcmp(first_field, ".string") == 0) {
        int i=0,len;
        char c;
        len= strlen(sentence);
        for(i=0;i<len;i++) {
            c = sentence[i];
            val = (int)c;
            if ((val >= 65 && val <= 90) || (val >= 97 && val <= 122)) {
                bin_val = binary(bin, val,14); /* קבלת המחרוזת הבינארית */
                if (*DC < DATA_ARR_SIZE) {
                    strcpy(data_arr[*DC], bin_val); /* העתקת המחרוזת הבינארית למערך */
                    (*DC)++;
                } else {
                    printf("Data array out of bounds\n");
                    free(bin);
                    return data_arr;
                }}}

        if (i==len){
            bin_val = binary(bin, 0,14); /* קבלת המחרוזת הבינארית */
            strcpy(data_arr[*DC], bin_val); /* העתקת המחרוזת הבינארית למערך */
            (*DC)++;
        }
        free(bin);
        return data_arr;
    }

    free(bin);
    return NULL;

}
int is_valid_number(const char *str) {
    if (*str == '-' || *str == '+') {
        str++;
    }
    if (!*str) {
        return 0;
    }

    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}


int sysnumber(char* operand) {
    int len=0;
    int num,i=0;
    len=strlen(operand);
    if(len>31)
        return -1;
    if(operand[0]=='#'){
        operand++;
        if ((((operand[0]=='-')||(operand[0]=='+'))&&(isdigit(operand[1])))){
       operand++;
       if(is_valid_number(operand)){
        num = atoi(operand);
        if((num>2047)||(num<(-2048)))
           return -1;
        }}
        if(is_valid_number(operand)){
            num = atoi(operand);
            if((num>4095)||(num<0))
                return -1;
            }

        return 0;}
    else if((operand[0]=='*')&& (operand[1]=='r')&&((operand[2]>='0') || (operand[2]<='7'))&&(len==3))
        return 2;
    else if((operand[0]=='r') && ((operand[1]>='0') || (operand[1]<='7'))&&(len==2))
        return 3;
    else if(isalpha(operand[0])){
        while(operand[i]){
            if (!isalnum((unsigned char)operand[i])) {
                return -1;
            }
            i++;
        }
        return 1;
    }
    else
        return -1;
}
int bin_to_octal(char* bin){
    int i,decimal = 0;
    int length = strlen(bin);

    for ( i = 0; i < length; i++) {
        if (bin[i] == '1') {
            decimal += pow(2, length - 1 - i);
        }
    }

    return decimal;
}

