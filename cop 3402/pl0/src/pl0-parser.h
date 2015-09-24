// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

#ifndef _pl0_parser_h
#define _pl0_parser_h

/////////////////////////////////////////////// C FILE HEADERS //////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


////////////////////////////////////////////// DEFINTIONS ///////////////////////////////////////////////////////////

#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_LEXI_LEVELS 3
#define CODE_SIZE 500

#define CONST 1
#define INT 2
#define PROC 3

///////////////////////////////////////////// DATA STRUCTURES FOR PARSER /////////////////////////////////////////////
struct symbol_table{
	int kind;
	char *name;
	int val;
	int level;
	int addr;
	int tx;
};

struct code_table{
    int     op;
    int     l;
    int     m;
};

struct token{
	int token;
};

typedef enum{
    LIT = 1, OPR = 2, LOD = 3,
    STO = 4, CAL = 5, INC = 6,
    JMP = 7, JPC = 8, WRITE = 9,
    READ = 10, OPR_RET = 0, OPR_NEG = 1,
    OPR_ADD = 2, OPR_SUB = 3, OPR_MUL = 4,
    OPR_DIV = 5, OPR_ODD = 6, OPR_MOD = 7,
    OPR_EQL = 8, OPR_NEQ = 9, OPR_LSS = 10,
    OPR_LEQ = 11, OPR_GTR = 12, OPR_GEQ = 13,
}isa_type;

char *data;
char *temp;
char token_table[20000][12];

// Stores integer forms of tokens
struct token tokens[500];

struct symbol_table sym[MAX_SYMBOL_TABLE_SIZE];
struct code_table assembly[MAX_SYMBOL_TABLE_SIZE];

////////////////////////////////////////// GLOBAL VARIABLE DECLARATIONS ////////////////////////////////////////////////


extern int program_length;
extern int token_table_length;
extern int sym_cnt;
extern int cx;
extern int m;
extern int table_index;

// This holds the current token;
int token;

// For jump locations
int cx1, cx2;


////////////////////////////////////////// FUNCTION PROTOTYPES FOR PARSER //////////////////////////////////////////////

int pl0_parser(int a_flag);

int loadFile_parser(FILE *input_file);
int create_table();
void print_internal();
void print_symbolic();
void print_symbol_rep(char *token);
void convert_table();

int getToken();
void program();
int block(int token, int level, int tx);
int statement(int token, int level, int tx);
int condition(int token, int level, int tx);
int expression(int token, int level, int tx);
int term(int token, int level, int tx);
int factor(int token, int level, int tx);
void emit(int op, int l, int m);
void enter(int kind, char *name, int val, int level, int address, int tx);
int find(int from,int to,const char *name);
void error(int error_code);
void print_gen_code();
void print_symbol_table();

void print_headers_parser();

#endif
