#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include "wheel_generator.h"
#include "chaocipher_functionality.h"
#define first_index 0
#define second_index 1
#define array_length(array,type)(sizeof(array)/sizeof(type))
#ifndef _WIN32
#include <strings.h> 
static inline int strnicmp(const char *s1, const char *s2, size_t len) {
    return strncasecmp(s1, s2, len);
}
#endif
struct choices{
    char* label;
    char* character_set;
    bool is_selected;
};

void get_input(char* buffer){
    printf("Input your message to be encoded or press 'q' to quit: ");
    if(!fgets(buffer,16384,stdin)){
        printf("Unable to get a reading, exiting...\n");
        exit(EXIT_FAILURE);
    }
    if(strlen(buffer)==2 && strnicmp(buffer,"q",1)==0){
        printf("Thank you for running the Chaocipher encoder, exiting...\n");
        exit(EXIT_SUCCESS);
    }
    buffer[strcspn(buffer,"\n")]=0;
}


void menu_selection(struct choices* array, int size){
    for(int i=0;i<size;i++){
        printf("%d. %s\n",i+1,array[i].label);
    }
}
char* case_insensitive(char* input, bool is_upper){
    char* copy=calloc(strlen(input)+1,sizeof(char));
    for(int i=0;i<strlen(input);i++){
        copy[i]= is_upper?toupper(input[i]):tolower(input[i]);
    }
    return copy;
}

int main(){
    printf("Welcome to the Chaocipher encoding program\n");
    for(;;){
    char* input_buffer = calloc(16384,sizeof(char));
    char* encoded_buffer=calloc(32768,sizeof(char));
    char* original_wheels = calloc(512,sizeof(char));
    char* selected_character_set = calloc(128,sizeof(char));
    char* case_insensitive_buffer=calloc(16384,sizeof(char));
    char* plaintext_wheel;
    char* cipher_wheel;
    char* encoded;
    char selection_confirmation[16];
    char character_set_confirmation[16];
    char case_confirmation[16];
    char filename[128];
    char selection_label[16];
    FILE* output_file;
    time_t current_time;
    struct tm* local_time;
    bool is_finished=false;
    bool is_case_insensitive=false;
    bool is_decode=false;
    struct choices choice_array[4] = {
        {"Uppercase letters","ABCDEFGHIJKLMNOPQRSTUVWXYZ",false},
        {"Lowercase letters","abcdefghijklmnopqrstuvwxyz",false},
        {"Numbers","0123456789",false},
        {"Symbols","@#&[]{}/,.?!:;()$*~_^|+-<>`'",false}
    };  
    int array_len=array_length(choice_array,choice_array[first_index]);
    bool is_uppercase=choice_array[first_index].is_selected;
    bool is_lowercase=choice_array[second_index].is_selected;
    if (input_buffer == NULL || encoded_buffer ==NULL || original_wheels==NULL ||selected_character_set==NULL ||case_insensitive_buffer==NULL){
        printf("Unable to allocate memory, quitting...\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    get_input(input_buffer);
    do{
        if(strlen(selected_character_set)>0){
            printf("%s\n",selected_character_set);
            printf("Press 'enter' to finish selection\n");
        }
        menu_selection(choice_array,array_len);
        printf("Enter a number between 1 and 4 or press 'q' to quit: ");
        if(!fgets(selection_confirmation,16,stdin)){
            printf("Unable to get a reading, exiting...\n");
            exit(EXIT_FAILURE);
        }
        char *endptr;
        errno = 0;
        long selection=strtol(selection_confirmation,&endptr,10);
        if(strlen(selection_confirmation)==2 && strnicmp(selection_confirmation,"q",1)==0){
            printf("Thank you for running the Chaocipher encoder, exiting...\n");
            exit(EXIT_SUCCESS);
        }
        else if(endptr==selection_confirmation && strlen(selected_character_set)>0 && *endptr == '\n'){
            
            printf("Are you sure you are finished?\nYour current character set is: %s\nYour input is: %s\nPress 'y' to continue with your character set for the cipher, 'q' to quit, 'i' to change the input, or anything else to change the wheel configuration: ",selected_character_set,input_buffer);
            if(!fgets(character_set_confirmation,16,stdin)){
                printf("Unable to get a reading, exiting...\n");
                exit(EXIT_FAILURE);
            }
            if(strlen(character_set_confirmation)==2 && strnicmp(character_set_confirmation,"y",1)==0){
                if(is_uppercase+is_lowercase<2){
                strcpy(selection_label,is_uppercase? "uppercase":"lowercase");
                printf("Do you want your input to be case-insensitive (i.e, treat all letters as %s)\nPress 'y' to change all letters to %s, 'n' for no, 'q' to quit, or anything else to change the wheel configuration: ",selection_label,selection_label);
                if(!fgets(case_confirmation,16,stdin)){
                printf("Unable to get a reading, exiting...\n");
                exit(EXIT_FAILURE);
                }
                if(strlen(case_confirmation)==2 && strnicmp(case_confirmation,"y",1)==0){
                    case_insensitive_buffer=case_insensitive(input_buffer,is_uppercase);
                    is_case_insensitive=true;
                    is_finished=true;
                }
                else if(strlen(case_confirmation)==2 && strnicmp(case_confirmation,"n",1)==0){
                    is_finished=true;
                }
                else if(strlen(case_confirmation)==2 && strnicmp(case_confirmation,"q",1)==0){
                printf("Thank you for running the Chaocipher encoder, exiting...\n");
                exit(EXIT_SUCCESS);
                }
                else{
                continue;
                 }
                }
                is_finished=true;
            }
            else if(strlen(character_set_confirmation)==2 && strnicmp(character_set_confirmation,"q",1)==0){
                printf("Thank you for running the Chaocipher encoder, exiting...\n");
                exit(EXIT_SUCCESS);
            }
            else if(strlen(character_set_confirmation)==2 && strnicmp(character_set_confirmation,"i",1)==0){
                get_input(input_buffer);
            }
        }
        //For invalid inputs
        else if(errno==ERANGE || selection<=0 || selection>4){
            printf("Please input a valid number between 1 and 4, as your input is out of bounds\n");
        }
        else if(selection>=1 && selection<=array_len && !choice_array[selection-1].is_selected){
            choice_array[selection-1].is_selected=true;
            is_uppercase=choice_array[first_index].is_selected;
            is_lowercase=choice_array[second_index].is_selected;
            strcat(selected_character_set,choice_array[selection-1].character_set);
            printf("Added %s into the character set\n",choice_array[selection-1].label);
            printf("Current set:%s\n",selected_character_set);
        }
        else if(choice_array[selection-1].is_selected){
            printf("The option %s is already been selected!\n",choice_array[selection-1].label);
        }
    }while(!is_finished);
    plaintext_wheel=wheel_generate(selected_character_set);
    cipher_wheel=wheel_generate(selected_character_set);
    wheel_uniqueness(plaintext_wheel,cipher_wheel);
    snprintf(original_wheels,512,"Ciphertext Wheel: %s\nPlaintext Wheel: %s",cipher_wheel,plaintext_wheel);
    encoded= is_case_insensitive ? chaocipher_function(cipher_wheel,plaintext_wheel,case_insensitive_buffer,is_decode):chaocipher_function(cipher_wheel,plaintext_wheel,input_buffer,is_decode);
    snprintf(encoded_buffer,32768,"Encoded text: %s\n%s",encoded,original_wheels);
    current_time= time(NULL);
    local_time= localtime(&current_time);
    strftime(filename,sizeof(filename),"chaocipher_encoded_%Y%m%d_%H%M%S.txt",local_time);
    output_file = fopen(filename,"w");
    if (output_file == NULL) {
        printf("Unable to open the file, exiting...\n");
        exit(EXIT_FAILURE);
    } 
    fprintf(output_file,"%s",encoded_buffer);
    fclose(output_file);
    printf("Successfully created %s!\n",filename);
    free(selected_character_set);
    free(input_buffer);
    free(encoded_buffer);
    free(original_wheels);
    free(case_insensitive_buffer);
    free(encoded);
    }
    return EXIT_SUCCESS;
}