# Question 1
#### Emmett Fitzharris | R00222357

_a.) Give a brief overview of how the editor in question 2 will work using the kputc
function and the lines array._

This program is broken down into the following files: 
- crc16_ccitt.c which handles the CRC16 checksum
- exceptions.c which handles the exceptions
- string.c which handles the string functions
- t.c which acts as the main file to run the program
- uart.c which handles the input from the user
- vid.c which handles the video output
- xmodem.c which handles the file transfer

Many of these functions are normally provided by the standard library, but in this case, they are implemented in the program.
The reason for this is that the program is designed to run on an ARM system that does not have a standard library built in.

kputc() is a function defined in vid.c that is handle user input.
depending on which character the user inputs, the function will either print the character to the screen or perform some other action.
Firstly it clears the cursor using the clrcursor() function.

It will then check if the character is an escape character, if it is, it will return. The escape character is used defined as 27 in the program.
It then checks for a square bracket character, which is used to indicate that the user is inputting a command.
If the character is A, and gotsquare is 1, it will scroll the screen up.

I will add logic for the other characters to handle the other commands that the user can input (down arrow, left arrow and right arrow).


The lines array is an array of strings that is used to store the lines of text that the user inputs.
The array is defined in t.c and is used in uart.c to store the lines of text that the user inputs.
It stores 100 lines of text, each with a maximum of 80 characters terminated by a null character.

---

b.)
_Explain how you would modify the code to handle (you cannot just use a huge
array and you don’t have the malloc function as there is no heap):
- i) long lines
- ii) scrolling left and right with long lines.
- iii)handle long files e.g. 1000 lines long._