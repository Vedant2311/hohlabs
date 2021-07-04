
HoHLabs
========

Built a kernel from scratch with menu based interactive shell, incorporating coroutines as well as fibers, along with implementing non-preemptive and preemptive scheduling, and SPSC queue for core-core communications. 

The kernel was built on top of the [HoHlabs](https://hohlabs.github.io/) framework designed to create a kernel from scratch. The Entire work is divided into three main sections, which are subsequently built on the top of the previous one, and can be found in different branches of this repository.

## Lab-1: Shell

- **MMIO**: We program a memory mapped device while enhancing our kernel by adding the functionality to display “Hello, world!”.
- **PMIO**: We program an I/O mapped device while enhancing our kernel by adding debugging routines which will print debug messages to serial port.
- **Abstract MMIO/PMIO**: We look at one way of abstracting out details of mmio::read8 vs io::read8 while enhance our kernel by adding a simple keyboard driver.
- **kShell**: Created a menu-based interactive shell consisting of computational tasks (one of them being a 'long computational task') and string operations. It also has a status bar showing the number of key pressed so far

## Lab-2: Threads

- **Stackless coroutine**: Implemented asymmetric-stackless coroutines while enhancing our kernel to make it responsive to key presses while long computation task is running. Implemented the long computation task as a stackless coroutine using the given APIs and added a new menu item for the same.
- **Fibers**: Implemented the above functionality by making use of Fibers. Again, on button presses with a long task going on, the status bar should get updated.
- **Non-preemptive scheduling**: Made use of the principles of non-preemptive scheduling and fibers (opening the possibility of the use of stack) to support multiple long computational tasks
- **Preemption (Threads)**: Enhanced the fiber implementation by adding preemption. Programmed one-shot LAPIC timer to raise an interrupt after a specified time and wrote the trap handler that would switch the control back to the main stack.

## Lab-3: Concurrency

