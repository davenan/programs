// Dave Nandlall
// Barry Shevlin
// COP 3402 - Systems Software
// Programming Assignment - PL/0 Compiler
// November 17th, 2013

// Include standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include all our sources
#include "tokens.h"
#include "pl0-lex.h"
#include "pl0-parser.h"
#include "pl0-vm.h"

// For function all()
#define MAX_LENGTH 2000

// Global file ptrs for all()
FILE *ifp1;
FILE *ifp2;
FILE *ifp3;

// Function Prototype
void all();


int main(int argc, char **argv) {
    
    // 3 compiler flag integers
    int l_flag = 0;
    int a_flag = 0;
    int v_flag = 0;
    
    // Need more than one argument count
    if(argc > 1){
        
        int i;
        for(i = 1; i < argc; i++){
            
            // Set l flag to 1 if "-l"
            if(strcmp(argv[i], "-l") == 0){
                l_flag = 1;
            }
            
            // Set a flag to 1 if "-a"
            else if (strcmp(argv[i], "-a") == 0){
                a_flag = 1;

            }
            
            // Set v flag to 1 if "-v"
            else if(strcmp(argv[i], "-v") == 0){
                v_flag = 1;
            }
            
            // If the compiler directive was not acceptable...
            else{
                printf("Bad Directives %s\n", argv[i]);
                exit(-1);
            }
        }
    }
    
    
    // Here we run all three files
    // If the particular flag was set by the user
    // it will print the specified contents of the files
    // to the console...
    pl0_lex(l_flag);
    pl0_parser(a_flag);
    pl0_vm(v_flag);
    
    // If no flags were entered then we have a "./compile"
    // Call all() to print full output files during the
    // compile process
    if(l_flag == 0 && a_flag == 0 && v_flag == 0){
        all();
    }

}



/********************************************************
 
 Pre-Conditions: @params global file pointers ifp1, ifp2
                  and ifp3
                  ifp1 = full lexer outputs
                  ifp2 = full parser outputs
                  ifp3 = full virtual machine outputs
 
 Post-Conditions: Prints each full output file to console
 
 ********************************************************/

void all(){
    
    // A line of 2000 characters
    char line[MAX_LENGTH];
    
    // Open each full output files
    ifp1 = fopen("lex_full.txt", "r");
    ifp2 = fopen("parser_full.txt", "r");
    ifp3 = fopen("vm_full.txt", "r");
    
    // Print the full output of the lexer
    while(fgets(line, MAX_LENGTH, ifp1)){
        printf("%s", line);
    }
    
    printf("\n\n");
    
    // Print the full output of the parser
    while (fgets(line, MAX_LENGTH, ifp2)) {
        printf("%s", line);
    }
    
    printf("\n\n");
    
    // Print the full output of the vm
    while(fgets(line, MAX_LENGTH, ifp3)){
        printf("%s", line);
    }
    
    printf("\n\n");
    
    // Close all instances of file ptrs
    fclose(ifp1);
    fclose(ifp2);
    fclose(ifp3);
}