//Dave Nandlall
//January 25, 2013
//COP 3503C-13 Spring 0001
//Programming Assignment #1 - Sudoku Solver

//Objective - Create a valid Sudoku Puzzle Solver. The technique used for this 
//program was a brute-force method, using recursion to complete the task.

// NOTE: Input file does not have to be specifically named sudoku.txt, it can 
// possibly be any file name, but my code is specifically written to read from
// a file named sudoku.txt, any other file will throw a FileNotFoundException.
// File also has to be saved in the path of the program folder, for this program.

// NOTE : initGrid method is probably not needed since java automatically sets arrays to
// all zeros initially, but just wrote it anyways.

import java.io.*;
import java.util.*;

public class SudokuSolver {
    
    public int grid[][]; //create 2-D grid
    
    
    //main method begins execution of Java Application
     public static void main(String args[]) throws IOException{
        
         //try the input file sudoku.txt
         try{
            
            //create scanner object to read from sudoku.txt
            Scanner sudoku = new Scanner(new File("sudoku.txt"));
            
            //read the amount of test cases, located in the file
            int num = sudoku.nextInt();
            
            //Create new grid object and initilize control while loop counter
            int grid[][] = new int[9][9];
            int control = 1;
            
            //run loop so long as there are test cases
            while(control <= num){
              
              //Prints the test case name, for corresponding grid
              System.out.printf("Test Case %d:\n\n", control);
              
               //create the grid, from the input sudoku.txt
               createGrid(grid, sudoku);
             
               //Brute-Force, solve by starting at first spot on grid
               if(solveSudoku(grid, 0, 0)){
                    printGrid(grid);
               }   
               
               //Else if the solveSudoku method returns false, then there isn't a valid solution
              else{
                  System.out.print("No Solutions Possible\n");
               }
              
               //Re-Initilize grid for next test case
               initGrid(grid);
                
               System.out.println();
                
               control++; //increment loop counter
            }
        }
        
        //Catch exception if file does not exist, and print message
        catch(FileNotFoundException e){
             System.out.println("File Can't Be Found\n" + e);
         }
         
  } // end main method


 //initGrid takes in the 2-D grid array, and sets everything to 0
 public static void initGrid(int grid[][]){
     
     for(int i = 0; i < 9; i++){
         for(int j = 0; j < 9; j++){
             grid[i][j] = 0;
         }
     }
 }// end method initGrid

 
 //createGrid creates the initial grid from the input file, sudoku.txt
 public static void createGrid(int grid[][], Scanner sudoku){
     
     for(int i = 0; i < 9; i++){
         for(int j = 0; j < 9; j++){
             grid[i][j] = sudoku.nextInt();
         }
     }
 }//end method createGrid
 
 
 //printGrid prints the grid, with the corresponding values assigned
 public static void printGrid(int grid[][]){
    
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            System.out.printf("%d" + " ", grid[i][j]);
        }
        System.out.println();
    }
}// end methof printGrid

 
 //gridStatus returns a boolean true/false
 public static boolean gridStatus(int grid[][]){
     
     for(int i = 0; i < 9; i++){
         for(int j = 0; j < 9; j++){
             if(grid[i][j] == 0){  //if grid contains empty spot 0 
                 return true;      //return true
             }
         }        
     }
     return false; //if no empty spots on grid return false
     
 } //end method gridStatus

 
 //checkMove returns boolean true/false for the following
 public static boolean checkMove(int grid[][], int row, int col, int dig){

     //Stay on the same row, loop through each column
     //If the particular digit is in those indexes, return true 
     for(int i = 0; i < 9; i++){
         if(grid[row][i] == dig){
             return true;
         }
     }
     
     //Stay on the same column, loop through each row
     //If the particular digit is in those indexes, return true
     for(int i = 0; i < 9; i++){
         if(grid[i][col] == dig){
             return true;
         }
     }
     
     //offsets for row and col
     //guaranteed for row and col to be between 0 and 8
     //integer division used to guarantee that
     int startRow = (row/3)*3;
     int startCol = (col/3)*3;
     
     //loop through 3 rows
     for(int i = 0; i < 3; i++){
         
         //Loop through 3 cols
         for(int j = 0; j < 3; j++){
             
             //if row + startRow index and col + startCol index 
             //which is the boxed section contains the digit
             //return true;
             if(grid[i+startRow][j+startCol] == dig){
                 return true;
             }
         }
     }
     return false; // if everything works out we return false, to 
                   // signify a valid move to that spot
     
 } //end method checkMove
 
 

 /************************* Now for the fun part *****************************/ 
  
 //solve will attempt to solve the grid in the following way
 public static boolean solveSudoku(int grid[][], int row, int col){
    
     //if gridStatus returns false, then the grid is complete
     if(gridStatus(grid) == false){
         return true; // so, return true
     }
    
     
     if(col == 9){ // if the col equals 9
         col = 0;  // start back at column 0
         if(row++ == 9){  // keep incrementing row 0, 1, 2, 3, 4, 5, 6, 7, 8
             return true; // until we get through all rows and cols, return
         }                // true if we are have ran through the entire board
     }
     
     // Case where (0,0) would be a preset digit, and can't place any digit there
     // Try a recursive call on another col in same row
     // Ex. If (0, 0) were a preset digit like 8, go to (0,1) try recursive
     // solveSudoku on same row, next col over
     if(grid[row][col] != 0){
       return solveSudoku(grid, row, col+1); //try to solve same row, next col
     }
     
     //solveSudoku considers the digits 1-9 for the game of sudoku
     for(int dig = 1; dig <= 9; dig++){

         //if checkMove returns false, meaning that the row, col and box
         //section can place that particular digit there
         if(checkMove(grid, row, col, dig) == false){
 
             grid[row][col] = dig; //then put the digit in that spot
            
             //recursive step, in that solve will attempt to solve next
             //valid EMPTY row and column indexes
             if(solveSudoku(grid, row, col+1)){
                 return true;  //if it is successful, return true to stop
             } 
         }
     }
     
     grid[row][col] = 0; // if the if case ^ above fails, that empty spot can't
                         // place that digit there 
     
     return false; //backtracking step, return false, and try solve again 
     
 }// end method solveSudoku

}//end class SudokuSolver
