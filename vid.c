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

  for (r=0; r<16; r++){
    byte = *(caddress + r);

    for (bit=0; bit<8; bit++){
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

  for (row=0; row<16; row++){
    byte = *(caddress + row);

    for (bit=0; bit<8; bit++){
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

  for (row=0; row<16; row++){
    byte = *(caddress + row);

    for (bit=0; bit<8; bit++){
     // if (byte & (1<<bit))
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
int kputc(char c)
{
int yy=0;
 clrcursor();
  if (c==27){
     escape = 1;
     uprintf("escape\n");
     return;
  }
  if (escape && c=='['){
      uprintf("got square\n");
     gotsquare=1;
     return;
  }
  escape=0;
  if (gotsquare && c == 'A'){
     // uparrow
 	uprintf("uparrow\n");
     if (row ==0 && linepos>0)
        linepos--;
     if (row>0)
       row--;
     
     putcursor(cursor);
     return;
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
    return;
}

  if (gotsquare && c == 'C') {
    // down arrow
    uprintf("rightarrow\n");
    putcursor(cursor);
    return;
}

  if (gotsquare && c == 'D') {
    // down arrow
    uprintf("leftarrow\n");
    putcursor(cursor);
    return;
}

  gotsquare=0;
  
  if (c=='\r'){
    col=0;
    row++;
    if (row>=30){
      row = 29;
      linepos++;
     // scroll();
    }
    //uprintf("2row=%d col=%d\n", row, col);
    putcursor(cursor);
    return;
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

