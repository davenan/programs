// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

// Our header for VM
#include "pl0-vm.h"


// Used to print the interpreted assembly
const char* OP_CODE[] = {
	"NULL", "LIT",
	"OPR",  "LOD",
    "STO", "CAL",
    "INC", "JMP",
    "JPC", "SIO",
    "SIO",
};

// Used to print the interpreted assembly
const char* OPR_OP[] = {
	"RET", "NEG",
	"ADD", "SUB",
	"MUL", "DIV",
	"ODD", "MOD",
	"EQL", "NEQ",
	"LSS", "LEQ",
	"GTR", "GTQ"
};

// Global Virtual Machine counters
int BP = 1;
int SP = 0;
int PC = 0;
int AR = 0;

int write = 0;
int read = 0;

// Save variable for CAL
int save = 0;

// File Pointers
FILE *input_file;
FILE *output_full;

int print_vm = 0;

// This serves as our main for the VM
int pl0_vm(int v_flag){
    
       print_vm = v_flag;
    
       // Open the generated machine code file from the parser
       input_file = fopen("parser_slim.txt", "r");
    
       // If the parser created an output, do the VM execution
       if(input_file != NULL){
           
            // Stores the full output of VM to an output file
            output_full = fopen("vm_full.txt", "w");
				
			// Initialize the entire stack = 0 for saftey...
			initializeStack();
			
			// Set stack 1,2,3 = 0
            stack[1] = 0;
			stack[2] = 0;
			stack[3] = 0;
			
		    // # of instructions returned by loadFile()
		    instruction_cnt = loadFile_vm(input_file);
		  	
			// Print interpreted instructions from input file
			printInterpretedAssem();
			
			// Print 2 new lines to seperate execution from interpreted Assembly
			fprintf(output_full, "\n\n");
			printf("\n\n");
			
			// Run the program...
			run();
    
            printf("\n\n");
       }
    
    // Else the parser never made a file, exit FAILURE
    // Print messages to console
    else{
        printf("\nVirtual Machine Error\n");
        printf("Run ./compile -a before -v\n\n");
        exit(EXIT_FAILURE);
    }
    
    // Close all instances of file pointers
    fclose(input_file);
    fclose(output_full);
	return 0;
}





/********************************************************
 Pre-Conditions: @params global stack[]
 
 Post-Conditions: Initialize each index in stack to 0
                  for saftey purposes
 ********************************************************/

void initializeStack(){
		int i;
		for(i = 0; i < MAX_STACK_HEIGHT; i++){
			stack[i] = 0;
		}
}



/********************************************************
 Pre-Conditions: @params input file pointer
                 Loads the program into an array of structs 
                 given by code[]
 
 Post-Conditions: returns the amount of instructions to process
 ********************************************************/

int loadFile_vm(FILE *input_file){
	
	// Loop counter and op code variables
	int i = 0;
	
	// Scan as long as we see an op code until end of file
	while(fscanf(input_file, "%d", &code[i].op) != EOF){
		fscanf(input_file, "%d%d", &code[i].l, &code[i].m);
		
		// Save the line number with the instruction
		code[i].line = i;
		
		// inc loop counter
		i++;
	}

	// Return i the amount of instructions
	return i;
}




/********************************************************
 Pre-Conditions: Runs the program from input file
 
 Post-Conditions: Generates the output of the execution 
                  of the program
 ********************************************************/

void run(){
	
	// Print our initial values
	printInitial();
	
	// Do->While to make sure our program executes at least once
	do{
	
        // Fetch and Execute code, while our PC is NOT
        // greater than our # of instructions
        fetch();
        execute();
        
        if(read == 1 && print_vm == 1){
            printf("\n(Read Operation) Enter number to push onto top of stack: ");
            fprintf(output_full, "\n(Read Operation) Enter number to push onto top of stack: ");
            scanf("%d", &stack[SP]);
            fprintf(output_full, "%d\n\n", stack[SP]);
            printf("\n");
            read = 0;
            print_PCBPSP();
            printStack(SP);
            
        }
        
        else if(write == 1 && print_vm == 1){
            print_PCBPSP();
            printStack(SP);
            printf("\n(Write Operation) Currently on the top of the stack: ");
            fprintf(output_full, "\n(Write Operation) Currently on the top of the stack: ");
            printf("%d\n\n", stack[SP]);
            fprintf(output_full, "%d\n\n", stack[SP]);
            write = 0;
        }
        
        else{
            print_PCBPSP();
            printStack(SP);
        }
        
    } while(BP != 0 && PC != 0); // So long as our BP != 0 or PC != 0
}



/********************************************************
 Pre-Conditions: @params global code[] struct
 
 Post-Conditions: Fetches code at PC counter and stores
                  into our instruction register IR, and 
                  then increments global PC counter.
 ********************************************************/
void fetch(){
	IR = code[PC];
	PC++;
}



/********************************************************
 Pre-Conditions: @params global instruction register IR
 
 Post-Conditions: Executes the code stored in the IR
 ********************************************************/

void execute(){
	
    // Switch IR.operand field..
    switch(IR.op){
		
		// LIT
		case 1:
			SP++;
			stack[SP] = IR.m;
			break;
		
		// OPR
		case 2:
			oper_stack();
			break;
		 
		// LOD -> (IR.m+1) for proper base
		case 3:			
		    SP++;
			stack[SP] = stack[base(IR.l, BP) + (IR.m+1)];
			break;
		
		// STO -> (IR.m+1) for proper base
		case 4:				
			stack[base(IR.l, BP) + (IR.m+1)] = stack[SP];
            SP--;
			break;
			
		// CAL
		case 5:
			stack[SP + 1] = 0; // space to return value
			stack[SP + 2] = base(IR.l, BP); // static link
			stack[SP + 3] = BP; // dynamic link
			stack[SP + 4] = PC;	// return address
			BP = SP + 1;
			PC = IR.m;
			
            // If our lexi levels surpass 4 <- accounts for main
			if(AR > MAX_LEXI_LEVELS){
				printf("OVERFLOW");  // Overflow
                exit(EXIT_FAILURE);
			}
            
            // Store the BP of the activation records
			activation_records[AR] = BP;
            
            // Inc. activation record
			AR++;
            
            // Save becomes 4...to print next activation record
            // In the call case
            save = 4;
			break;
		
            
		// INC	
		case 6:
			SP = SP + IR.m;
			break;
		
		// JMP
		case 7:
			PC = IR.m;
			break;
			
		// JPC
		case 8:
			if(stack[SP] == 0){
				PC = IR.m;			
			}				
			SP--;
			break;
		
		// SIO 0, 1 -> Write
		case 9:
			write = 1;
			SP--;
        break;

		// SIO 0, 2 -> Read
		case 10:
           read = 1;
		   SP++;
		 break;
	}
}



/********************************************************
 Pre-Conditions: @params global instruction register IR
 
 Post-Conditions: Computes OPR 0-13 on for stack execution
 ********************************************************/

void oper_stack(){
	 
	// Switch based on Instruction Registers M Field
    // for OPR 0, M instructions...
	switch(IR.m){
		
		// RET
		case 0:
			SP = BP - 1;
			PC = stack[SP + 4];
			BP = stack[SP + 3];
			
			// Return from our previous activation record
			activation_records[AR] = 0;
			AR--;
			break;
		
		// NEG
		case 1:
			stack[SP] *= -1;
			break;
		
		// ADD	
		case 2:
			SP--;
			stack[SP] = stack[SP] + stack[SP+1];
			break;
			
		// SUB	
		case 3:
			SP--;
			stack[SP] = stack[SP] - stack[SP+1];
			break;
		
		// MUL
		case 4:
			SP--;
			stack[SP] = stack[SP] * stack[SP+1];
			break;

		// DIV	
		case 5:
			SP--;
			stack[SP] = stack[SP] / stack[SP+1];
			break;
		
		// ODD	
		case 6:
			stack[SP] %= 2;
			break;
		
		// MOD
		case 7:
			SP--;
			stack[SP] = stack[SP] % stack[SP+1];
			break;
			
		// EQL	
		case 8:
			SP--;
			stack[SP] = stack[SP] == stack[SP+1];
			break;
			
		// NEQ
		case 9:
			SP--;
			stack[SP] = stack[SP] != stack[SP+1];
			break;
		
		// LSS
		case 10:
			SP--;
			stack[SP] = stack[SP] < stack[SP+1];
			break;
		
		// LEQ
		case 11:
			SP--;
			stack[SP] = stack[SP] <= stack[SP+1];
			break;
		
		// GTR
		case 12:
			SP--;
			stack[SP] = stack[SP] > stack[SP+1];
			break;
		
		// GEQ
		case 13:
			SP--;
			stack[SP] = stack[SP] >= stack[SP+1];
			break;
	}
}



/********************************************************
 Pre-Conditions: @params l = lexi level, 
                         base = Base pointer
 
 Post-Conditions: Returns the base of an activation record
 ********************************************************/
// Find the base for LOD, STO instructions L levels down
int base(int l, int base){
	  
  int b1; 
  b1 = base; 
  while (l > 0){
    b1 = stack[b1+1];
    l--;
  }
  return b1;
}



/********************************************************************************************
*                                PRINT FUNCTIONS                                            *
*********************************************************************************************/


/********************************************************
 Pre-Conditions: @params global code[] struct
 
 Post-Conditions: Prints the Interpreted Assembly Language
                  to console and output file
 ********************************************************/
void printInterpretedAssem(){
	
	int i;
    
    if(print_vm == 1){
        printf("\n");
        printf("+------------------------------------------------------+\n");
        printf("\n\n");
        printf("\t\tVirtual Machine Output\n");
        printf("\n\n");
        printf("+------------------------------------------------------+\n\n");
        printf("\nVirtual Machine Execution\n");
    }
    
    print_headers_vm();
    
	fprintf(output_full, "\nLine\t OP\t L  M");
	fprintf(output_full, "\n______________________________\n\n");
    
    if(print_vm == 1){
        printf("\nLine\t OP\t L  M");
        printf("\n______________________________\n\n");
	}
	
	// Loop through all the instructions, printing the interpreations
	// Prints to output/console simutaneously
	for(i = 0; i < instruction_cnt; i++){
       
        if(print_vm == 1){
            printf("%d\t %s     %d  %d\n", i , OP_CODE[code[i].op], code[i].l, code[i].m);
        }
        
        fprintf(output_full, "%d\t %s     %d  %d\n", i , OP_CODE[code[i].op], code[i].l, code[i].m);
	}
}



/********************************************************
 Pre-Conditions: @params global PC, BP and SP counters
 
 Post-Conditions: Prints intial values for the console and 
                  output as well as some headers
 ********************************************************/

void printInitial(){
	
    // Print our inital values for our PC, BP, SP, STACK....
    fprintf(output_full, "\t\t\t      PC   BP   SP      STACK\n");
    fprintf(output_full, "Initial Values\t\t      ");
    fprintf(output_full,"%d %4d %4d       Initialized to Zero\t", PC, BP, SP);
    fprintf(output_full, "\n_________________________________________________________________\n\n");

    
    if(print_vm == 1){
		// Print our inital values for our PC, BP, SP, STACK....
		printf("\t\t\t      PC   BP   SP       STACK\n");
		printf("Initial Values\t\t      ");
		printf("%d %4d %4d        Initialized to Zero\t", PC, BP, SP);
		printf("\n______________________________________________________________________\n\n");
    }
}



/********************************************************
 Pre-Conditions: @params global PC, BP and SP counters
 
 Post-Conditions: Prints the current state of each counter
                  to output and console for each instruction
 ********************************************************/

void print_PCBPSP(){
		fprintf(output_full, "%d\t %s %4d %4d %8d %4d %4d\t", 
		IR.line, OP_CODE[IR.op], IR.l, IR.m, PC, BP, SP);
		
    if(print_vm == 1){
		printf("%d\t %s %4d %4d %8d %4d %4d\t",
		IR.line, OP_CODE[IR.op], IR.l, IR.m, PC, BP, SP);
    }
}



/********************************************************
 Pre-Conditions: @params control -> is SP + save
                  save = 0 -> for every instr.
                  save = 4 -> when it recieves a CAL
 
 Post-Conditions: Prints the stack contents for each instruction
 ********************************************************/

void printStack(int control){
    
    // Return if BP == 0
	if(BP == 0){
		return;
	}
	
	// Else
	else{
        
        // Loop variable
		int i;
		
		// Print stack up to specified control
        // Save changes to 4 in the call statement
        // To print 4 more after stack pointer for next AR
		for (i = 1; i <= control+save; i++){
			
            // Inner Loop variable
			int j;
			
			// If our activation record changes 
			for (j = 0; j < AR; j++){
				
                // If a new AR
	            if (activation_records[j] == i){
		                 fprintf(output_full, " |"); // 0 then a |
                    
                    if(print_vm == 1){
						 printf(" |");
                    }
                }
            }
            
            fprintf(output_full, " %d", stack[i]); // then print stack
            
            if(print_vm == 1){
                printf(" %d", stack[i]);
            }
        }
        
	    fprintf(output_full, "\n");
        
        if(print_vm == 1){
            printf("\n");
        }
	}
    
    // After the call prints, reset save to 0,
    // Only changes when we recieve a CAL
    save = 0;
}


void print_headers_vm(){
    
    fprintf(output_full, "+------------------------------------------------------+\n");
	fprintf(output_full, "\n\n");
	fprintf(output_full, "\t\tVirtual Machine Output\n");
	fprintf(output_full, "\n\n");
	fprintf(output_full, "+------------------------------------------------------+\n\n");
    fprintf(output_full, "\nVirtual Machine Execution\n");

}
