
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shared_functions.h"

/*
 * Function Summaries:
 *
 * 1. build_new_file:
 *    - Concatenates a file name with an extension, allocating memory for the result.
 *
 * 2. remove_extra_spaces:
 *    - Removes extra spaces from a string, ensuring only one space between words.
 *
 * 3. binary:
 *    - Converts an integer to a binary string, handling both positive and negative values.
 */

char* build_new_file(char *string, char *endf) {
    char *full_file_name = (char *)malloc(strlen(string) + strlen(endf) + 1);
    if (full_file_name == NULL) {
        perror("Failed to allocate memory for full_file_name");
        return "0";
    }
    strcpy(full_file_name, string);
    strcat(full_file_name, endf);
    return full_file_name;
}

char* remove_extra_spaces(char* str) {
    int n = strlen(str);
    int i, j = 0;
    int space_found = 0;

    for (i = 0; i < n; i++) {
        if (str[i] != ' ') {
            str[j++] = str[i];
            space_found = 0;
        } else if (str[i] == ' ' && !space_found) {
            str[j++] = str[i];
            space_found = 1;
        }
    }
    str[j] = '\0';
    return str;
}

char* binary(char* bin, int j,int count) {
    int modulo = 0, n = 0, i, k = j, tmp[15] = { 0 },c;
    c=count;
    if(j==0){
        for (i = count; i >= 0; i--) {
            bin[i] = '0' + tmp[i];
        }}
    if (j < 0) {
        k = -j;
    }
    while (k > 0 && c >= 0) {
        modulo = k % 2;
        tmp[c] = modulo;
        bin[c] = '0' + modulo;
        k = k / 2;
        c--;
    }
    if (j >= 0)
        return bin;
    else {
        for (i = count; i >= 0; i--) {
            tmp[i] = !(tmp[i]);
            if (i == count) {
                tmp[i] = (tmp[i] + 1) % 2;
                bin[i] = '0' + tmp[i];
                if (tmp[i] == 0) {
                    n = 1;
                }
            }
            else {
                tmp[i] = (tmp[i] + n) % 2;
                bin[i] = '0' + tmp[i];
                if ((tmp[i] == 0) && (n == 1)) {
                    n = 1;
                }
                else
                    n = 0;
            }
        }
    }
    return bin;
}


