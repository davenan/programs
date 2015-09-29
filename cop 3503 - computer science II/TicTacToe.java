/* Dave Nandlall
 * COP 3330 - Object Oriented Programming
 * Section Leader: Andrew Eisler
 * Assignment 4
 * November 28th, 2012
 * 
 * Did not finish the tie if no player wins
 */


import java.util.*; //Import class java.util to create new Scanner object

//create class TicTacToe
public class TicTacToe {
	
	public enum Board{X, O, EMPTY}; //Enumeration to set values of the Board
	
	private Board[][] GameBoard; //Create a GameBoard 2-D array 
	
	public static Board turn = Board.X; //turn is automatically set to Player X for the first turn	
	
	//method main begins execution of Java Application
	public static void main(String[] args){
	
		//create a new scanner object, for use in reading player row/columns
		Scanner stdin = new Scanner(System.in);
		
		//create new game of object TicTacToe
		TicTacToe startGame = new TicTacToe();
		
		int row, col; // local variables row and column
		boolean done = false; //boolean value controls while loop
		
		//Start off by printing the board
		startGame.displayBoard();
		
		//while the loop is not done
		while(!done){
		
			//Print the player controlled by turn
			System.out.printf("\n\nPlayer %s: Enter a row (0, 1 or 2)\n", turn);
			row = stdin.nextInt();
			
			System.out.printf("Player %s: Enter a column (0, 1 or 2)\n", turn);
			col = stdin.nextInt();	
		
			//Make their move on the board
			startGame.makeMove(row, col);
			
			//Display the board once more to see their move if valid
			startGame.displayBoard();
			
			//If any of the if statements in determineWinner method are true
			//Break out of the while loop, signifying end of the game
			if(startGame.determineWinner() == true)
				done = true;
						
		}//end while loop
	
}//end method main
	

// TicTacToe is a constructor function used to create a new GameBoard
public TicTacToe(){
	
	//create a new object of a GameBoard
	GameBoard = new Board[3][3];
	
	//Initialize all components to hold enumeration value EMPTY
	for(int i = 0 ; i < 3; i++){
		for(int j = 0; j < 3; j++){
		GameBoard[i][j] = Board.EMPTY;
		}
	}
	
}//end constructor TicTacToe
	


// displayBoard is responsible for printing the board and displaying it to the console
public void displayBoard(){
	
	 for (int i = 0; i < 3; i++){
		 
		 System.out.println("   ");
		 	
		 for(int j = 0; j < 3; j++){
			 
			 //Can't print the enumeration value EMPTY so, use convertBoard to change it
			 System.out.print(convertBoard(GameBoard[i][j]) + "  ");
		 }
	 }			
}// end method displayBoard



//convertBoard takes in the enumeration values of Board, and converts it to the following...
public char convertBoard(Board b){
	
	//If the value of the cell is of Board.X, return a character X
	if(b == Board.X)
	   return 'X';
	
	//If the value of the cell is of Board.O, return a character O
	if(b == Board.O)
	   return 'O';
	
	//If the value of the cell is of Board.EMPTY, return character _ to signify an empty cell
	else
	   return '_';
	
}//end method convertBoard


// checkBounds has an important task
// it is responsible for checking array out of bounds errors,
// by passing the user input row/column as parameters and returning either boolean true or false
public boolean checkBounds(int row, int col, Board turn){
	
	//If the row or columns do not meet constraints of the board, return true
	if(row < 0 || row > 2 || col < 0 || col > 2){
		System.out.printf("Player %s: Invalid Row or Column Try Again!!!\n", turn);
		return true;
	}
	
	//If the players move does meet valid row/column, return false
	return false;
	
}//end method checkBounds



//checkMove checks whether move is valid or not
public boolean checkMove(int row, int col){
	
	//If the slot on the board is EMPTY, return true
	if(GameBoard[row][col] == Board.EMPTY)
		return true;
	
	//If not EMPTY return false
	else
		return false;
	
}//end method checkMove



//makeMove method is responsible for placing the players move to the board
//a lot of stuff is done in this one method, using input of row and column as parameters
public void makeMove(int row, int col){

	//Loop to stay on row the user requested
	for(int i = row; i <= row; i++){
		
		//Loop to stay on column the user requested
		for(int j = col; j <= col; j++){
		
		//For all cases, we have to check if the user input a valid row/column entry
		if(checkBounds(row, col, turn) != true){
			
			//So if turn is player X, and the desired slot is empty
			if(turn == Board.X && checkMove(row, col) == true){
				GameBoard[i][j] = Board.X; //Place an X in the slot
				turn = Board.O;	 //Change turn to player O
			}
			
			//Likewise, if turn is player O, and desired slot is empty
			else if(turn == Board.O && checkMove(row, col) == true){
				GameBoard[i][j] = Board.O; //Place an O in the slot
				turn = Board.X; //Change turn to player X
			}
			
			//If turn is player X, and slot is taken already
			else if(turn == Board.X && checkMove(row, col) != true){
				
				turn = Board.X; //Keep turn on player X
				
				//Print a message to show if either X already marked the spot or if player O did
				System.out.printf("\nPlayer %s has marked that spot\n", GameBoard[row][col]);
				System.out.print("Try Again!\n");
			}
			
			
			//If turn is now player O, and slot is taken already
			else if(turn == Board.O && checkMove(row, col) != true){
				
				turn = Board.O;	//Keep turn on player O
				
				//Print a message to show if either X already marked the spot or if player O did
				System.out.printf("\nPlayer %s has marked that spot\n", GameBoard[row][col]);
				System.out.print("Try Again!\n");
			}
						
			}// end if(checkBounds)
		
		}//end j for loop
		
	}//end i for loop
	
}//end method makeMove



//determineWinner calculates the winner on the GameBoard
//Although there are better algorithms to do this, I used it because it was easier

public boolean determineWinner(){
	
	 //check winner row 1
	 if (GameBoard[0][0] == GameBoard[0][1] && GameBoard[0][0] == GameBoard[0][2] &&
		 GameBoard[0][0] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[0][0]);
		 return true;
	 }
	 
	 //check winner row 2
	 if (GameBoard[1][0] == GameBoard[1][1] && GameBoard[1][0] == GameBoard[1][2] &&
		 GameBoard[1][0] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[1][0]);
		 return true;
	 }
	
	 //check winner row 3
	 if (GameBoard[2][0] == GameBoard[2][1] && GameBoard[2][0] == GameBoard[2][2] &&
		 GameBoard[2][0] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[2][0]);
		 return true;
	 }
	 
	 //check winner column 1
	 if (GameBoard[0][0] == GameBoard[1][0] && GameBoard[0][0] == GameBoard[2][0] &&
		 GameBoard[0][0] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[1][0]);
		 return true;
	 }
	 
	 //check winner column 2
	 if (GameBoard[0][1] == GameBoard[1][1] && GameBoard[0][1] == GameBoard[2][1] &&
		 GameBoard[0][1] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[1][1]);
	     return true;
	 }
	
	 //check winner column 3
	 if (GameBoard[0][2] == GameBoard[1][2] && GameBoard[0][2] == GameBoard[2][2] &&
		 GameBoard[0][2] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[0][2]);
		 return true;
	 }
	 
	 //check diagonal 1
	 if (GameBoard[0][0] == GameBoard[1][1] && GameBoard[0][0] == GameBoard[2][2] &&
		 GameBoard[0][0] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[1][1]);
		 return true;
	 }
	 
	 //check diagonal 2
	 if (GameBoard[2][0] == GameBoard[1][1] && GameBoard[2][0] == GameBoard[0][2] &&
		 GameBoard[2][0] != Board.EMPTY){
		 System.out.printf("\n\nCongratulations Player %s you won!", GameBoard[1][1]);
		 return true;
	 }
	 
	return false;
	
}//end method determineWinner

} // end class TicTacToe
