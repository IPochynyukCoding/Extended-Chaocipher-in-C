#ifndef CHAOCIPHER_H
#define CHAOCIPHER_H
void strslice(char* original_string, char* slice,size_t start,size_t stop);
char* ciphertext_shift(char* ciphertext_wheel,int index);
char* plaintext_shift(char* plaintext_wheel, int index);
char* chaocipher_function(char* cipher_wheel,char* plain_wheel,char* input, bool is_decode);
#endif