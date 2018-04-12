#include "blackdos.h"

int strCmp(char* str1, char* str2);
int isUpper(char* c);

void copyFile(char* cmd);
void deleteFile(char* cmd);
void directory();
void echo(char* comment);
void lprint(char* filename, char* buffer);
void runFile(char* cmd);
void color(char* cmd);
void help();
void tweet(char* cmd);
void type(char* filename, char* buffer);

int main(){
	char command[80];
	char buffer[12288];

	while(1){
		PRINTS("blackdos ~(__^> \0");
		SCANS(command);
		//Reboot System
		if(strCmp(command, "boot") == 1){interrupt(33,11,0,0,0);}
		else if(strCmp(command, "cls")){interrupt(33,12,2,11,0);}
		else if(strCmp(command, "copy")){copyFile(command);}
		else if(strCmp(command, "del")){deleteFile(command);}
		else if(strCmp(command, "dir")){directory();}
		else if(strCmp(command, "echo")){echo(command);}
		else if(strCmp(command, "run")){runFile(command);}
		else if(strCmp(command, "help")){help();}
		else if(strCmp(command, "lprint")){lprint(command, buffer);}
		else if(strCmp(command, "setenv")){color(command);}
		else if(strCmp(command, "tweet")){tweet(command);}
		else if(strCmp(command, "type")){type(command, buffer);}
		else{PRINTS("Bad command\r\n\0");}
	}
}

void copyFile(char* cmd){
	char* f1; char* f2;
	char buffer[12288];
	int size;
	//get file name starting location
	while(*cmd != ' '){cmd++;}
	//set file 1
	f1 = cmd; f1++;
	f2 = f1;
	while(*f2 != ' '){f2++;}
	//set file 2
	cmd = f2;
	f2++;
	*cmd = '\0';

	if(isUpper(f1) == 1){interrupt(33,15,1,0,0); return;}
	if(isUpper(f2) == 1){interrupt(33,15,1,0,0); return;}

	//read file 1 into buffer and copy to file 2
	interrupt(33,3,f1,buffer,&size);
	interrupt(33,8,f2,buffer,size);

}

void deleteFile(char* cmd){
	char* file;
	char buffer[12288];
	//get file name starting location
	while(*cmd != ' '){cmd++;}
	cmd++;

	//deletes file in command
	if(isUpper(cmd) == 1){interrupt(33,15,1,0,0);}
	else{interrupt(33,7,cmd,0,0);}

}

int strCmp(char* str1, char* str2){
	int i; int found = 1;
	while(*str1 != '\0' && *str1 != ' '){
		if(*str1 != *str2){found = 0;}
		str1++; str2++;
	}
	if(*str2 != '\0' && *str2 != ' '){found = 0;}

	return found;
}

int isUpper(char* c){if (*c >= 'A' && *c <= 'Z'){return 1;} else {return 0;}}

void directory(){
	char* sector[512];
	char* c; char* name;
	int i = 0; int j = 0; int sectors; int totalSectors = 0;

	c = sector;

	interrupt(33,2,sector,257);

	for(i = 0; i < 16; i++){
		sectors = 0;
		if(*c == 0){c = c + 32;}
		else{
			name = c;
			PRINTS(name);
			PRINTS("\t");
			c = c + 8;
			while(*c != 0){
				c++; sectors++; totalSectors++;
			}
			PRINTN(sectors);
			PRINTS(" - Sectors \r\n");
			c = c + (24 - sectors);
		}
	}

	PRINTS("Sectors Used\r\n");
	PRINTN(totalSectors);
	PRINTS("\r\n");
	PRINTS("Sectors Remaining\r\n");
	PRINTN(255 - totalSectors);
	PRINTS("\r\n");
}

void echo(char* comment){
	char* echo;

	//get file name starting location
	while(*comment != ' '){comment++;}
	comment++;
	echo = comment;

	PRINTS(echo);
	PRINTS("\r\n");
}


void color(char* cmd){
	char* f1; char* f2; char* d;
	char buffer[512];
	int num = 0; 

	while(*cmd != ' '){cmd++;}
	f1 = cmd; f1++;
	f2 = f1;
	while(*f2 != ' '){f2++;}
	cmd = f2;
	f2++;
	*cmd = '\0';
	d = f2;
	if(*d != '0'){
		while (*d != '\0') {
		  num = num * 10;
		  num = num + (*d - '0');
		  d++;
		}
	}
    else{num = 0;}

	if(strCmp(f1, "fg") == 1){
		if(num > 0 && num <= 16){
			interrupt(33,2,buffer,258,0);
			interrupt(33,12,buffer[0]+1,num,0);
			buffer[1] = num - 1;	
			interrupt(33,6,buffer,258,0);		
			//read sector 258 from disk, edit it and write it back out	

		} else {PRINTS("Bad environment value.\r\n\0");}
		
	}
	else if(strCmp(f1, "bg")){
		if(num > 0 && num <= 8){
			interrupt(33,2,buffer,258,0);
			interrupt(33,12,num,buffer[1]+1,0);
			buffer[0] = num - 1;
			interrupt(33,6,buffer,258,0);
			//edit it and write it back out
			
		} else {PRINTS("Bad environment value.\r\n\0");}
		
	} else {PRINTS("Bad or missing command interpreter.  \r\n\0");}
	
}

void runFile(char* cmd){
	while(*cmd != ' ' && cmd != '\0'){cmd++;}
	cmd++;
	interrupt(33,4,cmd,4,0);
	boot();

}

void help(){
	PRINTS("------------------------\r\n");
	PRINTS("boot:\treboot the system\r\n");
	PRINTS("cls:\tclear the screen\r\n");
	PRINTS("copy [file1] [file2]:\tcopy file1 to file2\r\n");
	PRINTS("del [file]:\tdelete file\r\n");
	PRINTS("dir:\tprints complete file directory\r\n");
	PRINTS("echo [comment]:\toutputs comment\r\n");
	PRINTS("lprint [file]:\tprints file to printer\r\n");
	PRINTS("run [file]:\texecutes file\r\n");
	PRINTS("setenv fg [color]:\tchanges text color\r\n");
	PRINTS("setenv bg [color]:\tchanges the background color\r\n");
	PRINTS("tweet [file]:\tcreates file and prompts for text\r\n");
	PRINTS("type [file]:\tdisplay the file\r\n");
	PRINTS("------------------------\r\n");
	PRINTS("\r\n");
}

void tweet(char* cmd){
	char* tweet[140];
	//get file name starting location
	while(*cmd != ' '){cmd++;}
	cmd++;

	PRINTS("Enter a tweet: ");
	SCANS(tweet);

	if(isUpper(cmd) == 1){interrupt(33,15,1,0,0); return;}
	interrupt(33,8,cmd,tweet,1);

}

void lprint(char* filename, char* buffer){
	char* start = filename;
	int i = 0;
	while(*filename != ' '){filename++;}
	filename = filename + 1;
	interrupt(33,3,filename,buffer,3);
	interrupt(33,0,buffer,1,0);
	for(i = 0; i < 12288; i++){*buffer = '\0'; buffer++;}
	LPRINTS("\r\n\0");
}

void type(char* filename, char* buffer){
	int i = 0;
	while(*filename != ' '){filename++;}
	filename = filename + 1;
	interrupt(33,3,filename,buffer,0);
	interrupt(33,0,buffer,0,0);
	for(i = 0; i < 12288; i++){*buffer = '\0'; buffer++;}
	PRINTS("\r\n\0");
}
