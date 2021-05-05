# Console application

### Usage

-----help-------<br>
This program needs to be specified with first flag -f and file name or
just with the flag -i and name of the input file<br>
flag [||--help] shows this help menu<br>
flag [-f||--file] "file_name" specify the file that will be used for
the rest of the program<br>
flag [-i||--input] "file_name" !!!should be the only specified flag
in program!!! specify the file name that have a file
with flags for the program <br>
flag [-n||--newlines] will output number of lines that are in the file<br>
flag [-d||--digits] will output number of digits in a file<br>
flag [-dd||--numbers] will output number of numbers in a file<br>
flag [-a||--anagrams] "words words" should be the last specified flag
and after the flag all the words in the file will be checked against
anagrams and the ones that match will be outputted<br>
flag [-p||--palindroms] "words words" should be the last specified
flag and after the flag all the words that are palindroms and are in
the file will be outputted<br>
flag [-s||--sorted] outputs all the words that are in the file in
alphabetical order<br>
flag [-rs||--reverse-sorted] outputs all the words that are in the file
in reversed alphabetical order<br>
flag [-o||--output] "output_file_name" outputs of the command are
saved in specified file<br>

## How to use

- compile with your compilator of choice eg. clang, gcc<br>
  Example: clang++ -O3 main.cpp -o main.out<br>
  use with ./main.out<br>
  Compile with -O3 flag for best performance<br>
