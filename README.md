
# Description
An implementation in C of encoding and decoding the extended Chaocipher cipher alphabet (ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@#&[]{}/,.?!:;()$*~_^|+-<>`') to make the cipher more secure and harder to brute force.


# How to use
## Encoding 
1. Input the text you want to encode
2. Customize your character set using lowercase letters, uppercase letters, numbers, and symbols
    
    2.1: If you don't select both lowercase and uppercase letters, you will get an additional confirmation to make your output case-insensitive
3. Confirm your character set selection and input.
4. Get the output from the chaocipher_encoded_YYYYMMDD_HH:MM:SS.txt file, which YYYYMMDD and HH:MM:SS represents the current date and time of the system at the time of encoding. The output contains the original wheels for encoding, alongside the encoded output.

## Decoding
1. Input your encoded text to decode
2. Input your ciphertext wheel
3. Input your plaintext wheel
4. If both wheels' base character set and length do not match, try to change your wheels and/or input to ensure you get it right.
5. Confirm all your inputs are correct
6. Get the output from the chaocipher_decoded_YYYYMMDD_HH:MM:SS.txt file, which YYYYMMDD and HH:MM:SS represents the current date and time of the system at the time of decoding. The output contains the original wheels for decoding, alongside the decoded output.

In case you want to build the executable yourself, use the following commands if you are using gcc:

Encoding:

```
gcc -o chaocipher_encode chaocipher_encode.c chaocipher_functionality.c wheel_generator.c
```

Decoding:

```
gcc -o chaocipher_decode chaocipher_decode.c chaocipher_functionality.c wheel_generator.c
```