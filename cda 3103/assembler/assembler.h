#ifndef __ASSEMBER_H__
#define __ASSEMBER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>

#include "utility.h"
#include "vector.h"


/*******************************************************************************
 *
 * Invokes the assembling of a collection of MIPS assembly instructions.
 *
 *******************************************************************************
 *
 * PARAMETERS
 *  mips_assembly       Vector holding the MIPS assembly instructions which
 *          are to be assembled.
 *  machine_code        Empty vector to hold the 32-bit binary instructions
 *          which will result from the assembling.
 *
 ******************************************************************************/
void
assemble(
         vector_string_t * mips_assembly,
         vector_uint_t * machine_code
        );

/*******************************************************************************
 ***** DEFINE YOUR FUNCTIONS BELOW *********************************************
 **************************************************************************/

// Process Instruction Type Functions
char instruction_type(char *instruction);
char *register_address(char *register_name);

// R-Type Instruction Functions
unsigned int r_type_instruction(char *instruction, char *registers);
char *process_r_type(char *opcode, char *rsbin, char *rtbin, char *rdbin, char *shamtbin, char *function);

// I-Type Instruction Functions
unsigned int i_type_instruction(vector_string_t * mips_assembly, char *instruction, char *registers, int current_index);
char *process_i_type(char *instruction, char *opcode, char *rsbin, char *rtbin, char *immediate, char *offset);

// J-Type Instruction Function
unsigned int j_type_instruction(vector_string_t * mips_assembly, char *instruction, char *registers, int current_index);

// Calculcation Functions
int get_index(vector_string_t * mips_assembly, char *label);
int get_labels(int branch_from, int branch_to, vector_string_t * mips_assembly);
int get_offset(int branch_from, int branch_to, int amount);

// Helper Functions
char* append(char* string1, char* string2);
char* decimal_to_binary(int n, int length);
int binary_to_decimal(const char * str);
static inline char *tokenizer(char *input_string, char *delimiters, char **output_string, char *delim_characters);



#endif // __ASSEMBER_H__
