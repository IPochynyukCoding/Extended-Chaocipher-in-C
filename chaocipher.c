#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "wheel_generator.h"
#define first_index 0
#define second_index 1
#define third_index 2
#define fourth_index 3
int index_finder(char* wheel, char letter){
    for(int i=0; i<strlen(wheel);i++){
        if(wheel[i]==letter){
            return i;
        }
    }
    return -1;
}
void strslice(char* original_string, char* slice,size_t start,size_t stop){
    strncpy(slice,original_string+start,stop-start);
    slice[stop-start]='\0';
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
        printf("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    //Split alphabet with the selected letter as the middle
    strslice(ciphertext_wheel,left,index,length);
    strslice(ciphertext_wheel,right,first_index,index);
    strcat(new_wheel,left);
    strcat(new_wheel,right);
    //Get letters from the third letter until the nadir (middle) for left side and get letters from the nadir to the end for the right side
    strslice(new_wheel,left,third_index,nadir);
    strslice(new_wheel,right,nadir,length);
    strslice(new_wheel,first_letter,first_index,second_index);
    strslice(new_wheel,second_letter,second_index,third_index);
    //Add the second letter to the left side
    strcat(left,second_letter);
    //Create the new wheel based on the above mentioned changes
    strcpy(new_wheel,first_letter);
    strcat(new_wheel,left);
    strcat(new_wheel,right);
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
    if(left == NULL||right== NULL||new_wheel == NULL){
        printf("Unable to allocate memory");
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
    //Crave out the third character to put to middle
    strslice(new_wheel,third_letter,third_index,fourth_index);
    //Get all characters from the fourth character to the nadir for the left side
    strslice(new_wheel,left,fourth_index,nadir);
    //Get all characters from the nadir to the end
    strslice(new_wheel,right,nadir,length);
    //Permutuate the new wheel
    strcpy(new_wheel,first_characters);
    strcat(new_wheel,left);
    strcat(new_wheel,third_letter);
    strcat(new_wheel,right);
    //Clean up memory
    free(plaintext_wheel);
    free(left);
    free(right);
    free(third_letter);    
    free(first_characters);
    return new_wheel;
}

char* chaocipher_encode(char* plain_wheel, char* cipher_wheel,char* input){
    char* encoded= calloc(strlen(input)+1,sizeof(char));
    int index;
    if (encoded == NULL){
        printf("Unable to allocate memory, quitting...");
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
    char* input_buffer = calloc(16384,sizeof(char));
    char* encoded_buffer=calloc(32768,sizeof(char));
    char* original_wheels = calloc(512,sizeof(char));
    char* plaintext_wheel;
    char* cipher_wheel;
    char* encoded;
    static char filename[96];
    FILE* input_file;
    FILE* output_file;
    time_t current_time;
    struct tm* local_time;
    if (input_buffer == NULL || encoded_buffer ==NULL || original_wheels==NULL){
        printf("Unable to allocate memory, quitting...");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    input_file = fopen("to_encode.txt","r");
    if(input_file==NULL ||!fgets(input_buffer,16384,input_file)){
        printf("Unable to read the file, exiting...");
        exit(EXIT_FAILURE);
    }
    fclose(input_file);
    plaintext_wheel=wheel_generate();
    cipher_wheel=wheel_generate();
    wheel_uniqueness(plaintext_wheel,cipher_wheel);
    sprintf(original_wheels,"Ciphertext Wheel:%s\nPlaintext Wheel:%s",cipher_wheel,plaintext_wheel);
    encoded=chaocipher_encode(plaintext_wheel,cipher_wheel,input_buffer);
    sprintf(encoded_buffer,"%s\nEncoded text:%s\n",original_wheels,encoded);
    current_time= time(NULL);
    local_time= localtime(&current_time);
    strftime(filename,sizeof(filename),"chaocipher_%Y%m%d_%H%M%S.txt",local_time);
    output_file = fopen(filename,"w");
    if (output_file == NULL) {
        printf("Unable to open the file, exiting...\n");
        return EXIT_FAILURE;
    } 
    fprintf(output_file,encoded_buffer);
    fclose(output_file);
    free(input_buffer);
    free(encoded_buffer);
    free(original_wheels);
    return EXIT_SUCCESS;
}