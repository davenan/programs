// Dave Nandlall
// Chris Rodriguez
// COP 3530C - Problem B Spreading News
// Recitation Program #5
// April 3rd, 2013

import java.io.*;
import java.util.*;

public class SpreadingNews {

    // main method begins execution of Java Application
    // throws IOException if the input file is not found
    public static void main(String[] args) throws IOException{
        
        // Create new Scanner object to read from input file
        Scanner stdin = new Scanner(System.in);
      
        // Create new PrintWriter object to write data to output file
        PrintWriter output = new PrintWriter("output.txt");
        
        // Scan the amount of test cases;
        int cases = stdin.nextInt();
        
        // Loop Counter;
        int cnt = 0;
        
        // While there are test cases...
        while(cnt < cases){
            
            // Scan in the number of employees in the company
            int num_employees = stdin.nextInt();
            
            // Create a new employees array of size num_employees
            int employees[] = new int[num_employees];
            
            // Starting at index 1 to num_employees, scan in each employee
            // Index 0 stores employee 0 which is you
            for(int i = 1; i < num_employees; i++){
                employees[i] = stdin.nextInt();
            }
            
            // Calculate the minimum time
            int time = time(employees);      
            
            // Print out the minimum times to output file
            output.printf("%d", time);
            
            // Print newline in between each case for output file
            output.println();
            
            // Increment counter
            cnt++;
            
        } // end while   
       
        // Print message to tell user where output is saved
        System.out.println("Check output file in directory of source");
            
        // Close outputfile
        output.close();
        
 } // end main method
    
    
 // Time takes in the employees array, and recursively calls
 // findtime of the employees starting at index = 0, which is 
 // you the first employee
 public static int time(int[] employees){
     
     return findtime(employees, 0);
     
 } // end time method

 
 // findtime is recursively called by time, to calculate the minimum time travel of the news 
 public static int findtime(int[] employees, int index) {
      
      // Linked List to store the spread of times as we go from employee to employee recursively
      LinkedList<Integer> spread = new LinkedList();
      
      // Index starts at 0, Index + 1 = 1
      // So for index 1 in the employees array
      for (int i = index + 1; i < employees.length; i++) {       
         
          // Creates our tree structure in the ArrayList
         // Index starts at 0, then 1, 2, 3 ... employees.length
         // If employee[1] == 0, employee[2] = 1
         // Then employee 0 calls employee 1 at time 0, employee 1 would call employee 2 at time 1...
         // The employee is stored in the index and index+1 is the time that employee sends to next employee
         if (employees[i] == index){
             
             // Keep recursively doing the above method iff employees[i] == index
             // and push the times it took to spread to the next employee as we go along to our linked list
             spread.add(findtime(employees, i));         
          }
         
      } // end for loop
      
      // Next we sort the LinkedList of times in order
      Collections.sort(spread);
      
      // Important to reverse the times, because we need to calculate it to get back to the source 0
      // This would give the times in descending order from end of list -> source 0
      Collections.reverse(spread);

      // Create a new time variable
      int time = 0;
      
      // From our sorted spreaded gossip time, in descending from end->source 0 at index 0
      for (int i = 0; i < spread.size(); i++) {
           
           // Okay, so time initally = 0
          // Time then = Max(0, 1+ spread.get(0))
          // Time then = The Above ^ value and is then recalculated using next employee time backwards from source
          // Time is calculated by max (1 + time[t0] + 2 + time[t1] + 3 + time[t2]....)
          // Which is the miniumum of the spread of the news from the last employee -> source
           time = Math.max(time, (i+1) + spread.get(i));
      }

       // time stores the final variable to return to main as the minimum time
       return time;
       
    } // end method findtime

} // end class SpreadingNews
