// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

#ifndef _pl0_lex_h
#define _pl0_lex_h

///////////////////////////// C FILE HEADERS ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///////////////////// DATA STRUCTURES FOR LEX /////////////////////////////////

struct lexicons{
	int token;
	char *idnumstring;
	int flag;
};
struct lexicons list[2000];
char *data;
char *temp;
char token_table[20000][12];


/////////////////////// GLOBAL VARIABLE DECLARATIONS //////////////////////////

extern const char invisible[];
extern const char special_symbols[];
extern int cnt;
extern int length;
extern int table_length;


////////////////////// FUNCTION PROTOTYPES FOR LEX /////////////////////////////

int pl0_lex(int l_flag);
int loadFile_lex(FILE *input_file);
int make_table();
int is_invisible(char character);
int is_special(char character);
void run_lexical_analyzer(int table_length);
void handle_reserved_words(char *word, struct lexicons list[]);
void handle_special_symbols(char *word, struct lexicons list[]);
void handle_identifiers(char *word, struct lexicons list[]);
void print_lexeme(int token_table_length);
void clear_token_table();


#endif
