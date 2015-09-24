
// Dave Nandlall
// Adam Stark
// CDA 3103-13Summer C001
// Final Project
// July 24th, 2013

// Objective: The assembler takes a text file containing formatted assembly instructions as input and outputs a text file
//            containing the decimal representation of the 32-bit binary machine code.

// WARNING: THIS PROGRAM IS HEAVILY COMMENTED!!!!

#include "assembler.h"

// Structure to store 5-bit binary values
// for our registers
// *alpha = the alphabetic representation of register (ex. $a0)
// *numberic = the numerical representation of the register (ex. $4)
// *address = the 5-bit representation of that register
struct {
	const char *alpha;
    const char *numeric;
	char *address;
} register_map[] = {
    { "$zero", "$0", "00000" },
    { "$at", "$1", "00001" },
    { "$v0", "$2", "00010" },
    { "$v1", "$3", "00011" },
    { "$a0", "$4", "00100" },
    { "$a1", "$5", "00101" },
    { "$a2", "$6", "00110" },
    { "$a3", "$7", "00111" },
    { "$t0", "$8", "01000" },
    { "$t1", "$9", "01001" },
    { "$t2", "$10", "01010" },
    { "$t3", "$11", "01011" },
    { "$t4", "$12", "01100" },
    { "$t5", "$13", "01101" },
    { "$t6", "$14", "01110" },
    { "$t7", "$15", "01111" },
    { "$s0", "$16", "10000" },
    { "$s1", "$17", "10001" },
    { "$s2", "$18", "10010" },
    { "$s3", "$19", "10011" },
    { "$s4", "$20", "10100" },
    { "$s5", "$21", "10101" },
    { "$s6", "$22", "10110" },
    { "$s7", "$23", "10111" },
    { "$t8", "$24", "11000" },
    { "$t9", "$25", "11001" },
    { "$k0", "$26", "11010" },
    { "$k1", "$27", "11011" },
    { "$gp", "$28", "11100" },
    { "$sp", "$29", "11101" },
    { "$s8", "$30", "11110" },
    { "$ra", "$31", "11111" },
    { NULL, NULL, 0 } };


// Structure to map R-Type instructions to their respective function field
// *name = the name of the instruction being processed
// *function = 5-bit binary format for FUNCTION field for specified R-Type inst.
struct {
	const char *name;
	char *function;
} r_func[] = {
    { "add", "100000" },
    { "sub", "100010" },
    { "div", "011010" },
    { "mult", "011000"},
    { "sll", "000000" },
    { "mflo", "010010"},
    { "jr",  "001000" },
    { NULL, 0 } };


// Structure to map I-Type instructions to their respective op codes
// *name = the name of the instruction being processed
// *address = 5-bit binary format for OP CODE field for specified I-Type inst.
struct {
	const char *name;
	char *address;
} i_op_code[] = {
    { "lw",   "100011" },
    { "sw",   "101011" },
    { "bltz", "000001" },
    { "bne",  "000101" },
    { "addi", "001000" },
    { NULL, 0 } };



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
void assemble(vector_string_t * mips_assembly, vector_uint_t * machine_code){
    
    // Loop counter
    int i = 0;
    
    // While there are still lines to read in...
    while(i < mips_assembly->size){
        
        // Points to the line of the instruction in the mips_assembly vector
        char *instruction_ptr;
        instruction_ptr = mips_assembly->elements[i];
        
        // Calculate the amount of char in that line, whitespace included
        int instruction_length = strlen(instruction_ptr);
        
        // Holds instr. label, +1 for NULL char
        char instruction_label[instruction_length + 1];
        
        // Temporary pointer, in order not to modify original string from vector
        char *instruction_holder;
        instruction_holder = strcpy(instruction_label, instruction_ptr);
        
        // Grab the instruction, after the first space will be the instruction
        char *instruction;
        instruction = strtok(instruction_holder, " ");
        
        // Grab the registers, rest of the line is the registers/labels
        char *registers;
        registers = tokenizer(instruction_ptr, " \n", &instruction_ptr, NULL);
        
        // Assign a pointer to the registers
        registers = instruction_ptr;
        
        // Stores the type of instruction we are processing...
        char type = instruction_type(instruction);
      
        // If NOT a valid instruction...
        // Terminate Program, Print vector and Line the Error occured...
        if(type == '0'){
            printf("Invalid instruction %s\nLine: %d\n", instruction, i);
            vector_string_print(mips_assembly, stdout);
            exit(EXIT_SUCCESS);
        }
        
        // Else Below -- if 'r', 'i', or 'j' type, then call appropriate function
        else {
            
            if (type == 'r'){
                
            // Returned decimal value from r_type_instruction()
            unsigned int final_value = r_type_instruction(instruction_holder, registers);
            
            // Add to machine_code vector, at index i
            vector_uint_add(machine_code, final_value, i);
            }
        
           else if(type == 'i'){
            
            // Returned decimal value from i_type_instruction()
            unsigned int final_value = i_type_instruction(mips_assembly, instruction_holder, registers, i);
            
            // Add to machine_code vector, at index i
            vector_uint_add(machine_code, final_value, i);
           }
        
           else if(type == 'j'){
            
            // Returned decimal value from j_type_instruction()
            unsigned int final_value = j_type_instruction(mips_assembly, instruction_holder, registers, i);
            
            // Add to machine_code vector, at index i
            vector_uint_add(machine_code, final_value, i);
           }
            i++; // inc. loop
      } // end else
   } // end while()

    // We have successful got our answers
    printf("Processing...\n", stdout);
    printf("Output File Successful\n", stdout);
    printf("Check Source Folder\n", stdout);
    
} // end assemble()






/*******************************************************************************
 ***** IMPLEMENT YOUR FUNCTIONS BELOW ******************************************
 ***** REMEMBER TO DEFINE YOUR FUNCTIONS IN assembler.h ************************
 ******************************************************************************/



/***********************************************************************************************************************************************************
 *                                                                                                                                                         *
 * Process Instruction Types                                                                                                                               *
 *                                                                                                                                                         *
 ***********************************************************************************************************************************************************
 *                                                                                                                                                         *
 * PARAMETERS                                                                                                                                              *
 *              Each parameter varies for different function. See Descriptions Below                                                                       *
 *                                                                                                                                                         *
 ***********************************************************************************************************************************************************/


// Pre-conditions: Takes in the 'string' of our register names passed down by assemble()
// Post-conditions: Returns the 5-bit representation of alpha or numeric
//                 register representation, from struct register_map
char *register_address(char *register_name) {
    
	int i;
    
    // Determine if the regsiter type is alpha or # is read in (ex. $v0)
	for (i = 0; register_map[i].alpha != NULL; i++) {
        
        // Return 5 bit binary format for alpha rep. of register
		if (strcmp(register_name, register_map[i].alpha) == 0) {
			return register_map[i].address;
		}
        
        // Return 5 bit binary format for numberic rep. of register (ex. $2)
        else if(strcmp(register_name, register_map[i].numeric) == 0){
            return register_map[i].address;
        }
    }
    
    // Return NULL if not found in struct
	return NULL;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

// Pre-conditions:  Function determines the type of instruction, we are processing,
//                 for determining invalid instructions.
// Post-conditions: Returns a char to function assemble()
//                 {r, i, j, d, l} for r-type, i-type, j-type, directive and label
char instruction_type(char *instruction){
    
    // If the pointer passed in strcmp's with the below instructions...then R-TYPE
    if(strcmp(instruction, "add") == 0 ||
       strcmp(instruction, "div") == 0 ||
       strcmp(instruction, "jr")  == 0 ||
       strcmp(instruction, "mflo") == 0 ||
       strcmp(instruction, "mult") == 0 ||
       strcmp(instruction, "sll") == 0 ||
       strcmp(instruction, "sub") == 0){
        
        return 'r';
    }
    
    // If the pointer passed in strcmp's with the below instructions...then I-TYPE
     else if(strcmp(instruction, "addi") == 0 ||
             strcmp(instruction, "bltz") == 0 ||
             strcmp(instruction, "bne")  == 0 ||
             strcmp(instruction, "lw")   == 0 ||
             strcmp(instruction, "sw")   == 0){
      
        return 'i';
    }
    
    // If the pointer passed in strcmp's with the below instructions...then J-TYPE
    else if(strcmp(instruction, "j") == 0 ||
            strcmp(instruction, "jal") == 0){
    
        return 'j';
    }
    
    // If the instruction contains a "." substring, its a directive
    else if(strstr(instruction, ".") != 0){
        return 'd';
    }
    
    // If the instruction contains a ":" substring, its a label
    else if (strstr(instruction, ":") != 0){
        return 'l';
    }
    
    // NOT VALID INSTRUCTION!!!
    return '0';
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//



/********************************************************************************************************************************************************
 *                                                                                                                                                      *
 * R-TYPE instructions                                                                                                                                  *
 *                                                                                                                                                      *
 ********************************************************************************************************************************************************
 *                                                                                                                                                      *
 * PARAMETERS                                                                                                                                           *
 *                                                                                                                                                      *
 *    instruction     Pointer that points to the current instruction                                                                                    *
 *    registers       Pointer that points to the registers of that instruction                                                                          *
 *                                                                                                                                                      *
 *                                                                                                                                                      *
 ********************************************************************************************************************************************************/

// Pre-conditions: @params instruction and registers, the R-type instruction and registers
//                to be processed, passed down from assemble
// Post-conditions : Returns an unsigned integer value back to function assemble()
//                  which is the decimal value of our 32-bit binary R-Type 'string'
unsigned int r_type_instruction(char *instruction, char *registers){
    
    // Below are the dest. source and source2 registers - opcode included
    char *opcode = "000000";
    char *rdbin = NULL;
    char *rsbin = NULL;
    char *rtbin = NULL;
    char *shamtbin = NULL;
    char *function = NULL;
    
    // Holds 32-bit integer string
    char *output = (char*)malloc(33);
    
    // If arithmetic instruction is called
    if(strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0){
        
        // Grab the registers
        char *reg_ptr = registers;
        
        // Below assigns the dest. source and source2 registers with their binary codes
        char* rd = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        rdbin = register_address(rd);
        
        char* rs = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        rsbin = register_address(rs);
        
        char* rt = reg_ptr;
        string_trim(rt);
        rtbin = register_address(rt);
        
        // Get binary shift amount, no shift amount for R-Type
        char* shamt = "00000";
        shamtbin = shamt;
    }
    
    // If mult or div instruction is called
    else if(strcmp(instruction, "mult") == 0 || strcmp(instruction, "div") == 0){
        
        // Grab the registers
        char *reg_ptr = registers;
        
        // Below assigns the dest. source and source2 registers with their binary codes
        char *rd = "00000";
        rdbin = rd;
        
        char *rs = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        rsbin = register_address(rs);
        
        char *rt = reg_ptr;
        string_trim(rt);
        rtbin = register_address(rt);
        
        // Get binary shift amount, no shift amount for R-Type
        char* shamt = "00000";
        shamtbin = shamt;
    }
    
    // If move from LO is calleed
    else if(strcmp(instruction, "mflo") == 0){
        
        // Grab the registers
        char *reg_ptr = registers;
        
        // Below assigns the dest. source and source2 registers with their binary codes
        char *rd = reg_ptr;
        string_trim(rd);
        rdbin = register_address(rd);
        
        char *rs = "00000";
        rsbin = rs;
        
        char *rt = "00000";
        rtbin = rt;
        
        // Get binary shift amount, no shift amount for R-Type
        char* shamt = "00000";
        shamtbin = shamt;
        
    }
    
    // If jump register is called
    else if(strcmp(instruction, "jr") == 0){
        
        // Grab the reigsters
        char *reg_ptr = registers;
        
        // Below assigns the dest. source and source2 registers with their binary codes
        char *rd = "00000";
        rdbin = rd;
        
        char *rs = reg_ptr;
        string_trim(rs);
        rsbin = register_address(rs);
        
        char *rt = "00000";
        rtbin = rt;
        
        // Get binary shift amount, no shift amount for R-Type
        char* shamt = "00000";
        shamtbin = shamt;
    }
    
    // If shift left logical is called
    else if(strcmp(instruction, "sll") == 0){
       
        // Grab the registers
        char *reg_ptr = registers;
        
        //Below assigns the dest. source and source2 registers with their binary codes
        char *rd = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        rdbin = register_address(rd);
        
        char *rs = "00000";
        rsbin = rs;
        
        char *rt = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        rtbin = register_address(rt);
        
        // Assign the binary shift amount
        char* shamt = reg_ptr;
        string_trim(shamt);
        int shamt_dec = atoi(shamt);
        
        // 5-bit binary shift amount
        shamtbin = decimal_to_binary(shamt_dec, 5);
    }
    
    // This loop sets the function field pointer for each R-Type instruction
    int i;
    for(i = 0; r_func[i].name != NULL; i++){
        
        if(strcmp(instruction, r_func[i].name) == 0){
            function = r_func[i].function;
        }
    }
    
    // Get the 32-bit number from process_r_type(), store into output
    output = process_r_type(opcode, rsbin, rtbin, rdbin, shamtbin, function);
    
    // Convert the output to an unsigned integer decimal value
    unsigned int final_decimal_value = binary_to_decimal(output);
    
    // Return to assemble()
    return final_decimal_value;
    
    // Free allocated space for output
    free(output);
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

// Pre-conditions: Func. takes in the parameters from r_type_instruction() ^
// Post-conditions: Returns a 32-bit binary 'string' to r_type_instruction
//                 of the specific R-Type instruction we have processed
char *process_r_type(char *opcode, char *rsbin, char *rtbin, char *rdbin, char *shamtbin, char *function){
    
    // Create space for our 32-bit binary string
    char *r_32_bit = (char*)malloc(33);

    // Concatenate the string, with the other strings passed in...
    strcat(r_32_bit, opcode);
    strcat(r_32_bit, rsbin);
    strcat(r_32_bit, rtbin);
    strcat(r_32_bit, rdbin);
    strcat(r_32_bit, shamtbin);
    strcat(r_32_bit, function);
    
    // Return the 32 bit string for the R-Type instruction 
    return r_32_bit;
    
    // Free allocated space for r_32_bit
    free(r_32_bit);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//



/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * I-TYPE instruction                                                                                                                               *
 *                                                                                                                                                  * 
 ****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * PARAMETERS                                                                                                                                       *
 *  mips_assembly       Vector holding the MIPS assembly instructions                                                                               *
 *  instruction         Pointer that points to our current instruction                                                                              *
 *  registers           Pointer pointing to the registers of that instruction                                                                       *
 *  current_index       Holds the index of the line we are at in our vector                                                                         *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************/

// Pre-conditions: @params mips_assembly, instruction, registers, and current index we are
//                currently at in our mips_assembly vector.
// Post-conditions: Returns an unsigned integer representations to function
//                 assemble(), of our 32-bit binary I-Type 'string'

unsigned int i_type_instruction(vector_string_t * mips_assembly, char *instruction, char *registers, int current_index){
    
    // Create our pointers and initialize them to NULL
    char *opcode = NULL;
    char *rsbin = NULL;
    char *rtbin = NULL;
    char *immediate = NULL;
    char *offset = NULL;
    
    // Holds our output string, Allocate 33 bits of size (char) for 32-bit output
    char *output = (char*)malloc(33);
    
    // Process the addi instruction
    if(strcmp(instruction, "addi") == 0){
        
        // Grab the registers that reg_ptr points to...
        char *reg_ptr = registers;
        
        // Tokenize the line using parse token to get $rs
        char* rs = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        
        // Get 5 bit binary format of register $rs from struct register_map
        rsbin = register_address(rs);
        
        // Tokenize the line using parse token to get $rt
        char* rt = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        
        // Get 5 bit binary format of register $rt from struct register_map
        rtbin = register_address(rt);
        
        // The rest of the line is the immediate value in addi instruction
        char* imm = reg_ptr;
        
        // Trim the point to delete any whitespace 
        string_trim(imm);
        
        // Convert immediate 'string' to integer value and store in immediate_dec
        int immediate_dec = atoi(imm);
        
        // Convert that immediate integer value back to a 16 bit immediate 'string'
        immediate = decimal_to_binary(immediate_dec, 16);
        
    }
    
    
    // Process the lw and sw instruction
    else if(strcmp(instruction, "lw") == 0 || strcmp(instruction, "sw") == 0){
        
        // Grab the registers from that reg_ptr points to...
        char *reg_ptr = registers;
        
        // Tokenize the line using parse token to get $rt
        char *rt = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        
        // Get 5 bit binary format of register $rt from struct register_map
        rtbin = register_address(rt);
        
        // Tokenize the line again to get the immediate value
        char *imm = tokenizer(reg_ptr, " ,()\t\n\r", &reg_ptr, NULL);
        
        // Trim any white space from immediate value
        string_trim(imm);
        
        // Convert immediate 'string' to integer value and store in immediate_dec
        int immediate_dec = atoi(imm);
        
        // Convert that immediate integer value back to 16 bit immediate 'string'
        immediate = decimal_to_binary(immediate_dec, 16);
        
        // Tokenize line again to remove the last ')' character to get our $rs
        char *rs = tokenizer(reg_ptr , " ,)\t\n\r", &reg_ptr, NULL);
        
        // Trim any white space from the string rs
        string_trim(rs);
        
        // Get the 5 bit binary format of $rs from struct registerMap/registerMap2
        rsbin = register_address(rs);
        
    }
    
    
    // Process the bne instruction
    else if(strcmp(instruction, "bne") == 0){
       
        // Grab the registers/label that reg_ptr points to...
        char *reg_ptr = registers;
        
        // Tokenize the line to get our register $rs
        char *rs = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        
        // Get the 5 bit binary format of register $rs from struct register_map
        rsbin = register_address(rs);
        
        // Tokenize the line again to get out register $rt
        char *rt = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        
        // Get the 5 bit binary format of register $rt from struct register_map
        rtbin = register_address(rt);
        
        // Last but not least, the remains of the reg_ptr is our label to branch to...
        char *label = reg_ptr;
        
        // Trim our label to remove any white space 
        string_trim(label);
        
        // Here we append a colon to our label in order to compare it to the elements in our vector mips assembly
        char *labelplus = append(label, ":");
        
        // Index 1 stores the current index, so ex. we are in the mip vector at index 4
        int branch_from = current_index;
        
        // Index 2 is the index that we need to branch to given by the function get_index()
        int branch_to = get_index(mips_assembly, labelplus);
        
        // Next we have to account for how many labels are between index1 and index2 our branch label
        int amount_of_labels = get_labels(branch_from, branch_to, mips_assembly);
        
        // The offset of the branch is given by this function get_offset()
        int branch_offset = get_offset(branch_from, branch_to, amount_of_labels);
        
        // Convert that specific offeset into a 16 bit binary 'string'
        offset = decimal_to_binary(branch_offset, 16);
    }
    
    
    // Process the bltz instruction
    else if(strcmp(instruction, "bltz") == 0){
        
        // Grab the registers/label that reg_ptr points to...
        char *reg_ptr = registers;
        
        // Tokenize the line to get the first register which is $rs
        char *rs = tokenizer(reg_ptr, " ,\t\n\r", &reg_ptr, NULL);
        
        // Get the 5 bit binary format of register $rs from struct register_map
        rsbin = register_address(rs);
        
        // Set $rt to "00000" which is the 5 bits for $rt field for the bltz instruction
        char *rt = "00000";
        
        // Just set rtbin to rt (rtbin = rt = "00000")
        rtbin = rt;
        
        // The remainder of the 'string' that reg_ptr points to is our label
        char *label = reg_ptr;
        
        // Trim the label to remove any white space
        string_trim(label);
        
        // Here we append a colon to the label in order to compare it to labels in our vector mips assembly
        char *labelplus = append(label, ":");
        
        // Branch From stores the current index we are currently at in our mips vector
        int branch_from = current_index;
        
        // Branch To stores the index in which we wish to branch to
        int branch_to = get_index(mips_assembly, labelplus);
        
        // Calculate the amount of labels between the indicies branch from -> branch to
        int amount_of_labels = get_labels(branch_from, branch_to, mips_assembly);
        
        // Calculate the branch off set given by function get_offset()
        int branch_offset = get_offset(branch_from, branch_to, amount_of_labels);
        
        // Convert that specific offset to a 16 bit offset 'string'
        offset = decimal_to_binary(branch_offset, 16);
    }

    
    // This loop here sets the respective I-Type op code pointer for each instruction above ^
    // Searches through each specific struct and stores the op code into *opcode;
    size_t i;
    for (i = 0; i_op_code[i].name != NULL; i++) {
        if (strcmp(instruction, i_op_code[i].name) == 0) {
            opcode = i_op_code[i].address;
        }
    }
       
    
    // Our 32-bit binary string for I-Type gets returned from process_i_type()
    output = process_i_type(instruction, opcode, rsbin, rtbin, immediate, offset);
    
    // Convert those 32 bits to decimal
    unsigned int final_decimal_value = binary_to_decimal(output);
    
    // Return to assemble()
    return final_decimal_value;
    
    // Free allocated space for output
    free(output);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

// Pre-conditions: Takes in all the specified @params from  i_type_instruction();
// Post-conditions: Returns a 32-bit I-Type binary 'string' representation
//                 to i_type_instruction() above ^, organized according to our
//                 I-Type of instruction
char *process_i_type(char *instruction, char *opcode, char *rsbin, char *rtbin, char *immediate, char *offset){
    
    // Create a string of 32-bits to store 32-bit int value
    char *i_32_bit = (char*)malloc(33);
    
    // If our instruction was addi, organize [opcode, $rt, $rs, immediate] format
    if(strcmp(instruction, "addi") == 0){
        strcat(i_32_bit, opcode);
        strcat(i_32_bit, rtbin);
        strcat(i_32_bit, rsbin);
        strcat(i_32_bit, immediate);
    }
    
    // If our instruction was lw/sw, organize [opcode, $rs, $rt, immediate] format
    if(strcmp(instruction, "lw") == 0 || strcmp(instruction, "sw") == 0){
        strcat(i_32_bit, opcode);
        strcat(i_32_bit, rsbin);
        strcat(i_32_bit, rtbin);
        strcat(i_32_bit, immediate);
        
    }
    
    // If our instruction was bne/bltz organize [opcode, $rs, $rt, offset]
    if(strcmp(instruction, "bne") == 0 || strcmp(instruction, "bltz") == 0){
        strcat(i_32_bit, opcode);
        strcat(i_32_bit, rsbin);
        strcat(i_32_bit, rtbin);
        strcat(i_32_bit, offset);
    }
    
    // Return 32 bit string to i_type_instruction() above ^
    return i_32_bit;
    
    // Free allocated space for i_32_bit
    free(i_32_bit);
}




/****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * J-TYPE instructions                                                                                                                              *
 *                                                                                                                                                  *
 ****************************************************************************************************************************************************
 *                                                                                                                                                  *
 * PARAMETERS                                                                                                                                       *
 *  mips_assembly       Vector holding the MIPS assembly instructions                                                                               *
 *  instruction         Pointer that points to our current instruction                                                                              *
 *  registers           Pointer pointing to the registers of that instruction                                                                       *
 *  current_index       Holds the index of the line we are at in our vector                                                                         *
 *                                                                                                                                                  *
 ***************************************************************************************************************************************************/


// Pre-conditions: @params passed into function from assemble(), the mips_assembly vector,
//                The J-Type instruction to be processed and the index we are at in our mips_assembly vector.
// Post-conditions: Returns a 32-bit integer 'string' representation of J-Type Instruction.
unsigned int j_type_instruction(vector_string_t * mips_assembly, char *instruction, char *registers, int current_index){

    // Create our pointers and initialize to NULL
    char *opcode = NULL;
    char *address = NULL;
    char *target = NULL;
    
    // Output string is allocated to hold 32-bits or 32 characters wide (33 for NULL char)
    char *output = (char*)malloc(33);

    // Process the j instruction
    if(strcmp(instruction, "j") == 0){
        
        // Our reg_ptr points to our registers/labels we need to jump to...
        char *reg_ptr = registers;
        
        // Hard code the op code for j instruction
        opcode = "000010";
        
        // Set our address pointer to the reg_ptr which is our address label to branch to 
        address = reg_ptr;
        
        // Trim the address to rid it of white space
        string_trim(address);
        
        // Append a colon to the address and store it to addressplus
        char *addressplus = append(address, ":");
        
        // Branch From is the current index we are at in the mips vector, what line the j instruction is located
        int branch_from = current_index;
        
        // Branch To is the index that we wish to provide an unconditional jump to
        int branch_to = get_index(mips_assembly, addressplus);
        
        // When we jump we calculate the "amount" of labels we found along the way
        int amount_of_labels = get_labels(branch_from, branch_to, mips_assembly);
        
        // Add means address, which stores the integer value of the offset for the address
        int add = get_offset(branch_from, branch_to, amount_of_labels);
        
        // Target is the binary representation of the offset value from the "add" int value or our Address we are jumping to...
        // 26-bits required for J and JAL instructions
        target = decimal_to_binary(add, 26);
        
        // Concatenate the opcode and the target address into output string
        strcat(output, opcode);
        strcat(output, target);
    }
    
    // Process the jal instruction
    else if(strcmp(instruction, "jal") == 0){
        
        // Our reg_ptr points to our registers/labels we need to jump and link to...
        char *reg_ptr = registers;
        
        // Hard code the op code for jal instruction
        opcode = "000011";
        
        // Set our address pointer to the reg_ptr which is our address label to branch to
        address = reg_ptr;
        
        // Trim the address to rid it of white space
        string_trim(address);
        
        // Append a colon to the address and store it to addressplus
        char *addressplus = append(address, ":");
        
        // Branch From is the current index we are at in the mips vector, what line the jal instruction is located
        int branch_from = current_index;
        
        // Branch To is the index that we wish to provide an unconditional jump and link to
        int branch_to = get_index(mips_assembly, addressplus);
        
        // When we jump we calculate the "amount" of labels we found along the way
        int amount_of_labels = get_labels(branch_from, branch_to, mips_assembly);
        
        // Add means address, which stores the integer value of the offset for the address
        int add = get_offset(branch_from, branch_to, amount_of_labels);
        
        // Target is the binary representation of the offset value from the "add" int value or our Address we are jumping and linking to...
        // 26-bits required for J and JAL instructions
        target = decimal_to_binary(add, 26);
        
        // Concatenate the opcode and the target address into output string
        strcat(output, opcode);
        strcat(output, target);
    }
    
    // Last but not least we take our output string and convert the 32-bit 'string' into an unsigned integer value
    unsigned int final_decimal_value = binary_to_decimal(output);
    
    // Return to assemble()
    return final_decimal_value;
    
    // Free allocated space for output
    free(output);
}



/******************************************************************************************************************************************************
 *                                                                                                                                                    *
 *              CALCULATION FUNCTIONS BELOW                                                                                                           *
 *                                                                                                                                                    *
 ******************************************************************************************************************************************************
 *                                                                                                                                                    *
 * PARAMETERS                                                                                                                                         *
 *              DIFFERENT TYPES OF PARAMETERS                                                                                                         *
 *              PROVIDES DIFFERENT TYPES OF OUTPUTS                                                                                                   *
 *              FOR GETTING CERTAIN INFORMATION WHILE PROCESSING                                                                                      *
 *              OUR INSTRUCTIONS, SEE DESCRIPTIONS BELOW                                                                                              *
 ******************************************************************************************************************************************************/


// Pre-conditions:  get_index() takes in the @params mips_assembly vector and 
//                 @params label for I-Type (BNE, BLTZ) and J-TYPE (J, JAL)
// Post-conditions: Returns the index we which to Branch To,
//                 for the specific instruction we are processing.
int get_index(vector_string_t * mips_assembly, char *label){
    
    // Loop variable
    int i;
    
    // First we loop through the original vector that contains all our instructions
    for(i = 0; i < mips_assembly->size; i++){
        
        // str1 is the line of each instruction, takes every line into account
        char *str1 = mips_assembly->elements[i];
        
        // str2 is the label passed into the function get_index() from I-type, J-type (bne, bltz, j, jal)
        // str2 has a ':' character that was appended to it in order for the next line of code...
        char *str2 = label;
        
        // We compare str1 and str2, if we find our label str2, we return the index of the loop + 1
        if(strcmp(str1, str2) == 0){
            
            // Return i+1 because we are looking for instruction after the label
            return i+1;
        }
        
    } // end for loop
        
    return 0; // Failsafe
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

// Pre-conditions: After get_index() has returned the index where our label was in our vector
//                we calculate the amount of labels that were in between (branch from -> branch to)
//                We do this because labels are not considered instructions, and while getting the indicies
//                we considered labels to BE instructions
// Post-conditions: Returns an integer representing how many labels we spot along the way
int get_labels(int branch_from, int branch_to, vector_string_t * mips_assembly){
    
    
    int i;
    int temp = 0; // Store the amount of labels here
    
    // Case 1: if branch from was less than branch to (ex. branch_from = 2 && branch_to = 10)
    if(branch_from < branch_to){
        
        // We need to branch foward through our vector... between these indicies
        for(i = branch_from; i < branch_to; i++){
            
            // Line is pointing to each element unmodified in the original vector as we loop through
            char *line = mips_assembly->elements[i];
            
            // If ever we see that the line of the vector has a ':', then it IS a label
            if(strstr(line, ":") != 0){
                
                // We increment our temp variable to store occurences of a label.
                temp = temp + 1;
            }
        } 
    }
    
    // Case 2: if branch from was greater than branch to (ex. branch from = 10 && branch to = 2)
    else if(branch_from > branch_to){
        
        // We need to branch backwards through our vector... between these indicies
        for(i = branch_from; i > branch_to; i--){
            
            // Line is pointing to each element unmodified in the original vector as we loop through
            char *line = mips_assembly->elements[i];
            
            // If ever we see that the line of the vector has a ':', then it IS a label
            if(strstr(line, ":") != 0){
                
                // We increment our temp variable to store the occurences of a label.
                temp = temp + 1;
            }
        }        
    }
    
    // Return the amount of labels to the caller
    return temp;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

// Pre-conditions: After we compute get_index() and get_labels(), we can compute our offsets
//                for BNE, BLTZ, J, JAL instructions
// Post-conditions: Returns an integer value of our offset required for I-Type(bne, bltz)
//                 or J-Type(j, jal)
int get_offset(int branch_from, int branch_to, int amount){
    
    // Our offset starts at -1, denoting the line before the current instruction
    int offset = -1;
    
    // Case 1: if branch from is less than branch to (ex. branch from = 2 && branch to = 10)
    if(branch_from < branch_to){
        
        // Subtract (branch to - branch from) to get amount of movements forward, and then subtract the 'amount' of labels in between
        int temp = (branch_to - branch_from - amount);
        
        // Add this number to our offset of -1, to get our final offset value (>>>>>>> Forwards >>>>>>)
        offset = offset + temp;
    }
    
    
    // Case 2: if branch from is greater than branch to (ex. branch from = 10 && branch to = 2)
    else if(branch_from > branch_to){
        
        // Subtract (branch from - branch to) to get amount of movements backwards, and then subtract the 'amount' of labels in between
        int temp = branch_from - branch_to - amount;
        
        // This time, we subtract from our offset, because we are moving backwards (<<<<< Backwards <<<<<)
        offset = offset - temp;
    }
    
    // Return the final offset to the caller
    return offset;
}




/******************************************************************************************************************************************************
 *                                                                                                                                                    *
 *              HELPER FUNCTIONS BELOW                                                                                                                *
 *                                                                                                                                                    *
 ******************************************************************************************************************************************************
 *                                                                                                                                                    *
 * PARAMETERS                                                                                                                                         *
 *              DIFFERENT TYPES OF PARAMETERS                                                                                                         *
 *              HELPS IN MANIPULATING DIFFERENT THINGS IN THE SOURCE CODE                                                                             *
 *              SOME ARE APPENDING TO A STRING, CONVERTING DECIMAL TO BINARY                                                                          *
 *              CONVERTING BINARY TO DECIMAL AND TOKENIZING A LINE                                                                                    *
 *              SEE DESCRIPTIONS BELOW                                                                                                                *
 *****************************************************************************************************************************************************/


// Pre-conditions: Takes in 2 string pointers, passed in from
//                I-type (bne, bltz) and J-Type (j, jal), one which is our
//                label and another is a char we wish to append. Safely appends
// Post-conditions: Returns the label, appended with a colon
char * append(char * string1, char * string2){
    
    // Our output string
    char * output_string = NULL;
    
    // Append string2 to string1
    asprintf(&output_string, "%s%s", string1, string2);
    
    // Return the appended string
    return output_string;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

// Pre-conditions:  Tokenizes a line safely without the use of strtok
// Post-conditions: Used in general implementation to tokenize the mip_assembly instruction
//                  passed into it as a string, without harming the original string. Returns the token
//                  in which we need to process, up to a certain delimiter character
//                  when it gets to that character, NULL char is placed infront of the rest
//                  of the string to signify the start of the next token.
static inline char *tokenizer(char *input_string, char *delimiters, char **output_string, char *delim_characters) {
   
    // Our Pointers to setup tokenizer()
    int length;
    char *ptr;
    char *token_ptr;
    char *token;
    
    // Find length of the intial portion of our input string, up to our delim char
    length = strspn(input_string, delimiters);
    
    // Pointer equals that length plus our input string, for whitespace included
    ptr = (input_string + length);
        
    // Token Pointer finds the substring of ptr, up to that delimiter, all valid characters
    // And strips Pointer up to that point
    token_ptr = strpbrk(ptr, delimiters);

    // If we have a NULL token_ptr return NULL
    if (token_ptr == NULL)
        return(NULL);
    
    // We subtract to find new length which is the length of token_ptr
    length = token_ptr - ptr;

    // If our delimiter characters != NULL
    if (delim_characters != NULL)
        
        // Set our delimiter characters to point to the token pointer
        // Which points to the line that has been stripped of its first token
        *delim_characters = *token_ptr;
    
    // Create an output string which is the size of our token_ptr + 1 to go past our delimiter
    *output_string = token_ptr + 1;
    
    // Allocate memory for the token we wish to return
    token = (char*)malloc(length + 1);
    
    // If our token is NULL we return NULL
    if (token == NULL)
        return(NULL);
    
    // Copy this block of memory
    // source = destination
    // ptr = source
    // length = # of bytes
    memcpy(token, ptr, length);
    
    // Everything up to the NULL char character is our token
    token[length] = (char)'\0';
    
    // Return our token
    return(token);
    
    // Free allocated space used for token
    free(token);
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//


// Pre-conditions: Takes in a SIGNED integer, and the amount of bits required (5/6/16/26) etc.
// Post-conditions: Returns the binary representation of that decimal number.
char *decimal_to_binary(int n, int length){
    
    // Create space of n-bits passed into function
    // from the variable length (decimal->binary)
    char *ptr;
    ptr = (char*)malloc(length+1);
    
    // Variables 
    int bits, binary;
    
    // Counter variable for bits produced
    int count;
    count = 0;

    // Loop backwards -> forwards for bits 
    for (bits = length-1 ; bits >= 0 ; bits--){
        
        // For n, bitwise right shift original number(n)
        binary = n >> bits;
        
        // If the result is 1, then that bit is 1 (bitwise AND)
        if (binary & 1)
            *(ptr+count) = 1 + '0';
        
        // Else it is a 0
        else
            *(ptr+count) = 0 + '0';
        
        // Increment counter for amount of bits
        count++;
    }
    
    // Everything ptr points to except NULL char
    // is our binary form of the decimal value
    *(ptr+count) = '\0';
    
    // Return those binary digits back to caller
    return ptr;
    
    // Free allocated space used for ptr
    free(ptr);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//


// Pre-conditions: Takes in a binary 'string' of n bits
// Post-conditions: Returns the decimal value of the binary number
int binary_to_decimal(const char * str){
    
    // Value stores our decimal value
    int val = 0;
    
    // While our binary string is not NULL character
    while (*str != '\0')
        
        // Equivalent to (2^n), while there is a 1 present in string
        val = 2 * val + (*str++ - '0');

    // Return decimal value
    return val;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>END OF PROGRAM>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//


