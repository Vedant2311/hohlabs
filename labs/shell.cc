#include "labs/shell.h"
#include "labs/vgatext.h"

//Helper Functions
// Computes a^b
int pow(int a, int b){
	
	int val = 1;
	for(int i=0; i<b; i++){
		val = val*a;
	}
	return val;
}
// Copies int array
void copy(int* a, const int* b,int n){
	
	// To initialize
	for(int i=0; i<n; i++)
		a[i] = 0;	
	
	for(int i=0;i<n;i++)
		a[i]=b[i];	
}
// Copies char array
void copy(char* a, const char* b,int n){

	// To initialize
	for(int i=0; i<400; i++)
		a[i] = '\0';
	
	for(int i=0;i<n;i++)
		a[i]=b[i];	
}

//capitilize
void cap(char* a, const char* b,int n){

	// To initialize
	for(int i=0; i<400; i++)
		a[i] = '\0';
	
	for(int i=0;i<n;i++){
		if((b[i]>='a')&&(b[i]<='z'))
			a[i]=b[i]-32;
		else
			a[i]=b[i];
	}
}

int strcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

int len(int n){
	if(n==0){
		return 1;
	}
	int ans =0;
	while(n){
		n/=10;
		ans++;
	}
	return ans;
}
//Helper Functions End

//
// initialize shellstate
//
void shell_init(shellstate_t& state){

	state.id=0;
	copy(state.name , "Factor_Selected",15);
	
	state.intLength=0;
	state.intResult_Length=0;
	
	state.inputLength=0;
	state.outputLength=0;

	state.error=false;
	
	state.error_inText = "Character Limit Reached";
	state.error_inTextL = 23;
	state.error_outText = "Output too big";
	state.error_outTextL = 14;
		
	copy(state.inputString,"",0);
	state.keyPresses=0;
	
	state.mapLen = 0x39;
	copy(state.map,"X 1233567890-=\b\tqwertyuiop[]\n^asdfghjkl;\'`  zxcvbnm,./    ",0x39);
	
	//Initialize
	
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+
//      | keys     | z   |  x |  c |  v |  b |  n |  m |
//      +----------+-----+----+----+----+----+----+----+
//      | scancode | 4c  | 4d | 4e | 4f | 50 | 51 | 52 |
//      +----------+-----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t& stateinout){

	hoh_debug("Got: "<<unsigned(scankey));
	hoh_debug(stateinout.map[scankey]);
	stateinout.keyPresses++;	//increase 
	
	hoh_debug(stateinout.name);	//##
	if(!strcmp(stateinout.name,"Factor_Selected")){
	// Factor is Highlighted
		if (scankey==0x1c){
			// Enter
			copy(stateinout.name,"Factor_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		else if(scankey==0x48){
			// Up
			copy(stateinout.name,"Quit_Selected",13);
		}
		else if(scankey==0x50){
			// Down
			copy(stateinout.name,"Echo_Selected",13);
		}	
	}

	else if(!strcmp(stateinout.name,"Echo_Selected")){
		// Echo is Highlighted
		
		if(scankey==0x1c){
			// Enter
			copy(stateinout.name,"Echo_Menu",9);			
			
			stateinout.inputLength=0;			
			copy(stateinout.inputString,"",0);						
			
			stateinout.outputLength=0;
			copy(stateinout.outputString,"",0);						
			stateinout.error = false;
		}

		else if(scankey==0x48){
			// Up
			copy(stateinout.name , "Factor_Selected",15);
		}
		else if(scankey==0x50){
			//Down
			copy(stateinout.name,"Quit_Selected",13);
		}

	}	


	else if(!strcmp(stateinout.name,"Quit_Selected")){
	// Quit is Highlighted
		if(scankey==0x1c){
			// Enter
			copy(stateinout.name,"Credits",7);
		}

		else if(scankey==0x48){
			// Up
			copy(stateinout.name,"Echo_Selected",13);
		}
		else if(scankey==0x50){
			//Down
			copy(stateinout.name , "Factor_Selected",15);
		}

	}

	else if(!strcmp(stateinout.name,"Factor_Menu")){
	// We have selected Factor from the menu

		if(scankey==0x1c){
			// Enter
			stateinout.intResult_Length=0;	//Clean output
			stateinout.error=false;

			if(stateinout.intLength==0){
				// Nothing has been entered lol
				stateinout.intResult[0] = 0;
				stateinout.intResult_Length = 1;	
				
				stateinout.intLength=0;	//Clean input
			}

			else{	
				int num = 0;

				int len = stateinout.intLength;
				for(int i=0; i<len; i++){
					num = num + stateinout.inputInt[i]*(pow(10,(len-i-1)));
				}
				hoh_debug(num);
				
				if(num==0){
					
					stateinout.intResult_Length = 1;
					stateinout.intResult[0] = 0;
					stateinout.intLength=0;	//Clean input
				
				}
				
				else{				
					int outSize=0;
				
					for (int i = 1; i <= num; i++){
				
						if(num%i==0){
							stateinout.intResult[outSize]=i;
							outSize++;
						}
					}
					hoh_debug("Outsize is " << outSize);
					if(outSize<=10){						
						stateinout.intResult_Length=outSize;
						stateinout.intLength=0;	//Clean input
					}
					
					else{
						stateinout.error = true;
						copy(stateinout.errorMessage,stateinout.error_outText,stateinout.error_outTextL);
					    hoh_debug(stateinout.error_outText);						
						stateinout.errorMessageL = stateinout.error_outTextL;
						stateinout.intLength=0;	//Clean input
					}
				}
				
			}


		}

		else if(scankey==0x01){
			//Escape
			copy(stateinout.name , "Factor_Selected",15);
		}
		else if(scankey==0x0E&&stateinout.intLength>0){	//Backspace
			stateinout.intLength--;
			stateinout.intResult_Length=0;	//Clean output
			stateinout.error=false;
		}
		else if(scankey>=0x02&&scankey<=0x0b&&strcmp(stateinout.errorMessage,stateinout.error_inText)){	//respond only if within character limits
			// 1,2,3,4,5,6,7,8,9,0 key has been pressed
			stateinout.inputInt[stateinout.intLength]=(scankey-1)%10;	//extract digit
			stateinout.intLength++;
//			stateinout.intResult_Length=0;	//Clean output
			if(stateinout.intLength>=10){
				stateinout.error=true;
				hoh_debug(stateinout.error_inTextL);	//##				
				copy(stateinout.errorMessage,stateinout.error_inText,stateinout.error_inTextL);
				hoh_debug(stateinout.errorMessage);	//##				
				stateinout.errorMessageL = stateinout.error_inTextL;
				//don't clean input
			}
		}

	}

	else if(!strcmp(stateinout.name,"Echo_Menu")){
	// We have selected Echo from the menu
	// Note that the user would not know what is he typing. So we will hide it from him.

		if(scankey==0x1c){
			// Enter
			cap(stateinout.outputString,stateinout.inputString,stateinout.inputLength);
			stateinout.outputLength=stateinout.inputLength;
			// Here you would have obtained the required string as well as the string lenth
			// So just print the characters of stateinout.inputString[] from i=0 to i=stateinout.inputLength-1
			//stateinout.name="Echo_Result";
			stateinout.inputLength=0;	//Clear input
			stateinout.error =false;	//go out of error state
			hoh_debug(stateinout.inputString);
			hoh_debug(stateinout.outputString);
			hoh_debug("IN Len: "<<unsigned(stateinout.inputLength));
			hoh_debug("OUT Len: "<<unsigned(stateinout.outputLength));

		}

		else if(scankey==0x01){
			//Escape
			copy(stateinout.name,"Echo_Selected",13);
		}
		else if(scankey==0x0E&&stateinout.inputLength>0){	//Backspace
			stateinout.inputLength--;
			stateinout.outputLength=0;	//Clean output
			stateinout.error=false;			//get out of error state
		}
		// Input characters which can be parsed by our Algorithm
		else if(scankey&&(!stateinout.error)){
			// any other key has been pressed
			stateinout.inputString[stateinout.inputLength]=stateinout.map[scankey];
			stateinout.inputLength++;
			//Clean output
			stateinout.outputLength =0;
			if(stateinout.inputLength>=60){
				stateinout.error=true;
				copy(stateinout.errorMessage,stateinout.error_inText,400);
				stateinout.errorMessageL = stateinout.error_inTextL;	
			}
		}
	}
	
	else if (!strcmp(stateinout.name,"Credits")){
		// Prints our names
		if(scankey==0x01){
			//Escape
			copy(stateinout.name,"Quit_Selected",13);			
		}	
	
	}


}


//
// do computation
//
void shell_step(shellstate_t& stateinout){

  //
  //one way:
  // if a function is enabled in stateinout
  //   call that function( with arguments stored in stateinout) ;
//stateinout.args[0] = 5;
//stateinout.args[1] = 5;
  //
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
  	render.id = shell.id;
    copy(render.name,shell.name,400);
	// Inpur Parameters for Factor
	render.intLength = shell.intLength;
  	copy(render.inputInt,shell.inputInt,shell.intLength);

	
	// Output Parameters for Factor
	copy(render.intResult, shell.intResult,shell.intResult_Length);
	render.intResult_Length = shell.intResult_Length;

	// Input paramters for Echo	
	copy(render.inputString,shell.inputString,400);
	render.inputLength = shell.inputLength;
	
	// Output paramters for Echo	
	copy(render.outputString,shell.outputString,400);
	render.outputLength = shell.outputLength;
	
	//No. of key pressed
	render.keyPresses = shell.keyPresses;
	
	//error message and length
	render.error = shell.error;
	copy(render.errorMessage,shell.errorMessage,400);
	render.errorMessageL = shell.errorMessageL;

  
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
	//bool same = (a.name == b.name)&&(a.intLength==b.intLength)&&(a.inputLength==b.inputLength)&&(a.outputLength==b.outputLength);
	//same=same&&(a.intResult_Length==b.intResult_Length)&&(a.intResult==b.intResult)&&(a.keyPresses==b.keyPresses);
	//same=same&&(a.error==b.error)&&(a.errorMessage==b.errorMessage)&&(a.errorMessageL==b.errorMessageL);
	return (a.keyPresses==b.keyPresses);
}


static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberindec(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumber(int x,int y,  const int* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){


  // this is just an example:
  //
  // Please create your own user interface
  //
  // You may also have simple command line user interface
  // or menu based interface or a combination of both.
  //
	//Shell Dimensions and corner coordinates and color
	int shellX = 0;	//Starting X coordinate of shell
	int shellY = 0;	//Starting Y coordinate of shell
	int shellW = 80;	//Width
	int shellH = 25;	//Height
	int shellXf = shellX+shellW;	//Final X coord of shell
	int shellYf = shellY+shellH;	//Final Y coord of shell
	
	uint8_t shellFG = 0x4;	//Text color
	uint8_t shellBG = 0xF;	//Background color
	uint8_t highlFG = 0x4;	//Highlighted Foreground
	uint8_t highlBG = 0xD;	//Highlighted Background
	
	//Title bar and status bar specifications
	int titleH = 2;
	int statusH = 2;
	uint8_t titleFG = 0x4;	
	uint8_t statusFG = 0x4;	
	uint8_t titleBG = 0xD;
	uint8_t statusBG = 0xD;

	uint8_t Umargin = 4;

	//Menu Items
	char* factorText = "Factor";
	int factorTextL = 6;
	char* echoText = "Caps";
	int echoTextL = 4;
	char* quitText = "Credits";
	int quitTextL = 7;
	int Y = shellY+titleH+Umargin;
	int X=shellX;
	
	//Instructions
	char* escapeText = "Press <ESC> to go to Menu";
	int escapeTextL = 26;
	char* enterExecText = "Press <ENTER> to execute command";
	int enterExecTextL = 32;
	char* outputText = "Output: ";
	int outputTextL = 8;
	
	//static variables
	int maxDigits = 10;	//5 digit key presses are maximum
	


	
	
	//Clear screen
	fillrect(shellX,shellY , shellXf,shellYf, shellBG, shellFG,  w, h,vgatext_base);
	//Draw Tile Bar rectangle
	fillrect(shellX,shellY , shellXf,shellY+titleH, titleBG, titleFG,  w, h,vgatext_base);
	
	if(!strcmp(state.name,"Factor_Selected")){
		//Write text on title bar
		char* titleText = "Menu";
		int titleTextL = 4;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw whole menu*/
		//FACTOR
		drawtext((shellX+shellW/2)-titleTextL/2,Y, factorText,factorTextL, highlBG,highlFG, w,h,vgatext_base);
		Y++;
		//echo
		drawtext((shellX+shellW/2)-echoTextL/2,Y, echoText,echoTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//quit
		drawtext((shellX+shellW/2)-quitTextL/2,Y, quitText,quitTextL, shellBG,shellFG, w,h,vgatext_base);
	}
	
	else if(!strcmp(state.name,"Echo_Selected")){
		//Write text on title bar
		char* titleText = "Menu";
		int titleTextL = 4;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw whole menu*/
		//factor
		drawtext((shellX+shellW/2)-titleTextL/2,Y, factorText,factorTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//ECHO
		drawtext((shellX+shellW/2)-echoTextL/2,Y, echoText,echoTextL, highlBG,highlFG, w,h,vgatext_base);
		Y++;
		//quit
		drawtext((shellX+shellW/2)-quitTextL/2,Y, quitText,quitTextL, shellBG,shellFG, w,h,vgatext_base);
	}
	
	else if(!strcmp(state.name,"Quit_Selected")){
		//Write text on title bar
		char* titleText = "Menu";
		int titleTextL = 4;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw whole menu*/
		//FACTOR
		drawtext((shellX+shellW/2)-titleTextL/2,Y, factorText,factorTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//echo
		drawtext((shellX+shellW/2)-echoTextL/2,Y, echoText,echoTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//QUIT
		drawtext((shellX+shellW/2)-quitTextL/2,Y, quitText,quitTextL, highlBG,highlFG, w,h,vgatext_base);
	}
	//Enter pressed
	
	else if(!strcmp(state.name,"Factor_Menu")){
		//Write text on title bar
		char* titleText = "Factor";
		int titleTextL = 6;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw workspace*/

		char* enterText= "Enter Number: ";
		int enterTextL = 14;
		drawtext(shellX,Y,enterText,enterTextL, shellBG,shellFG, w,h,vgatext_base);
		X+=enterTextL;
		drawnumber(X,Y, state.inputInt, state.intLength, shellBG, shellFG,w,h, vgatext_base);
		
		//Print insctructions
		Y=shellYf-statusH;
		X=shellX;
		char* helpText = "This commands calculates all the factors of a given number";
		int helpTextL = 58;
		char* commaText = ", ";
		int commaTextL = 2;
		Y--;
		drawtext(X,Y, escapeText,escapeTextL, shellBG,shellFG, w,h,vgatext_base);
		Y--;
		drawtext(X,Y, enterExecText,enterExecTextL, shellBG,shellFG, w,h,vgatext_base);
		Y--;
		drawtext(X,Y, helpText,helpTextL, shellBG,shellFG, w,h,vgatext_base);
		Y--;
		//Showing result (results are stored in array of integers)
		Y-=state.intResult_Length;
		drawtext(X,Y, outputText,outputTextL, shellBG,shellFG, w,h,vgatext_base);	//Output: 
		
		X+=outputTextL;
		
		//error checking	
		if(state.error)
			drawtext(X,Y,state.errorMessage,state.errorMessageL,shellBG,shellFG, w,h,vgatext_base);
		else{
			const int* results = state.intResult;
			for(int i=0;i<state.intResult_Length;i++){	//print all the factors
				drawnumberindec(X,Y,results[i],state.maxLen,shellBG, shellFG,w,h, vgatext_base);
				Y++;
				//X+=len(results[i])+1;
				//drawtext(X,Y,commaText,commaTextL,shellBG,shellFG, w,h,vgatext_base);
			}
		}
	}
	else if(!strcmp(state.name,"Echo_Menu")){
		//Write text on title bar
		char* titleText = "Capitalize";
		int titleTextL = 10;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw workspace*/

		char* enterText= "Enter Text: ";
		int enterTextL = 12;
		drawtext(shellX,Y,enterText,enterTextL, shellBG,shellFG, w,h,vgatext_base);
		X+=enterTextL;
		drawtext(X,Y, state.inputString, state.inputLength, shellBG, shellFG,w,h, vgatext_base);
		
		//Print instructions
		Y=shellYf-statusH;
		X=shellX;
		char* helpText = "This commands changes the input to uppercase";
		int helpTextL = 44;
		
		Y--;
		drawtext(X,Y, escapeText,escapeTextL, shellBG,shellFG, w,h,vgatext_base);
		Y--;
		drawtext(X,Y, enterExecText,enterExecTextL, shellBG,shellFG, w,h,vgatext_base);
		Y--;
		drawtext(X,Y, helpText,helpTextL, shellBG,shellFG, w,h,vgatext_base);
		Y--;
		//Showing result (results is stored in char array)
		drawtext(X,Y, outputText,outputTextL, shellBG,shellFG, w,h,vgatext_base);	//Output: 
		X+=outputTextL;
		
		//error checking	
		if(state.error)
			drawtext(X,Y,state.errorMessage,state.errorMessageL,shellBG,shellFG, w,h,vgatext_base);
		else
			drawtext(X,Y,state.outputString, state.outputLength,shellBG,shellFG, w,h,vgatext_base);

	}
	
	else{	//Credits bar
		//Write text on title bar
		char* titleText = "Credits";
		int titleTextL = 7;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw workspace*/

		char* errorText = "Shayan Aslam Saifi & Vedant Raval";
		int errorTextL = 33;
		drawrect(shellX+10,shellY+5 , shellXf-10,shellYf-5, titleBG, titleFG,  w, h,vgatext_base);
		X=(shellX+shellXf)/2-errorTextL/2;
		Y=(shellY+shellYf)/2-2;
		drawtext(X,Y,errorText, errorTextL,shellBG,shellFG, w,h,vgatext_base);
		Y+=2;
		errorText = "2017CS10375 & 2017CS10366";
		errorTextL = 26;
		drawtext(X,Y,errorText, errorTextL,shellBG,shellFG, w,h,vgatext_base);
	}
	
	// status bar
	fillrect(shellX,shellYf-statusH , shellXf,shellYf, statusBG, statusFG,  w, h,vgatext_base);
	
	char* statusText = "Key Presses: ";
	int statusTextL = 13;
	
	X=shellX-statusTextL+shellW/2;
	drawtext(X,shellYf-statusH/2, statusText,statusTextL, statusBG,statusFG, w,h,vgatext_base);
	X+=statusTextL;
	drawnumberindec(X,shellYf-statusH/2, state.keyPresses,state.maxLen, statusBG,statusFG, w,h,vgatext_base);
	
}


//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

//static void drawtext2(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
//  for(int i=0;i<maxw;i++){
//    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
//    if(!str[i]){
//      break;
//    }
//  }
//}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

static void drawnumberindec(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%10);
    number=number/10;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

static void drawnumber(int x,int y, const int* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i]+48/*to covernvt to ascii*/,bg,fg,w,h,vgatext_base);
    //if(!str[i]){
     // break;
    //}
  }
}

