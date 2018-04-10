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
/* Signed:____________Kevin Lewis______________ Date:__3-5-2018___        s*/
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2018.             */

void handleInterrupt21(int,int,int,int);
void printLogo();
char* readString(char str[]);
void readInt(int* n);
void writeInt(int x);
void readSector(char* buffer, int sector);
void writeSector(char* buffer, int sector);

void readFile(char* name, char* buffer, int* numberOfSectors);
void writeFile(char* name, char* buffer, int numberOfSectors);
void deleteFile(char* name);

void clearScreen(int bg, int fg);

void runProgram(char* name, int segment);
void stop(launchProgram(12288));

void error(int e);

int mod(int a, int b);
int div(int a, int b);


void main(){
	char buffer[512];
	makeInterrupt21();
	interrupt(33,2,buffer,258,0);
	interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
	printLogo();
	interrupt(33,4,"Shell\0",2,0);
	interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);
	while(1);
}

void printString(char* c, int d)
{

	if( d == 0){
		do{
			interrupt(16, 14*256 + *c, 0, 0, 0);
			c++;
		} while(*(c) != '\0');
	}
	else if (d == 1){
		do{
			interrupt(23, *c, 0, 0, 0);
			c++;
		} while(*(c) != '\0');
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

    //writeInt(*n);
    
}

void writeInt(int x){
    int len = 0; 
    char tmp[80];
    char* c = tmp;
    char tmp2[80];
    char* c2 = tmp2;
    // Check for zero
    if(x==0){interrupt(33,0,"0\0\r\n",0,0); return;}
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
    interrupt(33,0,tmp2,0,0);
}

void readSector(char* buffer, int sector){

	int relSecNo = mod(sector,18) + 1;
	int headNo = mod(div(sector,18), 2);
	int trackNo = div(sector,36);

	interrupt(19,2*256+1,buffer,trackNo*256+relSecNo,headNo*256+0);

}
void writeSector(char* buffer, int sector){
	int relSecNo = mod(sector,18) + 1;
	int headNo = mod(div(sector,18), 2);
	int trackNo = div(sector,36);

	interrupt(19,3*256+1,buffer,trackNo*256+relSecNo,headNo*256+0);

}
void clearScreen(int bg, int fg){
	int i;
	//for (i = 0; i < 24; i++){
		interrupt(33,0,"k\r\n",0,0);
	//}
	interrupt(16,512,0,0,0);

	if(bg > 0 && fg > 0 && bg <= 8 && fg <= 16){
		interrupt(16, 1536, (4096 * (bg - 1) + 256 * (fg - 1)), 0, 6223);
	}
}

void readFile(char* name, char* buffer, int* numberOfSectors){
	char* sector[512];
	char* c; char* f; char* b; char* p;
	int length = 0; int nameMatch = 0; int location = 0;
	int i, j, k, l = 0;
	int address = 0;
	
	//load sector into character array
	readSector(sector, 257);

	*numberOfSectors = 0;

	//match file name
	c = name;
	f = name;
	b = sector;

	length = 0;
	nameMatch = 0;
	location = 0;

	while(*c != '\0'){c++; length = length + 1;}	//get the length of the name

	for(i = 0; i < 16; i++){						//loop through 16 file positions
		if(nameMatch == 0){							//keep looking if match isn't found
			
			f = name;								//set f to name at the start of each new name location
			nameMatch = 1;							//assume a match until proven otherwise
			
			if(*b == 0){nameMatch = 0;}				//if first char is 0, there is no file at that location
			
			else{									//if a char is found, compare it to name
				for(j = 0; j < length; j++){		//compares char by char
					if(*b == *f){b++; f++;}			//if two chars match, continue comparing
					else{nameMatch = 0; b++;}		//if two chars do not match, skip to end of name location
				}

				for(k = 0; k < (8 - length); b++){k++;}	//increment b past padded zeroes
			}
		}
		if(nameMatch == 0){b = b + 24;}				//jump to the beginning of next name location
		else{										//if name is found, load sector by sector into buffer
			while(*b != 0){							//read each sector in the file to the buffer
				readSector(buffer + address, *b);
				address = address + 512;
				b++;
				*numberOfSectors = *numberOfSectors + 1;
			}
		}
	}

	if(nameMatch == 0){interrupt(33,15,0,0,0);}

}

void writeFile(char* name, char* buffer, int numberOfSectors){
	char* directory[512]; char* map[512]; char bufferTemp[512];
	int length = 0; int size;
	char* c = name; char* dir; char* n = name; char* mapLoc; char* writePoint; char* b = buffer;
	int i; int j; int k; int l; int m; int p; int nameMatch = 0; int count = 0; int sector; int found = 0;

	//Used when writing to sectors
	char* buf = buffer; int x = 0;

	//load the disk and map directory
	readSector(directory, 257);
	readSector(map, 256);

	//get the length of the name
	while(*c != '\0'){c++; length = length + 1;}

	//find a free directory or a duplicate
	dir = directory;
	for(i = 0; i < 16; i++){
		if(nameMatch == 1){error(1); i = 16;}
		else{
			if(*dir == 0 && found == 0){	//Checks if directory is empty
				sector = count;
				//copy name to this directory
				n = name;
				for(l = 0; l < length; l++){
					*dir = *n;
					n++; dir++;
				}
				for(m = 0; m < (8 - length); dir++){m++;}   //increments dir past padded zeroes
				writePoint = dir;
				dir = dir + 24;
				found = 1;

			} 
			else{											//compares names char by char
				count++; 
				nameMatch = 1;
				n = name;
				for(j = 0; j < length; j++){  
					if(*n == *dir){n++; dir++;}
					else{nameMatch = 0; dir++;}
				}
				for(k = 0; k < (8 - length); dir++){k++;}	//increments dir past padded zeroes
				dir = dir + 24;
			}
		}
	}

	//Search through map for open sectors
	mapLoc = map; count = 0;
	for(size = 0; size < numberOfSectors; size++){
		while(*mapLoc != 0 && count < 256){
			mapLoc++;
			count++;
		}
		//Set as FF on map
		*mapLoc = 255;

		//Write sector numbers to directory
		*writePoint = count;
		writePoint++;
		
		//Write to sector
		for(x = 0; x < 512; x++){
		bufferTemp[x] = *buf;
		buf++;
		}
		writeSector(bufferTemp, count);
	}

	if(count == 256){error(2);} //Error if there is not enough free sectors

	for(k = 0; k < (24 - numberOfSectors); k++){mapLoc++; *mapLoc = 0;} //Pad remaining locations with 0

	writeSector(directory, 257);
	writeSector(map, 256);

}
void deleteFile(char* name){
	char* directory[512]; char* map[512];
	int length = 0; int nameMatch = 0; int found = 0;
	int i; int j; int k; int l;
	char* n; char*c = name; char* dir; char* fb; char* m;

	//load disk directory and map
	readSector(directory, 257);
	readSector(map, 256);

	//get the length of the name
	while(*c != '\0'){c++; length = length + 1;}

	//look for file
	dir = directory;
	for(i = 0; i < 16; i++){
		if(*dir == 0){dir = dir + 32;} //Jump to next location if empty
		else if(found == 0){//compares names char by char 
			nameMatch = 1; found = 1;
			n = name;
			fb = dir; //first byte of correct name location
			for(j = 0; j < length; j++){
				if(*n == *dir){n++; dir++;}
				else{nameMatch = 0; found = 0; dir++;}
			}
			for(k = 0; k < (8 - length); dir++){k++;}	//increments dir past padded zeroes
			dir = dir + 24;
		}
	}
	if(nameMatch == 0){error(0);}
	else{
		*fb = 0; //Set first byte to zero
		fb = fb + 8;
		while(*fb != 0){
			m = map;
			for(l = 0; l < *fb; l++){m++;}
			*m = 0;
			fb++;
		}
	}

	writeSector(directory, 257);
	writeSector(map, 256);

}

void runProgram(char* name, int segment){
	char* buffer[4096];
	int baseLocation = 0;
	char* c; int i;

	readFile(name, buffer, 1);
	baseLocation = segment * 4096;
	c = buffer;
	for(i = 0; i < 4096; i++){
		putInMemory(baseLocation, i, *c);
		c++;
	}

	launchProgram(baseLocation);
}

void error(int e){
	switch(e){
		case 0: interrupt(33,0,"File not found.\r\n\0",0,0); writeInt(1); break;
		case 1: interrupt(33,0,"Duplicate or invalid file name.\r\n\0",0,0); writeInt(1); break;
		case 2: interrupt(33,0,"Disk full.\r\n\0",0,0); writeInt(2); break;
		default: interrupt(33,0,"General error.\r\n\0",0,0); break;
	}
}

void stop(){launchProgram(8192);}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   //return;
   switch(ax) { 
      case 0: printString(bx,cx); break;
      case 1: readString(bx); break;
      case 2: readSector(bx,cx); break;
      case 3: readFile(bx,cx,dx); break;
      case 4: runProgram(bx,cx); break;
      case 5: stop(); break;
      case 6: writeSector(bx,cx); break;
      case 7: deleteFile(bx); break;
      case 8: writeFile(bx,cx,dx); break;
      case 11: interrupt(25,0,0,0,0); break;
      case 12: clearScreen(bx,cx); break;
      case 13: writeInt(bx); break;
      case 14: readInt(bx); break;
      case 15: error(bx); break;

	  default: printString("General BlackDOS error.\r\n\0");
   }  
}
