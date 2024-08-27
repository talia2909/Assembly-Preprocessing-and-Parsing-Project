#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "shared_functions.h"
#include "pre_assembler_functions.h"
#include "part2read.h"

#define MAX_LINE 80
#define MAX_MACRO_LINES 80

int main(int argc, char *argv[]) {
    FILE *ifp; /* File pointer for input file */
    macro_table *macro_head; /* Pointer to the head of the macro table */
    char *input_file, **pre_arr; /* Input file name and array for pre-processed lines */
    char *full_file_name, *endf; /* Full file name and file extension */
    int i, g, j, file_len; /* Loop counters and file length */
    file_len = 0; /* Initialize file length to 0 */

    /* Check if the user provided at least one input file */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file_name1> <input_file_name2> ...\n", argv[0]);
        return 1;
    }

    /* Loop through each input file provided in the command line arguments */
    for (i = 1; i < argc; i++) {
        input_file = argv[i];
        input_file[strcspn(input_file, "\n")] = '\0';  /* Remove newline character from input file name */

        endf = ".as"; /* Set the file extension for the input file */
        full_file_name = build_new_file(input_file, endf); /* Build the full file name with the extension */
        if (full_file_name == NULL) {
            printf("Memory allocation error for full_file_name\n");
            continue;
        }
        /* Check if the file can be opened for reading */
        if ((strcmp(full_file_name, "0") == 0) || ((ifp = fopen(full_file_name, "r")) == NULL)) {
            printf("Cannot open file %s, exiting.\n", full_file_name);
            free(full_file_name); /* Free the allocated memory for the file name */
            continue;
        } else {
            /* Allocate memory for pre-processed lines array */
            pre_arr = (char**)malloc(DATA_ARR_SIZE * sizeof(char*));
            if (pre_arr == NULL) {
                printf("Memory allocation error for data_arr\n");
                return 1;
            }

            /* Initialize each line in the pre-processed lines array */
            for (g = 0; g < DATA_ARR_SIZE; g++) {
                pre_arr[g] = (char*)malloc((MAX_LINE + 1) * sizeof(char));
                if (pre_arr[g] == NULL) {
                    printf("Memory allocation error for data_arr[%d]\n", g);
                    for (j = 0; j < g; j++) {
                        free(pre_arr[j]); /* Free the previously allocated lines */
                    }
                    free(pre_arr); /* Free the pre-processed lines array */
                    return 1;
                }
                memset(pre_arr[g], '0', MAX_LINE); /* Initialize the line with '0's */
                pre_arr[g][MAX_LINE] = '\0'; /* Null-terminate the line */
            }
            printf("\n");
            printf("file number : %d  - named %s\n", i, full_file_name);

            /* Process the file with pre-assembler and then read the second part */
            macro_head = pre_assembler(ifp, pre_arr, &file_len);
            part2read(input_file, pre_arr, &file_len, &macro_head);

        }
        if (ifp != NULL)
            fclose(ifp); /* Close the input file */
        free(full_file_name); /* Free the allocated memory for the file name */
        file_len = 0; /* Reset file length for the next file */

        /* Free the pre-processed lines array */
        if (pre_arr != NULL) {
            for (g = 0; g < DATA_ARR_SIZE; g++) {
                if (pre_arr[g] != NULL) {
                    free(pre_arr[g]); /* Free each line in the array */
                    pre_arr[g] = NULL; /* Set the pointer to NULL */
                }
            }
        }
        free(pre_arr); /* Free the pre-processed lines array itself */
    }

    return 0; /* Return 0 to indicate successful execution */
}

