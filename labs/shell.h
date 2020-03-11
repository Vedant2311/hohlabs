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
	static const int MAX_RET_VALUES=10;	//Max return values for factor
	static const int SPEED = 1;		//AFter how many iteration should a function yield?
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
	int errorCode = 0;	//0:No Error, -1: Input too big, 2: Output too big	##Though Not using rightnow can be used if needed
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
	
	//Task Pane	##Add to render state
	static const int MAX_TASKS=5;	//Max Possible tasks
	static const int MAX_TASKS_F=3;	//Max Possible tasks for a method
	static const int MAX_TASKS_P=10;	//Max Possible tasks for history
	//Struct to keep info about the tasks
	struct taskInfo_t{
		bool done = true;
		char* name;		//Name of the task
		int arg;		//Argument of the task
		int ret;		//Return Value of task
		int rets[10];	//Return Value for Factor Function
		int retN;		// The length of the Output
		bool error;
		// *** Starting the scheduler! Initialize it as False for all and make it true once a function is called
		bool startScheduler;
	};
	taskInfo_t prevTasks[MAX_TASKS_P];	//Array of Completed Tasks
	int tasksCompleted=0;
	//## Copy task form runnig to completed use MEMCOPY
/*	in scheduler check for running task and if not zero then schedule one form the running tasks in the funciton in fiber scheduler when done copy the struct to prevtasks*/
	taskInfo_t tasks[MAX_TASKS];	//Array of Running Tasks
	int tasksRunning=0;	//Number of tasks running
	/* NOt needed?
	taskInfo_t primeTasks[MAX_TASKS_F];	//Running Tasks of prime
	taskInfo_t fibTasks[MAX_TASKS_F];	//Running Tasks of fib
	taskInfo_t factorTasks[MAX_TASKS_F];	//Running Tasks of factor
	*/
	int primeRunning = 0;
	int fibRunning = 0;
	int factorRunning =0;
	
	// ### New variable for getting the index of a current function. To be incremented by one when anyone calls it
	int index = -1;	
	// ### For the current index getting scheduled
	int schedulerIndex;
	

	
	//Function Enable Variables
	bool factor = false;
	bool prime = false;
	bool fib = false;	
	
	bool coro_factor=false;
	bool coro_prime = false;
	bool coro_fib = false;
	
	bool fiber_factor =false;
	bool fiber_fib = false;
	bool fiber_prime = false;
	
	//For 2.3 Implementation
	bool fiber_factor_old=false;
	bool fiber_old=false;
	
	//Coroutine Variables
	bool coroutine=false;
	
	bool coroFactor_done=false;
	int coroFactor_ret=0;
	
	bool coroFib_done=false;
	int coroFib_ret=0;

	bool coroPrime_done=false;
	int coroPrime_ret=0;
	
	// Factor Fibre Variables
	bool fiber = false;
	
	// Factor Fibre
	bool fiberFactor_done=false;
	int fiberFactor_ret=0;
	
	// Fib Fibre variables
	bool fiberFib_done=false;
	int fiberFib_ret=0;
	
	// Prime Fibre variable
	bool fiberPrime_done= false;
	int fiberPrime_ret=0;
	
/*	// ### The different Index variables
	int totalFunc = 0;
	int totalFactor = 0;
	int totalFib = 0;
	int totalPrime = 0;
	*/

	
	//New Stuff above this add to render##
/*----------------------------------METHODS----------------------------------*/

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
	
	//Returns ptr to task struct if free else null
	int getSlot(int arg,char* name){
		for(int i=0;i<MAX_TASKS;i++){
			if(tasks[i].done){
				tasksRunning++;			//Increment the taskCounter
//Will set in fiber scheduler				tasks[i].done=false;	//##//##See if need to allocate new variable for free
				tasks[i].arg = arg;
				tasks[i].name=name;
				tasks[i].ret=0;
				tasks[i].retN=0;	//Necessary for errro handling
				tasks[i].error=false;
				return i;
			}
		}
		return -1;
	}
	//copies b to a ##Shayan ese hi kaam ho jaye normal = se
/*	void structCopy(taskInfo_t* a, taskInfo_t* b){*/
/*		a=b*/
/*	}*/
/*------Helper Functions End------*/

	//As Coroutine:Long Computation Task Function: Factors of a number
	void Factor(){	//##Put variables all as local??
									hoh_debug("rslen0:"<<intResult_Length);						
		if(intLength==0){
			// Nothing has been entered lol
			intResult[0] = 0;
			intResult_Length = 1;	
			
			intLength=0;	//Clean input

		}

		else{	
			int num = 0;

			int len = intLength;
			for(int i=0; i<len; i++){
				num = num + inputInt[i]*(pow(10,(len-i-1)));
			}
			hoh_debug(num);
			
			if(num==0){
				
				intResult_Length = 1;
				intResult[0] = 0;
				intLength=0;	//Clean input
			
			}
			
			else{				
				int outSize=0;
												hoh_debug("rslen0:"<<intResult_Length);						
				for (int i = 1; i <= num; i++){
			
					if(num%i==0){
						intResult[outSize]=i;
						outSize++;
					}
				}
									hoh_debug("rslen1:"<<intResult_Length);		
																	hoh_debug("rslen2:"<<intResult_Length);										
				hoh_debug("Outsize is " << outSize);
				if(outSize<=10){		
					hoh_debug("<10");				
					intResult_Length=outSize;
					intLength=0;	//Clean input
				}

				else{
					error = true;
					copy(errorMessage,error_outText,error_outTextL);
					hoh_debug("rslen3:"<<intResult_Length);						
					errorMessageL = error_outTextL;
					intLength=0;	//Clean input
				}
			}
			
		}
	}
	


	
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
	
	//Task Pane	##Add to render state
	static const int MAX_TASKS=10;	//Max Possible tasks
	static const int MAX_TASKS_F=3;	//Max Possible tasks
	int tasksRunning=0;	//Number of tasks running
	int tasksCompleted=0;
/*	//Struct to keep info about the previous tasks*/
	//Struct to keep info about the tasks
	struct taskInfo_t{
		bool done = true;
		char* name;		//Name of the task
		int arg;		//Argument of the task
		int ret;		//Return Value of task
		int rets[10];	//Return Value for Factor Function
		int retN;		// The length of the Output
		bool error;
		// *** Starting the scheduler! Initialize it as False for all and make it true once a function is called
		bool startScheduler;
	};
	
	taskInfo_t prevTasks[MAX_TASKS];	//Array of Previous Tasks
	
	//Function Done
	//Not Neede as always enter pressed
	//Coroutine Done
	bool coroFactor_done=false;
	//Fiber Done
	bool fiberFactor_done=false;

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

