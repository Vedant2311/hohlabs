#pragma once
#include "util/config.h"
#include "util/debug.h"

struct shellstate_t{

	/* 	:	Name Mapping
		0		Factor_Selected
		1		Echo_Selected
		2		Quit_Selected
		3		Factor_Menu
		4		Echo_Menu
		5		Quit_Menu	?? No such state
		6		Factor_Result
		7		Echo_Result
		8		Quit_Result	?? No such state
		9		Factor_Limit_Reached
		10		Echo_Limit_Reached
		11		Quit_Limit_Reached No such state
		
	*/	
	static const int maxLen = 400;	//400 max string len
	// General String and int Identifier
	char name[maxLen];
	int id;		//not in use currently
	

	// Inpur Parameters for Factor
	int inputInt[maxLen];//={0};
	int intLength=0;
	
	// Output Parameters for Factor
	int intResult[maxLen];
	int intResult_Length=0;

	// Input paramters for Echo	
	char inputString[maxLen]="";
	int inputLength=0;
	
	//Output parameters for Echo
	char outputString[maxLen]="";
	int outputLength=0;
	
	//No. of key pressed
	int keyPresses=0;

	//error flags
	bool error = false;	//input/output too big
	char errorMessage[maxLen] = "";
	int errorMessageL =0;
	
	//Errors
	int error_inTextL = 23;
	char* error_inText;
	
	int error_outTextL = 14;
	char* error_outText; 

	//can give flexible error messages
	//Key scan map
	char mapLen = 0x39;
	char map[0x39];
	//= {'X',' ','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n','^','a','s','d','f','g','h','j','k','l',';','\'','`','\0','z','x','c','v','b','n','m',',','.','/','\0','\0','\0',' '};
	//char* map = "  1233567890-=\b@qwertyuiop[]\n~asdfghjkl;\'   zxcvbnm,./       ";	//63
	
};

struct renderstate_t{

	/* 	:	Name Mapping
		0		Factor_Selected
		1		Echo_Selected
		2		Quit_Selected
		3		Factor_Menu
		4		Echo_Menu
		5		Quit_Menu	?? No such state
		6		Factor_Result
		7		Echo_Result
		8		Quit_Result	?? No such state
		9		Factor_Limit_Reached
		10		Echo_Limit_Reached
		11		Quit_Limit_Reached No such state
		
	*/	
	static const int maxLen = 400;	//400 max string len
	// General String and int Identifier
	char name[maxLen];
	int id;		//not in use currently
	

	// Inpur Parameters for Factor
	int inputInt[maxLen];//={0};
	int intLength=0;
	
	// Output Parameters for Factor
	int intResult[maxLen];
	int intResult_Length=0;

	// Input paramters for Echo	
	char inputString[maxLen]="";
	int inputLength=0;
	
	//Output parameters for Echo
	char outputString[maxLen]="";
	int outputLength=0;
	
	//No. of key pressed
	int keyPresses=0;
	
	//error flag, message and length
	bool error = false;	//input/output too big
	char errorMessage[maxLen] = "";
	int errorMessageL =0;
	
	//static variables
	int maxDigits = 10;	//5 digit key presses are maximum
	

/*	//Shell Dimensions and corner coordinates and color*/
/*	int shellX = 0;	//Starting X coordinate of shell*/
/*	int shellY = 0;	//Starting Y coordinate of shell*/
/*	int shellW = 80;	//Width*/
/*	int shellH = 24;	//Height*/
/*	int shellXf = shellX+shellW;	//Final X coord of shell*/
/*	int shellYf = shellY+shellH;	//Final Y coord of shell*/
/*	*/
/*	uint8_t shellFG = 0x4;	//Text color*/
/*	uint8_t shellBG = 0xF;	//Background color*/
/*	uint8_t highlFG = 0x4;	//Highlighted Foreground*/
/*	uint8_t highlBG = 0xD;	//Highlighted Background*/
/*	*/
/*	//Title bar and status bar specifications*/
/*	int titleH = 2;*/
/*	int statusH = 2;*/
/*	uint8_t titleFG = 0x4;	*/
/*	uint8_t statusFG = 0x4;	*/
/*	uint8_t titleBG = 0xD;*/
/*	uint8_t statusBG = 0xD;*/

/*	uint8_t Umargin = 4;*/

};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);
