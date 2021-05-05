#Console application
use
-----help-------
This program needs to be specified with first flag -f and file name or
just with the flag -i and name of the input file
flag [||--help] shows this help menu
flag [-f||--file] "file_name" specify the file that will be used for
the rest of the program
flag [-i||--input] "file_name" !!!should be the only specified flag
in program!!! specify the file name that have a file
with flags for the program
flag [-n||--newlines] will output number of lines that are in the file
flag [-d||--digits] will output number of digits in a file
flag [-dd||--numbers] will output number of numbers in a file
flag [-a||--anagrams] "words words" should be the last specified flag
and after the flag all the words in the file will be checked against
anagrams and the ones that match will be outputted
flag [-p||--palindroms] "words words" should be the last specified
flag and after the flag all the words that are palindroms and are in
the file will be outputted
flag [-s||--sorted] outputs all the words that are in the file in
alphabetical order
flag [-rs||--reverse-sorted] outputs all the words that are in the file
in reversed alphabetical order
flag [-o||--output] "output_file_name" outputs of the command are
saved in specified file

## How to use

- compile with your compilator of choice eg. clang, gcc
  Example: clang++ -O3 main.cpp -o main.out
  use with ./main.out
  Compile with -O3 flag for best performance
