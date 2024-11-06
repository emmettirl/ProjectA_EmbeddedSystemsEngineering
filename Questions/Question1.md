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

Many of these functions are normally provided by the standard library, but in this case, they are implemented directly in the program.
The reason for this is that the program is designed to run on an ARM system that does not have a standard library built in.

kputc() is a function defined in vid.c that is used to handle user input.
depending on which character the user inputs, the function will either print the character to the screen or perform some other action.
Firstly it clears the cursor using the clrcursor() function.

It will then check if the character is an escape character, if it is, it will return. The escape character is used defined as 27 in the program.
It then checks for a square bracket character, which is used to indicate that the user is inputting a command.
If the character is A, and a square bracket was found, it will scroll the screen up.

I will add logic here for the other characters to handle the other commands that the user can input. 


The lines array is an array of strings that is used to store the lines of text that the user inputs.
The array is defined in t.c and is used in uart.c to store the lines of text that the user inputs.
It stores 100 lines of text, each with a maximum of 80 characters terminated by a null character.
This acts as a 100 x 80 grid of columns and rows that the user can input text into.

---

b.)
_Explain how you would modify the code to handle (you cannot just use a huge
array and you don’t have the malloc function as there is no heap):
- i) long lines

There are a few methods which could be used to handle long lines. Firstly wrapping. When adding a character, if the line is full, we can move the cursor to the next line. Any lines below would be pushed down to make space. 
To ensure the text remains readable, we could check for separators such as spaces or punctuation and wrap the line at these points.

alternatively, we could implement horizontal scrolling. An option for handling this would be to use a linked list. When the line is full, we could create a new node in the linked list and move the cursor to the next node. This would allow the user to scroll left and right to view the text.

- ii) scrolling left and right with long lines.

To allow scrolling of long lines there are a few options. Firstly the method mentioned above would allow a pagination like approach, showing only a portion of the text at a time. This would allow the user to scroll left and right.

Alternatively, if we were to increase the possible line size, we could implement a frame which displays a certain index range of text at a time. The text displayed on screen would be decided by the cursor position. When scrolling close enough to the edge of the line, the text would shift left or right to display the portion of the string that surrounds the cursor. 

- iii)handle long files e.g. 1000 lines long._

Similar to my suggestion for handling long lines, we could implement a linked list to store the lines of text, rather than an array. With this we would be able to store as many lines of text as there is memory available. 

When the user reaches the end of the screen, the text would shift up to display the next portion of the text. We would want to be sure to use a doubly linked list to allow the user to scroll up and down. 

Another option would be to have the linked list store the text in pages. When the user reaches the end of the page, which would be stored in the same manner as our current 2d array, the program would load the next page of text. 