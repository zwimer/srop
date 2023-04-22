#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "colors.h"


// For customization
#ifndef CUSTOM
#define CUSTOM (1*8)
#endif


// Constants
#define LEN 0x4000
#define SYS_EXIT 60
#define SYS_SIGRETURN 15

// For printing
#define STR(x) STR_HELPER(x)
#define STR_HELPER(x) #x


// Gadgets
int set_eax() {  return SYS_SIGRETURN; }
void fire()   {     asm("syscall");    }


// Main function
int main() {
	printf ( "%s%s",
		"\n\n                       " KBOLDBLK "Welcome master hackers!" KNRM "\n\n\n"

		"If you have reached this challenge, you are a master of constructing\n"
		"rop chains! This challenge is an interesting twist on ROP called " KRED "SROP" KNRM "!\n"

		"\nWhat is SROP? SROP is a form of ROP using " KMAG "syscall 15" KNRM ": sigreturn.\n"
		"According to the man pages: \"sigreturn() exists only to allow the \n"
		"implementation of signal handlers.  It should never be called directly.\n"
		"(Indeed, a simple sigreturn() wrapper in the GNU C library simply\n"
		"returns -1, with errno set to ENOSYS.)\" I " KRED "STRONGLY" KNRM " urge you to read\n"
		"the man page for this: " KBLU "man 2 sigreturn" KNRM ".\n"

		"\nIn SROP, we call a sigreturn without ever having a signal set to\n"
		"the program (seriously, read " KBLU "man 2 sigreturn" KNRM ").\n"

		"\nWhy SROP? Consider this binary! Your end goal is to " KMAG "cat /flag" KNRM " or " KMAG "/bin/sh", KNRM "\n"
		"but you do not have the ability to set one or more of your registers as\n"
		"you need. Maybe you don't have enough gadgets or you just can't find one\n"
		"that gives you control of a particular register. With SROP, only " KRED "2" KNRM " gadgets\n"
		"are needed! One to set " KMAG "rax" KNRM " to " KMAG "15" KNRM
			", and one to do a " KMAG "syscall" KNRM ".\n"

		"\nBefore going into this more, I recommend doing a bit of reading on\n"
		"SROP at " KCYN "https://en.wikipedia.org/wiki/Sigreturn-oriented_programming" KNRM "\n"

		"\nBefore reading anymore, install " KMAG "pwntools" KNRM ". For SROP, this will make\n"
		"this exploit " KRED "MUCH" KNRM " easier! Run "
			KBLU "pip install pwntools" KNRM " to install them.\n"

		"\nHow SROP works: The hacker, you, ROPs to call " KMAG "syscall 15" KNRM " aka sigreturn.\n"
		"As you the man pages stated \"Using  the  information that was earlier\n"
		"saved on the userspace stack sigreturn() restores the process's signal\n"
		"mask, switches  stacks,  and restores the process's context (processor\n"
		"flags and registers, including the stack pointer and instruction pointer)\"\n"
		"That's right! The sigreturn syscall reads data from the stack then sets every\n"
		"register in the program from that data! " KRED "In other words, if we can control\n"
		"the stack and can execute a sigreturn we can set every register of the program" KNRM "!\n"

		"\nHere is an " KYEL "example" KNRM " visual of what the stack could look like after\n"
		KMAG "read(0, buf, " STR(LEN) ")" KNRM", addresses will vary:\n"

		"\n--------                     --------------------                                                \n"
		"0x7ffe00              " KCYN "rsp" KNRM " -> |                  |                                  \n"
		"--------                     --------------------                                                  \n"
		"0x7ffe08              " KBLU "buf" KNRM " -> |                  |                                  \n"
		"--------                     --------------------                                                  \n"
		"    .                        |         .        |                                                  \n"
		"    .                        |         .        |                                                  \n"
		"    .                        |         .        |                                                  \n"
		"--------                     --------------------                                                  \n"
		"0x7ffe28   " KBLU "return address " KNRM "-> |          " KGRN "--------" KNRM "|" KGRN "--------> " KYEL " gadget: " KRED "mov rax, 15; ret " KNRM "\n"
		"--------                     --------------------                                                  \n"
		"0x7ffe30                     |          " KGRN "--------" KNRM "|" KGRN "--------> " KYEL " gadget: " KRED "syscall" KNRM "\n"
		"--------                     --------------------                                                  \n"
		"0x7ffe08                     |                  |    \\                                            \n"
		"--------                     --------------------      \\                                          \n"
		"    .                        |         .        |        \\                                        \n"
		"    .                        |         .        |         | - " KBLU "Saved register values" KNRM "\n"
		"    .                        |         .        |        /                                         \n"
		"--------                     --------------------      /                                           \n"
		"0x7fff00                     |                  |    /                                             \n"
		"--------                     --------------------                                                  \n"

		"\nBut what exactly do we write to the stack? Remember, we are calling a\n"
		"sigreturn without receiving a corresponding signal. So we fake the signal\n"
		"by setting up the stack exactly as receiving a signal would. How is that?\n"
		KMAG "pwntools" KNRM " helps us out by doing the heavy lifting here! The function you\n"
		"are going to want to use is: " KBLU "pwn.SigreturnFrame(kernel='amd64')" KNRM ". I " KRED "strongly" KNRM "\n"
		"urge you to read up on it here: " KCYN "http://docs.pwntools.com/en/stable/rop/srop.html" KNRM "\n"

		"\nFinal hints:\n"
		"1. Do " KRED "not" KNRM " forget to set " KMAG "context.arch" KNRM ".\n"
		"2. This binary is not position independent.\n"
		"3. You may be able to find a useful string to get the flag in this block\n"
		"of text! It might just be " KMAG "magenta" KNRM " :)\n"
		"4. Your final payload should look something like this:\n"
		KMAG "payload = offset + gadget_to_set_rax_to_15 + syscall_gadget + bytes(my_frame)" KNRM "\n"
	);

	// Overflow
	char buf[CUSTOM];
	printf("\nNow reading %d bytes into buf. Good luck!\n", LEN);
	const int read_b = read(0, buf, LEN);
	printf("Read %d bytes. Returning!\n", read_b);
	return SYS_EXIT;
}
