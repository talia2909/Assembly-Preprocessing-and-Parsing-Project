#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shared_functions.h"
#include "pre_assembler_functions.h"

#define MAX_LINE 80

/* Function to create a new macro node in the macro table */
macro_table* create_macro_node(macro_table* prev, char* str) {
    macro_table *macro_node = (macro_table *)malloc(sizeof(macro_table)); /* Allocate memory for the new macro node */
    if (macro_node == NULL) {
        perror("Failed to allocate memory for macro_node");
        exit(EXIT_FAILURE);
    }
    if (prev != NULL) {
        prev->next = macro_node; /* Link the new node to the previous one */
    }
    macro_node->flag = 1; /* Initialize flag */
    macro_node->text = (char **)malloc(MAX_MACRO_LINES * sizeof(char *)); /* Allocate memory for the text lines of the macro */
    if (macro_node->text == NULL) {
        perror("Failed to allocate memory for macro_node->text");
        exit(EXIT_FAILURE);
    }
    macro_node->name = (char *)malloc(strlen(str) + 1); /* Allocate memory for the macro name */
    if (macro_node->name == NULL) {
        perror("Failed to allocate memory for macro_node->name");
        exit(EXIT_FAILURE);
    }
    strcpy(macro_node->name, str); /* Copy the macro name */
    macro_node->text_size = 0; /* Initialize text size */
    macro_node->next = NULL; /* Set next node to NULL */
    return macro_node; /* Return the new macro node */
}

/* Function to process the input file and handle macros */
macro_table *pre_assembler(FILE *ifp, char **pre_arr, int* file_len) {
    char line[MAX_LINE], *check_macr, *name;
    char macro_copy[MAX_LINE];
    char if_macr[MAX_LINE];
    macro_table *macro_head = NULL; /* Head of the macro table */
    macro_table *macro_node = NULL; /* Current macro node */
    int j = 0;
    macro_table *current;
    int i;
    int inside_macro = 0; /* Flag to check if inside a macro definition */
    int macro_found;

    printf("Starting reading\n");
    while (fgets(line, MAX_LINE, ifp) != NULL) { /* Read each line from the input file */
        line[strcspn(line, "\n")] = '\0';  /* Remove newline character */
        remove_extra_spaces(line); /* Remove extra spaces from the line */
        if (strlen(line) == 0) { /* Skip empty lines */
            continue;
        }
        if (((line[0] == ' ')&&(line[1]==';'))||(line[0]==';')) { /* Skip comment lines */
            continue;
        }
        strcpy(macro_copy, line); /* Copy the line for processing */
        check_macr = strtok(macro_copy, " "); /* Check the first word in the line */

        if (check_macr != NULL && strlen(check_macr) >= 4) { /* Check if the word is a macro definition */
            strncpy(if_macr, check_macr, 4);
            if_macr[4] = '\0';
        } else {
            if_macr[0] = '\0';
        }

        if (strcmp(if_macr, "macr") == 0) { /* If a macro definition is found */
            name = strtok(NULL, "\n");
            name = cut_start_whitespace(name); /* Remove leading whitespace from the macro name */
            if((strlen(name)<32)&&(isNotOperation(name))&&(isNotRegister(name))){ /* Validate the macro name */
                macro_node = create_macro_node(macro_node, name); /* Create a new macro node */
                if (macro_head == NULL) {
                    macro_head = macro_node; /* Set the head of the macro table */
                }
                j = 0;
                inside_macro = 1; /* Set the flag to indicate inside a macro */
            } else {
                printf("line %d there is an error in definition of macro\n",*file_len);
                continue;
            }
        } else if (inside_macro) { /* If inside a macro definition */
            strcpy(macro_copy, line); /* Copy the line for processing */
            check_macr = strtok(macro_copy, " ");
            if (check_macr != NULL && strlen(check_macr) >= 7) {
                strncpy(if_macr, check_macr, 7);
                if_macr[7] = '\0';
            } else {
                if_macr[0] = '\0';
            }
            if (strcmp(if_macr, "endmacr") == 0) { /* If the end of a macro is found */
                inside_macro = 0; /* Reset the flag */
                macro_node->flag = 0; /* Reset the macro flag */
                macro_node->text_size = j; /* Set the text size */
            } else {
                macro_node->text[j] = (char *)malloc(strlen(line) + 1); /* Allocate memory for the macro line */
                if (macro_node->text[j] == NULL) {
                    perror("Failed to allocate memory for macro_node->text[j]");
                    exit(EXIT_FAILURE);
                }
                strcpy(macro_node->text[j], line); /* Copy the line into the macro node */
                j++;
            }
        } else { /* If not inside a macro, check if the line matches a macro name */
            current = macro_head;
            macro_found = 0;
            while (current != NULL && check_macr != NULL) {
                if (strcmp(current->name, check_macr) == 0) { /* If the line matches a macro name */
                    for (i = 0; i < current->text_size; i++) {
                        strcpy(pre_arr[(*file_len)], current->text[i]); /* Copy the macro's text to the output array */
                        (*file_len)++;
                    }
                    macro_found = 1;
                    break;
                }
                current = current->next;
            }

            if (!macro_found) { /* If the line does not match any macro name */
                strncpy(pre_arr[(*file_len)], line, MAX_LINE); /* Copy the line to the output array */
                (*file_len)++;
            }
        }
    }

    printf("Finished reading input file\n");

    return macro_head; /* Return the head of the macro table */
}

/* Function to check if a string is not a register name */
int isNotRegister(const char *str) {
    int i;
    const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    for (i = 0; i < 8; i++) {
        if (strcmp(str, registers[i]) == 0) {
            return 0; /* Return 0 if the string matches a register name */
        }
    }
    return 1; /* Return 1 if the string is not a register name */
}

/* Function to check if a string is not an operation name */
int isNotOperation(const char *str) {
    int i;
    const char *operations[] = {
            "mov", "cmp", "add", "sub", "lea", "clr", "not",
            "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
            "rts", "stop"
    };
    for (i = 0; i < 16; i++) {
        if (strcmp(str, operations[i]) == 0) {
            return 0; /* Return 0 if the string matches an operation name */
        }
    }
    return 1; /* Return 1 if the string is not an operation name */
}

/* Function to remove leading whitespace from a string */
char* cut_start_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;
    return str; /* Return the modified string */
}

