#pragma once

#include "util/config.h"


//
// preempt_t : State for your timer/preemption handler
//
// Note:
//  We've one instance of core_t per each core.
//  To access this instance, you need to use %gs:0
//  (The entire kernel doesn't have any global/static variables)
//
// %gs:core_offset_preempt will point to start of preempt_t instance
//
// for example: 
// %gs:0 will return pointer to core_t
// %gs:core_offset_mainstack will return core_t::main_stack
// %gs:core_offset_preempt+0 will return core_t::saved_stack
//
// etc.
//
// See Definition of core_t in x86/main.h
//

struct preempt_t{
  // your data structure, if any
  addr_t saved_stack; //feel free to change it - provided as an example
  bool yielding = false;

};


//
// 
// This macro is being called from x86/except.cc
//
//
// _name: label name
// _f   : C function to be called 
//        ex: we may have to do send EOI to LAPIC or PIC etc.
//
#  define  _ring0_preempt(_name,_f)            \
  __asm(                                       \
      "  .text                            \n\t"\
      " " STR(_name) ":                   \n\t"\
      "  pushl %edx                       \n\t"\
      "  pushl %ecx                       \n\t"\
      "  pushl %eax                       \n\t"\
      "  call " STR(_f) "                 \n\t"\
      "  popl  %eax                       \n\t"\
      "  popl  %ecx                       \n\t"\
      "  popl  %edx                       \n\t"\
      "                                   \n\t"\
      "  # insert your code here          \n\t"\
      "                                   \n\t"\
      "  #___RACE Check___                \n\t"\
      "  pushl %eax                       \n\t"\
	  "  movl %gs: "STR(core_offset_preempt+4)", %eax 	\n\t"\
      "  test  $1, %eax	                  \n\t"\
      "  popl %eax		                  \n\t"\
      "  jne iret_toring0                 \n\t"\
      "                                   \n\t"\
      "  #___PROLOGUE____				  \n\t"\
      "  #Push Required Registers		  \n\t"\
      "  pushal		       				  \n\t"\
      "                                   \n\t"\
      "                                   \n\t"\
      "  #___BODY____					  \n\t"\
      "  pushl $1f                        \n\t"\
   	  "  movl %esp, %gs:"STR(core_offset_preempt+0) " 	\n\t"\
   	  "  movl %gs:"STR(core_offset_mainstack) ", %esp 	\n\t"\
   	  "  sti                              \n\t"\
      "  ret                              \n\t"\
      "                                   \n\t"\
      "1:	                              \n\t"\
      "  #___EPILOGUE____				  \n\t"\
      "                                   \n\t"\
  	  "  #_Pop Required Registers		  \n\t"\
      "  popal		       				  \n\t"\
      "  jmp iret_toring0                 \n\t"\
      )                                        \
/*

      "  #_Push FP/SIMD Registers		  \n\t"\
      "  mov %ebp, %esp					  \n\t"\
      "  sub $512, %esp					  \n\t"\
      "  and $0x0fffffff0, %esp			  \n\t"\
      "  FXSAVE (%esp)					  \n\t"\
      
      "  #Pop FP/SIMD Registers		  	  \n\t"\
      "  FXRSTOR (%esp)                   \n\t"\
      "  mov %esp, %ebp                   \n\t"\
can jump to iret label if race
push eax so that I can use that to do computaions

Notice that you need to create a space for 512 bytes, aligned at a 16-byte boundary to be able to execute the FXSAVE instruction.
#define stack_saverestore(from_stack,to_stack) do {                  \
 asm volatile(                                                       \
   "  pushl %%eax      \n\t"                                         \
   "  pushl %%ecx      \n\t"                                         \
   "  pushl %%ebp      \n\t"                                         \
   "  pushl $1f        \n\t"                                         \
   "                   \n\t"                                         \
   "  movl  %%esp,(%0) \n\t"                                         \
   "  movl  (%1),%%esp \n\t"                                         \
   "                   \n\t"                                         \
   "  ret              \n\t"                                         \
   "1:                 \n\t"                                         \
   "  popl %%ebp       \n\t"                                         \
   "  popl %%ecx       \n\t"                                         \
   "  popl %%eax       \n\t"                                         \
  :                                                                  \
  :"a" (&from_stack), "c"  (&to_stack)                               \
  :_ALL_REGISTERS, "memory"                                          \
 );                                                                  \
} while(false)
*/
// %gs:core_offset_preempt will point to start of preempt_t instance
//      "  push [ebp+eflags_offset]         \n\t"\???##
// for example: 
// %gs:0 will return pointer to core_t
// %gs:core_offset_mainstack will return core_t::main_stack
// %gs:core_offset_preempt+0 will return core_t::saved_stack
// %gs:core_offset_preempt+1 will return bool yielding
