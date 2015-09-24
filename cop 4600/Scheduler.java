// Dave Nandlall
// COP 4600 
// Programming Assingment
// April 21, 2015

// Note: Decided to keep as one class file, just to keep everything together

import java.util.*;
import java.io.*;
        
public class Scheduler {
    
     // Inner Class - Store the information of each Process
     public static class Process 
     {       
        public String id;
        public int arrival, burst, remain;
        public int finished, wait_time, complete; 
        
        public Process(String id, int arrival, int burst, int remain, 
                       int finished, int wait_time, int complete) 
        {
          this.id = id;
          this.arrival = arrival;
          this.burst = burst;
          this.remain = remain;      // Remain time = Burst time
          this.finished = finished;
          this.wait_time = wait_time;
          this.complete = complete;
        }
     }
     
     // Inner Class - Stores the information to print our Gantt Chart
     public static class Gantt
     {  
        // information to print 
        String id;
        int time, burst, code;
  
        public Gantt(int time, String id, int burst, int code){
            this.time = time;
            this.id = id;
            this.burst = burst;
            this.code = code;
        }
        
        // Getter to return time for Gantt Chart
        public int getTime(){
            return this.time;
        }
    }
     
    
    // main method begins execution of application
    public static void main(String[] args) throws FileNotFoundException, IOException{ 
          
        
          // Input file
          System.out.println("Enter the input file name: ");         
          Scanner read = new Scanner(System.in);         
          String  input_file = read.nextLine();         
          Scanner stdin = new Scanner(new File(input_file));       
          
          
          // Output file         
          System.out.println("Enter the output file name: ");         
          Scanner write  = new Scanner(System.in);
          String output_file = write.nextLine(); 
          PrintWriter pw = new PrintWriter(new File(output_file));
          
          
          // Parse File, Save Controls, then Execute algorithm
          ArrayList<Process> q = new ArrayList();
          int controls[] = new int[4];    
          parse(stdin, controls, q);      
          execute(pw, q, controls);    
          
          // Print to console program is successful
          System.out.println("\nSuccessful, check directory for output file\n");
    }
    
    
   
    
    // Pre-Conditions - @params input file, store array, process q which is empty
    // Post-Conditions - stores the information given by the input file
    public static void parse(Scanner stdin, int store[], ArrayList<Process> q){
            
        // Read each line in the input file
        while(stdin.hasNextLine()){
        
            // Edit each line to get necessary tokens from the file
            String line = stdin.nextLine();
            String cut_comment[] = line.split("#");
            String pure_instruction = cut_comment[0];
            String instruction[] = pure_instruction.split(" ");
            String command = instruction[0];
            
            // Check the command and add information to our store[]
            // Case "process" we add our processes to our Process q arraylist
            switch(command){
                           
                case "processcount":
                    store[0] = Integer.parseInt(instruction[1].trim());
                break;
              
                case "runfor":
                    store[1] = Integer.parseInt(instruction[1].trim());
                break;
               
                case "use":
                    String scheduler = instruction[1];
                    switch(scheduler)
                    {
                        case "fcfs": store[2] = 1;
                        break;
                        case "sjf" : store[2] = 2;
                        break;
                        case "rr"  : store[2] = 3;
                        break;
                    }
                break;
                
                case "quantum":
                    store[3] = Integer.parseInt(instruction[1].trim());
                break;
                    
                case "process":                
                    String id = instruction[2];
                    int arrival = Integer.parseInt(instruction[4].trim());
                    int burst = Integer.parseInt(instruction[6].trim());
                    q.add(new Process(id, arrival, burst, burst, 0, 0, 0));
                break;
  
            }           
        }
     }
    
    
    
    // Pre-Conditions - @params pw, process q, and controls
    // Post-Conditions - execute one of the given CPU scheduler algorithms
    public static void execute(PrintWriter pw, ArrayList<Process> q, int controls[]){
        
        // Holds an integer that is the type of scheduler, check parse function
        int type_sched = controls[2];

        switch(type_sched){
            case 1: FCFS(pw, q, controls);
            break;
            case 2: SJF(pw, q, controls);
            break;
            case 3: RR(pw, q, controls);
            break;
        }
     }
    
   
    // Pre-Conditions - @params pw, process q, and controls
    // Post-Conditions - executes the FCFS scheduling algorithm
    public static void FCFS(PrintWriter pw, ArrayList <Process> q, int controls[])
    {
       
        // stores the gantt chart
        ArrayList<Gantt> results = new ArrayList();
        
        // Header FCFS, controls[0] = processcount
        pw.printf("%d processes\n", controls[0]);
        pw.println("Using First Come First Served\n");

        
        // Loop and FCFS algorithm variables
        int time = 0;
        int runfor = controls[1];
        int start;
        int finish = 0;
        
        // Run until the time is up...
        while(time < runfor)
        {
            // Iterate the process array
            for(int i = 0; i < q.size(); i++)
            {
                // Get a process
                Process p = q.get(i);
                
                // Initial process in the q
                Process first_process = q.get(0);  
                
                // The first process in the q is our start arrival time
                start = first_process.arrival;
                
                // Add process arrivals to our gantt
                results.add(new Scheduler.Gantt(p.arrival, p.id, 0, 1));
             
                // If the process hasnt run out of time, and its not finished
                if(time < runfor && p.finished == 0)
                {
                    // Set next start time to the end of the process burst
                    start = start + finish;
                
                    // Add to gantt, which process we selected
                    results.add(new Gantt(time, p.id, p.remain, 2));

                    // Finish time for the process, is the start + burst time
                    finish = start + p.burst;
                    
                    // Set the finished marker for the process, and completion time
                    p.finished = 1;
                    p.complete = finish;
                    
                    // Add to gantt, that the process finished
                    results.add(new Scheduler.Gantt(p.complete, p.id, 0, 3));
                 
                    // If the process runs out of time, print it to output
                    if(finish > runfor){
                       pw.printf("\n%s did not finish\n", p.id);
                       break;
                    }
                }               
               // Increase the time based on the process that is serviced
               time = time + p.burst;   
               
            } // end for          
        }  // end while
        
        // Display Gantt chart, and wait and turnaround time
        displayGantt(pw, results);        
        pw.printf("Finished at time %d\n\n", runfor);
        displayTime(pw, q);
        pw.close();
        
   }
    

    
    // Pre-Conditions - @params pw, process q, and controls
    // Post-Conditions - executes the SJF Premeptive scheduling algorithm       
    public static void SJF(PrintWriter pw, ArrayList<Process> q, int controls[])
    {
         // stores the gantt chart
         ArrayList<Gantt> results = new ArrayList();

         // Header SJF, controls[0] = processcount
         pw.printf("%d Processes\n", controls[0]);
         pw.println("Using Shortest Job First (Pre)\n");
              
         
         // Add process arrivals in process q to gantt chart
         for(int i = 0; i < q.size(); i++){
             results.add(new Gantt(q.get(i).arrival, q.get(i).id, 0, 1));
         }
                 
         // SJF control variables
         int time = 0;
         int runfor = controls[1];
         int new_p = 0;
         
         // Run until the time is up...
         while(time < runfor)
         {
              // Here we get the shortest job process based on arrival times
              // explained later...
              Process p = q.get(returnMin(q, time));  
           
              // old_p = old process or current process
              // new_p = new process or upcoming process
              int old_p = new_p;
              new_p = returnMin(q, time);
              
              // Print service time only when the process changes
              // Or if we have a process at time 0
              if(old_p != new_p || time == 0){            
                
                  // Prints only if process has remaining burst
                  if(p.remain != 0){
                    results.add(new Gantt(time, p.id, p.remain, 2));
                 }
              }
              
              // Only decrement the current process by 1 burst
              p.remain = p.remain - 1;

              // If there is no more CPU burst for process, it is done
              // Save the completion time and mark it as finished
              if(p.remain == 0){
                 p.complete = time + 1;
                 p.finished = 1;
                 results.add(new Gantt(p.complete, p.id, p.complete, 3));
              }
           
              // The CPU is IDLE if all processes are finished and (time < runfor)
              else if(p.finished == 1){
                  results.add(new Gantt(time, null, 0, 4));
              }

            // Increase the time by one unit
            time++;             
        }
        
         // Print Gantt, Time info to the output file
         displayGantt(pw, results);
         pw.printf("Finished at time %d\n\n", runfor);
         displayTime(pw, q);
         pw.close();
         
    }
    
    
    
    // Pre-Conditions - @params process q, and time
    // Post-Conditions - returns the process to be serviced for SJF Preemptive
     public static int returnMin(ArrayList<Process> q, int time)
     {
         // Index of the process in the q, to return
         int index = 0;   
         
         // Dummy value for shortest CPU burst, until we find a smaller one
         int val = 9999;
         
         
         // Iterate the process q
         for(int i = 0; i < q.size(); i++)
         {
             // Get a process
             Process p = q.get(i);
             
             // Check the arrival and if there are more bursts
             // Check wether that process is also shorter than the rest
             if(p.arrival <= time && p.remain > 0 && p.remain < val)
             {
                 // This process is the shortest job, set val to its burst
                 val = p.remain;
                 
                 // Set the index to return
                 index = i;
             }   
        }       
        // return the index to the process
        return index;
     }
     

    
    // Pre-Conditions - @params pw, process q, controls
    // Post-Conditions - executes the Round Robin Scheduler algorithm
    public static void RR(PrintWriter pw, ArrayList<Process> q, int controls[])
    {
         // stores gantt chart 
         ArrayList<Gantt> result = new ArrayList();
                  
         // Print Header RR, set our quantum variable
         int quantum = controls[3];
         pw.printf("%d processes\n", controls[0]);
         pw.println("Using Round-Robin");
         pw.printf("Quantum %d\n", quantum);
         pw.println();

         // Add process arrivals to our gantt chart
         for(int i = 0; i < q.size(); i++){
            result.add(new Gantt(q.get(i).arrival, q.get(i).id, 0, 1));
         }
         
         
         // RR algorithm control varibles
         int time = 0;
         int runfor = controls[1];
         int val = 0;

         // Run until time is up...
         while(time < runfor){
             
            // Iterate through our process q
            for(int i = 0; i < q.size(); i++){
                
                // Get a process
                Process p = q.get(i);
                
                // Check if the process arrived, and its NOT finished
                if(p.arrival <= time && p.finished == 0){
                    
                    // Select this process, add to gantt chart
                    result.add(new Gantt(time, p.id, p.remain, 2));
                    
                    // If the remain burst of the process is less than quantum
                    // The we set the value variable to the remaining burst
                    if(p.remain < quantum){
                        val = p.remain;
                    }
                        
                    // Else we just set this variable to the time slice of the quantum
                    else{
                        val = quantum;
                    }
                    
                    // Subtract this quantity from the burst of the current process
                    p.remain = p.remain - val;
                    
                    // If the process now has no more burst, then its finished
                    // Mark it as a finished process, and store its completion time
                    if(p.remain == 0){
                        p.finished = 1;
                        p.complete = time + val; // Current time, plus remaining burst
                    }
                    
                    // If the process selected is finished, add finished to gantt 
                    if(p.finished == 1){
                        result.add(new Gantt(p.complete, p.id, p.remain, 3));
                    }
                    
                   // increment the time, by means of the value variable 
                   time = time + val;
                }
                
                else if(p.finished == 1 && time != runfor){    
                    result.add(new Gantt(time, null, 0, 4));
                    time++;
                }       
            }       
       }
        
            // Display the time and gantt chart information.
            displayGantt(pw, result);
            pw.printf("Finished at time %d\n\n", runfor);
            displayTime(pw, q);
            pw.close();
     }

    
    
     // Pre-Conditions - @params pw, arraylist results which is our gantt chart
     //                  for each method of the CPU schedulers
     // Post-Condition - Organizes and prints the gantt chart to output file
     public static void displayGantt(PrintWriter pw, ArrayList<Gantt> gantt)
     {
         // Sort the Gantt chart by the time, using the Gantt Comparator
         Collections.sort(gantt, new GanttComp());
         
         // Iterate and print each statement based on the code at the index
         for(int i = 0; i < gantt.size(); i++){
             
             // Prints process that arrives
             if(gantt.get(i).code == 1){
                pw.printf("Time %d: %s arrived\n", gantt.get(i).time, gantt.get(i).id);
             }
             
             // Prints the selected process
             if(gantt.get(i).code == 2){
               pw.printf("Time %d: %s selected (burst %d)\n", gantt.get(i).time, 
               gantt.get(i).id, gantt.get(i).burst);

             }
             
             // Prints the process that finishes
             if(gantt.get(i).code == 3){
               pw.printf("Time %d: %s finished\n", gantt.get(i).time, gantt.get(i).id);
             }
             
             // Prints the CPU when it is IDLE
             if(gantt.get(i).code == 4){
               pw.printf("Time %d: IDLE\n", gantt.get(i).time);
             }      
         }
       
        pw.println();         
    }
     
   
     // Pre-Conditions - @params pw, process q
     // Post-Conditions - displays the turn around and wait time of the processes
     public static void displayTime(PrintWriter pw, ArrayList<Process> q)
     {
         int turnaround;
         int wait;
         
         // Each process object has its complete time stored 
         for(int i = 0; i < q.size(); i++)
         {
             // Get each process
             Process p = q.get(i);
             
             // Calculate turnaround time
             turnaround = p.complete - p.arrival;
             
             // Calculate wait time
             wait = turnaround - p.burst;
             
             // Print it to output file
             pw.printf("%s wait %d turnaround %d\n", p.id, wait, turnaround);
         }
         
     }

     // Comparator used to sort the printing of the Gantt Chart
     // Sorts based on the time 
     public static class GanttComp implements Comparator<Gantt> {
            
            @Override
            public int compare(Gantt p1, Gantt p2) {
                if (p1.getTime() < p2.getTime()) {
                     return -1;
                } else if (p1.getTime() > p2.getTime()) {
                     return 1;
                }
                    return 0;
                }
         }
     
     
} // end class Scheduler
   
      

   
    
    
    

