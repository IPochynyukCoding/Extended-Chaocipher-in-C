#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "wheel_generator.h"
#define first_index 0
#define second_index 1
#define third_index 2
#define fourth_index 3

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
        printf("Unable to allocate memory for ciphertext shifting\n");
        exit(EXIT_FAILURE);
    }
    //Split selected_character_set with the selected letter as the middle
    strslice(ciphertext_wheel,left,index,length);
    strslice(ciphertext_wheel,right,first_index,index);
    snprintf(new_wheel,length+1,"%s%s",left,right);
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

char* chaocipher_function(char* cipher_wheel,char* plain_wheel,char* input, bool is_decode){
    char* encoded= calloc(strlen(input)+1,sizeof(char));
    int index;
    if (encoded == NULL){
        printf("Unable to allocate memory to encoding, quitting...\n");
        exit(EXIT_FAILURE);
    }
    for(size_t i=0;i<strlen(input);i++){
        int index =is_decode? index_finder(cipher_wheel,input[i]):index_finder(plain_wheel,input[i]);
        if(index == -1){
            encoded[i]=input[i];
            continue;
        }
        encoded[i]= is_decode?plain_wheel[index]:cipher_wheel[index];
        cipher_wheel=ciphertext_shift(cipher_wheel,index);
        plain_wheel=plaintext_shift(plain_wheel,index);
    }
    encoded[strlen(input)+1];
    free(plain_wheel);
    free(cipher_wheel);
    return encoded;
}