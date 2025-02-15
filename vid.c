/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

// Modified by Emmett Fitzharris | R00222357

extern char _binary_font_start;
extern int color;
extern char lines[100][81];
extern int linepos;
u8 cursor;
int volatile *fb;

int row, col, scroll_row;
unsigned char *font;  // using a RAW or binary font file (bitmap)
int WIDTH = 640;
int framebuffer[640*480];
int fbuf_init()
{
	int i;
	fb = (int *)framebuffer;
	font = &_binary_font_start;
	/********* for 640x480 ************************/
	*(volatile unsigned int *)(0x1000001c) = 0x2C77;
	*(volatile unsigned int *)(0x10120000) = 0x3F1F3F9C;
	*(volatile unsigned int *)(0x10120004) = 0x090B61DF;
	*(volatile unsigned int *)(0x10120008) = 0x067F1800;
	*(volatile unsigned int *)(0x10120010) = fb;//0x200000;
	*(volatile unsigned int *)(0x10120018) = 0x82B;
	/********** for 800X600 **********************
	*(volatile unsigned int *)(0x1000001c) = 0x2CAC; // 800x600
	*(volatile unsigned int *)(0x10120000) = 0x1313A4C4;
	*(volatile unsigned int *)(0x10120004) = 0x0505F6F7;
	*(volatile unsigned int *)(0x10120008) = 0x071F1800;
	*(volatile unsigned int *)(0x10120010) = 0x200000;
	*(volatile unsigned int *)(0x10120018) = 0x82B;
	**********/
	cursor = 127; // cursor bit map in font0 at 127
	for (i=0; i<480*640; i++){
		fb[i] = 0x00000000;   // black screen
	}
}


int clrpix(int x, int y)
{
	int pix = y*640 + x;
	fb[pix] = 0x00000000;
}


int setpix(int x, int y)
{
	int pix = y*640 + x;
	if (color==RED)
	fb[pix] = 0x000000FF;
	if (color==BLUE)
	fb[pix] = 0x00FF0000;
	if (color==GREEN)
	fb[pix] = 0x0000FF00;
}


int dchar(unsigned char c, int x, int y)
{
	int r, bit;
	unsigned char *caddress, byte;

	caddress = font + c*16;
	//   uprintf("c=%x %c caddr=%x\n", c, c, caddress);

	for (r=0; r<16; r++)
    {
		byte = *(caddress + r);

		for (bit=0; bit<8; bit++)
        {
			if (byte & (1<<bit))
			setpix(x+bit, y+r);
		}
	}
}


int undchar(unsigned char c, int x, int y)
{
	int row, bit;
	unsigned char *caddress, byte;

	caddress = font + c*16;
	//  printf("c=%x %c caddr=%x\n", c, c, caddress);

	for (row=0; row<16; row++)
    {
		byte = *(caddress + row);

		for (bit=0; bit<8; bit++)
    	{
			if (byte & (1<<bit))
			clrpix(x+bit, y+row);
		}
	}
}


int cchar(unsigned char c, int x, int y)
{
	int row, bit;
    unsigned char *caddress, byte;
    caddress = font + c*16;
    //  printf("c=%x %c caddr=%x\n", c, c, caddress);

    for (row=0; row<16; row++)
    {
    	byte = *(caddress + row);
        for (bit=0; bit<8; bit++)
        {
        //	 if (byte & (1<<bit))
        	clrpix(x+bit, y+row);
		}
	}
}

int dstring(char *s, int x, int y)
{
	while(*s){
		dchar(*s, x, y);
		x+=8;
		s++;
	}
}

int scrollup()
{
	int i;
	for (i=64*640; i<640*480; i++){
	fb[i] = fb[i + 640*16];
	}
}


int clearScreen()
{
	int i;
	for (i=64*640; i<640*480; i++){
	fb[i] = 0x00000000;;
	}
}


int kpchar(char c, int ro, int co)
{
	int x, y;
	x = co*8;
	y = ro*16;
	// uprintf("c=%d [%d%d] (%d%d)\n", c, ro,co,x,y);
	dchar(c, x, y);
}


int clearchar(char c, int ro, int co)
{
	int x, y;
	x = co*8;
	y = ro*16;
	// uprintf("c=%d [%d%d] (%d%d)\n", c, ro,co,x,y);
	cchar(c, x, y);
}

int unkpchar(char c, int ro, int co)
{
int x, y;
x = co*8;
y = ro*16;
//printf("c=%x [%d%d] (%d%d)\n", c, ro,co,x,y);
undchar(c, x, y);
}


int erasechar()
{
// erase char at (row,col)
int r, bit, x, y;
unsigned char *caddress, byte;

x = col*8;
y = row*16;

//printf("ERASE: row=%d col=%d x=%d y=%d\n",row,col,x,y);

for (r=0; r<16; r++){
for (bit=0; bit<8; bit++){
clrpix(x+bit, y+r);
}
}
}


int clrcursor()
{
unkpchar(127, row, col);
}

int putcursor(unsigned char c)
{
kpchar(c, row, col);
}
void display(){
int i,j;
for (i=0; i<480*640; i++){
fb[i] = 0;   // black screen
}
for (j=linepos;j<linepos+30;j++){
//for (i=0;i<80;i++){
//       clearchar(' ',j-linepos,i);
//     }
for (i=0;i<80;i++){
if (!lines[j][i])
break;
kpchar(lines[j][i],j-linepos,i);
}


}
putcursor(cursor);

}
int escape,gotsquare;


/********************************************************************/
// Start of changes
/********************************************************************/

int kputc(char c)
{
	int yy=0;
	clrcursor();
	// I included these variables because the delete and insert key was inputing keystrokes of both 126 and 51/52.
	static int deleteCheck = 0;
	static int insertCheck = 0;

    // This variable is used to toggle between insert and overwrite mode
	static int mode = 0;

    // This variable is used to store current line in a when copying and pasting
    static char copyBuffer[81];

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

	// escape character
	if (c == 27) {
		escape = 1;
		uprintf("escape\n");
		return 0;
	}

    // Ctrl+C
    if (c == 3) {
		uprintf("copy\n");

        for (int i = 0; i < 80; i++) {
        	copyBuffer[i] = lines[row + linepos][i];
    	}
        // Null-terminate the copied line
    	copyBuffer[80] = '\0';
    	uprintf("Line copied\n");
        return 0;
    }

    // Ctrl+Shift+V as the emulation of the normal paste keybind Ctrl+V is pasting the host os clipboard
    if (c == 22) {
		uprintf("paste\n");
        for (int i = 0; i < 80; i++) {
        	if (copyBuffer[i] == '\0') break;
        	lines[row + linepos][i] = copyBuffer[i];
    	}
    	uprintf("Line pasted\n");
    	return 0;
    }

	//check for command sequence
	if (escape && c == '[') {
		gotsquare = 1;
		uprintf("got square\n");
		return 0;
	}

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

    // check for the backspace key
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

    // Handle Up Arrow Press
	if (gotsquare && c == 'A'){
		uprintf("uparrow\n");
		if (row ==0 && linepos>0)
			{linepos--;}
		if (row>0)
			{row--;}

		putcursor(cursor);
		return 0;
	}

    // Handle Down Arrow Press
	if (gotsquare && c == 'B') {
		uprintf("downarrow\n");
		if (row == 29) {
			linepos++;
		} else {
			row++;
	    }
		putcursor(cursor);
		return 0;
	}

    // Handle Right Arrow Press
	if (gotsquare && c == 'C') {
		uprintf("rightarrow\n");
		if (col < 79) {
			col++;
		}
		putcursor(cursor);
		return 0;
	}

    // Handle Left Arrow Press
	if (gotsquare && c == 'D') {
		uprintf("leftarrow\n");
		if (col > 0) {
			col--;
		}
		putcursor(cursor);
		return 0;
	}

	// for debugging, print the key stroke
	uprintf("Key stroke: %d\n", c);
	gotsquare=0;

    // check for newline character
	if (c=='\r'){
		col=0;
		row++;
		if (row>=30){
			row = 29;
			linepos++;
			// scroll();
		}
		putcursor(cursor);
		return 0;
	}

    // based on the mode, either insert by shifting the following characters to the right or overwrite by replacing the character
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
}

/********************************************************************/
// End of changes
/********************************************************************/


int kprints(char *s)
{
while(*s){
kputc(*s);
s++;
}
}

int krpx(int x)
{
char c;
if (x){
c = tab[x % 16];
krpx(x / 16);
}
kputc(c);
}

int kprintx(int x)
{
kputc('0'); kputc('x');
if (x==0)
kputc('0');
else
krpx(x);
kputc(' ');
}

int krpu(int x)
{
char c;
if (x){
c = tab[x % 10];
krpu(x / 10);
}
kputc(c);
}

int kprintu(int x)
{
if (x==0)
kputc('0');
else
krpu(x);
kputc(' ');
}

int kprinti(int x)
{
if (x<0){
kputc('-');
x = -x;
}
kprintu(x);
}

int kprintf(char *fmt,...)
{
int *ip;
char *cp;
cp = fmt;
ip = (int *)&fmt + 1;

while(*cp){
if (*cp != '%'){
kputc(*cp);
if (*cp=='\n')
kputc('\r');
cp++;
continue;
}
cp++;
switch(*cp){
case 'c': kputc((char)*ip);      break;
case 's': kprints((char *)*ip);  break;
case 'd': kprinti(*ip);          break;
case 'u': kprintu(*ip);          break;
case 'x': kprintx(*ip);          break;
}
cp++; ip++;
}
}

