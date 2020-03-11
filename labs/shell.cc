#include "labs/shell.h"
#include "labs/vgatext.h"

//##Remove fucntions from two palces keep at only one palce
/*-----	Helper Functions----*/
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

	//	Compares two strings
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

//Convert digit array to interger
int atoi(int *inputInt, int len){
	// Nothing has been entered lol	##Can Add Error for no input instead of returning 0
	if(len==0) return 0;
	
	int num = 0;
	for(int i=0; i<len; i++)
		num = num + inputInt[i]*(pow(10,(len-i-1)));
	return num;
}

/*------Helper Functions End------*/


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
	state.errorCode=0;
	state.error_inText = "Character Limit Reached";
	state.error_inTextL = 23;
	state.error_outText = "Too big";
	state.error_outTextL = 7;
		
	copy(state.inputString,"",0);
	state.keyPresses=0;
	
	state.mapLen = 0x39;
	copy(state.map,"X 1233567890-=\b\tqwertyuiop[]\n^asdfghjkl;\'`  zxcvbnm,./    ",0x39);
	
	//Task Pane initalizations
//	state.MAX_TASKS =5;	

	struct taskInfo_t{
		bool done = true;
		char* name;		//Name of the task
		int arg;		//Argument of the task
		int ret;		//Return Value of task
		int rets[10];	//Return Value fo Factor Function
	
	};
	//Initailze all tasks slots as free
	for(int i=0;i<state.MAX_TASKS;i++){
		state.tasks[i].done=true;
		state.tasks[i].retN=-1;	//-1 means the function calles is not factor
	}
	/*##NOt NEEDED	
	//Methods tasks initialize as true
	for(int i=0;i<state.MAX_TASKS_F;i++)
		state.primeTasks[i].done=true;
		state.fibTasks[i].done=true;
		state.factorTasks[i].done=true;
	*/
	state.tasksRunning = 0;	//Running Tasks
	state.tasksCompleted=0;
	
	state.primeRunning = 0;	
	state.fibRunning = 0;
	state.factorRunning =0;
	
	//--Method Switches
	state.factor=false;
	state.prime=false;
	state.fib = false;
	
	//--Coroutine Variables
	state.coroutine=false;
	
	// Factor
	state.coro_factor=false;
	state.coroFactor_done=true;
	state.coroFactor_ret=0;

	// Prime	
	state.coro_prime=false;
	state.coroPrime_done=true;
	state.coroPrime_ret=0;

	// Fib
	state.coro_fib=false;
	state.coroFib_done=true;
	state.coroFib_ret=0;
	
	//--Fiber Variables//
	state.fiber=false;

	// Factor
	state.fiber_factor=false;
	state.fiberFactor_done=true;
	state.fiberFactor_ret=0;

	// Prime
	state.fiber_prime=false;
	state.fiberPrime_done=true;
	state.fiberPrime_ret=0;

	// Fib	
	state.fiber_fib=false;
	state.fiberFib_done=true;
	state.fiberFib_ret=0;	

	//For 2.3 implementaion
	state.fiber_old=false;
	state.fiber_factor_old=false;
//	// ### The different Index variables
//	state.totalFunc=0;
//	state.totalFactor=0;
//	state.totalFib=0;
//	state.totalPrime=0;
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
	
	if(!strcmp(stateinout.name,"Prime_Selected")){
	// Prime is Highlighted
		if (scankey==0x1c){
			// Enter
			stateinout.fiber=true;
			copy(stateinout.name,"Prime_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		if (scankey==0x2e){	//Enable Coroutine if C pressed
			// C
//			stateinout.coroutine=true;
			stateinout.fiber=true;
			copy(stateinout.name,"Prime_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		if (scankey==0x21){	//Enable Fiber if F pressed
			// F
			stateinout.fiber=true;
			copy(stateinout.name,"Prime_Menu",11);
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
			copy(stateinout.name,"Fib_Selected",12);
		}	
	}
	//--
	else if(!strcmp(stateinout.name,"Fib_Selected")){
	// Fib is Highlighted
		if (scankey==0x1c){	//Enable Subroutine
			// Enter
			stateinout.fiber=true;
			copy(stateinout.name,"Fib_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		if (scankey==0x2e){	//Enable Coroutine if C pressed
			// C
//			stateinout.coroutine=true;
			stateinout.fiber=true;
			copy(stateinout.name,"Fib_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		if (scankey==0x21){	//Enable Fiber if F pressed
			// F
			stateinout.fiber=true;
			copy(stateinout.name,"Fib_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		else if(scankey==0x48){
			// Up
			copy(stateinout.name,"Prime_Selected",14);
		}
		else if(scankey==0x50){
			// Down
			copy(stateinout.name,"Factor_Selected",15);
		}	
	}
	//--
	else if(!strcmp(stateinout.name,"Factor_Selected")){
	// Factor is Highlighted
		if (scankey==0x1f){	//Enable fiber with scheduling
			// Enter
			copy(stateinout.name,"Factor_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		else if (scankey==0x2e){	//Enable Coroutine if C pressed
			// C
			stateinout.coroutine=true;
			copy(stateinout.name,"Factor_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		else if (scankey==0x21){	//Enable OLD Fiber if F pressed
			// F
			stateinout.fiber_old=true;
			copy(stateinout.name,"Factor_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		else if (scankey==0x1c){	//Enable subroutine if S pressed
			// S
			stateinout.fiber=true;
			copy(stateinout.name,"Factor_Menu",11);
			stateinout.intLength=0;
			stateinout.intResult_Length=0;
			stateinout.error = false;
		}
		else if(scankey==0x48){
			// Up
			copy(stateinout.name,"Fib_Selected",12);
		}
		else if(scankey==0x50){
			// Down
			copy(stateinout.name,"Echo_Selected",13);
		}	
	}
	//--
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
	//--
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
			copy(stateinout.name , "Prime_Selected",14);
		}

	}
	//--
	else if(!strcmp(stateinout.name,"Prime_Menu")){
	// We have selected Factor from the menu

		if(scankey==0x1c && stateinout.tasksRunning<stateinout.MAX_TASKS && stateinout.primeRunning<stateinout.MAX_TASKS_F){//Check if task slot available ##Show error if not available???
			// Enter only if Task Slot Available 
			
			//Fiber Call				
			if(stateinout.fiber){
				//Get the alloted task struct taskCounter incemented in the function
				int index = stateinout.getSlot(atoi(stateinout.inputInt,stateinout.intLength),"Prime: ");
				hoh_debug("Slot: "<<index);
				hoh_debug(stateinout.tasks[index].name);
//				for(int i=0;i<stateinout.intLength;i++)
//					hoh_debug("i:"<<stateinout.inputInt[i]);
				hoh_debug("In:"<<stateinout.tasks[index].arg);
				//## Can do error check here also?? for unavailable slots
				
				// ### Incrementing the Index variables
				stateinout.primeRunning++;
				stateinout.index = index;	//## FOr the index of the current task??				
				stateinout.fiber_prime=true;	//Call the Prime Fiber	
									
				stateinout.error=false;
			}
			//##Scrape all the useless if else can remove the below lines as only fiber allowed
			//Coroutine Call
			else if(stateinout.coroutine)
				stateinout.coro_prime=true;	//Call the Factor Coroutine
			//Normal Subroutine Call
			else
				stateinout.prime=true;			//Call the Factor Function
				
		}

		else if(scankey==0x01){
			//Escape
			stateinout.coroutine=false;		//Disable coroutine
			stateinout.fiber=false;			//Disable Fiber
			copy(stateinout.name , "Prime_Selected",15);
		}
		else if(scankey==0x0E&&stateinout.intLength>0){	//Backspace
			stateinout.intLength--;
			stateinout.intResult_Length=0;	//Clean output
			stateinout.error=false;
			stateinout.errorCode=0;
			copy(stateinout.errorMessage , "None",4);	//Remove Error from message ##Can keep error codes!!
		}
		else if(scankey>=0x02&&scankey<=0x0b&&stateinout.intLength<10){	//respond only if within character limits
			// 1,2,3,4,5,6,7,8,9,0 key has been pressed
			stateinout.inputInt[stateinout.intLength]=(scankey-1)%10;	//extract digit
			stateinout.intLength++;
//			stateinout.intResult_Length=0;	//Clean output
			if(stateinout.intLength>=10){
				stateinout.error=true;
				stateinout.errorCode=1;	//Input too long errorCode
				hoh_debug(stateinout.error_inTextL);	//##				
				copy(stateinout.errorMessage,stateinout.error_inText,stateinout.error_inTextL);
				hoh_debug(stateinout.errorMessage);	//##				
				stateinout.errorMessageL = stateinout.error_inTextL;
				//don't clean input
			}
		}

	}
	//--
	else if(!strcmp(stateinout.name,"Fib_Menu")){
	// We have selected Factor from the menu

		if(scankey==0x1c && stateinout.tasksRunning<stateinout.MAX_TASKS && stateinout.fibRunning<stateinout.MAX_TASKS_F){ //##Error in slot not available
			// Enter only if slot available
			
			//Fiber Call				
			if(stateinout.fiber){

				//Get the alloted task struct taskCounter incemented in the function
				int index = stateinout.getSlot(atoi(stateinout.inputInt,stateinout.intLength),"Fib: ");
				hoh_debug(stateinout.tasks[index].name);
				//## Can do error check here also?? for unavailable slots
				
				// ### Incrementing the Index variables
				stateinout.fibRunning++;
				stateinout.index = index;	//## FOr the index of the current task??				
				stateinout.fiber_fib=true;	//Call the fib Fiber	
								
			}
			
			//##Scrape all the useless if else can remove the below lines as only fiber allowed
			//Coroutine Call
			else if(stateinout.coroutine)
				stateinout.coro_fib=true;	//Call the Factor Coroutine
			//Normal Subroutine Call
			else
				stateinout.fib=true;			//Call the Factor Function
		}

		else if(scankey==0x01){
			//Escape
			stateinout.coroutine=false;		//Disable coroutine
			stateinout.fiber=false;			//Disable Fiber
			copy(stateinout.name , "Fib_Selected",15);
		}
		else if(scankey==0x0E&&stateinout.intLength>0){	//Backspace
			stateinout.intLength--;
			stateinout.intResult_Length=0;	//Clean output
			stateinout.error=false;
			stateinout.errorCode=0;
			copy(stateinout.errorMessage , "None",4);	//Remove Error from message ##Can keep error codes!!
		}
		else if(scankey>=0x02&&scankey<=0x0b&&stateinout.intLength<10){	//respond only if within character limits
			// 1,2,3,4,5,6,7,8,9,0 key has been pressed
			stateinout.inputInt[stateinout.intLength]=(scankey-1)%10;	//extract digit
			stateinout.intLength++;
//			stateinout.intResult_Length=0;	//Clean output
			if(stateinout.intLength>=10){
				stateinout.error=true;
				stateinout.errorCode=1;	//Input too long errorCode
				hoh_debug(stateinout.error_inTextL);	//##				
				copy(stateinout.errorMessage,stateinout.error_inText,stateinout.error_inTextL);
				hoh_debug(stateinout.errorMessage);	//##				
				stateinout.errorMessageL = stateinout.error_inTextL;
				//don't clean input
			}
		}

	}
	//--
	else if(!strcmp(stateinout.name,"Factor_Menu")){
	// We have selected Factor from the menu

		if(scankey==0x1c && stateinout.tasksRunning<stateinout.MAX_TASKS && stateinout.factorRunning<stateinout.MAX_TASKS_F){ //##Error in slot not available
			// Enter
			
			stateinout.intResult_Length=0;	//Clean output
			stateinout.error=false;
			
			//Fiber_scheduler Call				
			if(stateinout.fiber){

				//Get the alloted task struct taskCounter incemented in the function
				int index = stateinout.getSlot(atoi(stateinout.inputInt,stateinout.intLength),"Factor: ");
				hoh_debug(stateinout.tasks[index].name);
				//## Can do error check here also?? for unavailable slots
				
				// ### Incrementing the Index variables
				stateinout.factorRunning++;
				stateinout.index = index;	//## FOr the index of the current task??				
				stateinout.fiber_factor=true;	//Call the fib Fiber	
								
			}
			else if(stateinout.fiber_old){	//2.3 fiber factor implemenation
				stateinout.fiber_factor_old=true;	//Call the old fib Fiber									
			}
			else if(stateinout.coroutine)
				stateinout.coro_factor=true;	//Call the Factor Coroutine
			//Normal Subroutine Call	
			else
				stateinout.factor=true;			//Call the Factor Function
		}

		else if(scankey==0x01){
			//Escape
			stateinout.coroutine=false;		//Disable coroutine
			stateinout.fiber=false;			//Disable Fiber
			stateinout.fiber_old=false;
			copy(stateinout.name , "Factor_Selected",15);
		}
		else if(scankey==0x0E&&stateinout.intLength>0){	//Backspace
			stateinout.intLength--;
			stateinout.intResult_Length=0;	//Clean output
			stateinout.error=false;
			stateinout.errorCode=0;
			copy(stateinout.errorMessage , "None",4);	//Remove Error from message ##Can keep error codes!!
		}
		else if(scankey>=0x02&&scankey<=0x0b&&stateinout.intLength<10){	//respond only if within character limits
			// 1,2,3,4,5,6,7,8,9,0 key has been pressed
			stateinout.inputInt[stateinout.intLength]=(scankey-1)%10;	//extract digit
			stateinout.intLength++;
//			stateinout.intResult_Length=0;	//Clean output
			if(stateinout.intLength>=10){
				stateinout.error=true;
				stateinout.errorCode=1;	//Input too long errorCode
				hoh_debug(stateinout.error_inTextL);	//##				
				copy(stateinout.errorMessage,stateinout.error_inText,stateinout.error_inTextL);
				hoh_debug(stateinout.errorMessage);	//##				
				stateinout.errorMessageL = stateinout.error_inTextL;
				//don't clean input
			}
		}

	}
	//--
	else if(!strcmp(stateinout.name,"Echo_Menu")){
	// We have selected Echo from the menu

		if(scankey==0x1c){
			// Enter
			cap(stateinout.outputString,stateinout.inputString,stateinout.inputLength);	//##Move to shell_step?? and make factor like this?
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
	//--
	else if (!strcmp(stateinout.name,"Credits")){
		// Prints our names
		if(scankey==0x01){
			//Escape
			copy(stateinout.name,"Quit_Selected",13);			
		}	
	}
	//--ERROR STATE PRINTS IN CONSOLE
	else{
		// Prints our names
		hoh_debug("State Not Defined");
		if(scankey==0x01){
			//Escape
			copy(stateinout.name,"Quit_Selected",13);			
		}	
	}
	//Clear Task Pane whenever delete key pressed
	if(scankey==0x53)	//Delete key
		stateinout.tasksCompleted=0;
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

  //Factor Enabled	##DO we have to pass the arguments to the function???and not take from variables inside 'stateinout'
	if(stateinout.factor){	//##&&REMOVE LATER
		//Call Factor in the state
		stateinout.Factor();
		//Stop calling the function
		stateinout.factor=false;
		hoh_debug("erro:"<<stateinout.errorMessage);
		hoh_debug("erro:"<<stateinout.error);
	}

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
	
	//Task Pane
	render.tasksRunning=shell.tasksRunning;
	render.tasksCompleted=shell.tasksCompleted;
	//Copy Struct
	for(int i=0;i<shell.MAX_TASKS_P;i++){
		render.prevTasks[i].done = shell.prevTasks[i].done;
		render.prevTasks[i].name = shell.prevTasks[i].name;
		render.prevTasks[i].arg = shell.prevTasks[i].arg;
		render.prevTasks[i].ret = shell.prevTasks[i].ret;
		render.prevTasks[i].retN = shell.prevTasks[i].retN;
		for (int j=0;j<render.prevTasks[i].retN ;j++)
			render.prevTasks[i].rets[j] = shell.prevTasks[i].rets[j];
		render.prevTasks[i].error = shell.prevTasks[i].error;
	}
	//## Copy all that is required
	//Function Done
	//Not Needed as Always Key pressed

	//Coroutine Done
	render.coroFactor_done=shell.coroFactor_done;
	
	//Fiber Done
	render.fiberFactor_done=shell.fiberFactor_done;

  
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
	//bool same = (a.name == b.name)&&(a.intLength==b.intLength)&&(a.inputLength==b.inputLength)&&(a.outputLength==b.outputLength);
	//same=same&&(a.intResult_Length==b.intResult_Length)&&(a.intResult==b.intResult)&&(a.keyPresses==b.keyPresses);
	//same=same&&(a.error==b.error)&&(a.errorMessage==b.errorMessage)&&(a.errorMessageL==b.errorMessageL);
	//NO KEY PRESSED AND COROUTINE NOT DONE
	//Check if any function finished by checking taskCompleted
	return (a.keyPresses==b.keyPresses&&a.coroFactor_done==b.coroFactor_done&&a.fiberFactor_done==b.fiberFactor_done&&a.tasksCompleted==b.tasksCompleted);
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
	int shellW = 60;	//Width		80
	int shellH = 25;	//Height	25
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

	//Task Pane Dimensions and corner coordinates and Color
	int paneX= shellXf;
	int paneXf = 80;
	int paneY=	shellY;
	int paneYf=	shellYf;
	int paneW = paneXf-paneX;
	int paneH = shellH;
	
	uint8_t paneFG = 0x5;	//Text color
	uint8_t paneBG = 0xE;	//Background color
	uint8_t TPhighlFG = 0x5;	//Highlighted Foreground	## Can do for currently running tasks
	uint8_t TPhighlBG = 0xF;	//Highlighted Background	## Can do for currently running tasks
	
	//Title bar and status bar specifications for Task Pane ##
	uint8_t TPtitleFG = 0x3;	
	uint8_t TPstatusFG = 0x3;	
	uint8_t TPtitleBG = 0xD;
	uint8_t TPstatusBG = 0xD;
	
	//Writint Variables
	int PY = paneY+titleH;
	int PX = paneX;
	/*---Task Pane Specification END---*/

	//Menu Items
	char* primeText = "Prime";
	int primeTextL = 5;
	char* fibText = "Fibonacci";
	int fibTextL = 9;
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
	
	//Do same as above for task pane ## Can do at same time if of same color
	
	//Clear Screen for Task Pane and write text
	fillrect(paneX,paneY , paneXf,paneYf, paneBG, paneFG,  w, h,vgatext_base);
	fillrect(paneX,paneY , paneXf,paneY+titleH, TPtitleBG, TPtitleFG,  w, h,vgatext_base);
	char* titleText = "Input-Output";
	int titleTextL = 12;
	drawtext(paneX-titleTextL/2+paneW/2,paneY+titleH/2, titleText,titleTextL, TPtitleBG,TPtitleFG, w,h,vgatext_base);
	//Loop to print all the task history
	for(int i=0;i<state.tasksCompleted;i++){
		char* name = state.prevTasks[i].name;
		int nameL = 8;
		int arg = state.prevTasks[i].arg;
		int ret = state.prevTasks[i].ret;
		drawtext(PX,PY,name,nameL, TPhighlBG,TPhighlFG, w,h,vgatext_base);
		PX+=nameL;
		drawnumberindec(PX,PY,arg,state.maxLen,TPhighlBG, TPhighlFG,w,h, vgatext_base);
		PY++; PX=paneX;	//New Line
		drawtext(PX,PY, outputText,outputTextL, paneBG,paneFG, w,h,vgatext_base);	//Output:
		PX+=outputTextL; 
		
		//Print multiple output for factor will run only if retN>0
		for(int j=0;j<state.prevTasks[i].retN;j++){
			drawnumberindec(PX,PY,state.prevTasks[i].rets[j],state.maxLen,paneBG, paneFG,w,h, vgatext_base);
			PY++;
		}
		//if negative then toomany output error
		if(state.prevTasks[i].error){
			drawtext(PX,PY,state.errorMessage,state.errorMessageL,shellBG,shellFG, w,h,vgatext_base);
			PY++;
		}
		else{	//Noraml single figit output
			drawnumberindec(PX,PY,ret,state.maxLen,paneBG, paneFG,w,h, vgatext_base);
			PY++;
		}
		PX=paneX;	//New Line

	}
	
	
	//Task Pane End
	//Main Window
	hoh_debug("render: "<<state.name);
	if(!strcmp(state.name,"Prime_Selected")){
		//Write text on title bar
		char* titleText = "Menu";
		int titleTextL = 4;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw whole menu*/
		//Prime
		drawtext((shellX+shellW/2)-titleTextL/2,Y, primeText,primeTextL, highlBG,highlFG, w,h,vgatext_base);
		Y++;
		//Fib
		drawtext((shellX+shellW/2)-titleTextL/2,Y, fibText,fibTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//FACTOR
		drawtext((shellX+shellW/2)-titleTextL/2,Y, factorText,factorTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//echo
		drawtext((shellX+shellW/2)-echoTextL/2,Y, echoText,echoTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//quit
		drawtext((shellX+shellW/2)-quitTextL/2,Y, quitText,quitTextL, shellBG,shellFG, w,h,vgatext_base);
	}
	
	else if(!strcmp(state.name,"Fib_Selected")){
		//Write text on title bar
		char* titleText = "Menu";
		int titleTextL = 4;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw whole menu*/
		//Prime
		drawtext((shellX+shellW/2)-titleTextL/2,Y, primeText,primeTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//Fib
		drawtext((shellX+shellW/2)-titleTextL/2,Y, fibText,fibTextL, highlBG,highlFG, w,h,vgatext_base);
		Y++;
		//FACTOR
		drawtext((shellX+shellW/2)-titleTextL/2,Y, factorText,factorTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//echo
		drawtext((shellX+shellW/2)-echoTextL/2,Y, echoText,echoTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//quit
		drawtext((shellX+shellW/2)-quitTextL/2,Y, quitText,quitTextL, shellBG,shellFG, w,h,vgatext_base);
	}
	
	else if(!strcmp(state.name,"Factor_Selected")){
		//Write text on title bar
		char* titleText = "Menu";
		int titleTextL = 4;
		drawtext(shellX-titleTextL/2+shellW/2,shellY+titleH/2, titleText,titleTextL, titleBG,titleFG, w,h,vgatext_base);
		
		/* Draw whole menu*/
		//Prime
		drawtext((shellX+shellW/2)-titleTextL/2,Y, primeText,primeTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//Fib
		drawtext((shellX+shellW/2)-titleTextL/2,Y, fibText,fibTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
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
		//Prime
		drawtext((shellX+shellW/2)-titleTextL/2,Y, primeText,primeTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//Fib
		drawtext((shellX+shellW/2)-titleTextL/2,Y, fibText,fibTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
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
		//Prime
		drawtext((shellX+shellW/2)-titleTextL/2,Y, primeText,primeTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//Fib
		drawtext((shellX+shellW/2)-titleTextL/2,Y, fibText,fibTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//factor
		drawtext((shellX+shellW/2)-titleTextL/2,Y, factorText,factorTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//ECHO
		drawtext((shellX+shellW/2)-echoTextL/2,Y, echoText,echoTextL, shellBG,shellFG, w,h,vgatext_base);
		Y++;
		//QUIT
		drawtext((shellX+shellW/2)-quitTextL/2,Y, quitText,quitTextL, highlBG,highlFG, w,h,vgatext_base);
	}
	
	//Enter pressed
	else if(!strcmp(state.name,"Prime_Menu")){
		//Write text on title bar
		char* titleText = "nth Prime";
		int titleTextL = 9;
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
		char* helpText = "This commands calculates nth prime";
		int helpTextL = 35;
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
//			int result = 59;	//#See from where to get the result
//			drawnumberindec(X,Y,result,state.maxLen,shellBG, shellFG,w,h, vgatext_base);
//			Y++;
			
			const int* results = state.intResult;	//where is the result?## have a array of size three for al the functions 
			for(int i=0;i<state.intResult_Length;i++){	//print all the factors
				drawnumberindec(X,Y,results[i],state.maxLen,shellBG, shellFG,w,h, vgatext_base);
				Y++;
				//X+=len(results[i])+1;
				//drawtext(X,Y,commaText,commaTextL,shellBG,shellFG, w,h,vgatext_base);
			}
			
		}
	}
	
	else if(!strcmp(state.name,"Fib_Menu")){
		//Write text on title bar
		char* titleText = "Fibonacci";
		int titleTextL = 9;
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
		char* helpText = "This commands calculates nth fibonacci number";
		int helpTextL = 45;
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
//			int result = -1;	//#See from where to get the result
//			drawnumberindec(X,Y,result,state.maxLen,shellBG, shellFG,w,h, vgatext_base);
//			Y++;
			//Here output length in always one
			const int* results = state.intResult;	//where is the result?## have a array of size three for al the functions 
			for(int i=0;i<state.intResult_Length;i++){	//print all the factors
				drawnumberindec(X,Y,results[i],state.maxLen,shellBG, shellFG,w,h, vgatext_base);
				Y++;
				//X+=len(results[i])+1;
				//drawtext(X,Y,commaText,commaTextL,shellBG,shellFG, w,h,vgatext_base);
			}
			
		}
	}
	
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
					hoh_debug(state.errorMessage);
					hoh_debug(state.error);
		if(state.error){
			hoh_debug(state.errorMessage);
			drawtext(X,Y,state.errorMessage,state.errorMessageL,shellBG,shellFG, w,h,vgatext_base);
		}
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
	//##Write Instruction to 
	// status bar
	fillrect(shellX,shellYf-statusH , shellXf,shellYf, statusBG, statusFG,  w, h,vgatext_base);
	
	char* statusText = "Key Presses: ";
	int statusTextL = 13;
	
	X=shellX-statusTextL+shellW/2;
	drawtext(X,shellYf-statusH/2, statusText,statusTextL, statusBG,statusFG, w,h,vgatext_base);
	X+=statusTextL;
	drawnumberindec(X,shellYf-statusH/2, state.keyPresses,state.maxLen, statusBG,statusFG, w,h,vgatext_base);
	
	// Task Pane status bar
	fillrect(paneX,paneYf-statusH , paneXf,paneYf, TPstatusBG, TPstatusFG,  w, h,vgatext_base);
	
	statusText = "Running Tasks: ";
	statusTextL = 15;
	
	X=paneX-statusTextL+paneW/2;
	drawtext(X,paneYf-statusH/2, statusText,statusTextL, TPstatusBG,TPstatusFG, w,h,vgatext_base);
	X+=statusTextL;
	hoh_debug("running:"<<state.tasksRunning);
	uint32_t n=5;
	drawtext(X,paneYf-statusH/2, statusText,statusTextL, TPstatusBG,TPstatusFG, w,h,vgatext_base);
	drawnumberindec(X,paneYf-statusH/2-4,n,1, TPstatusBG,TPstatusFG, w,h,vgatext_base);
	
	
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

