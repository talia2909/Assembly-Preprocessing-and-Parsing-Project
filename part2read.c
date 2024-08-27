#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "part2_functions.h"
#include "shared_functions.h"
#include "part2read.h"

#define MAX_LINE 80
#define MAX_INT 16384
#define MIN_INT -16384
#define DATA_ARR_SIZE 4096
#define MAX_BITS 15


/* sign table -linked list of the sign table is implement with struct signed_table while it's head is signe,
  the labels list is implement as linked list with struct signed_table while it's head is label ,
  the ent list is also implement as linked list with struct signed_table while it's head is ent,
  the ext list is also implement as linked list with struct signed_table while it's head is ext,
  code_arr is 2D string array that holds all the code sentences as binary machine code- IC is counter of code_arr,
  data_arr is 2D string array that holds all the data sentences as binary machine code - DC is a counter of data_arr,
  macro_table **macro_head - the pointer to the head of the macro table linked list
  test - is an integer that represent if an error was found in a code sentence,
  test_d  is an integer that represent if an error was found in a data sentence,
  error_flag - integer that equal to 1 if an error was found in the source code
 */

int part2read(char *input_file,char **pre_arr,int *file_len, macro_table **macro_head) {
    FILE  *file_connection_ob, *ps_ext, *ps_ent,*ps_am;
    int i = 0, IC = 0, DC = 0, len, test,test_d=0, len_first_field, table_flag = 0, g, decimal;
    int j, error_flag,count;
    char *file_name = NULL, *file_name_org, *line_copy, *rest_of_sentence, *first_field, **code_arr = NULL, **data_arr;
    char *new_full_file_name, *endfile, *s_label, *d_label, *rest_of_sentence_copy, *file_am, *endf;
    struct signed_table *signe, *current, *label, *curr_label;
    struct signed_table *label_iter, *signe_iter, *tmp, *tmp_l, *ent, *ext, *curr_ent, *curr_ext, *ent_iter, *ext_iter;
    struct signed_table *new_ent, *new_ext;
    struct macro_table *curr_macro,*macro_temp;
    ;

    file_name_org = malloc(MAX_LINE * sizeof(char));
    if (file_name_org == NULL) {
        printf("Memory allocation error for file_name_org\n");
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }
    strncpy(file_name_org,input_file,MAX_LINE);
    line_copy = malloc(MAX_LINE * sizeof(char));
    if (line_copy == NULL) {
        printf("Memory allocation error for line_copy\n");
        free(file_name_org);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }
    rest_of_sentence = malloc(MAX_LINE * sizeof(char));
    if (rest_of_sentence == NULL) {
        printf("Memory allocation error for rest_of_sentence\n");
        free(file_name_org);
        free(line_copy);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }

    data_arr = (char **) malloc(DATA_ARR_SIZE * sizeof(char *));
    if (data_arr == NULL) {
        printf("Memory allocation error for data_arr\n");
        free(file_name_org);
        free(line_copy);
        free(rest_of_sentence);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }

    for (g = 0; g < DATA_ARR_SIZE; g++) {
        data_arr[g] = (char *) malloc((MAX_BITS + 1) * sizeof(char));
        if (data_arr[g] == NULL) {
            printf("Memory allocation error for data_arr[%d]\n", g);
            for ( i = 0; i < g; i++) free(data_arr[i]);
            free(data_arr);
            free(file_name_org);
            free(line_copy);
            free(rest_of_sentence);
            curr_macro=(*macro_head);
            while(curr_macro!=NULL){
                macro_temp = curr_macro->next;
                if(curr_macro->name) free(curr_macro->name);
                free(curr_macro);
                curr_macro = macro_temp;
            }
            return 1;
        }
        memset(data_arr[g], '0', MAX_BITS);
        data_arr[g][MAX_BITS] = '\0';
    }
    code_arr = (char**)malloc(DATA_ARR_SIZE * sizeof(char*));
    if (code_arr == NULL) {
        printf("Memory allocation error for data_arr\n");
        if(data_arr!=NULL) {
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                if (data_arr[j] != NULL) {
                    free(data_arr[j]);
                }
            }
            free(data_arr);
        }
        free(file_name);
        free(line_copy);
        free(rest_of_sentence);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }

    for (g = 0; g < DATA_ARR_SIZE; g++) {
        code_arr[g] = (char*)malloc((MAX_BITS + 1) * sizeof(char));
        if (code_arr[g] == NULL) {
            printf("Memory allocation error for data_arr[%d]\n", g);
            for (j = 0; j < g; j++) {
                free(code_arr[j]);
            }
            free(code_arr);
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                free(data_arr[j]);
            }
            free(data_arr);
            free(file_name);
            free(line_copy);
            free(rest_of_sentence);
            curr_macro=(*macro_head);
            while(curr_macro!=NULL){
                macro_temp = curr_macro->next;
                if(curr_macro->name) free(curr_macro->name);
                free(curr_macro);
                curr_macro = macro_temp;
            }
            return 1;
        }
        memset(code_arr[g], '0', MAX_BITS);
        code_arr[g][MAX_BITS] = '\0';
    }

    file_name = malloc(MAX_LINE * sizeof(char));
    signe=NULL;

    label = NULL;
    curr_label = NULL;

    s_label = malloc(32 * sizeof(char)); /* would hold the label that would be mentioned in the source part of the command */
    if (s_label == NULL) {
        printf("Memory allocation error for s_label\n");
        if(data_arr!=NULL) {
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                if (data_arr[j] != NULL) {
                    free(data_arr[j]);
                }
            }
            free(data_arr);
        }
        if (code_arr != NULL) {
            for (j = 0; j < IC; j++) {
                if (code_arr[j] != NULL) {
                    free(code_arr[j]);
                }}
            free(code_arr);
        }
        free(line_copy);
        free(rest_of_sentence);
        free(file_name);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }

    d_label = malloc(32 * sizeof(char)); /* would hold the label that would be mentioned in the destination part of the command */
    if (d_label == NULL) {
        printf("Memory allocation error for d_label\n");
        if(data_arr!=NULL) {
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                if (data_arr[j] != NULL) {
                    free(data_arr[j]);
                }
            }
            free(data_arr);
        }
        if (code_arr != NULL) {
            for (j = 0; j < IC; j++) {
                if (code_arr[j] != NULL) {
                    free(code_arr[j]);
                }}
            free(code_arr);
        }
        free(line_copy);
        free(rest_of_sentence);
        free(s_label);
        free(file_name);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 1;
    }

    ent = NULL;
    curr_ent = NULL;

    ext = NULL;
    curr_ext = NULL;

    error_flag = 0;
    g=0;
    while(g<(*file_len)){
        strncpy(file_name, pre_arr[g], MAX_LINE - 1);
        file_name[MAX_LINE - 1] = '\0';  /* Remove newline character */
        remove_extra_spaces(file_name);
        strcpy(line_copy, file_name);
        first_field = strtok(line_copy, " ");
        rest_of_sentence_copy = strtok(NULL, "");
        if (rest_of_sentence_copy != NULL) {
            strncpy(rest_of_sentence, rest_of_sentence_copy, MAX_LINE - 1);
            rest_of_sentence[MAX_LINE - 1] = '\0';
        } else {
            rest_of_sentence[0] = '\0';
        }
        if (first_field != NULL) {
            len = strlen(first_field);
            if (len > 31) {
                printf("Error: the label is longer than 31 bits\n");
                continue;
            } else if (first_field[0] == '.') {       /* if it's a data sentence  */
                if (strcmp(first_field, ".entry") == 0) {
                    if (sysnumber(rest_of_sentence) != 1) {
                        printf("line %d - error in the value of the  entry label\n",g);
                        error_flag=1;
                        continue;
                    }
                    new_ent = (struct signed_table *) malloc(sizeof(struct signed_table));
                    if (new_ent == NULL) {
                        printf("new entry memory isn't allowed");
                        if(data_arr!=NULL) {
                            for (j = 0; j < DATA_ARR_SIZE; j++) {
                                if (data_arr[j] != NULL) {
                                    free(data_arr[j]);
                                }
                            }
                            free(data_arr);
                        }
                        if (code_arr != NULL) {
                            for (j = 0; j < IC; j++) {
                                if (code_arr[j] != NULL) {
                                    free(code_arr[j]);
                                }}
                            free(code_arr);
                        }
                        free(line_copy);
                        free(rest_of_sentence);
                        free(s_label);
                        free(file_name);
                        curr_macro=(*macro_head);
                        while(curr_macro!=NULL){
                            macro_temp = curr_macro->next;
                            if(curr_macro->name) free(curr_macro->name);
                            free(curr_macro);
                            curr_macro = macro_temp;
                        }
                        return -1;
                    }
                    new_ent->line_num = DC;
                    new_ent->name = "data";
                    new_ent->ext_or_relo = "rel";
                    strncpy(new_ent->val, rest_of_sentence, 31);
                    new_ent->next = NULL;
                    if (ent == NULL) {
                        ent = new_ent;
                        curr_ent = new_ent;
                    } else {
                        curr_ent->next = new_ent;
                        curr_ent = new_ent;
                    }
                } else if (strcmp(first_field, ".extern") == 0) {
                    if (sysnumber(rest_of_sentence) != 1) {
                        printf("line %d - error in the value of the extern label\n",g);
                        error_flag=1;
                        continue; }
                    new_ext = (struct signed_table *) malloc(sizeof(struct signed_table));
                    if (new_ext == NULL) {
                        printf("new extern memory isn't allowed");
                        if(data_arr!=NULL) {
                            for (j = 0; j < DATA_ARR_SIZE; j++) {
                                if (data_arr[j] != NULL) {
                                    free(data_arr[j]);
                                }
                            }
                            free(data_arr);
                        }
                        if (code_arr != NULL) {
                            for (j = 0; j < IC; j++) {
                                if (code_arr[j] != NULL) {
                                    free(code_arr[j]);
                                }}
                            free(code_arr);
                        }
                        free(line_copy);
                        free(rest_of_sentence);
                        free(s_label);
                        free(file_name);
                        curr_macro=(*macro_head);
                        while(curr_macro!=NULL){
                            macro_temp = curr_macro->next;
                            if(curr_macro->name) free(curr_macro->name);
                            free(curr_macro);
                            curr_macro = macro_temp;
                        }
                        return -1;
                    }
                    new_ext->line_num = DC;
                    new_ext->name = "data";
                    new_ext->ext_or_relo = "ext";
                    strncpy(new_ext->val, rest_of_sentence, 31);
                    new_ext->next = NULL;
                    if (ext == NULL) {
                        ext = new_ext;
                        curr_ext = new_ext;
                    } else {
                        curr_ext->next = new_ext;
                        curr_ext = new_ext;
                    }
                } else {
                    data_arr = get_sentence_id(first_field, rest_of_sentence, &DC, data_arr,&test_d,g);
                    if(test_d==-1)
                        error_flag=1;
                }
            } else if ((code_arr = get_command_id(first_field, rest_of_sentence, &IC, code_arr, &test, &s_label, &d_label,g)) && (test != -1)) {
                if (s_label != NULL) {
                    struct signed_table *new_label = (struct signed_table *) malloc(sizeof(struct signed_table));
                    if (new_label == NULL) {
                        printf("new label memory isn't allowed");
                        if(data_arr!=NULL) {
                            for (j = 0; j < DATA_ARR_SIZE; j++) {
                                if (data_arr[j] != NULL) {
                                    free(data_arr[j]);
                                }
                            }
                            free(data_arr);
                        }
                        if (code_arr != NULL) {
                            for (j = 0; j < IC; j++) {
                                if (code_arr[j] != NULL) {
                                    free(code_arr[j]);
                                }}
                            free(code_arr);
                        }
                        free(line_copy);
                        free(rest_of_sentence);
                        free(s_label);
                        free(file_name);
                        curr_macro=(*macro_head);
                        while(curr_macro!=NULL){
                            macro_temp = curr_macro->next;
                            if(curr_macro->name) free(curr_macro->name);
                            free(curr_macro);
                            curr_macro = macro_temp;
                        }
                        return -1;
                    }
                    new_label->line_num = IC - 2;
                    new_label->name = "code";
                    strncpy(new_label->val, s_label, 31);
                    new_label->next = NULL;
                    if (label == NULL) {
                        label = new_label;
                        curr_label = new_label;
                    } else {
                        curr_label->next = new_label;
                        curr_label = new_label;
                    }
                }
                if (d_label != NULL) {
                    struct signed_table *new_label = (struct signed_table *) malloc(sizeof(struct signed_table));
                    if (new_label == NULL) {
                        printf("new label memory isn't allowed");
                        if(data_arr!=NULL) {
                            for (j = 0; j < DATA_ARR_SIZE; j++) {
                                if (data_arr[j] != NULL) {
                                    free(data_arr[j]);
                                }
                            }
                            free(data_arr);
                        }
                        if (code_arr != NULL) {
                            for (j = 0; j < IC; j++) {
                                if (code_arr[j] != NULL) {
                                    free(code_arr[j]);
                                }}
                            free(code_arr);
                        }
                        free(line_copy);
                        free(rest_of_sentence);
                        free(s_label);
                        free(file_name);
                        curr_macro=(*macro_head);
                        while(curr_macro!=NULL){
                            macro_temp = curr_macro->next;
                            if(curr_macro->name) free(curr_macro->name);
                            free(curr_macro);
                            curr_macro = macro_temp;
                        }
                        return -1;
                    }
                    new_label->line_num = IC - 1;
                    new_label->name = "code";
                    strncpy(new_label->val, d_label, 31);
                    new_label->next = NULL;
                    if (label == NULL) {
                        label = new_label;
                        curr_label = new_label;
                    } else {
                        curr_label->next = new_label;
                        curr_label = new_label;
                    }
                }
            } else if (first_field[len - 1] == ':') {   /*a  label declaration  */
                char *second_field, *rest_fields;
                struct signed_table *new_signe;
                second_field = strtok(rest_of_sentence, " ");
                rest_fields = strtok(NULL, "");

                new_signe = (struct signed_table *) malloc(sizeof(struct signed_table));  /* create sign node   */
                if (new_signe == NULL) {
                    printf("Memory allocation error for new_signe\n");
                    if(data_arr!=NULL) {
                        for (j = 0; j < DATA_ARR_SIZE; j++) {
                            if (data_arr[j] != NULL) {
                                free(data_arr[j]);
                            }
                        }
                        free(data_arr);
                    }
                    if (code_arr != NULL) {
                        for (j = 0; j < IC; j++) {
                            if (code_arr[j] != NULL) {
                                free(code_arr[j]);
                            }}
                        free(code_arr);
                    }
                    free(line_copy);
                    free(rest_of_sentence);
                    free(s_label);
                    free(file_name);
                    curr_macro=(*macro_head);
                    while(curr_macro!=NULL){
                        macro_temp = curr_macro->next;
                        if(curr_macro->name) free(curr_macro->name);
                        free(curr_macro);
                        curr_macro = macro_temp;
                    }
                    return 0;
                }
                /*   adding it to the sign table  */
                if (table_flag == 0) {
                    signe = new_signe;
                    current = signe;
                    table_flag = 1;
                } else if (table_flag == 1) {
                    current->next = new_signe;
                    current = new_signe;
                }
                if (second_field[0] == '.') {
                    new_signe->line_num = DC;
                    data_arr = get_sentence_id(second_field, rest_fields, &DC, data_arr,&test_d,g);
                    if(test_d==-1)
                        error_flag=1;
                    new_signe->name = "data";
                    len_first_field = strlen(first_field);
                    first_field[len_first_field - 1] = '\0';
                    strncpy(new_signe->val, first_field, 31);
                    new_signe->next = NULL;

                    /* checking if the label definition is an order */

                } else if ((code_arr = get_command_id(second_field, rest_fields, &IC, code_arr, &test, &s_label, &d_label,g)) && (test != -1)) {
                    new_signe->line_num = (IC - test);
                    new_signe->name = "code";
                    len_first_field = strlen(first_field);
                    first_field[len_first_field - 1] = '\0';
                    strncpy(new_signe->val, first_field, 31);
                    new_signe->next = NULL;
                    /* checking if there is a label that mention as source of an order */

                    if (s_label != NULL) {
                        struct signed_table *new_label = (struct signed_table *) malloc(sizeof(struct signed_table));
                        if (new_label == NULL) {
                            printf("new label memory isn't allowed");
                            if(data_arr!=NULL) {
                                for (j = 0; j < DATA_ARR_SIZE; j++) {
                                    if (data_arr[j] != NULL) {
                                        free(data_arr[j]);
                                    }
                                }
                                free(data_arr);
                            }
                            if (code_arr != NULL) {
                                for (j = 0; j < IC; j++) {
                                    if (code_arr[j] != NULL) {
                                        free(code_arr[j]);
                                    }}
                                free(code_arr);
                            }
                            free(line_copy);
                            free(rest_of_sentence);
                            free(s_label);
                            free(file_name);
                            curr_macro=(*macro_head);
                            while(curr_macro!=NULL){
                                macro_temp = curr_macro->next;
                                if(curr_macro->name) free(curr_macro->name);
                                free(curr_macro);
                                curr_macro = macro_temp;
                            }
                            return -1;
                        }
                        new_label->line_num = IC - 2;
                        new_label->name = "code";
                        strncpy(new_label->val, s_label, 31);
                        new_label->next = NULL;
                        if (label == NULL) {
                            label = new_label;
                            curr_label = new_label;
                        } else {
                            curr_label->next = new_label;
                            curr_label = new_label;
                        }
                    }
                    /* checking if there is a label that mention as destination of an order */

                    if (d_label != NULL) {
                        struct signed_table *new_label = (struct signed_table *) malloc(sizeof(struct signed_table));
                        if (new_label == NULL) {
                            printf("new label memory isn't allowed");
                            return -1;
                        }
                        new_label->line_num = IC - 1;
                        new_label->name = "code";
                        strncpy(new_label->val, d_label, 31);
                        new_label->next = NULL;
                        if (label == NULL) {
                            label = new_label;
                            curr_label = new_label;
                        } else {
                            curr_label->next = new_label;
                            curr_label = new_label;
                        }
                    }
                }
                else if(test==-1){
                    printf("line %d -error in syntax undefined commandorginal line:%s \n",g,pre_arr[g]);
                    error_flag = 1;
                }
                /* checking that there is no label that defined in the same name as a macro  */
                curr_macro=(*macro_head);
                while(curr_macro!=NULL&& new_signe != NULL){

                    if(strcmp(curr_macro->name,new_signe->val)==0){
                        printf("line %d - macro can't have a name of a label %s\n",g, curr_macro->name);
                        error_flag=1;
                        }
                    curr_macro=curr_macro->next;		}
            } else {
                printf("line %d -error in syntax orginal line:%s \n",g,pre_arr[g]);
                error_flag = 1;
            }
        }
        g++;
        test=0;
        test_d=0;

    }



    label_iter = label;
    signe_iter = signe;


    while (label_iter != NULL) {
        count=0;
        while (signe_iter != NULL) {
            if (strcmp(label_iter->val, signe_iter->val) == 0) {
                if(count>1){
                    printf("more than one decleration for this label line -%d %s\n", (label_iter->line_num), code_arr[(label_iter->line_num)]);
                    error_flag=1;
                } else{
                    char *label_bin = (char *) malloc(sizeof(char) * 16);
                    if (label_bin == NULL) {
                        printf("Memory allocation error for bin\n");
                        if(data_arr!=NULL) {
                            for (j = 0; j < DATA_ARR_SIZE; j++) {
                                if (data_arr[j] != NULL) {
                                    free(data_arr[j]);
                                }
                            }
                            free(data_arr);
                        }
                        if (code_arr != NULL) {
                            for (j = 0; j < IC; j++) {
                                if (code_arr[j] != NULL) {
                                    free(code_arr[j]);
                                }}
                            free(code_arr);
                        }
                        free(code_arr);
                        free(data_arr);
                        free(line_copy);
                        free(rest_of_sentence);
                        free(s_label);
                        free(file_name);
                        free(d_label);
                        return 1;                    }
                    memset(label_bin, '0', 15);
                    label_bin = binary(label_bin, ((signe_iter->line_num) + 100), 11);
                    label_bin[15] = '\0';
                    label_bin[14] = '0';
                    label_bin[13] = '1';
                    label_bin[12] = '0';
                    signe_iter->ext_or_relo = "relo";
                    label_iter->ext_or_relo = "relo";
                    strcpy(code_arr[(label_iter->line_num)], label_bin);
                    count++;
                } }
            signe_iter = signe_iter->next;
        }
        /* if a lable isn't declarer in the sign table we need to check in the ext linked list or error  */
        if(count==0){
            ext_iter=ext;
            while(ext_iter!=NULL){
                if (strcmp(label_iter->val, ext_iter->val) == 0){
                    label_iter->ext_or_relo="ext";
                    count++;
                    break;
                }
                ext_iter=ext_iter->next;
            }
            if(count==0){
                printf("line %d - the label %s  -undefined in this file and not as extern as well \n ",label_iter->line_num,label_iter->val);
                error_flag=1;
            }
        }
        signe_iter = signe;
        label_iter = label_iter->next;
        count=0;
    }



    if (error_flag == 1) {
        if(data_arr!=NULL) {
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                if (data_arr[j] != NULL) {
                    free(data_arr[j]);
                }
            }
            free(data_arr);
        }
            if (code_arr != NULL) {
            for (j = 0; j < IC; j++) {
                if (code_arr[j] != NULL) {
                    free(code_arr[j]);
            }}
            free(code_arr);
        }
        free(line_copy);
        free(rest_of_sentence);
        free(file_name_org);
        free(file_name);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return 0;
    }
    endf = ".am";
    file_am = build_new_file(file_name_org, endf);
    ps_am = fopen(file_am, "w");
    printf("Trying to open file: %s\n",file_am);

    if(ps_am==NULL){
        printf("Cannot open file %s, exiting.\n", file_am);
        free(file_am);
        if(data_arr!=NULL) {
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                if (data_arr[j] != NULL) {
                    free(data_arr[j]);
                }
            }
            free(data_arr);
        }
        if (code_arr != NULL) {
            for (j = 0; j < IC; j++) {
                if (code_arr[j] != NULL) {
                    free(code_arr[j]);
                }}
            free(code_arr);
        }
        free(line_copy);
        free(rest_of_sentence);
        free(s_label);
        free(file_name);
        free(d_label);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return -1;

    } else {
        printf("Opening file %s\n", file_am);
        for(i=0;i<(*file_len);i++) {
            fprintf(ps_am, "%s\n", pre_arr[i]);
        }
    }
    free(file_am);
    fclose(ps_am);


    ext_iter = ext;
    if (ext != NULL) {
        endfile = ".ext";
        new_full_file_name = build_new_file(file_name_org, endfile);
        ps_ext = fopen(new_full_file_name, "w"); /* for the file ps.ext */
        printf("Trying to open file: %s\n",new_full_file_name);
        if (ps_ext == NULL) {
            printf("Error opening ps.ext file\n");
            if(data_arr!=NULL) {
                for (j = 0; j < DATA_ARR_SIZE; j++) {
                    if (data_arr[j] != NULL) {
                        free(data_arr[j]);
                    }
                }
                free(data_arr);
            }
            if (code_arr != NULL) {
                for (j = 0; j < IC; j++) {
                    if (code_arr[j] != NULL) {
                        free(code_arr[j]);
                    }}
                free(code_arr);
            }
            free(line_copy);
            free(rest_of_sentence);
            free(s_label);
            free(file_name);
            free(d_label);
            curr_macro=(*macro_head);
            while(curr_macro!=NULL){
                macro_temp = curr_macro->next;
                if(curr_macro->name) free(curr_macro->name);
                free(curr_macro);
                curr_macro = macro_temp;
            }
            return -1;
        }
    }

    if (ent != NULL) {
        endfile = ".ent";
        new_full_file_name = build_new_file(file_name_org, endfile);
        ps_ent = fopen(new_full_file_name, "w"); /* for the file ps.ent */
        printf("Trying to open file: %s\n",new_full_file_name);
        if (ps_ent == NULL) {
            printf("Error opening ps.ent file\n");
            if(data_arr!=NULL) {
                for (j = 0; j < DATA_ARR_SIZE; j++) {
                    if (data_arr[j] != NULL) {
                        free(data_arr[j]);
                    }
                }
                free(data_arr);
            }
            if (code_arr != NULL) {
                for (j = 0; j < IC; j++) {
                    if (code_arr[j] != NULL) {
                        free(code_arr[j]);
                    }}
                free(code_arr);
            }
            free(line_copy);
            free(rest_of_sentence);
            free(s_label);
            free(file_name);
            free(d_label);
            curr_macro=(*macro_head);
            while(curr_macro!=NULL){
                macro_temp = curr_macro->next;
                if(curr_macro->name) free(curr_macro->name);
                free(curr_macro);
                curr_macro = macro_temp;
            }
            return -1;
        }
    }

    label_iter = label;
    signe_iter = signe;

    while (signe_iter != NULL) {
        signe_iter->line_num = (signe_iter->line_num) + 100;
        if (strcmp(signe_iter->name, "data") == 0) {
            signe_iter->line_num = (signe_iter->line_num) + IC;
        }
        signe_iter = signe_iter->next;
    }

    while (label_iter != NULL) {
        label_iter->line_num = (label_iter->line_num) + 100;
        label_iter = label_iter->next;
    }

    label_iter = label;
    while (ext_iter != NULL) {
        while (label_iter != NULL) {
            if (strcmp(ext_iter->val, label_iter->val) == 0) {
                char *ext_bin = (char *) malloc(sizeof(char) * 16);
                if (ext_bin == NULL) {
                    printf("Memory allocation error for bin\n");
                    if(data_arr!=NULL) {
                        for (j = 0; j < DATA_ARR_SIZE; j++) {
                            if (data_arr[j] != NULL) {
                                free(data_arr[j]);
                            }
                        }
                        free(data_arr);
                    }
                    if (code_arr != NULL) {
                        for (j = 0; j < IC; j++) {
                            if (code_arr[j] != NULL) {
                                free(code_arr[j]);
                            }}
                        free(code_arr);
                    }
                    free(line_copy);
                    free(rest_of_sentence);
                    free(s_label);
                    free(file_name);
                    free(d_label);
                    curr_macro=(*macro_head);
                    while(curr_macro!=NULL){
                        macro_temp = curr_macro->next;
                        if(curr_macro->name) free(curr_macro->name);
                        free(curr_macro);
                        curr_macro = macro_temp;
                    }
                }
                memset(ext_bin, '0', 15);
                ext_bin[15] = '\0';
                ext_bin[14] = '1';

                label_iter->ext_or_relo = "ext";
                strcpy(code_arr[(label_iter->line_num) - 100], ext_bin);
                fprintf(ps_ext, "%s %04d\n", label_iter->val, label_iter->line_num);
                label_iter = label_iter->next;
            } else {
                label_iter = label_iter->next;
            }
        }
        label_iter = label;
        ext_iter = ext_iter->next;
    }
    if (ext != NULL)
        fclose(ps_ext);

    ent_iter = ent;
    signe_iter = signe;
    while (ent_iter != NULL) {
        while (signe_iter != NULL) {
            if (strcmp(ent_iter->val, signe_iter->val) == 0) {
                char *ent_bin = (char *) malloc(sizeof(char) * 16);
                if (ent_bin == NULL) {
                    printf("Memory allocation error for bin\n");
                    if(data_arr!=NULL) {
                        for (j = 0; j < DATA_ARR_SIZE; j++) {
                            if (data_arr[j] != NULL) {
                                free(data_arr[j]);
                            }
                        }
                        free(data_arr);
                    }
                    if (code_arr != NULL) {
                        for (j = 0; j < IC; j++) {
                            if (code_arr[j] != NULL) {
                                free(code_arr[j]);
                            }}
                        free(code_arr);
                    }
                    free(line_copy);
                    free(rest_of_sentence);
                    free(s_label);
                    free(file_name);
                    free(d_label);
                    curr_macro=(*macro_head);
                    while(curr_macro!=NULL){
                        macro_temp = curr_macro->next;
                        if(curr_macro->name) free(curr_macro->name);
                        free(curr_macro);
                        curr_macro = macro_temp;
                    }
                }
                memset(ent_bin, '0', 15);
                ent_bin = binary(ent_bin, (signe_iter->line_num), 11);
                ent_bin[15] = '\0';
                ent_bin[14] = '0';
                ent_bin[13] = '1';
                ent_bin[12] = '0';
                signe_iter->ext_or_relo = "relo";
                label_iter->ext_or_relo = "relo";
                fprintf(ps_ent, "%s %04d\n", signe_iter->val, signe_iter->line_num);
                break;
            } else {
                signe_iter = signe_iter->next;
            }
        }
        signe_iter = signe;
        ent_iter = ent_iter->next;
    }
    if (ent != NULL)
        fclose(ps_ent);
    file_name_org[strcspn(file_name_org, "\n")] = '\0';
    endfile = ".ob";
    new_full_file_name = build_new_file(file_name_org, endfile);
    file_connection_ob = fopen(new_full_file_name, "w"); /* for the file ps.ob */
    printf("Trying to open file: %s\n",new_full_file_name);
    if (file_connection_ob == NULL) {
        printf("Error opening ps.ob file\n");
        if(data_arr!=NULL) {
            for (j = 0; j < DATA_ARR_SIZE; j++) {
                if (data_arr[j] != NULL) {
                    free(data_arr[j]);
                }
            }
            free(data_arr);
        }
        if (code_arr != NULL) {
            for (j = 0; j < IC; j++) {
                if (code_arr[j] != NULL) {
                    free(code_arr[j]);
                }}
            free(code_arr);
        }
        free(line_copy);
        free(rest_of_sentence);
        free(s_label);
        free(file_name);
        free(d_label);
        curr_macro=(*macro_head);
        while(curr_macro!=NULL){
            macro_temp = curr_macro->next;
            if(curr_macro->name) free(curr_macro->name);
            free(curr_macro);
            curr_macro = macro_temp;
        }
        return -1;
    }

    printf("Opening file: %s\n",new_full_file_name);

    for (i = 0; i < IC; i++) {
        if (i == 0)
            fprintf(file_connection_ob, "%d %d\n", IC, DC);
        decimal = bin_to_octal(code_arr[i]);
        fprintf(file_connection_ob, "%04d %05o\n", i + 100, decimal);
        if (i == IC - 1) {
            for (j = 0; j < DC; j++) {
                decimal = bin_to_octal(data_arr[j]);
                fprintf(file_connection_ob, "%04d %05o\n", j + 100 + i + 1, decimal);
            }
        }
    }

    fclose(file_connection_ob);
    curr_macro=(*macro_head);
    while(curr_macro!=NULL){
        macro_temp = curr_macro->next;
        if(curr_macro->name) free(curr_macro->name);
        free(curr_macro);
        curr_macro = macro_temp;
    }
    while (signe != NULL) {
        tmp = signe;
        signe = signe->next;
        free(tmp);
    }

    while (label != NULL) {
        tmp_l = label;
        label = label->next;
        free(tmp_l);
    }

    free(rest_of_sentence);
    rest_of_sentence = NULL;

    free(file_name);
    file_name = NULL;

    free(line_copy);
    line_copy = NULL;

    if(data_arr!=NULL) {
        for (j = 0; j < DATA_ARR_SIZE; j++) {
            if (data_arr[j] != NULL) {
                free(data_arr[j]);
            }
        }
        free(data_arr);
    }
    if (code_arr != NULL) {
        for (j = 0; j < IC; j++) {
            if (code_arr[j] != NULL) {
                free(code_arr[j]);
            }}
        free(code_arr);
    }
    free(file_name_org);
    printf("All memory freed. Exiting...\n");

    return 0;
}




