// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

// Our headers for the parser
#include "tokens.h"
#include "pl0-parser.h"

// Used for printing the token names
// for symbolic representation
struct {
	const char *num;
    const char *alpha;
} symbol[] = {
    { "1", "nullsym"}, { "2", "identsym"},
	{ "3", "numbersym"},{ "4", "plussym"},
	{ "5", "minussym"},{ "6", "multsym"},
	{ "7", "slashsym"},{ "8", "oddsym"},
	{ "9", "eqlsym"}, { "10", "neqsym"},
	{ "11", "lessym"},{ "12", "leqsym"},
	{ "13", "gtrsym"},{ "14", "geqsym"},
	{ "15", "lparentsym"},{ "16", "rparentsym"},
	{ "17", "commasym"},{ "18", "semicolonsym"},
	{ "19", "periodsym"},{ "20", "becomessym"},
	{ "21", "beginsym"},{ "22", "endsym"},
	{ "23", "ifsym"},{ "24", "thensym"},
	{ "25", "whilesym"},{ "26", "dosym"},
	{ "27", "callsym"},{ "28", "constsym"},
	{ "29", "intsym"},{ "30", "procsym"},
	{ "31", "writesym"},{ "32", "readsym"},
	{ "33", "elsesym"}, { NULL , NULL},};


// Stores the amount of tokens given by lexer
int program_length = 0;

// Stores the amount of rows in a generated table
int token_table_length = 0;

// This is the counter for the symbol table
int sym_cnt = 0;

// Code index when we emit code-gen
int cx = 0;

// Base address for variables
int m = 3;

// Global to keep track what index variable is in
int table_index = 0;

// File Pointers
FILE *input_file;
FILE *output_full;
FILE *output_slim;

int print_parser = 0;

// Functions as our main for the parser
int pl0_parser(int a_flag){
    
    print_parser = a_flag;
	    
    // Create the input file pointer
    input_file = fopen("lex_slim.txt", "r");
    
    // Check if the lexer created a file
    if(input_file != NULL){
   
            // This stores the program file with full detail
            output_full = fopen("parser_full.txt", "w");

            // This stores ONLY the code generation we need for VM
            output_slim = fopen("parser_slim.txt", "w");

            // Start up by loading lexeme file
            program_length = loadFile_parser(input_file);

            // Create a table of tokens from the lexeme list
            token_table_length = create_table();
        
            // Print our internal token rep. to full output
            print_internal();
        
            // Print our symbolic token rep. to full outoput
            print_symbolic();
         
            // Converts the strings in token_table to integers
            convert_table();	

            // Run the parser at program();
            program();
            
            // Uncomment this statement to print the symbol table
            // For debugging purposes...
            //print_symbol_table();
        
            // Print the Generated Machine Code for VM
            printf("\n");
            print_gen_code();
        
    }
    
    // The lexer never made a file, print error
    // to console...exit failure
    else{
        printf("\nParser Error\n");
        printf("Run ./compile -l before -a\n\n");
        exit(EXIT_FAILURE);
    }
	
    // Close all instances of input/output files
    fclose(input_file);
    fclose(output_full);
    fclose(output_slim);
    return 0;

}



/********************************************************
 Pre-Conditions: @params global input file pointer
 
 Post-Conditions: Loads the input file of the parser
 into a linear dynamic char array
 and returns amount of characters in the
 array.
 ********************************************************/

int loadFile_parser(FILE *input_file){
	
	int i;
	char character;
	data = malloc(sizeof(char));
	while(fscanf(input_file, "%c", &character) != EOF){
        
        data[i] = character;
        temp = realloc(data, (i+2)*sizeof(char));
        
        if(temp != NULL)
			data = temp;
		else{
			free(data);
			printf("Allocation error!");
		}
		i++;
	}
	return i;
}


/********************************************************
 Pre-Conditions: @params global token_table[][]
 
 Post-Conditions: Seperates each parser token into a table
                  each token put into a new row, in the table
                  and then returns the amount of rows that
                  were created.
 ********************************************************/

int create_table(){
    
    int index;
    int col;
    int row = 0;
    
    for(index = 0; index < program_length; index++){
        col = 0;
        
        // Splits identifiers into rows
        if(isalpha(data[index])){
            while(isalpha(data[index]) || isdigit(data[index])){
                token_table[row][col] = data[index];
                index++;
                col++;
            }
            token_table[row][col] = '\0';
            index--;
        }
        
        // Splits numbers into rows
        else if(isdigit(data[index])){
            while(isdigit(data[index])){
                token_table[row][col] = data[index];
                index++;
                col++;
            }
            index--;
        }
        
        else{row--;}
        row++;
    }
    return row;
}



/********************************************************
 Pre-Conditions: @params global token tokens[] struct
 
 Post-Conditions: All this does is take each token in our
                  created token table[][] that the function
                  above ^^ creates and converts each string
                  in that token table[][] to integers using
                  atoi(), and stores it into token tokens[]
 ********************************************************/

void convert_table(){
	
    int i;
    
    // Loop thru token table[][]
	for(i = 0; i < token_table_length; i++){
        
        // Convert each row, from strings to ints
        // Store into tokens[] struct
		tokens[i].token = atoi(token_table[i]);
	}
}



/********************************************************
 Pre-Conditions: @params global token table[][]
 
 Post-Conditions: Prints the internal token representation
                  RAW to the console/output files
 ********************************************************/
void print_internal(){
	
    int i;
    
    // Just prints headers
    print_headers_parser();
    
    // Raw token header
    fprintf(output_full, "1) Token Internal Representation:\n\n");
    
    // Go through our table with the string tokens and print
	for(i = 0; i < token_table_length; i++){
		fprintf(output_full, "%s ", token_table[i]);
	}
    
	printf("\n");
	printf("\n");
    
	fprintf(output_full, "\n");
	fprintf(output_full, "\n");
}




/********************************************************
 Pre-Conditions: @params global token table[][]
 
 Post-Conditions: Prints symbolic representation
                  SYMBOLIC form of the tokens. Uses first
                  struct in this file to map each integer
                  token to its symbolic counterpart.
 ********************************************************/

void print_symbolic(){
    
    int i;
    
    // Just print headers
    fprintf(output_full, "\n2) Token Symbolic Representation:\n\n");
    
    // Run through the table again this time matching it to struct symbol[]
    for(i = 0; i < token_table_length; i++){
        
        // Here we print identsym then the identifier after it...
        if(strcmp(token_table[i], "2") == 0){
            fprintf(output_full, "%s %s ", symbol[1].alpha, token_table[i+1]);
            i++;
        }
        
        // Here we print a numbersym then number after it...
        if(strcmp(token_table[i], "3") == 0){
            fprintf(output_full, "%s %s ", symbol[2].alpha, token_table[i+1]);
            i++;
        }
        
        // Every other token just gets printed as normal
        else{
            print_symbol_rep(token_table[i]); // Uses function below
        }
    }
    
    // Some new line in outputfile
    fprintf(output_full, "\n");
    fprintf(output_full, "\n");
}

/********************************************************
 Pre-Conditions: @params global symbol[] struct
                 and a string token, from token table[][]
 
 Post-Conditions: Maps each string token, to its symbolic
                  counterpart in first struct in the file.
                  Ex. string "29" -> prints -> "intsym"
 ********************************************************/

void print_symbol_rep(char *token){
	
	int i;
    
    // Goes through struct and converts each token to symbolic representation
	for(i = 0; symbol[i].num != NULL; i++){
		if(strcmp(token, symbol[i].num) == 0){
			fprintf(output_full, "%s ", symbol[i].alpha);
		}
	}
}



/********************************************************
 Pre-Conditions: @params global tokens token[] struct
 
 Post-Conditions: Gets a token from the struct stored in
                  the at table_index, then returns
                  this token to int token global, and increments
                  table_index counter.
 
                  
           NOTE:  table_index variable != tx, totally
                  different variables !!! as you will
                  see below
 ********************************************************/

int getToken(){
    token = tokens[table_index].token;
    table_index++;
    return token;
}





/********************************************************
 Pre-Conditions: @params None

 Post-Conditions: Starts Running Parser
 ********************************************************/

void program(){
	
	// Get the first token
	token = getToken();
	
	// Send it to the block function();
	// First 0 -> our lexi level
	// Second 0 -> our table index
	token = block(token, 0, 0);
	
	// If we never found a period at EOF, then ERROR
	if(token != periodsym){
		error(18);
		exit(EXIT_FAILURE);
	}
	
	// If we get here the program is correct -> emit a OPR_RET to finish
	else{
        emit(OPR, 0, OPR_RET);
        
        error(19); // No error here just did it to keep it simple
                  // This is a success!!!!
        
	}
		
}

/********************************************************
 Pre-Conditions: @params token, level, tx
			   token = our current token
			   level = lexigraphical level
			   tx = table index 
 			   Each of these 3 parameters are passed by program()
 			   The token is the first token, the lexi level starts at 0
			   and the table-index starts at 0

 Post-Conditions: Evaluates a Block
 ********************************************************/

int block(int token, int level, int tx){
	
    
    // String ptrs to hold const, int and proc identifiers
	char *const_name;
 	char *int_name;
	char *proc_name;
	
    
    // Emit space on stack for SL/DL/RA/Functional Ret. Val
	emit(INC, 0, 4);
	
    
    // Constant Declaration...
	if(token == constsym){
		
		do{
            
            // Identifier Symbol
			token = getToken();			
			if(token != identsym){
				error(1);
				exit(EXIT_FAILURE);
			}
			
            // Constant Identifier
			token = getToken();
			const_name = token_table[table_index-1];   // Search token_table[][]
			
            
            // Equal Symbol
			token = getToken();
			if(token != eqlsym){
				error(2);
				exit(EXIT_FAILURE);
			}
			
            
            // Number Symbol
			token = getToken();					
			if(token != numbersym){
				error(3);
				exit(EXIT_FAILURE);
			}
			
			// The number for the constant
            token = getToken();
            enter(CONST, const_name, token, 0, 0, tx++);
			
            // Should end at a semicolon...
			token = getToken();
				
		} while(token == commasym);
		
		if(token != semicolonsym){
			error(4);
			exit(EXIT_FAILURE);
		}
		
          // Next token...
		  token = getToken();
	  }
		
    
    // Integer Declaration....
	if(token == intsym){
		
        // Keep a count of the number of integers declared
		int variable_cnt = 0;
		
		do{
            // Identifier Symbol
			token = getToken();				
			if(token != identsym){
				error(5);
				exit(EXIT_FAILURE);
			}	
			
            // This is the name of the integer...
			token = getToken();
			int_name = token_table[table_index-1]; // Search token_table[][]
            enter(INT, int_name, 0, level, m++, tx++);

            // Each time we hit an int increment variable counter
			variable_cnt++;
		   
            // This should end at a semicolon...
			token = getToken();
							
		} while(token == commasym);
		
		if(token != semicolonsym){
			error(6);
			exit(EXIT_FAILURE);
		}
		
        // Now we emit space on the stack for the number of integer variables
		emit(INC, 0, variable_cnt);
		
        
        // Next token should be a procedure...
		token = getToken();
	}


    
    // Procedure Declaration...
	if(token == procsym){
		
        // Identifier Symbol
		token = getToken();		
		if(token != identsym){
			error(7);
			exit(EXIT_FAILURE);
		}
		
        // Next token is the procedure name
		token = getToken();
		proc_name = token_table[table_index-1]; // Search token_table[][]
		enter(PROC, proc_name, 0, level, cx+1, tx++);
        

        // Here we reset the address for ints and consts
        // Since we are in a nested procedure now...
		m = 3;
        
        // Should be end of procedure block with a semicolon
		token = getToken();		
		if(token != semicolonsym){
			error(8);
			exit(EXIT_FAILURE);
		}
		
        // Get the next token
		token = getToken();
		
        // Save current code index into c1
		int c1 = cx;
        
        // We emit an initial jump for proc
		emit(JMP, 0, 0);
        
        // Recursively call block and now increment level + 1
        // Since we are in another procedure now...
		token = block(token, level+1, tx);
		
        // Emit a return from the procedure...
		emit(OPR, 0, OPR_RET);
        
        // Save the jump location for the procedure
		assembly[c1].m = cx;
        
        
        // After calling block we should end at another semicolon
		if(token != semicolonsym){
			error(9);
			exit(EXIT_FAILURE);
		}
		
        // Next token is a statement
		token = getToken();	
	}
	
    // Call statement...
	token = statement(token, level, tx);
    
	return token;
}



/********************************************************
 Pre-Conditions: @params token, level, tx
                 token = our current token
                 level = lexigraphical level
                 tx = table index
                 Each of these 3 parameters are now passed by block();
 
 Post-Conditions: Evaluates a Statement
 ********************************************************/

int statement(int token, int level, int tx){
	
    // Name string ptr to hold identifier name
	char *name;
    
    
    // Identifier Symbol...
	if(token == identsym){
		
        // This is the name of the identifier
		token = getToken();
		name = token_table[table_index-1];
        
        // Find the name of the id in the symbol table, at its table index
		int i = find(0, tx, name);
		
        // Checks if the symbol exists in the table
        if(i == -1){
            error(21);
            exit(EXIT_FAILURE);
        }
        
        // The symbol has to be a integer value
        // If not then print error and exit
		if(sym[i].kind != 2){
            error(22);
            exit(EXIT_FAILURE);
		}
        
        // Next token should be a becomes after identifier
		token = getToken();			
		if(token != becomessym){
			error(10);
			exit(EXIT_FAILURE);	
		}
		
        // Get the next token, and call expression...
		token = getToken();	
		token = expression(token, level, tx);
		
        
        // Now we have to emit a store
		emit(STO, abs(sym[i].level - level), sym[i].addr);
	}
		
    
    // Call Symbol...
	else if(token == callsym){
        
        // Next token should be an identifier
		token = getToken();			
		if(token != identsym){
			error(11);
			exit(EXIT_FAILURE);
		}
		
        // Store the name of the identifier -> call has to "call" a procedure name
        // Should be a name of a procedure
		token = getToken();
		name = token_table[table_index-1];
		
        // Find the name of the function to call, located at tx
		int i = find(0, tx, name);
        
        // Emit a call to that procedure on that level
		emit(CAL, abs(sym[i].level - level), sym[i].addr);
		
        // Get the next token
		token = getToken();
	}
		
	
    // Begin Symbol...
	else if(token == beginsym){
		
        // Get the token, should be an identifier
		token = getToken();
        
        // Recurse back into statement()
		token = statement(token, level, tx);
		
        // Keep going until we reach a semicolon...
        // Keep getting tokens, and recursing statement();
		while(token == semicolonsym){
			token = getToken();				
			token = statement(token, level, tx);
		}
		
        // If we never reach an ends symbol -> ERROR!
		if(token != endsym){
			error(12);
			exit(EXIT_FAILURE);
		}
		
        
        // Get the next token...
		token = getToken();
	 }
	
    
    // If Symbol...
	else if(token == ifsym){
		
        // This should be a condition...
		token = getToken();
        
        // Call condition();
		token = condition(token, level, tx);
		
        // After condition if we dont have a "then" -> ERROR
		if(token != thensym){
			error(13);
			exit(EXIT_FAILURE);
		}
		
        // Get the next token
		token = getToken();
        
        // Save the current code index into c1
		int c1 = cx;
        
        // Emit a JPC
		emit(JPC, 0, 0);
		
        // Now we have to evaluate the statement for "if"
		token = statement(token, level, tx);
        
        // Set up an intial jump for an "else"
        // Save the address of else
		int c2 = cx;
        
        // Emit an intial jump
		emit(JMP, 0, 0);
		
        // Set our jump location, for the "if" statement
		assembly[c1].m = cx;
		
        // Get the next token after else, send to statement
		if(token == elsesym){
			token = getToken();
			token = statement(token, level, tx);
		}
		
        // Set our jump location, for the "else" statement
		assembly[c2].m = cx;
	}
	
    // While Symbol...
	else if(token == whilesym){
		
        // First save the current code index, right after "while" token
		cx1 = cx;
        
        // Get the next token, the "while" <condition>
		token = getToken();
        
        // Send to condition
		token = condition(token, level, tx);
        
        // Save the code index after the cond.
		cx2 = cx;
        
        // Emit a JMP for now...
		emit(JPC, 0, 0);
		
        // If we never see a "do" -> ERROR
		if(token != dosym){
			error(14);
			exit(EXIT_FAILURE);
		}
		
        // Next token after the "do"
		token = getToken();

        // Evaluate the statement
		token = statement(token, level, tx);
        
        // Emit a JMP to our first code index
		emit(JMP, 0, cx1);
        
        // Edit the address for the other JMP
		assembly[cx2].m = cx;
	}
    
    
    // Write Symbol...
	else if(token == writesym){
        
        // This is the expression to write
		token = getToken();
        
        // Send it to expression
		token = expression(token, level, tx);
        
        // Emit a Write operation
		emit(WRITE, 0, 1);
	}
	
	// Read Symbol...
	else if(token == readsym){
        
        // This is an identsym
		token = getToken();
        
		if(token == identsym){
			
            // Get the name of the identifier
			token = getToken();
			name = token_table[table_index-1];
			
            // Find in the symbol table
			int i = find(0, tx, name);
            
            // First emit a Read Operation
			emit(READ, 0, 2);
            
            // Then emit a store to store it...
			emit(STO, abs(sym[i].level - level), sym[i].addr);
            
            // Get next token
			token = getToken();
			
		}
    }
	return token;
}



/********************************************************
 Pre-Conditions: @params token, level, tx
                  token = our current token
                  level = lexigraphical level
                  tx = table index
                  Each of these 3 parameters are
                  now passed by the caller function();
 
 Post-Conditions: Evaluates a Condition
 ********************************************************/

int condition(int token, int level, int tx){
	
	// Holds a relational operator
	int relop;

    // Odd Symbol...
	if(token == oddsym){
		relop = token;
		token = getToken();		
	}
	
	// Any other relational symbol...
	else{
		
        // We evaluate expression after the relop
		token = expression(token, level, tx);
		
        // If it is not a relational operator -> ERROR
		if(token != gtrsym && token != lessym && token != leqsym && 
		   token != eqlsym && token != neqsym && token != geqsym){
			error(15);
			exit(EXIT_FAILURE);
		}
		
        // Store the relop, which is token
		relop = token;
        
        // Get the next token
		token = getToken();	
	}
    
        // Continue evaluating an expression
	    token = expression(token, level, tx);
		
        // We switch and emit the relational op
		switch(relop){
			
            // Equal
			case eqlsym: 
				emit(OPR, 0, OPR_EQL);
				break;
			
            // Not Equal
			case neqsym:
				emit(OPR, 0, OPR_NEQ);
				break;
            
            // Less Than
			case lessym:
				emit(OPR, 0, OPR_LSS);
				break;
			
            // Less than or Equal To
			case leqsym:
				emit(OPR, 0, OPR_LEQ);
				break;
			
            // Greater Than
			case gtrsym:
				emit(OPR, 0, OPR_GTR);
				break;
			
            // Greater than or Equal To
			case geqsym:
			     emit(OPR, 0, OPR_GEQ);
				break;
		}

	return token;
}


/********************************************************
 Pre-Conditions: @params token, level, tx
                  token = our current token
                  level = lexigraphical level
                  tx = table index
                  Each of these 3 parameters are
                  passed by the caller function();
 
 Post-Conditions: Evaluates an Expression
 ********************************************************/

int expression(int token, int level, int tx){
	
    // Stores an add operation
	int addop;
	
    // Plus or Minus
	if(token == plussym || token == minussym){
		addop = token;
		token = getToken();
        
        // Send to term...
		token = term(token, level, tx);
		
        // Negate
		if(addop == minussym){
			emit(OPR, 0, OPR_NEG);
		}
	}
	
    // Else send to term..., not a sub or add
	else{
		token = term(token, level, tx);
	}
	
    // While + or -
	while(token == plussym || token == minussym){
		
        addop = token;
		token = getToken();
        
        // Send to term
		token = term(token, level, tx);
		
        // If add op -> is (+)
		if(addop == plussym){
            
            // Emit -> addition
			emit(OPR, 0, OPR_ADD);
		}
		
        // Else emit -> subtraction
		else{
		    emit(OPR, 0, OPR_SUB);
		}
	}
	
	return token;
}


/********************************************************
 Pre-Conditions: @params token, level, tx
                 token = our current token
                 level = lexigraphical level
                 tx = table index
                 Each of these 3 parameters are
                 now passed by the caller function();
 
 Post-Conditions: Evaluates a Term
 ********************************************************/

int term(int token, int level, int tx){
	
    // Holds a mult or div op
	int mulop;
    
    // Send token to Factor
	token = factor(token, level, tx);
	
    // While (*) or (/)
	while(token == multsym || token == slashsym){
		
        mulop = token;
		token = getToken();
        
        // Send to factor
		token = factor(token, level, tx);
		
        // Multiply -> emit multiplication
		if(mulop == multsym){
			emit(OPR, 0, OPR_MUL);
		}
		
        // Divide -> emit division
		else{
		   emit(OPR, 0, OPR_DIV);
		}
	}
	
	return token;
}


/********************************************************
 Pre-Conditions: @params token, level, tx
                 token = our current token
                 level = lexigraphical level
                 tx = table index
                 Each of these 3 parameters are
                 now passed by the caller function();
 
 Post-Conditions: Evaluates a Factor
 ********************************************************/

int factor(int token, int level, int tx){
	
    
    // String pointer, holds an id name
	char *name;
    
    // Identifier Symbol...
	if(token == identsym){
        
        // Get the identifier -> store into name ptr
		token = getToken();
		name = token_table[table_index-1];	
		
        // Search symbol table for the identifier
		int i = find(0, tx, name);	
        
        // If the symbol doesn't exists in the table
        // print error and exit
        if(i == -1){
			error(21);
			exit(EXIT_FAILURE);
		}

        // If the kind of symbol is a constant
        // We load the constant literal to top of stack
		if(sym[i].kind == 1){
			emit(LIT, 0, sym[i].val);
		}	
		
        // If it is an integer identifier
        // We have to load the integer from its abs(scope - the current level)
        // Absolute Value, to avoid a negative level, range has to be 0 to 3 levels deep
		else if(sym[i].kind == 2){
			emit(LOD, abs(sym[i].level - level), sym[i].addr);
		}	
		
        // Get net token...
		token = getToken();
	}
	
    
    // Number Symbol...
	else if (token == numbersym) {
        
        // This token is the number literal
		token = getToken();
        
        // Just push number to top of the stack
		emit(LIT, 0, token);
        
        // Continue with next token
		token = getToken();
	}
	
    
    // Left Parentheses -> Right Parentheses
	else if(token == lparentsym){
        
        // Get next token...
		token = getToken();
        
        // Inside a open paren is an expression
        // Thus we send it to expression...and evaluate it
		token = expression(token, level, tx);
		
        // If expression is finished evaluating,
        // and no closing paren -> then ERROR
		if(token != rparentsym){
			error(16);
			exit(EXIT_FAILURE);
		}
		
        // Get next token
		token = getToken();
	}
	
    // If nothing, then there is a factor ERROR!
	else{
		error(17);
		exit(EXIT_FAILURE);
	}

	return token;
}


/********************************************************
 Pre-Conditions: @params op, l, m
                  op = operation
                  l = lexigraphical level
                  m = address of token
 
 Post-Conditions: Stores Generated Machine Code into 
                  the assembly structure
 ********************************************************/

void emit(int op, int l, int m){
	
    // Code Generation Overflow
	if(cx > CODE_SIZE){
        error(20);
		exit(EXIT_FAILURE);
	}
	
    // Else store assembly code...
	else{
		assembly[cx].op = op;
		assembly[cx].l = l;
		assembly[cx].m = m;
		cx++;
	}
}


/********************************************************
 Pre-Conditions: @params global assembly[] struct
 
 Post-Conditions: Prints out our generated machine code
                  to console/output files
 ********************************************************/

void print_gen_code(){
	
	int i;
    
    // If our flag command was "-a"
    if(print_parser == 1){
        printf("Parser Output:\n\n");
        printf("Generated Machine Code\n\n");
    }
    
    // Print to output full file
    fprintf(output_full, "\nParser Output:\n\n");
    fprintf(output_full, "Generated Machine Code\n\n");

    
    // Loop through assembly struct
	for(i = 0; i < cx; i++){
        
        // If our command "-a" flag
        if(print_parser == 1){
            
            // Print machine code to console
            printf("%d %d %d\n", assembly[i].op, assembly[i].l, assembly[i].m);
        }
        
        // Print machine code to full output file
        fprintf(output_full, "%d %d %d\n", assembly[i].op, assembly[i].l, assembly[i].m);
        
        // Save to a slimmer output for use with VM,
        // no text just machine code
		fprintf(output_slim, "%d %d %d\n", assembly[i].op, assembly[i].l, assembly[i].m);
	}
}



/********************************************************
 Pre-Conditions: @params from, to, name
                 from = where we are starting from in sym table
                 to = which value at the table index we need
                      to look up
                 name = the name of the symbol in the table
 
 Post-Conditions: Searches the symbol table backwards to 
                  find our symbol and returns the table index
                  if it doesn't find the symbol returns -1
 ********************************************************/

int find(int from,int to,const char *name){
    int i;
	for(i = to-1;i >= from;i--)
		if(!strcmp(name,sym[i].name))
			return i;
	return -1;
}




/********************************************************
 Pre-Conditions: @params kind, name, val, level, address, tx
                 kind = kind of symbol, 1 = const, 2 = int, 3 = proc
                 name = the name of the symbol
                 val = the value of the symbol if it exists
                 address = the address of the symbol
                 tx  = the table index, the index in the symbol table
 
 Post-Conditions: Enters all the information into the symbol table
                  and increments the symbol table counter
 ********************************************************/

void enter(int kind, char *name, int val, int level, int address, int tx){
	  sym[sym_cnt].kind = kind;
	  sym[sym_cnt].name = name;
	  sym[sym_cnt].val = val;
	  sym[sym_cnt].level = level;
	  sym[sym_cnt].addr = address;
	  sym[sym_cnt].tx = tx;
	  sym_cnt++;
}




/********************************************************
 Pre-Conditions: @params global symbol table sym[]
 
 Post-Conditions: Prints symbol table if necessary for left for
                  debugging purposes.
 ********************************************************/

void print_symbol_table(){
	
    int i;
	for(i = 0; i < sym_cnt; i++){
			printf("Kind: %d Name: %s Value: %d Level: %d Address: %d Table Index: %d\n",
			sym[i].kind, sym[i].name, sym[i].val, sym[i].level, sym[i].addr, sym[i].tx);
	}
}


/********************************************************
 Pre-Conditions: @params a valid error code integer
 
 Post-Conditions: Prints a parser error to console/output
                  file if the we get a parser error.
 ********************************************************/

void error(int error_code){
	
	switch(error_code){
		
		case 1:
		printf("Constant declaration must be followed by an identifier\n");
		fprintf(output_full, "Constant declaration must be followed by an identifier\n");
		break;
		
		case 2:
		printf("Constant identifier must be followed by an =\n");
		fprintf(output_full, "Constant identifier must be followed by an =\n");
		break;
		
		case 3:
		printf("Constant = must be followed by a number\n");
		fprintf(output_full, "Constant = must be followed by a number\n");
		break;
		
		case 4:
		printf("Constant declaration must end with a semicolon (;) \n");
		fprintf(output_full, "Constant declaration must end with a semicolon (;) \n");
		break;
		
		case 5:
		printf("Integer declaration must be followed by an identifier\n");
		fprintf(output_full, "Integer declaration must be followed by an identifier\n");
		break;
		
		case 6:
		printf("Integer declaration must end with a semicolon (;) \n");
		fprintf(output_full, "Integer declaration must end with a semicolon (;) \n");
		break;
		
		case 7:
		printf("Procedure declaration must be followed by an identifier\n");
		fprintf(output_full, "Procedure declaration must be followed by an identifier\n");
		break;
		
		case 8:
		printf("Procedure declaration must end with semicolon (;) \n");
		fprintf(output_full, "Procedure declaration must end with semicolon (;) \n");
		break;
		
		case 9:
		printf("Procedure block statement must end with a semicolon (;) \n");
		fprintf(output_full, "Procedure block statement must end with a semicolon (;) \n");
		break;
		
		case 10:
		printf("Becomes := missing in statement\n");
		fprintf(output_full, "Becomes := missing in statement\n");
		break;
		
		case 11:
		printf("Call must be followed by an identifier\n");
		fprintf(output_full, "Call must be followed by an identifier\n");
		break;
		
		case 12:
		printf("Begin declaration must be closed with an end\n");
		fprintf(output_full, "Begin declaration must be closed with an end\n");
		break;
		
		case 13:
		printf("If statement must be followed by then\n");
		fprintf(output_full, "If statement must be followed by then\n");
		break;
		
		case 14:
		printf("While condition must be followed by do\n");
		fprintf(output_full, "While condition must be followed by do\n");
		break;
		
		case 15:
		printf("Relational Operator is Expected\n");
		fprintf(output_full, "Relational Operator is Expected\n");
		break;
		
		case 16:
		printf("Left parentheses has not been closed by a right parentheses\n");
		fprintf(output_full, "Left parentheses has not been closed by a right parentheses\n");
		break;
		
		case 17:
		printf("Factor Error: Identifier, Left Parentheses or Number is Expected\n");
		fprintf(output_full, "Factor Error: Identifier, Left Parentheses or Number is Expected\n");
		break;
		
		case 18:
		printf("No Period at End of File\n");
		fprintf(output_full, "No Period at End of File\n");
		break;
		
		case 19:
            if(print_parser == 1){
                printf("No errors program is syntactically correct!\n");
            }
		fprintf(output_full, "No errors program is syntactically correct!\n");
		break;
		
		case 20:
		printf("Code Generation Overflow\n");
		fprintf(output_full, "Code Generation Overflow\n");
		break;
            
        case 21:
        printf("Undeclared Identifier\n");
        fprintf(output_full, "Undeclared Identifier\n");
        break;
            
		case 22:
        printf("Assignment to constant or procedure is not allowed.\n");
        fprintf(output_full, "Assignment to constant or procedure is not allowed.\n");
        break;

	}	
}


/********************************************************
 Pre-Conditions: @params None
 
 Post-Conditions: Just prints headers to console/output file
 ********************************************************/


void print_headers_parser(){
    
    if(print_parser == 1){
        printf("\n");
        printf("+------------------------------------------------------+\n");
        printf("\n\n");
        printf("\t\tParser Output\n");
        printf("\n\n");
        printf("+------------------------------------------------------+\n\n");
    }
    
    fprintf(output_full, "+------------------------------------------------------+\n");
	fprintf(output_full, "\n\n");
	fprintf(output_full, "\t\tParser Output\n");
	fprintf(output_full, "\n\n");
	fprintf(output_full, "+------------------------------------------------------+\n\n");

}
