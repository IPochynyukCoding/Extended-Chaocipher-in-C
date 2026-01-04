#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "wheel_generator.h"
void letter_swap(char* wheel,char value1,char value2){
    char old= wheel[value1];
    wheel[value1]=wheel[value2];
    wheel[value2]=old;
}

int index_finder(char* wheel,char character){
    if(strlen(wheel) == 0){
        return -1;
    }
    for(int i=0; i<strlen(wheel);i++){
        if(wheel[i]== character){
            return i;
        }
    }
    return -1;
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

char* wheel_generate(char* character_set){
    int length = strlen(character_set);
    char* wheel= (char *)calloc(length+1,sizeof(char));
    if(wheel == NULL){
        printf("Unable to allocate memory, quitting...\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<length;i++){
        char random_letter = character_set[rand()%length];
        int index = index_finder(wheel,random_letter);
        while(index != -1){
            random_letter = character_set[rand()%length];
            index=index_finder(wheel,random_letter);
        }
        wheel[i]=random_letter;
    }
    return wheel;
}