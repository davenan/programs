// Dave Nandlall
// Matt Rouse
// COP 3503C-Spring0013
// Lab Program #3 - Minimum Diaper Cost
// February 28th 2013
// Note: Still buggy....


import java.io.*;
import java.util.*;

public class GreedyDiapers {
    
     public static int amount, milli, total, index;
     public static double price;
             
     public static void main(String[] args) throws IOException{
         
         Scanner stdin = new Scanner(System.in); 
         
         int cases = stdin.nextInt();
         int count = 0;
         double newprice = 0.0;
         int tot = 0;
         int max = 0;

          //Setup ArrayList for Items and Possible Price Answers
          ArrayList<Items> items = new ArrayList<>();
          ArrayList<Answer> answer = new ArrayList<>();
             
             
           //While there are test cases...
           while(count < cases){
            
             //Scan in the waste, boxes of diapers
             int waste = stdin.nextInt();
             int boxes = stdin.nextInt();   
             
             //Create a capacity array to hold all waste products, of size waste
             int cap[] = new int[waste]; 
             
             //Target and max variables declared 
             int target = 0;
             
             //for each diaper set, scan in its attributes
             for(int i = 0; i < boxes; i++){ 
                 
                 amount = stdin.nextInt();
                 price = stdin.nextDouble();
                 milli = stdin.nextInt();
                 total = (amount * milli);   //total capacity for one set of diapers
                 index = i;
                
                 
                 items.add(new Items(amount, price, milli, total, index)); //add all values to Items Array List
             }
             
             //Next scan in the waste product into waste array
             for(int j = 0; j < waste; j++){
                 cap[j] = stdin.nextInt();
                 target += cap[j];          //calculate the sum of all waste combined
                 max = Max(cap, waste);     //return the diaper with the highest amount of waste for that waste set
             }   

             //for the items array if the amount of milliliters can accomodate max
             //add the price to answer array
             for(int i = 0; i < items.size(); i++){
                 
                 Items i1 = items.get(i);
                 
                 if(i1.milli >= max){
                     answer.add(new Answer(i1.price, i1.index));
                 }
                
             }
             
             // add totals and newprices, until target is satisfied
             for(int i = 0; i < answer.size(); i++){
             
                 while(tot <= target){
                     
                     tot += items.get(answer.get(i).index).total;
                     newprice += items.get(answer.get(i).index).price;
                     
                     //set answer index to new price
                     if(tot >= target){
                         answer.get(i).answer = (newprice);
                         tot = 0;
                         newprice = 0;
                         break; //break
                     }
                 }
             }
             
             
             //Sort the prices by lowest price value
             Collections.sort(answer, new AnswerComparator());
             
             
             //The first index wins, it is guaranteed to be the lowest
             for(int i = 0; i < 1; i++){
                System.out.printf("Diaper Scenario #%d: Buying box %d, you spend $%.2f\n", count+1, answer.get(i).index+1, answer.get(i).answer);         
             }
             
             
              count++;               
              items.clear();    
              answer.clear();                       
         }
         
}              
     

// Max returns the highest diaper load int the waste array
 public static int Max(int cap[], int waste){
     
     int max = 0;
     
     for(int i = 0; i < waste; i++){
         
         if(cap[i] > max){
             max = cap[i];
         }
     }
     
     return max; //returns that value
 }

 
 //Items class creates the items with the attributes
 //then stores it into our arrayList
 public static class Items{
     
     public int amount;
     public double price;
     public int milli;
     public int total;
     public int index;
     public int max;
     
     public Items(int amount, double price, int milli, int total, int index){
         this.amount = amount;
         this.price = price;
         this.milli = milli;
         this.total = total;  
         this.index = index;
     }
 }
 
 
 //Class answer takes the prices in the answer array, and the index to keep track of the diaper set
 //index
 public static class Answer{
     
     double answer;
     int index;
     
     public Answer(double answer, int index){
         this.answer = answer;
         this.index = index;
     }   
 }
 
 //Answer comparator compares two prices, returns the lowest prices in descending order
 public static class AnswerComparator implements Comparator<Answer>{
         
       @Override
       public int compare(Answer a1, Answer a2){
         return Double.compare(a1.answer, a2.answer);
        }
  }
 

}
