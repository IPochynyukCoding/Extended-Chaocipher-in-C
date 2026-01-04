#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include "wheel_generator.h"
#define first_index 0
#define second_index 1
#define third_index 2
#define fourth_index 3
#define array_length(array,type)(sizeof(array)/sizeof(type))

struct choices{
    char* label;
    char* character_set;
    bool is_selected;
};

void strslice(char* original_string, char* slice,size_t start,size_t stop){
    strncpy(slice,original_string+start,stop-start);
    slice[stop-start]='\0';
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


char* ciphertext_shift(char* ciphertext_wheel,int index){
    size_t length = strlen(ciphertext_wheel);
    size_t nadir = ((length)/2)+1;
    char* left = calloc(length+1,sizeof(char));
    char* right = calloc(length+1,sizeof(char));
    char* new_wheel = calloc(length+1,sizeof(char));
    char* first_letter = calloc(2,sizeof(char));
    char* second_letter = calloc(2,sizeof(char));
    if(left == NULL||right== NULL||new_wheel == NULL || first_letter == NULL || second_letter == NULL){
        printf("Unable to allocate memory for ciphertext shifting\n");
        exit(EXIT_FAILURE);
    }
    //Split selected_character_set with the selected letter as the middle
    strslice(ciphertext_wheel,left,index,length);
    strslice(ciphertext_wheel,right,first_index,index);
    sprintf(new_wheel,"%s%s",left,right);
    //Get letters from the third letter until the nadir (middle) for left side
    strslice(new_wheel,left,third_index,nadir);
    //Get letters from the nadir to the end for the right side
    strslice(new_wheel,right,nadir,length);
    //Get the first and second letters for the left side
    strslice(new_wheel,first_letter,first_index,second_index);
    strslice(new_wheel,second_letter,second_index,third_index);
    strcat(left,second_letter);
    //Create the new wheel based on the above mentioned changes
    snprintf(new_wheel,length+1,"%s%s%s",first_letter,left,right);
    //Clean up memory
    free(ciphertext_wheel);
    free(left);
    free(right);
    free(first_letter);
    free(second_letter);
    return new_wheel;
}
char* plaintext_shift(char* plaintext_wheel, int index){
    size_t length = strlen(plaintext_wheel);
    size_t nadir = ((length)/2)+1;
    char* left = calloc(length+1,sizeof(char));
    char* right = calloc(length+1,sizeof(char));
    char* new_wheel = calloc(length+1,sizeof(char));
    char* third_letter = calloc(2,sizeof(char));
    char* first_characters = calloc(3,sizeof(char));
    if(left == NULL||right== NULL||new_wheel == NULL || third_letter == NULL || first_characters == NULL){
        printf("Unable to allocate memory for plaintext shifting\n");
        exit(EXIT_FAILURE);
    }
    //Get all characters from the character next to the selected character to the end as the left side
    strslice(plaintext_wheel,left,index+1,length);
    //Get all characters from the start to the selected character
    strslice(plaintext_wheel,right,first_index,index+1);
    strcat(new_wheel,left);
    strcat(new_wheel,right);
    //Get the first two characters for the left side
    strslice(new_wheel,first_characters,first_index,third_index);
    //Get the third character to put to middle
    strslice(new_wheel,third_letter,third_index,fourth_index);
    //Get all characters from the fourth character to the nadir for the left side
    strslice(new_wheel,left,fourth_index,nadir);
    //Get all characters from the nadir to the end
    strslice(new_wheel,right,nadir,length);
    //Permutuate the new wheel
    snprintf(new_wheel,length+1,"%s%s%s%s",first_characters,left,third_letter,right);
    //Clean up memory
    free(plaintext_wheel);
    free(left);
    free(right);
    free(third_letter);    
    free(first_characters);
    return new_wheel;
}

char* chaocipher_function(char* plain_wheel, char* cipher_wheel,char* input){
    char* encoded= calloc(strlen(input)+1,sizeof(char));
    int index;
    if (encoded == NULL){
        printf("Unable to allocate memory to encoding, quitting...\n");
        exit(EXIT_FAILURE);
    }
    for(size_t i=0;i<strlen(input);i++){
        int index =index_finder(plain_wheel,input[i]);
        if(index == -1){
            encoded[i]=input[i];
            continue;
        }
        encoded[i]=cipher_wheel[index];
        cipher_wheel=ciphertext_shift(cipher_wheel,index);
        plain_wheel=plaintext_shift(plain_wheel,index);
    }
    encoded[strlen(input)+1];
    free(plain_wheel);
    free(cipher_wheel);
    return encoded;
}


int main(){
    for(;;){
    char* input_buffer = calloc(16384,sizeof(char));
    char* encoded_buffer=calloc(32768,sizeof(char));
    char* original_wheels = calloc(512,sizeof(char));
    char* selected_character_set = calloc(128,sizeof(char));
    char* case_insensitive_buffer=calloc(16384,sizeof(char));
    char* plaintext_wheel;
    char* cipher_wheel;
    char* encoded;
    char file_confirmation[16];
    char selection_confirmation[16];
    char character_set_confirmation[16];
    char case_confirmation[16];
    char filename[128];
    char selection_label[16];
    FILE* input_file;
    FILE* output_file;
    time_t current_time;
    struct tm* local_time;
    bool input_confirmation=false;
    bool is_finished=false;
    bool is_case_insensitive=false;
    struct choices choice_array[4] = {
        {"Uppercase letters","ABCDEFGHIJKLMNOPQRSTUVWXYZ",false},
        {"Lowercase letters","abcdefghijklmnopqrstuvwxyz",false},
        {"Numbers","0123456789",false},
        {"Symbols","@#&[]{}/,.?!:;()$*~_^|+-<>`'",false}
    };  
    int array_len=array_length(choice_array,choice_array[first_index]);
    bool is_uppercase=choice_array[first_index].is_selected;
    bool is_lowercase=choice_array[second_index].is_selected;
    if (input_buffer == NULL || encoded_buffer ==NULL || original_wheels==NULL ||selected_character_set==NULL){
        printf("Unable to allocate memory, quitting...\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    do{
    input_file = fopen("to_encode.txt","r");
    if(input_file==NULL ||!fgets(input_buffer,16384,input_file)){
        printf("Unable to read the file, exiting...");
        exit(EXIT_FAILURE);
    }
    fclose(input_file);
    printf("Your input is %s\nDo you want to use this text to encode?\nPress 'y' for yes, 'q' for quit, or anything else to give you time to edit your input: ",input_buffer);
    if(!fgets(file_confirmation,16,stdin)){
        printf("Unable to get a reading, exiting...\n");
        exit(EXIT_FAILURE);
            }
    if(strlen(file_confirmation)==2 && strnicmp(file_confirmation,"y",1)==0){
            input_confirmation=true;
        }
    else if(strlen(file_confirmation)==2 && strnicmp(file_confirmation,"q",1)==0){
        printf("Thank you for running the extended chaocipher program, exiting...\n");
        exit(EXIT_SUCCESS);
    }
    }while (!input_confirmation);
    selected_character_set[first_index]='\0';
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
            printf("Thank you for running the extended chaocipher program, exiting...\n");
            exit(EXIT_SUCCESS);
        }
        else if(endptr==selection_confirmation && strlen(selected_character_set)>0 && *endptr == '\n'){
            
            printf("Are you sure you are finished?\nYour current character set is: %s\nPress 'y' to continue with your character set for the cipher, 'q' to quit, or anything else to get out of this prompt: ",selected_character_set);
            if(!fgets(character_set_confirmation,16,stdin)){
                printf("Unable to get a reading, exiting...\n");
                exit(EXIT_FAILURE);
            }
            if(strlen(character_set_confirmation)==2 && strnicmp(character_set_confirmation,"y",1)==0){
                if(is_uppercase+is_lowercase<2){
                strcpy(selection_label,is_uppercase? "uppercase":"lowercase");
                printf("Do you want your input to be case-insensitive (i.e, treat all letters as %s)\nPress 'y' to change all letters to %s, 'n' for no, 'q' to quit, or anything else to get out of this prompt: ",selection_label,selection_label);
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
                printf("Thank you for running the extended chaocipher program, exiting...\n");
                exit(EXIT_SUCCESS);
                }
                else{
                continue;
                 }
                }
            }
            else if(strlen(character_set_confirmation)==2 && strnicmp(character_set_confirmation,"q",1)==0){
                printf("Thank you for running the extended chaocipher program, exiting...\n");
                exit(EXIT_SUCCESS);
            }
        }
        //For invalid inputs
        else if(errno==ERANGE || selection<=0 || selection>4){
            printf("Please input a valid number between 1 and 4, as your input is out of bounds\n");
        }
        else if(1<=selection<=array_len && !choice_array[selection-1].is_selected){
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
    encoded= is_case_insensitive ? chaocipher_function(plaintext_wheel,cipher_wheel,case_insensitive_buffer):chaocipher_function(plaintext_wheel,cipher_wheel,input_buffer);
    snprintf(encoded_buffer,32768,"%s\nEncoded text: %s\n",original_wheels,encoded);
    current_time= time(NULL);
    local_time= localtime(&current_time);
    strftime(filename,sizeof(filename),"chaocipher_%Y%m%d_%H%M%S.txt",local_time);
    output_file = fopen(filename,"w");
    if (output_file == NULL) {
        printf("Unable to open the file, exiting...\n");
        exit(EXIT_FAILURE);
    } 
    fprintf(output_file,encoded_buffer);
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