#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "wheel_generator.h"
void letter_swap(char* wheel,char value1,char value2){
    char old= wheel[value1];
    wheel[value1]=wheel[value2];
    wheel[value2]=old;
}

bool letter_comparison(char* wheel,char character){
    if(strlen(wheel) == 0){
        return true;
    }
    for(int i=0; i<strlen(wheel);i++){
        if(wheel[i]== character){
            return false;
        }
    }
    return true;
}

void wheel_uniqueness(char* wheel1,char* wheel2){
    int length = strlen(wheel1);
    for(int i=0;i<strlen(wheel1);i++){
        if(wheel1[i] == wheel2[i]){
            while(wheel1[i]==wheel2[i]){
                int random1=rand()%length;
                int random2=rand()%length;
                while(wheel1[random1] == wheel2[random2]){
                    random1=rand()%length;
                    random2=rand()%length;
                }
                letter_swap(wheel1,random1,i);
                letter_swap(wheel2,random2,i);
            }
        }
    }
}

char* wheel_generate(){
    char alphabet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@#&[]{}/,.?!:;()$*~_^|+-<>`'";
    int length = strlen(alphabet);
    char* wheel= (char *)calloc(length+1,sizeof(char));
    if(wheel == NULL){
        printf("Unable to allocate memory, quitting...");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<strlen(alphabet);i++){
        char random_letter = alphabet[rand()%length];
        bool is_unique = letter_comparison(wheel,random_letter);
        while(!is_unique){
            random_letter = alphabet[rand()%length];
            is_unique=letter_comparison(wheel,random_letter);
        }
        strncat(wheel,&random_letter,1);
    }
    return wheel;
}