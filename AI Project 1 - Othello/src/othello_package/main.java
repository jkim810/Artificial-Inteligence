//
//Author: Junbum Kim
//
//To enable color coding in console
//In Eclipse select “Help” -> “Install New Software...” and click “Add...” to add the following URL:
//https://www.mihai-nita.net/eclipse
//Select all and install
//Then go to "Windows" -> "Preferences" -> "ANSI Console" and check "Standard VGA colors"
//
//credits to - https://github.com/mihnita/ansi-econsole

package othello_package;

import java.io.IOException;
import java.util.Scanner;
import java.util.concurrent.TimeoutException;

public class main {
	public static void main(String[] args) {
		boolean isHuman1 = true;
		boolean isHuman2 = true;
		boolean loaded = false;
		int startingTurn = 1;
		
		OthelloBoard mainboard = new OthelloBoard();
		Scanner scan = new Scanner(System.in);
		String input;
		
		System.out.println("***WECOME TO OTHELLO***");
		
		//Program run configurations
		do{
			System.out.print("Would you like to load file? (Y/N):");
			input = scan.nextLine();
			
		} while(!input.equalsIgnoreCase("y") && !input.equalsIgnoreCase("n"));
		
		if(input.equalsIgnoreCase("Y")) {
			System.out.print("Enter File Directory: ");
			input = scan.nextLine();
			try {
				startingTurn = mainboard.load(input);
			} catch (IOException e) {
				System.err.println("File " + input + " Not Found.");
				System.exit(0);
			}
			System.out.println("Board Loaded.");
			loaded = true;
		}else{
			do{
				System.out.print("Does Player 1 go first? (Y/N): ");
				input = scan.nextLine();
			} while(!input.equalsIgnoreCase("Y") && !input.equalsIgnoreCase("N"));
			startingTurn = (input.equalsIgnoreCase("Y")) ? 1:2;
		}
				
		do{
			System.out.print("Is Player 1 Human? (Y/N): ");
			input = scan.nextLine();
		} while(!input.equalsIgnoreCase("Y") && !input.equalsIgnoreCase("N"));
		isHuman1 = (input.equalsIgnoreCase("Y")) ? true:false;
		
		do{
			System.out.print("Is Player 2 Human? (Y/N): ");
			input = scan.nextLine();
		} while(!input.equalsIgnoreCase("Y") && !input.equalsIgnoreCase("N"));
		isHuman2 = (input.equalsIgnoreCase("Y")) ? true:false;
		
		
		if(!loaded && (!isHuman1 || !isHuman2)) { 
			System.out.print("Set time limit for computer in seconds (1~60): ");		
			int t_limit = scan.nextInt();
			while(t_limit < 0 || t_limit > 60) {
				System.out.print("Time limit out of range");
				t_limit = scan.nextInt();
			}
			mainboard.timeLimit = t_limit;
		}
		
		System.out.println("------------------------------------");
		mainboard.print();
		int result = 0;
		
		//game start
 		do {
 			if(startingTurn == 1) {
				mainboard.moves = mainboard.getMoves(mainboard.self, mainboard.opponent);
				if(mainboard.moves.isEmpty()) {
					System.out.println("Player1 has no move. Skip turn.");
				}  else {
					System.out.println("Current Turn: Player1(\u001b[31mO\u001b[0m)");	
					if(isHuman1) {
						System.out.println("Legal Moves: " + mainboard.moves);
						do{
							System.out.println("Sample Input: [47] indicates row 4, column 7.");
							System.out.print("Enter a move for Player1(\u001b[31mO\u001b[0m): ");
							result = scan.nextInt();
						}while(!mainboard.moves.contains(result));
						
					} else {
						System.out.println("Legal Moves: " + mainboard.moves);			
						long startTime = System.nanoTime(); 
						double t = 0;
						int i = 1;
						if(mainboard.moves.size() == 1) {
							for(Integer v : mainboard.moves) result = v;
							System.out.println("Only one legal move available : " + result);
						} else {
							while(true) {
								try {
									OthelloBoard tmp = new OthelloBoard(mainboard.self, mainboard.opponent, mainboard.moves, mainboard.timeLimit);
									tmp.alphabeta(tmp, startTime, true, Integer.MIN_VALUE, Integer.MAX_VALUE, 0, i++);
									if(tmp.tiemoves.size() == 1) result = tmp.tiemoves.get(0);
									else{
										int rand = 0;
										for(Integer index:tmp.tiemoves)rand+=index;
										result = tmp.tiemoves.get(rand%tmp.tiemoves.size());
									}
									if(i > 64 - tmp.self.size() - tmp.opponent.size()) {
										i--;
										break;							
									}
									t = (double) (System.nanoTime() - startTime)*1e-9;
									if(t > tmp.timeLimit * 0.7){
										break;	
									}
								} catch (TimeoutException e){
									break;
								}		
							}
							System.out.println("Searched to depth " + i + " in " + t + " seconds, plays " + result);
						}
						if(result == 0) System.err.println("Alphabeta search returned nothing. Something went wrong.");
					}
					mainboard.play(mainboard.self, mainboard.opponent, result);
					System.out.println("------------------------------------");
					mainboard.print();
				}
 			} else {
 				startingTurn = 1;
 			}
			
			mainboard.omoves = mainboard.getMoves(mainboard.opponent, mainboard.self);
			if(mainboard.omoves.isEmpty()) {
				System.out.println("PLAYER2 HAS NO MOVES. Skip turn.");
			} else {
				System.out.println("Current Turn: Player2(\u001b[36m@\u001b[0m)");	
				if(isHuman2) {
					System.out.println("Legal Moves : " + mainboard.omoves);
					do{
						System.out.println("Sample Input: [47] indicates row 4, column 7.");
						System.out.print("Enter a move for Player2(\u001b[36m@\u001b[0m): ");
						result = scan.nextInt();
					}while(!mainboard.omoves.contains(result));
				} else {
					System.out.println("Legal Moves: " + mainboard.omoves);			
					long startTime = System.nanoTime(); 
					double t = 0;
					int i = 1;
					if(mainboard.omoves.size() == 1) {
						for(Integer v : mainboard.omoves) result = v;
						System.out.println("Only one legal move available : " + result);
					} else {
						while(true) {
							try {
								OthelloBoard tmp = new OthelloBoard(mainboard.opponent, mainboard.self, mainboard.omoves, mainboard.timeLimit);
								tmp.alphabeta(tmp, startTime, true, Integer.MIN_VALUE, Integer.MAX_VALUE, 0, i++);
								if(tmp.tiemoves.size() == 1) result = tmp.tiemoves.get(0);
								else{
									int rand = 0;
									for(Integer index:tmp.tiemoves)rand+=index;
									result = tmp.tiemoves.get(rand%(tmp.tiemoves.size()));
								}
								if(i > 64 - tmp.self.size() - tmp.opponent.size()) {
									i--;
									break;							
								}
								if(t > tmp.timeLimit * 0.7){
									break;	
								}
							} catch (TimeoutException e){
								break;
							}
							t = (double) (System.nanoTime() - startTime)*1e-9;
						}
						System.out.println("Searched to depth " + i + " in " + t + " seconds, plays " + result);
					}
					if(result == 0)System.err.println("Alphabeta search returned nothing. Something went wrong.");
				}
				mainboard.play(mainboard.opponent, mainboard.self, result);
				System.out.println("------------------------------------");
				mainboard.print();
			}
			
		}while(!(mainboard.moves.isEmpty() && mainboard.omoves.isEmpty()) && (mainboard.self.size() + mainboard.opponent.size() != 64));
 		
 		//game end. print stats
 		System.out.println("***End of Game***");
 		System.out.println("Player1(\u001b[31mO\u001b[0m) score: " + mainboard.self.size());
 		System.out.println("Player2(\u001b[36m@\u001b[0m) score: " + mainboard.opponent.size());
 		if(mainboard.self.size() > mainboard.opponent.size()) {
 			System.out.println("Congrats! Player1(\u001b[31mO\u001b[0m) wins!");
 		} else if(mainboard.self.size() < mainboard.opponent.size()) {
 			System.out.println("Congrats! Player2(\u001b[36m@\u001b[0m) wins!");
 		} else {
 			System.out.println("TIE");
 		}
 		
 		scan.close();
	}	
}