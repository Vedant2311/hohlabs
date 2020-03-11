#include "labs/fiber.h"


void f(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone){
	
      addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
      addr_t& f_stack    = *pf_stack;
      int& ret           = *pret;
      bool& done         = *pdone;

      int i;
      int j;
	hoh_debug("F");
      for(i=1;i<=300;i++){
        for(j=1;j<=300;j++){
          ret=i*j;done=false; 
          hoh_debug("fstack ptr: "<<unsigned(pmain_stack));
          hoh_debug("mainstack ptr: "<<unsigned(pf_stack));
          hoh_debug("fstack: "<<f_stack);
          hoh_debug("mainstack: "<<main_stack);
          stack_saverestore(f_stack,main_stack);
        }
      }
      hoh_debug("FF");
      for(;;){
        ret=0;done=true; stack_saverestore(f_stack,main_stack);
      }
    }


// ### Factor function with the tast part implemented
// Function to get the factors of a given number    
void fiberFactor(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,shellstate_t* shellstate){	//##Put variables all as local??

	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;
	int& ret           = *pret;
	bool& done         = *pdone;
	
	int iter,outSize,num;

	hoh_debug(shellstate->intLength);
	if(shellstate->intLength==0){
		// Nothing has been entered lol
		shellstate->intResult[0] = 0;
		shellstate->intResult_Length = 1;	
		shellstate->intLength=0;	//Clean input
	}

	else{
		num = 0;
		int len = shellstate->intLength;
		for(int i=0; i<len; i++){
			num = num + shellstate->inputInt[i]*(shellstate->pow(10,(len-i-1)));
		}
		hoh_debug("Number: "<<num);
		
		if(num==0){
			shellstate->intResult_Length = 1;
			shellstate->intResult[0] = 0;
			shellstate->intLength=0;	//Clean input
		}
		
		else{		
				
			outSize=0;
			for (iter = 1; iter <= num; iter++){
		
				if(num%iter==0){
					shellstate->intResult[outSize]=iter;
					outSize++;
				}
				done=false;
//				hoh_debug(int(ret));
//				hoh_debug(int(iter));
//				hoh_debug(num);
				if(iter%shellstate->SPEED==0)	//To Get better performance as yielding at every iteration very slow
					stack_saverestore(f_stack,main_stack);	//Pass control to Kernel
//				hoh_debug("After yield");
//				hoh_debug("NUM: "<<num);
//				hoh_debug("ITER: "<<iter);
			}
			hoh_debug("Outsize is " << outSize);
			if(outSize<=10){	
				hoh_debug("outsize: "<<outSize);
				shellstate->intResult_Length=outSize;
				shellstate->intLength=0;	//Clean input
			}
			
			else{
				shellstate->error = true;
				shellstate->copy(shellstate->errorMessage,shellstate->error_outText,shellstate->error_outTextL);
				hoh_debug(shellstate->error_outText);						
				shellstate->errorMessageL = shellstate->error_outTextL;
				shellstate->intLength=0;	//Clean input
			}
		}
	}
	
	//END COROUTINE
	hoh_debug("END");
	for(;;){//##What is this infinite loop for?
		hoh_debug("in end loop: "<<done);
		ret=0;done=true; stack_saverestore(f_stack,main_stack);
	}
}


void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){

	//For the Factor code
	if(shellstate.fiber_factor_old){	//Called First Time for old implementaion only
        stack_init5(f_stack, f_array, f_arraysize, &fiberFactor, &main_stack, &f_stack, &shellstate.fiberFactor_ret, &shellstate.fiberFactor_done, &shellstate);
		shellstate.fiberFactor_done = false;
		shellstate.fiber_factor_old=false;
    }
    
    //Loop to pick unfinished execution
    if(!shellstate.fiberFactor_done){
        stack_saverestore(main_stack,f_stack); 
    }

}

