#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include "chaocipher_functionality.h"
#ifndef _WIN32
#include <strings.h> 
static inline int strnicmp(const char *s1, const char *s2, size_t len) {
    return strncasecmp(s1, s2, len);
}
#endif

int character_sort(const void* a,const void* b){
    return (*(const char*)a-*(const char*)b);
}

void message_prompt(char* final_buffer, char* intermediary_buffer,char* input_message,size_t buffer_size){
    printf(input_message);
    if(!fgets(intermediary_buffer,buffer_size,stdin)){
        printf("Unable to access input, exiting...\n");
        exit(EXIT_FAILURE);
    }
    if(strlen(intermediary_buffer)==2 && strnicmp(intermediary_buffer,"q",1)==0){
        printf("hank you for running the Chaocipher decoder, exiting...\n");
        exit(EXIT_SUCCESS);
    }
    strncpy(final_buffer,intermediary_buffer,strlen(intermediary_buffer)+1);
}

void numerical_selection(char* ciphertext_wheel,char* plaintext_wheel,char* buffer,char* ciphertext_wheel_buffer,char* plaintext_wheel_buffer, char* correction_buffer, char* encoded_input, char* encoded_buffer, bool confirmation_toggle,bool is_confirmation){
    if(!fgets(correction_buffer,16,stdin)){
        printf("Unable to access input, exiting...\n");
        exit(EXIT_FAILURE);
    }
    char *endptr;
    errno = 0;    
    long selection=strtol(correction_buffer,&endptr,10);
    if(strlen(correction_buffer)==2 && strnicmp(correction_buffer,"q",1)==0){
            printf("Thank you for running the chaocipher decoder, exiting...\n");
            exit(EXIT_SUCCESS);
        }
    else if(endptr==correction_buffer && *endptr == '\n' && is_confirmation){
        confirmation_toggle=true;
    }
    else if(selection ==1){
        message_prompt(encoded_input,encoded_buffer,"Enter the encoded input: ",16384);
        }
    else if(selection==2){
        message_prompt(ciphertext_wheel,ciphertext_wheel_buffer,"Enter the ciphertext wheel: ",128);
        }
    else if(selection==3){
        message_prompt(plaintext_wheel,plaintext_wheel_buffer,"Enter the plaintext wheel: ",128);
    }
}

bool wheel_set_checker(char* ciphertext_wheel,char* plaintext_wheel){
    char sorted_ciphertext[128];
    char sorted_plaintext[128];
    strcpy(sorted_ciphertext,ciphertext_wheel);
    strcpy(sorted_plaintext,plaintext_wheel);
    qsort(sorted_ciphertext,strlen(sorted_ciphertext),sizeof(char),character_sort);
    qsort(sorted_plaintext,strlen(sorted_plaintext),sizeof(char),character_sort);
    printf("Sorted Ciphertext:%s\nSorted Plaintext:%s\n",sorted_ciphertext,sorted_plaintext);
    int result=strcmp(sorted_ciphertext,sorted_plaintext);
    return result==0?true:false;
}



int main(){
    printf("Welcome to the Chaocipher decoding program!\nPress 'q' at any time to quit the program!\n");
    for(;;){
    char* encoded_input=calloc(16384,sizeof(char));
    char* decoded_input=calloc(32768,sizeof(char));
    char* ciphertext_wheel=calloc(128,sizeof(char));
    char* plaintext_wheel=calloc(128,sizeof(char));
    char* original_wheels=calloc(512,sizeof(char));
    char* ciphertext_wheel_buffer=calloc(128,sizeof(char));
    char* plaintext_wheel_buffer=calloc(128,sizeof(char));
    char* correction_buffer=calloc(16,sizeof(char));
    char* confirmation_buffer=calloc(16,sizeof(char));
    char* decoded;
    char encoded_buffer[16384];
    char file_name[128];
    bool is_finished=false;
    bool is_same_set=false;
    bool is_confirmed=false;
    FILE* output_file;
    time_t current_time;
    struct tm* local_time;
    if(encoded_input == NULL || decoded_input == NULL || plaintext_wheel == NULL || ciphertext_wheel == NULL || original_wheels == NULL|| ciphertext_wheel_buffer==NULL|| plaintext_wheel_buffer==NULL || correction_buffer==NULL){
        printf("Unable to allocate memory to start decoding, exiting...\n");
        exit(EXIT_FAILURE);
    }
    message_prompt(encoded_input,encoded_buffer,"Enter the encoded output to decode: ",16384);
    message_prompt(ciphertext_wheel,ciphertext_wheel_buffer,"Enter the ciphertext wheel: ",128);
    message_prompt(plaintext_wheel,plaintext_wheel_buffer,"Enter the plaintext wheel: ",128);
    if(strlen(ciphertext_wheel) != strlen(plaintext_wheel)){
        do{
        printf("The ciphertext and plaintext wheel length do not match\nCiphertext Wheel:%s\nPlaintext Wheel:%s\nPress 1 to change the input, 2 to change the ciphertext wheel, or 3 to change the plaintext wheel: ",ciphertext_wheel,plaintext_wheel);
        numerical_selection(ciphertext_wheel,plaintext_wheel,correction_buffer,ciphertext_wheel_buffer,plaintext_wheel_buffer,correction_buffer,encoded_input,encoded_buffer,is_confirmed,false);
        }while(strlen(ciphertext_wheel) != strlen(plaintext_wheel));
    }
    is_same_set=wheel_set_checker(ciphertext_wheel,plaintext_wheel);
    if(!is_same_set){
        do{
        printf("The ciphertext and plaintext base character set do not match\nCiphertext Wheel:%s\nPlaintext Wheel:%s\nPress 1 to change the input, 2 to change the ciphertext wheel, or 3 to change the plaintext wheel: ",ciphertext_wheel,plaintext_wheel);
        numerical_selection(ciphertext_wheel,plaintext_wheel,correction_buffer,ciphertext_wheel_buffer,plaintext_wheel_buffer,correction_buffer,encoded_input,encoded_buffer,is_confirmed,false);
        is_same_set=wheel_set_checker(ciphertext_wheel,plaintext_wheel);
        if(is_same_set){
            break;
        }
        }while(!is_same_set);
        }
    do{
        printf("Encoded input:%sCiphertext wheel:%sPlaintext wheel:%sPress 1 to change the input, 2 to change the ciphertext wheel, 3 to change the plaintext wheel, or 'enter' to confirm your choices: ",encoded_input,ciphertext_wheel,plaintext_wheel);
            if(!fgets(correction_buffer,16,stdin)){
        printf("Unable to access input, exiting...\n");
        exit(EXIT_FAILURE);
        }
        char *endptr;
        errno = 0;    
        long selection=strtol(correction_buffer,&endptr,10);
        if(strlen(correction_buffer)==2 && strnicmp(correction_buffer,"q",1)==0){
                printf("Thank you for running the chaocipher decoder, exiting...\n");
                exit(EXIT_SUCCESS);
            }
        else if(endptr==correction_buffer && *endptr == '\n'){
            is_confirmed=true;
        }
        else if(selection ==1){
            message_prompt(encoded_input,encoded_buffer,"Enter the encoded input: ",16384);
            }
        else if(selection==2){
            message_prompt(ciphertext_wheel,ciphertext_wheel_buffer,"Enter the ciphertext wheel: ",128);
            }
        else if(selection==3){
            message_prompt(plaintext_wheel,plaintext_wheel_buffer,"Enter the plaintext wheel: ",128);
        }
    }while(!is_confirmed);
    //Remove newline characters from input
    ciphertext_wheel[strcspn(ciphertext_wheel,"\n")]=0;
    plaintext_wheel[strcspn(plaintext_wheel,"\n")]=0;
    encoded_input[strcspn(encoded_input,"\n")]=0;
    snprintf(original_wheels,512,"Ciphertext Wheel: %s\nPlaintext Wheel: %s",ciphertext_wheel,plaintext_wheel);
    decoded=chaocipher_function(ciphertext_wheel,plaintext_wheel,encoded_input,true);
    printf("Result: %s\n",decoded);
    snprintf(decoded_input,32768,"%s\nDecoded text: %s\n",original_wheels,decoded);
    current_time= time(NULL);
    local_time= localtime(&current_time);
    strftime(file_name,sizeof(file_name),"chaocipher_decoded_%Y%m%d_%H%M%S.txt",local_time);
    output_file = fopen(file_name,"w");
    if (output_file == NULL) {
        printf("Unable to open the file, exiting...\n");
        exit(EXIT_FAILURE);
    } 
    fprintf(output_file,decoded_input);
    fclose(output_file);
    printf("Successfully created %s!\n",file_name);
    free(encoded_input);
    free(decoded_input);
    free(original_wheels);
    free(ciphertext_wheel_buffer);
    free(plaintext_wheel_buffer);
    free(correction_buffer);
    free(decoded);
    }
    return EXIT_SUCCESS;
}