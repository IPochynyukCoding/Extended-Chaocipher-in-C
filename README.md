
# Description
An implementation in C of encoding a file using the extended Chaocipher cipher alphabet (ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@#&[]{}/,.?!:;()$*~_^|+-<>`') to make the cipher more secure and harder to brute force.


# How to use

1. Type the input in the to_encode.txt file
2. Run the downloaded Chaocipher.exe file from the releases section
3. Get the output from the chaocipher_YYYYMMDD_HH:MM:SS.txt file, which YYYYMMDD and HH:MM:SS represents the current date and time of the system at the time of encoding.

In case you want to build the executable yourself, use the following command if you are using gcc:

```
gcc -o chaocipher chaocipher.c wheel_generator.c
```



# TODO

Implement decoding method based on the outputs from the encoded files

Allow user to select a Chaocipher alphabet with a combination of either uppercase, lowercase, numbers, or symbols.

Allow for multi-line encoding instead of just one line
