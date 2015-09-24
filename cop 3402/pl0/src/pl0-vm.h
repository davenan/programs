// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

#ifndef _pl0_vm_h
#define _pl0_vm_h

//////////////////////////////////////////////////// C FILE HEADERS //////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


///////////////////////////////////////////////////// DEFINTIONS /////////////////////////////////////////////////////

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

////////////////////////////////////////////////// DATA STRUCTURES FOR VM ////////////////////////////////////////////

// Structure for instructions
struct instruction{
    int op;
    int l;
    int m;
	int line;  // Modified to carry line # with it
};

struct instruction IR;
struct instruction code[MAX_CODE_LENGTH];
extern const char* OP_CODE[];
extern const char* OPR_OP[];
int stack[MAX_STACK_HEIGHT];
int activation_records[MAX_LEXI_LEVELS];


////////////////////////////////////////////  GLOBAL VARIABLE DECLARATIONS ///////////////////////////////////////////

int instruction_cnt;
extern int BP;
extern int SP;
extern int PC;
extern int AR;

//////////////////////////////////////////// FUNCTION PROTOTYPES FOR VM //////////////////////////////////////////////

int pl0_vm(int a_flag);
int loadFile_vm(FILE *input_file);
void run();
void fetch();
void execute();
void oper_stack();
int base(int l, int base);
void initializeStack();
void printInitial();
void printInterpretedAssem();
void printStack(int control);
void print_PCBPSP();
void print_headers_vm();

#endif
