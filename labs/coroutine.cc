#include "labs/coroutine.h"
//##Need ret if not remove from function signature and etc
void coroFactor(coroutine_t* pf_coro,f_t* pf_locals,int* pret,bool* pdone,shellstate_t* shellstate){	//##Put variables all as local??

	coroutine_t& f_coro = *pf_coro; // boilerplate: to ease the transition from existing code
	bool& done          = *pdone;

	//Get Local Variable iterator
	int& iter = pf_locals->iter;
	int& outSize = pf_locals->outSize;
	int& num = pf_locals->num;
	
	//Set up PC value for coroutine
	h_begin(f_coro);

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
				if(iter%1000==0)	//To Get better performance as yielding at every iteration very slow
					h_yield(f_coro);	//Pass control to Kernel
//				hoh_debug("After yield");
//				hoh_debug("NUM: "<<num);
//				hoh_debug("ITER: "<<iter);
			}
			hoh_debug("Outsize is " << outSize);
			if(outSize<=10){	
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
	done=true;
	h_end(f_coro);
}


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){

    //insert your code here
    if(shellstate.coro_factor){	//Called First Time
    	hoh_debug("In shellstate");
//		coroutine_t f_coro;
		coroutine_reset(f_coro);
//		f_t f_locals;
		shellstate.coroFactor_done = false;
//		coroFactor(&f_coro,&f_locals,&shellstate.coroFactor_ret,&shellstate.coroFactor_done,&shellstate);
//		f(&f_coro,&f_locals,&shellstate.coroFactor_ret,&shellstate.coroFactor_done);
		shellstate.coro_factor=false;
    }
    //Loop to pick unfinished execution
    if(!shellstate.coroFactor_done){
//    	hoh_debug(shellstate.coroFactor_ret);
//    	f(&f_coro,&f_locals,&shellstate.coroFactor_ret,&shellstate.coroFactor_done);
		coroFactor(&f_coro,&f_locals,&shellstate.coroFactor_ret,&shellstate.coroFactor_done,&shellstate);
    }
	
}


