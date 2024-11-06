# Question 2:
#### Emmett Fitzharris | R00222357

**See vid.c for the code that was used to complete this exercise.**

_Simple editor program for the emulated versatilepb board. You may use the supplied sample code in your editor._

_The supplied code is not optimised to aid usability e.g. it re-displays the whole editor window on a key press._

_You will need to implement at least the following in your editor :_

- a). _left, right and down arrow keys (up arrow is ‘done’). You can determine the appropriate escape sequence by using the 2.6 example and printing out the key stroke values._

```
if (gotsquare && c == 'A'){
		// uparrow
		uprintf("uparrow\n");
		if (row ==0 && linepos>0)
			{linepos--;}
		if (row>0)
			{row--;}

		putcursor(cursor);
		return 0;
	}

	if (gotsquare && c == 'B') {
		// down arrow
		uprintf("downarrow\n");
		if (row == 29) {
			linepos++;
		} else {
			row++;
	    }
		putcursor(cursor);
		return 0;
	}

	if (gotsquare && c == 'C') {
		// down arrow
		uprintf("rightarrow\n");
		if (col < 79) {
			col++;
		}
		putcursor(cursor);
		return 0;
	}

	if (gotsquare && c == 'D') {
		// down arrow
		uprintf("leftarrow\n");
		if (col > 0) {
			col--;
		}
		putcursor(cursor);
		return 0;
	}
```
```
	// for debugging, print the key stroke
	uprintf("Key stroke: %d\n", c);
```

---

- b). _backspace and delete._
```
	// check for the first element of the delete key
	if (gotsquare && c == '3') {
		deleteCheck = 1;
		return 0;
	}

	// check for the second element of the delete key
	if (deleteCheck && c == '~') {
		uprintf("delete\n");
		lines[row + linepos][col] = ' ';
		clearchar(' ', row, col);
		if (col < 79) {
			col++;
		} else if (row < 29) {
			col = 0;
			row++;
	    }
	putcursor(cursor);
	deleteCheck = 0;
	return 0;
	}

	if (c==127){
		uprintf("backspace\n");
		if (col > 0) {
		col--;
	} else if (row > 0) {
		row--;
		col = 79;
	}
		lines[row + linepos][col] = ' ';
		clearchar(' ', row, col);
		putcursor(cursor);
		return 0;
	}
```
---
- c). _delete a line e.g. ctrl -d._

```
// check for control + d, which will be used to delete a full line
	if (c == 4) {
		uprintf("Delete Row\n");
		for (int i = row + linepos; i < 99; i++) {
			for (int j = 0; j < 80; j++) {
				lines[i][j] = lines[i + 1][j];
			}
		}
		for (int j = 0; j < 80; j++) {
			lines[99][j] = ' ';
	    }
		putcursor(cursor);
		return 0;
	}
```
---
- d) _insert mode – currently key strokes overwrite the previous character. Need to avoid pushing characters over the edge of the screen._

```
    // check for the first element of the insert key
	if (gotsquare && c == '2') {
		insertCheck = 1;
		return 0;
	}

	// check for the second element of the insert key
	if (insertCheck && c == '~') {
		uprintf("insert\n");
		mode = !mode;
		uprintf("Mode: %d\n", mode);
		insertCheck = 0;
		return 0;
	}
```

```
if (mode == 1){
		for (int i = 79; i > col; i--) {
			lines[row + linepos][i] = lines[row + linepos][i - 1];
		}
	}

	lines[row+linepos][col]=c;
	col++;
	if (col>=80){
		col = 0;
		row++;
		if (row >= 30){
			row = 29;
			scrollup();
		}
	}
```
---
- e). _copy a line and insert the line in a new location_
```
 if (c == 3) { // Ctrl+C
		uprintf("copy\n");

        for (int i = 0; i < 80; i++) {
        	copyBuffer[i] = lines[row + linepos][i];
    	}
    	copyBuffer[80] = '\0'; // Null-terminate the copied line
    	uprintf("Line copied\n");
        return 0;
    }

    if (c == 22) { // Ctrl+Shift+V as the emulation of the normal paste keybind Ctrl+V is pasting the host os clipboard
		uprintf("paste\n");
        for (int i = 0; i < 80; i++) {
        	if (copyBuffer[i] == '\0') break;
        	lines[row + linepos][i] = copyBuffer[i];
    	}
    	uprintf("Line pasted\n");
    	return 0;
    }
```
