Patrick Stumbaugh


This program reads from user input (either from console or by inputting the
file name in the command line (ie: "< input.txt"). 

Thread 1, called the Input Thread, reads in lines of characters from the 
standard input.

Thread 2, called the Line Separator Thread, replaces every line separator in 
the input by a space.

Thread 3, called the Plus Sign thread, replaces every pair of plus signs, 
i.e., "++", by a "^".

Thread 4, called the Output Thread, writes this processed data to standard 
output as lines of exactly 80 characters. As soon as 80 characters are 
produced, the output thread will print that line. As in, it does not wait for 
all the input to be finished before printing. 

Each function is called as a separate thread, thus allowing the input through 
the output threads all to work simultaneously. 

Thread 1 produces to a buffer
Thread 2 and 3 both consume from a buffer and produce to a buffer
Thread 4 consumes from a buffer 

The program will stop reading and printing once "STOP" is found. This must be 
on it's own line followed immediately by a newline character.

Max line to be read: 50 lines
Max characters per line: 1000 char


--------------------------------------------------------------------------------
create executable by typing in the command line:
    gcc --std=gnu99 -pthread -o main main.c
then run using:
    ./main 
    
NOTE - if you don't give any command line arguments after ./main, it will 
assume you will be entering your own text. You may include a text file to read 
from by having it in the same directory and calling it after. 
ie: You have a file named input.txt to read from. Run it by creating the
executable and then in the command line, run by typing: "./main < input.txt".
This will redirect stdin to that text file. 

You may also output to a specific file (create/overwrite only) similarly, by 
typing in the command line (ie): "./main > output.txt"

Or, you can combine these functions by typing (ie):
"./main < input.txt > output.txt"
This will read from input.txt and then print to output.txt


