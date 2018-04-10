/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:____________Kevin Lewis______________ Date:__3-16-2018___        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2018.             */

void handleInterrupt21(int,int,int,int);
void printLogo();
char* readString(char str[]);
void readInt(int* n);
void writeInt(int x);

int mod(int a, int b);
int div(int a, int b);

//variables
char str[80];
int* x;
char* c;

void main()
{
   char food[25], adjective[25], color[25], animal[25];
   int temp;
   makeInterrupt21();
   printLogo();
   interrupt(33,0,"\r\nWelcome to the Mad Libs kernel.\r\n\0",0,0);
   interrupt(33,0,"Enter a food: \0",0,0);
   interrupt(33,1,food,0,0);
   temp = 0;
   while ((temp < 100) || (temp > 120)) {
      interrupt(33,0,"Enter a number between 100 and 120: \0",0,0);
      interrupt(33,14,&temp,0,0);
   }
   interrupt(33,0,"Enter an adjective: \0",0,0);
   interrupt(33,1,adjective,0,0);
   interrupt(33,0,"Enter a color: \0",0,0);
   interrupt(33,1,color,0,0);
   interrupt(33,0,"Enter an animal: \0",0,0);
   interrupt(33,1,animal,0,0);
   interrupt(33,0,"Your note is on the printer, go get it.\r\n\0",0,0);
   interrupt(33,0,"Dear Professor O\'Neil,\r\n\0",1,0);
   interrupt(33,0,"\r\nI am so sorry that I am unable to turn in my program at this time.\r\n\0",1,0);
   interrupt(33,0,"First, I ate a rotten \0",1,0);
   interrupt(33,0,food,1,0);
   interrupt(33,0,", which made me turn \0",1,0);
   interrupt(33,0,color,1,0);
   interrupt(33,0," and extremely ill.\r\n\0",1,0);
   interrupt(33,0,"I came down with a fever of \0",1,0);
   interrupt(33,13,temp,1,0);
   interrupt(33,0,". Next my \0",1,0);
   interrupt(33,0,adjective,1,0);
   interrupt(33,0," pet \0",1,0);
   interrupt(33,0,animal,1,0);
   interrupt(33,0," must have\r\nsmelled the remains of the \0",1,0);
   interrupt(33,0,food,1,0);
   interrupt(33,0," on my computer, because he ate it. I am\r\n\0",1,0);
   interrupt(33,0,"currently rewriting the program and hope you will accept it late.\r\n\0",1,0);
   interrupt(33,0,"\r\nSincerely,\r\n\0",1,0);
   interrupt(33,0,"Kevin Lewis\r\n\0",1,0);
   while(1);
}

void printString(char* c, int d)
{

	if( d == 0){
		do{
			interrupt(16, 14*256 + *c, 0, 0, 0);
			c++;
		} while(*(c) != '\0');
		*c = '\0';
	}
	else if (d == 1){
		do{
			interrupt(23, *c, 0, 0, 0);
			c++;
		} while(*(c) != '\0');
		*c = '\0';
	}

   return;
}

char* readString(char str[]){
	char* c = str;
	char* start = c;
	while(1){
		*c = interrupt(22, 0, 0, 0);
		if(*c == 13){ //if enter is pressed

			//append null terminator to string
			*c = '\0';

			//output string on new line
			interrupt(16, 14*256 + '\0', 0, 0, 0);
			interrupt(16, 14*256 + '\r', 0, 0, 0);
			interrupt(16, 14*256 + '\n', 0, 0, 0);
			return str;	
		}
		if(*c == 8 && start != c){interrupt(16, 14*256 + *c, 0, 0, 0); c--;} //if backspace is pressed
		else if(c != 8) {interrupt(16, 14*256 + *c, 0, 0, 0); c++;} //print to screen
	}
}

void printLogo()
{
   interrupt(33,0,"       ___   `._   ____  _            _    _____   ____   _____ \r\n\0",0,0);
   interrupt(33,0,"      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0",0,0);
   interrupt(33,0,"     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0",0,0);
   interrupt(33,0,"    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0",0,0);
   interrupt(33,0,"   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0",0,0);
   interrupt(33,0,"._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0",0,0);
   interrupt(33,0," BlackDOS2020 v. 1.01, c. 2018. Based on a project by M. Black. \r\n\0",0,0);
   interrupt(33,0," Author: Kevin Lewis and Mikyla Wilfred\r\n\r\n\0",0,0);
}

/* MAKE FUTURE UPDATES HERE */
/* VVVVVVVVVVVVVVVVVVVVVVVV */

int mod(int a, int b) {
int x = a;
while (x >= b) x = x - b;
return x;
}

int div(int a, int b) {
int q = 0;
while (q * b <= a) q++;
return (q - 1);
}

void readInt(int* n){
	int num = 0;
    char str[80];
    char* c = readString(str);
    char* d = c;
   
    if(*d != '0'){
      while (*d != '\0') {
          num = num * 10;
          num = num + (*d - '0');
          d++;
      }
    }
    else{num = 0;}

    *n = num;

    writeInt(*n);
    
}

void writeInt(int x){
    int len = 0;
 
    char tmp[80];
    char* c = tmp;
    char tmp2[80];
    char* c2 = tmp2;

 
    // Check for zero
    if(x==0){interrupt(33,0,"0\0\r\n",1,0); return;}

    // Fill buffer with digit characters in reverse order.
    if(x != 0){
      while (x != 0) {
          *c = (char) (mod(x,10) + '0');
          x = div(x,10);
          c++;
          len++;
      }

      *c = '\0';

      c--;

      while(len > 0){
      	*c2 = *c;
      	c--;
      	c2++;
      	len--;
      }
      *c2 = '\0'; c2++; c2 = '\r'; c2++; c2 = '\n';
    }
    interrupt(33,0,tmp2,1,0);


}

/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   //return;
   switch(ax) { 
      case 0: printString(bx,cx); break;
      case 1: readString(bx); break;
      case 13: writeInt(bx); break;
      case 14: readInt(bx); break;
/*      case 2: case 3: case 4: case 5: */
/*      case 6: case 7: case 8: case 9: case 10: */
/*      case 11: case 12: case 15: */
	  default: printString("General BlackDOS error.\r\n\0");
   }  
}
