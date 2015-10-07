// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

// Our headers for the lexer
#include "tokens.h"
#include "pl0-lex.h"

// Our array of invisible characters
const char invisible[] = { 32, '\t', '\n', '\r'};

// Our array of special symbols
const char special_symbols[] = {
	'+', '-', '*', '/', '(', ')', '=', ',', '<', '>', ';', ':', '.'
};

// Length of the amount of characters in the program
int length = 0;

// Amount of rows in our table, when we create one
int table_length = 0;

// Counter to struct lexicons list[], stores our lexeme
int cnt = 0;

// File Pointers
FILE *input_file;
FILE *output_full;
FILE *output_slim;

int print_lexer = 0;

// This serves as the main function for the lexer
int pl0_lex(int l_flag){
    
    print_lexer = l_flag;
    
    // Prompt user on console to enter input file
    char filename[20];
    printf("\n\nPlease enter the name of the source code file to be compiled:\n");
    scanf("%s", filename);
    
    // Open the filename, user inputs
    input_file = fopen(filename, "r");
   	
	// If the input file was valid...run program
	if(input_file != NULL)
	{
        // Creates a full output of the lexer
        output_full = fopen("lex_full.txt", "w");
        
        // Creates ONLY lex list for Parser use
        output_slim = fopen("lex_slim.txt", "w");

		// Load the input file, store the length
		length = loadFile_lex(input_file);
	
		// Create our token table, store the length
		table_length = make_table();
		
		// Run the lexical analyzer on the token table...
       	run_lexical_analyzer(table_length);
    	 
        // Here we loop through our token table....
        int i;
		
        // Lexeme Table, Lexeme and Token Type Header
		fprintf(output_full, "\n\nLexeme Table:\n\n");
		fprintf(output_full, "Lexeme\t\t\tToken Type\n");
		
		// Loop through the token table
		for(i = 0; i < table_length; i++){
			
			// If procedure print it this way then the token... 
			if(strcmp(token_table[i], "procedure") == 0){
				fprintf(output_full, "%s\t\t%d\n", token_table[i], list[i].token);
			}
			// If const print it this way then the token...
			else if(strcmp(token_table[i], "const") == 0){
				fprintf(output_full, "%s\t\t\t%d\n", token_table[i], list[i].token);
			}
			
			// Else just print it this way...
			else{
		 	  fprintf(output_full, "%s\t\t\t%d\n", token_table[i], list[i].token);
		    }
		}
	
	    // Print our lexeme list, after we print Lexeme Table
    	print_lexeme(table_length);
        
        // Clear table for parser...
        clear_token_table();

	}
	
	// Else if the input file doesn't exsist exit...
	else{
		// Print failure messages, exit failure!
		printf("\nERROR: File Not Found Exception!!!\n");
		printf("Case #1: Check the name of the input file\n");
        printf("Case #2: Check that the input file is in the same directory\n");
        printf("         as your source code\n");
		printf("\nRun Program Again!\n");
		printf("Exiting....\n\n");
		exit(EXIT_FAILURE);
	}
    
    
    // Close all instances of file pointers
    fclose(input_file);
    fclose(output_slim);
    fclose(output_full);
	
	return 0;
}




/********************************************************
 Pre-Conditions: @params input file
 
 Post-Conditions: Loads the program into a dynamic linear array
                  and returns the number of characters
 ********************************************************/

int loadFile_lex(FILE *input_file){
	
    // Variables
	int i;
	char character;	
	
	// Dynamically allocate and add
	data = malloc(sizeof(char));

    if(print_lexer == 1){
        printf("+------------------------------------------------------+\n");
        printf("\n\n");
        printf("\t\tLexical Analyzer Output\n");
        printf("\n\n");
        printf("+------------------------------------------------------+\n\n");
	}
    
    fprintf(output_full, "+------------------------------------------------------+\n");
	fprintf(output_full, "\n\n");
	fprintf(output_full, "\t\tLexical Analyzer Output\n");
	fprintf(output_full, "\n\n");
	fprintf(output_full, "+------------------------------------------------------+\n\n");
	
	// This is to print the original Source Program
	// This is just the header...
	fprintf(output_full, "Source Program:\n");

	// Loop until EOF, Scanning each character....
	while(fscanf(input_file, "%c", &character) != EOF){
		
		// Print the original characters to output file...
		fprintf(output_full, "%c", character);
		
	 	data[i] = character;
	
		// If we need more space for a valid character,
         // reallocate more memory for data[]
		temp = realloc(data, (i+2)*sizeof(char));
		
        if(temp != NULL)
			data = temp;
			
		// If we cant allocate for some reason free the memory
		else{
			free(data);
			printf("Allocation error!");
		}
		
		// Increment our counter
		i++;
	}
	
	// Return the number of chars we have...(our Program Length)
	return i;
}




/********************************************************
 
 Pre-Conditions: @ Global Parameters Program Length, Data Array[]
 
 Post-Conditions: Creates our token table, and returns the table size
 
 ********************************************************/

int make_table(){
   
   // Variables 
   int index;
   int row = 0;
   int col;
   int comment;
	
	// Index loops through our original data[] array
	// given by size of the program length with all characters
	for(index = 0; index < length; index++)
	{	
		
		// Set our column index to zero for 2-d token table[][]
		col = 0;
		
		// Identifiers or Reserved Words->(letter(letter|digit)*
		// If it is an alphabetic character in our data[]...
		if(isalpha(data[index])){
			
			// While it is still an alpha or digit
			while(isalpha(data[index]) || isdigit(data[index])){
				
				// Store in token table [row][col]
				token_table[row][col] = data[index];
				
				// Inc to next character in our data[]
				index++;
				
				// Increment our column
				col++;
				
				// This ensures our identifier is 11 characters max
				// While procesing our identifier and the column exceeds 11 characters,
				// print message and exit with failure!
				if(col > 11){
					printf("\nError 2: Identifier (%s...) is too long!\n", token_table[row]);
					printf("Identifier has to be 11 characters max.\n");
					printf("Exiting...\n");
					exit(EXIT_FAILURE);
				}
			}
            // Initialize our next row, next col...    
			token_table[row][col] = '\0';
		 
            // Backtrack if it isn't alpha or digit...
			index--;
		}
		
		
		// Numbers->(digit)*
		// If it is a digit in our data[]....
		else if(isdigit(data[index])){
			
			// While it is still a digit...
			while(isdigit(data[index])){
								
				// Store in our token table [row][col]
				token_table[row][col] = data[index];
				data[index] = '\0';
				
				// Inc to next character in our data[]
				index++; 
				
				// Increment our column
				col++;
				
			    // This ensures our number is 5 characters max
		        // While procesing our number and the column exceeds 5 characters,
			   // print message and exit with failure!
				if(col > 5){
					printf("\nError 3: Number (%s...) is too long!\n", token_table[row]);
					printf("Number has to be 5 digits max.\n");
					printf("Exiting...");
					exit(EXIT_FAILURE);
				}
				
				if(isalpha(data[index])){
					printf("Identifier cannot start with a number\n");
					exit(EXIT_FAILURE);
				}
			}
            
            // Backtrack if it isn't a number...
			index--;
		}
		
       // Special Symbols and Comment Checks...
       else{
        
         // We look for a '/*', beginning to a comment in data[]
		 if(data[index] == '/' && data[index+1] == '*'){
            
            // Comment variable points to index after the '*'
            int comment = index+1;
            
            // Loop until we find a trailing end comment '*/'
			while(!(data[comment] == '*' && data[comment+1] == '/')){
                
                // and inc. comment
			 	comment++;
			}
             
            // Once the loop terminates, we continue from this index+1
            // after the terminating '/' in a comment
			index = comment+1;
             
			continue;
		}
           
           
        /* The 4 else if statements below compress each multi-character symbols
           into one row, example ':' + '=' -> ":=" */
		
        // Becomes Symbol...
		else if(data[index] == ':' && data[index+1] == '='){
			token_table[row][0] = data[index];
			token_table[row][1] = data[index + 1];
			index++;
		}
		
        // Not Equal Symbol..
		else if(data[index] == '<' && data[index+1] == '>'){
			token_table[row][0] = data[index];
			token_table[row][1] = data[index+1];
			index++;
		}
        
        // Less than or Equal To Symbol...
		else if(data[index] == '<' && data[index+1] == '='){
			token_table[row][0] = data[index];
			token_table[row][1] = data[index+1];
			index++;
		}
		
        // Greater than or Equal To Symbol...
		else if(data[index] == '>' && data[index+1] == '='){
			token_table[row][0] = data[index];
			token_table[row][1] = data[index+1];
			index++;
		}
		
        // If its just a single character special symbol
        // Add it to the table...
		else if(is_special(data[index])){
			token_table[row][col] = data[index];
		}
		
        // Decrement rows, so no gaps are in between
		else{
			row--;
		}
           
        } // end else for special symbols
        
	    // increment rows, in body of for loop
        row++;
		   
	 } // end our for loop
    
	
    // Return the amount of rows, that stores each token in our table
	return row;
}




/********************************************************
 Pre-Conditions: Iterates through the generated token table
 
 Post-Conditions: Runs the Lexical Analyzer starting with
                  handle_reserved_words function()
 ********************************************************/

void run_lexical_analyzer(int token_table_length){
    
    // Loop through the rows of the token table[][]
	int i;
	for(i = 0; i < token_table_length; i++){

        // All we do is call handle reserved words....
		handle_reserved_words(token_table[i], list);
	}
}



/********************************************************
 Pre-Conditions: @params word, list
                 Takes in a pointer to a word in a row of our token table
 
 Post-Conditions: Adds to our token type to our lexeme list (list)
 ********************************************************/

void handle_reserved_words(char *word, struct lexicons list[]){
    
     // BEGIN...?
	if(strcmp(word, "begin") == 0){
		list[cnt].token = beginsym;
		cnt++;
	}
	
     // CONST...?
	else if(strcmp(word, "const") == 0){
		list[cnt].token = constsym;
		cnt++;
	}
	
     // CALL...?
	else if(strcmp(word, "call") == 0){
		list[cnt].token = callsym;
		cnt++;
	}
    
     // DO...?
	else if(strcmp(word, "do") == 0){
		list[cnt].token = dosym;
		cnt++;
	 }
	
     // ELSE...?
	else if(strcmp(word, "else") == 0){
		list[cnt].token = elsesym;
		cnt++;
	 }
	
     // END...?
	else if(strcmp(word, "end") == 0){
		list[cnt].token = endsym;
		cnt++;
	}
	
     // INT..?
	else if(strcmp(word, "int") == 0){
		list[cnt].token = intsym;
		cnt++;
	}
	
     // IF...?
	else if(strcmp(word, "if") == 0){
		list[cnt].token = ifsym;
		cnt++;
	}
	
	// ODD...?
	else if(strcmp(word, "odd") == 0){
		list[cnt].token = oddsym;
		cnt++;
	}
	
     // PROCEDURE...?
	else if(strcmp(word, "procedure") == 0){
		list[cnt].token = procsym;
		cnt++;
	}
	
     // READ...?
	else if(strcmp(word, "read") == 0){
		list[cnt].token = readsym;
		cnt++;
	}
	
	// THEN...?
	else if(strcmp(word, "then") == 0){
		list[cnt].token = thensym;
		cnt++;
	}
	
     // WHILE...?
	else if(strcmp(word, "while") == 0){
		list[cnt].token = whilesym;
		cnt++;
	}
	
     // WRITE...?
	else if(strcmp(word, "write") == 0){
		list[cnt].token = writesym;
		cnt++;
	}
	
     // If all fails, then it might be an identifier...
	else{
		handle_identifiers(word, list);
	}

}





/********************************************************
 Pre-Conditions: @params word, list
                 Takes in a pointer to a word in a row of our token table
 
 Post-Conditions: Adds to our token type to our lexeme list (list)
                  if it is an (IDENTIFIER OR A NUMBER)
 ********************************************************/

void handle_identifiers(char *word, struct lexicons list[]){
    
     // Check if the pointer to the row is an alpha
     // If it is it's an identifier...
	if(isalpha(*word)){
        
        // We store token identsym
		list[cnt].token = identsym;
        
        // Store in our struct block the identifier string
		list[cnt].idnumstring = word;
        
        // Set the flag of that struct block to 1
		list[cnt].flag = 1;
        
        // Increment Global counter
		cnt++;
	}
    
    
	// Same logic as above ^^
    // This time it's a number...
	else if(isdigit(*word)){
        
        // Store in our struct that its a number
		list[cnt].token = numbersym;
        
        // Store in our struct block the number string
		list[cnt].idnumstring = word;
        
        // Set the flag of that struct block to 1
		list[cnt].flag = 1;
        
        // Increment Global counter
		cnt++;
	}
	
   // If all fails, it might be a special symbol...
   else{
	  handle_special_symbols(word, list);
    }
}




/********************************************************
 Pre-Conditions: @params word, list
                 Takes in a pointer to a row in our token table
                 (PTR -> TOKEN TABLE -> ROW ->(SYMBOL))
 
 Post-Conditions: Adds to our lexemes list if it is a special symbol
 ********************************************************/

void handle_special_symbols(char *word, struct lexicons list[]){
	
     // PLUS...?
	if(strcmp(word, "+") == 0){
		list[cnt].token = plussym;
		cnt++;
	}
	
     // MINUS..?
	else if(strcmp(word, "-") == 0){
		list[cnt].token = minussym;
		cnt++;
	}
	
	// MULTIPLY...?
	else if(strcmp(word, "*") == 0){
		list[cnt].token = multsym;
		cnt++;
	}
    
     // SLASH...?
	else if(strcmp(word, "/") == 0){
	    list[cnt].token = slashsym;
		cnt++;
	}

     // NOT EQUAL TO...?
	else if(strcmp(word, "<>") == 0){
		list[cnt].token = neqsym;
		cnt++;
	}
	
     // BECOMES...?
	else if(strcmp(word, ":=") == 0){
		list[cnt].token = becomessym;
		cnt++;
	}
    
     // RIGHT PARENTHESES....?
	else if(strcmp(word, ")") == 0){
		list[cnt].token = rparentsym;
		cnt++;
	}
	
     // LEFT PARENTHESES....?
	else if(strcmp(word, "(") == 0){
		list[cnt].token = lparentsym;
		cnt++;
	}
	
	// LESS THAN...?
	else if(strcmp(word, "<") == 0){
		list[cnt].token = lessym;
		cnt++;
	}
	
     // LESS THAN OR EQUAL TO...?
	else if(strcmp(word, "<=") == 0){
		list[cnt].token = leqsym;
		cnt++;
	}
	
     // GREATER THAN...?
	else if(strcmp(word, ">") == 0){
		list[cnt].token = gtrsym;
		cnt++;
	}
	
     // GREATER THAN OR EQUAL TO....?
	else if(strcmp(word, ">=") == 0){
		list[cnt].token = geqsym;
		cnt++;
	}
	
     // COMMA....?
	else if(strcmp(word, ",") == 0){
		list[cnt].token = commasym;
		cnt++;
	}
	
     // SEMICOLON....?
	else if(strcmp(word, ";") == 0){
		list[cnt].token = semicolonsym;
		cnt++;
	}
	
     // PERIOD....?
	else if(strcmp(word, ".") == 0){
		list[cnt].token = periodsym;
		cnt++;
	}
	
    // EQUAL...?
	else if(strcmp(word, "=") == 0){
		list[cnt].token = eqlsym;
		cnt++;
	}
	
    // No identified Symbol
	else{
		printf("Unidentified Symbol\n");
        exit(EXIT_FAILURE);
	}
}




/********************************************************
 Pre-Conditions: @params the amount of rows in our token table
 
 Post-Conditions:  Prints the Tokens stored in our lexeme 
                   struct list[] to the output files
 ********************************************************/

void print_lexeme(int token_table_length){
    
    // Print Header Lexeme List for the output file...
    if(print_lexer == 1){
        printf("\nLexeme List\n\n");
    }
    
	fprintf(output_full, "\nLexeme List:\n\n");

    // Loop through our lexeme list array of structs[]
    int i;
    for(i = 0; i < token_table_length; i++){
		   		 
       	   // If the flag in the struct block is 0...
           if(list[i].flag == 0)
           {
            
               if(print_lexer == 1){
                 printf("%d ", list[i].token);
               }
               
               fprintf(output_full, "%d ", list[i].token);
 			   fprintf(output_slim, "%d ", list[i].token);
            
            }
            
            // Else if the flag in the struct block is 1...
            else if(list[i].flag == 1)
            {
                if(print_lexer == 1){
                  printf("%d ", list[i].token);
                  printf("%s ", list[i].idnumstring);
                }
                
                fprintf(output_full, "%d ", list[i].token);
                fprintf(output_full, "%s ", list[i].idnumstring);
				fprintf(output_slim, "%d ", list[i].token);
				fprintf(output_slim, "%s ", list[i].idnumstring);
            }
    }
    
    
    printf("\n");
}




/********************************************************
 Pre-Conditions: @params a valid character
 
 Post-Conditions: Returns 1 if is an invisible character, 0 if not
 ********************************************************/

int is_invisible(char character){
	
    int i;
	
	// Loop our const char invisible[]
	for(i = 0; i < 4; i++){
		if(character == invisible[i]){
			return 1;
		}
	}
	return 0;
}




/********************************************************
 Pre-Conditions: @params a valid character
 
 Post-Conditions: Return 1 if it is a special symbol, 0 if not
 ********************************************************/

int is_special(char character){
    
	int i;
    
	// Loop our const char special symbols[]
	for(i = 0; i < 13; i++){
        
		if(character == special_symbols[i]){
			return 1;
		}
	}
    
	return 0;
}



/********************************************************
 Pre-Conditions: @params global token_table[][]
 
 Post-Conditions: Clears the entire table, inorder to be used for parser
 ********************************************************/
void clear_token_table(){
    int i,j;
    for(i = 0; i < 2000; i++){
        for(j = 0; j < 12; j++){
            token_table[i][j] = '\0';
        }
    }
}
