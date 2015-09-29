//Dave Nandlall
//COP-3503C Programming Assigment 3
//March 15th, 2013


//Objective: To solve an 8 Puzzle with the least required amount of moves. The code
//below takes in the initial puzzle as a String representation, and then does a BFS search
//for that initial board that is a String. Used a string representation because it was 
//easier to link new generated states to the root or initial state that the starting puzzle
//was originally in, and kept the newly created state as one new String. 

/*Import Declarations*/

import java.io.*;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;

public class EightPuzzleSolver {
        
        public static final String goal = "123456780"; //Create our winning board state

        //main method begins execution of Java Application
        public static void main(String[] args) throws IOException{
         
            //Create new Scanner object and input new 8 puzzle File
            Scanner stdin = new Scanner(System.in); 
            
            int num_puzzles = stdin.nextInt(); // Scan the test cases
            int count = 0; //counter for test cases
            
            while(count < num_puzzles){ //while there are test cases...
                   
                String board = "";
                   
                   int puzzle[] = new int[9]; 
                   for(int j = 0; j < 9; j++){
                       puzzle[j] = stdin.nextInt(); //input puzzle as linear array of integers
                       board += puzzle[j]; //Add puzzle to empty board string, for the board in a String state
                   }
                   
                   BFS b = new BFS(board, goal); //create a new object of BFS class
                   b.BFS_Solve(); //solve the board
                   
                count++; //counter increment
             }

   }//end main method
 


public final static class BFS{
  
     String start;   //Start string passed to BFS constructor
     String goal;    //Goal string passed to BFS constructor
     String currentstate; //Temporary string to hold the states when shifting the blank
     
     int level; // current level away from our root node
     int blank; // int holder for the blank spot index in our string
     
     Queue<String> Q; //Create a Queue of Type: String
     HashMap<String, Integer> L; //Create a HashMap to keep track of our States and Level
     boolean solved = false;  //boolean counter to break
     
     //Constructor initializes the start and goal passed from main
     public BFS(String start, String goal)
     {
         this.start = start;       //this is our starting board as string
         this.goal = goal;        // this is the goal string final String
         Q = new LinkedList<>();  //Create a LinkedList of Strings for our Queue
         L = new HashMap<>();    //Create a HashMap to store the currenstate and its level from root
     
     }
    
    //the BFS Solve Function...
    public void BFS_Solve()
    {
      pushtoQputinL(start, null); //Push the inital state to the Queue and store its state in our HashMap
                             //null signifies the root state, no other states are generated yet
      
      while(!Q.isEmpty()){   // if Q is not empty
        
          currentstate = Q.remove();  // remove the entry from the Queue
             
             if(currentstate.equals(goal)){  // If this entry is in a solved state
                 solved = true;              // Flag boolean to true
                 break;                      // Exit while loop
             }
             
             
             //If the first state in the Queue is not in a solved state, solve it
             else{                
                 blank = currentstate.indexOf("0"); // Get the index of our empty tile on the board
                 genSuccessors(currentstate, blank); // Generate successor states, moving blank left, up, right, down        
             }            
          }
         
         // When our currentstate is finally solved by the shifting of tiles
         if(solved){
           System.out.println("Minimumum Moves Required: " + L.get(currentstate)); // Print the Level or minimum moves it took to get there
         }    
        
         else{
            System.out.println("Puzzle Requires to Many Moves");  // Else there is no solution, or to many states generated or moves needed
        }
    }
  


  //genSuccessor takes root state and the blank position
  // @ param currentstate and blank if it is not in a goal state and does the following
  public void genSuccessors(String currentState, int blank)
  {
         // If the blank is in these indexes, MOVE BLANK UP 
         if(blank != 0 && blank != 1 && blank != 2){
             String nextState = swap(currentState, 1); // Switch 1 controls UP
             pushtoQputinL(nextState, currentState);        // Add Generated State, and our prev state to Queue and HashMap
         }
         
         // If the blank is in these indexes, MOVE BLANK DOWN 
         if(blank != 6 && blank != 7 && blank != 8){      
             String nextState = swap(currentState, 2); // Switch 2 controls DOWN
             pushtoQputinL(nextState, currentState);        // Add Generated State, and our prev state to Queue and HashMap
         }
         
         // If the blank is in these indexes, MOVE BLANK LEFT
         if(blank != 0 && blank != 3 && blank != 6){           
             String nextState = swap(currentState, 3); // Switch 3 controls LEFT
             pushtoQputinL(nextState, currentState);        // Add Generated State, and our prev state to Queue and HashMap
         }
         
         // If the blank is in these indexes, MOVE BLANK RIGHT 
         if(blank != 2 && blank != 5 && blank != 8){         
             String nextState = swap(currentState, 4); // Switch 4 controls RIGHT
             pushtoQputinL(nextState, currentState);       // Add Generated State, and our prev state to Queue and HashMap
         }
 }
  
  
  //Swap Function @param currentstate and on
  public String swap(String currentState, int on)
  {
      
      String nextState = ""; // holder for our new generated state
      char s[] = currentState.toCharArray(); // take our currentstate @param currentstate, convert it to a character array
      char temp;  //temp swap value
      
      //switch case on! @ param on
      switch(on){
          
          //Case 1: Move Blank UP
          case 1: 
             temp = s[blank];
             s[blank] = s[blank-3];
             s[blank-3] = temp;
             nextState = new String(s);
          break;
          
          //Case 2: Move Blank DOWN
          case 2:
             temp = s[blank];
             s[blank] = s[blank+3];
             s[blank+3] = temp;
             nextState = new String(s);
          break;
          
          //Case 3: Move Blank LEFT
          case 3:
             temp = s[blank];
             s[blank] = s[blank-1];
             s[blank-1] = temp;
             nextState = new String(s);
          break;  
          
          //Case 4: Move Blank RIGHT
          case 4:
             temp = s[blank];
             s[blank] = s[blank+1];
             s[blank+1] = temp;
             nextState = new String(s);
          break;
      }      
      
      return nextState; //return swapped blank tile states
    }
  
  
  
 
 //pushtoQputinL is the main important function
 //takes in a generated State, and the previous state
// @param genState, prevState
 public void pushtoQputinL(String genState, String prevState)
 {
     if(!L.containsKey(genState)){ //Iff the HashMap does NOT contain this String of the generated state
        
       if(prevState == null){      //If the last state was null, then it was the root so...
         level = 0;                //the next level of our search would be 0
         Q.add(genState);          //add our generated state to the Queue structure
         L.put(genState, level);   //put that generated state into our HashMap Structure along with its level 
      }
      
      else if(prevState != null){  //Now if the last state was not in fact null, the we should...
          level = L.get(prevState)+1; //Find the level integer value of the oldstate, and inc by 1 
          Q.add(genState);            //Add this generated state to the Queue structure
          L.put(genState, level);     //put that generated state into our HashMap structure along with its level
      }
   }
}
  
  
  } //end class BFS

} //end class EightPuzzleSolver



 
 

 


 
 
 

