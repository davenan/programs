/* Dave Nandlall
 * COP 3503C-13Spring0001
 * February 4, 2013
 * Programming Assignment #2
*/

/* Objective - To connect power poles in a given city with underground cables 
 * so that no two cables cross, given a set of points (x,y). The program implements 
 * the use of Kruskal's Algorithm a Disjoint Set Data Structure and Path Compression
 * to create a Minimum Spanning Tree, to connect the cities with the least amount
 * of cable necessary.
 * 
 * NOTE: Input file has to be named cables_in.txt and must be saved in the same
 * directory as the program. Output file cables_out.txt will also be generated
 * in the same directory for use of multiple test cases, not provided in program
 * description, instead of writing to the console.
 */

//Import declarations 
import java.io.*;
import java.util.*;


//create class UndergroundCables
public class UndergroundCables {
    
    
        //main method begins execution of Java Application
        //throws IOException if file isn't found 
        public static void main(String[] args) throws IOException{
            
            //create new Scanner object to read from file cables_in.text
            Scanner cables_in;
            cables_in = new Scanner(new File("cables_in.txt"));
            
            //create new PrintWriter object to write to new output file cables_out.txt
            PrintWriter cables_out;
            cables_out = new PrintWriter(new FileWriter("cables_out.txt"));
            
            //Scan in integer N, the number of vertices for the city
            int n = cables_in.nextInt();
            
            //Holds x and y values of the points N
            int x[] = new int[1000];  // -1000 <= X <= 1000
            int y[] = new int[1000];  // -1000 <= Y <= 1000          
            
            //As long as the input file has test cases, and input doesn't read 0
            while(n != 0){
                
                // create a disjoint set of N possible points
                DisjointSet set = new DisjointSet(n);
                
                // create an arraylist to store the edges
                ArrayList<Edge> edges = new ArrayList<>();
                
                // input the N points from the input file
                for(int i = 0 ; i < n; i++){
                    x[i] = cables_in.nextInt();
                    y[i] = cables_in.nextInt();
                    
                } // end for i
  
                // Loop through n vertices from the file input
                for(int i = 0; i < n; i++){
                    
                    // Offset j to get next vertice to compare 2 sets of vertices
                    // For n possible vertices
                    for(int j = i+1; j < n; j++){
                        
                        //Calculate the edge between the two sets of vertices 
                        double weight = Weight(x[i], x[j], y[i], y[j]);
                        
                        //Store the edges into the arraylist
                        //For each vertex V (make the set of Edges)
                        edges.add(new Edge(i, j, weight));  
                        
                        
                    } // end for i
                    
                } // end for j
                
                
                // Take the edges in the ArrayList and now sort them from decreasing to increasing weight
                Collections.sort(edges, new EdgeComparator());

                // Variable to hold the total least amount of cable needed
                double total_cable = 0.0;

                //Loop through the sorted edges arraylist
                for(int i = 0; i < edges.size(); i++){
                    
                    //Remove an edge from the array list of sorted edges
                    Edge e = edges.get(i);
                    
                    //For each (u,v)-> (e.from, e.to)
                    //If the edge connects two different trees
                    if(set.find(e.from) != set.find(e.to)){
                        
                        total_cable += e.weight; //Add its weight (e.weight) -> cost

                        set.Union(e.from, e.to); //Union those edges into a single tree
                        
                    } //end if
                    
                    
                } // end for edges.size()
                
            //Clear our edge arraylist for next data test case
            edges.clear();
            
            //We display the Least Amount of Cable to Output file
            cables_out.printf("Amount of Cable: %.2f\n", total_cable);    
            
            //Scan in the next input of N points
            n = cables_in.nextInt();
            
         }// end while
       
       //Close both file pointers, after all test cases have been completed
       cables_in.close();
       cables_out.close();
            
  } //end main method
        


//Edge weight is calculated using distance formula between two points
public static double Weight(int x1, int x2, int y1, int y2){
    
    //return the distance between these 2 sets of vertices, which is our edge weight
    return Math.sqrt( Math.pow(x2-x1, 2) + Math.pow(y2-y1, 2) ); 
    
}//end Weight method
        


//DisjointSet class creates the disjoint set data structure
public static class DisjointSet{
  
   //Stores the value for the disjoint set
   public class Set{
       
       public int root;
       
       public Set(int r){
          root = r;
       }
       
    }//end class Set

   
   //stores our disjoint set data structure (sets)
   private Set[] sets;

   
   //create a new disjoint set
   //@ param n
   public DisjointSet(int n){
    
     //create a set of n items
     sets = new Set[n];
     
     //set each index in the set to new root which is i
     //Ex: index[0] = 0; index[1] = 1; index[n] = n;
     // for n-1 items
     for(int i = 0; i < n; i++){      
         sets[i] = new Set(i);   //each set is a subset of itself
       }
    }
   
   
   //returns the root of the tree that stores the value if there is no parent
   public int find(int root){
       
       //Traverse up the tree, if no parent found...then it must be a root 
       while(sets[root].root != root){
          
          //Get the root of this node
          root = sets[root].root;
       }
       
       //return the root
       return root;
   }
   
   
   // Union the 2 sets 
   public void Union(int indexA, int indexB){
       
       //Get each root node
       int root1 = find(indexA);
       int root2 = find(indexB);
       
       
       //root1 is a parent node based on find() function
       //root2 becomes the child of root1
       sets[root2].root = root1;
   }
 
} // end class Disjoint Set



/* Class to store the necessary information about our Edge Weights*/

public static class Edge{
    
    //variables to keep track of edges along vertices 
    private int from;
    private int to;
    private double weight;
    
    //Constructor to build our edges
    public Edge(int from, int to, double weight){
            
        this.from = from; //start of edge (from)
        this.to = to;     //end of edge   (to)
        this.weight = weight; //edge weight (weight)
        
    } //end Edge constructor
    
} //end class Edge

//Class EdgeComparator compares the edges in the ArrayList edges()
public static class EdgeComparator implements Comparator<Edge> {
    
     
     @Override
     //compares 2 edge weights by value and sorts accordingly...
     public int compare(Edge e1, Edge e2) {
         return Double.compare(e1.weight, e2.weight); 
     }//end compare
     
   } //end class EdgeComparator


} // end class UndergroundCables