package othello_package;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.concurrent.TimeoutException;

public class OthelloBoard {
	public double timeLimit;
	public ArrayList <Integer> tiemoves = new ArrayList <Integer>();
	public HashSet <Integer> self = new HashSet <Integer> (64);
	public HashSet <Integer> opponent = new HashSet <Integer> (64);
	public HashSet <Integer> moves = new HashSet <Integer> (64);
	public HashSet <Integer> omoves = new HashSet <Integer> (64);
	
	//initializer
	public OthelloBoard(){
		self.add(54);
		self.add(45);
		opponent.add(44);
		opponent.add(55);
	}
	
	//alpha beta search initializer
	public OthelloBoard(HashSet <Integer> sp, HashSet <Integer> op, HashSet <Integer> m, double timeLimit){
		self = (HashSet<Integer>) sp.clone();
		opponent = (HashSet<Integer>) op.clone();
		moves = (HashSet<Integer>) m.clone();
		this.timeLimit = timeLimit;
	}
	
	//internal class for alphabeta search
	private OthelloBoard(HashSet <Integer> sp, HashSet <Integer> op){
		self = (HashSet<Integer>) sp.clone();
		opponent = (HashSet<Integer>) op.clone();
	}	

	//internal class for alphabeta search heuristic evaluation
	private OthelloBoard(HashSet <Integer> sp, HashSet <Integer> op, HashSet <Integer> m, HashSet <Integer> om){
		self = (HashSet<Integer>) sp.clone();
		opponent = (HashSet<Integer>) op.clone();
		moves = (HashSet<Integer>) m.clone();
		omoves = (HashSet<Integer>) om.clone();
	}

	public int alphabeta(OthelloBoard parent, long startingTime, boolean max, int alpha, int beta, int depth, int limit) throws TimeoutException {
		double timeStamp = (System.nanoTime() - startingTime)*1e-9;
		if(timeStamp > parent.timeLimit * 0.98) throw new TimeoutException();
		int val = (max) ? Integer.MIN_VALUE:Integer.MAX_VALUE;
		int searchVal;
		OthelloBoard child;
		
		if(depth==0)tiemoves.clear();
		if(parent.moves.contains(0)) parent.moves.remove(0);
		
		if(parent.self.size() + parent.opponent.size() == 64 && depth!=0) {
			if(max) return parent.self.size();
			else return parent.opponent.size();			
		}
		
		if(depth == limit) {
			if(max) child = new OthelloBoard(parent.self, parent.opponent, parent.moves, parent.getMoves(parent.opponent, parent.self));
			else child = new OthelloBoard(parent.opponent, parent.self, parent.getMoves(parent.self, parent.opponent),parent.moves);
			return heuristic(child);			
		}

		if(parent.moves.isEmpty())	parent.moves.add(0);
		
		int r = parent.moves.iterator().next();
		for(Integer k: parent.moves) {
			OthelloBoard tmp = new OthelloBoard(parent.self, parent.opponent);
			tmp.play(tmp.self, tmp.opponent, k);
			tmp.moves = tmp.getMoves(tmp.opponent, tmp.self);
			child = new OthelloBoard(tmp.opponent, tmp.self, tmp.moves, parent.timeLimit);
			if(max) {
				searchVal = alphabeta(child, startingTime, !max, alpha, beta, depth+1, limit);
				if(searchVal > val) {					
					if(depth == 0) tiemoves.add(k);
					val = searchVal;
				}
				if(val > alpha) alpha = val;
				if(val >= beta) break;
				
			} else {
				searchVal = alphabeta(child, startingTime, !max, alpha, beta, depth+1, limit);
				if(val > searchVal) val = searchVal;
				if(beta > val) beta = val;
				if(val <= alpha) break;
			}			
		}
		return val;
	}
	
	private int heuristic(OthelloBoard b) {
		int corners = 0, edges = 0;
		for(Integer k: b.self) {
			int p = k.intValue();
			if(p == 11 || p == 18 || p == 81 || p == 88) corners++;
			if(((p%10 == 8) || (p%10 == 1)) && ((p/10) < 7 && (p/10) > 2)) edges++;
			if(((p/10 == 8) || (p/10 == 1)) && ((p%10) < 7 && (p%10) > 2)) edges++;
		}
		
		for(Integer k : b.opponent) {
			int p = k.intValue();
			if(p == 11 || p == 18 || p == 81 || p == 88) corners--;
			if(((p%10 == 8) || (p%10 == 1)) && ((p/10) < 7 && (p/10) > 2)) edges--;
			if(((p/10 == 8) || (p/10 == 1)) && ((p%10) < 7 && (p%10) > 2)) edges--;
		}

		int size = b.self.size() - b.opponent.size();
		int move = b.moves.size() - b.omoves.size();
		if(size > 40) {
			return move + size * 2 + corners * 6 + edges;
		} else {
			return move * 5 + corners * 12 + size + edges * 4;
		}
	}
	
	public HashSet<Integer> getMoves(HashSet<Integer> self, HashSet<Integer> opponent) {
		HashSet<Integer> moves = new HashSet<Integer>();
		int itr;
		for (Integer p : self) {
			if((p%10) != 1 && opponent.contains(p-1)) {
				itr = p-2;
				while((itr%10) > 0 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr--;
				}
			}
			if((p%10) != 8 && opponent.contains(p+1)) {
				itr = p+2;
				while((itr%10) < 9 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr++;
				}
			}
			if((p/10) != 1 && opponent.contains(p-10)) {
				itr = p-20;
				while((itr/10) > 0 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr-=10;
				}
			}
			if((p/10) != 8 && opponent.contains(p+10)) {
				itr = p+20;
				while((itr/10) < 9 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr+=10;
				}
			}
			if((p%10) != 1 && (p/10) != 1 &&  opponent.contains(p-11)) {
				itr = p-22;
				while((itr%10) > 0 && (itr/10) > 0 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr-=11;
				}
			}
			if((p%10) != 8 && (p/10) != 1 &&  opponent.contains(p-9)) {
				itr = p-18;
				while((itr%10) < 9 && (itr/10) > 0 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr-=9;
				}
			}
			if((p%10) != 8 && (p/10) != 8 &&  opponent.contains(p+11)) {
				itr = p+22;
				while((itr%10) < 9 && (itr/10) < 9 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr+=11;
				}
			}
			if((p%10) != 1 && (p/10) != 8 &&  opponent.contains(p+9)) {
				itr = p+18;
				while((itr%10) > 0 && (itr/10) < 9 && !self.contains(itr)) {
					if(!opponent.contains(itr)) {
						moves.add(itr);
						break;
					}
					itr+=9;
				}
			}
		}
		return moves;		
	}

	public void play(HashSet<Integer> self, HashSet<Integer> opponent, int p) {
		ArrayList<Integer> tmp = new ArrayList<Integer>();
		ArrayList<Integer> piecesToAdd = new ArrayList<Integer>();
		int itr;
		itr = p-1;
		while((itr%10) > 1 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr-1)) {
				piecesToAdd.addAll(tmp);
				break;
			}			
			itr--;
		}
		tmp.clear();

		itr = p+1;
		while((itr%10) < 9 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr+1)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr++;
		}
		tmp.clear();

		itr = p-10;
		while((itr/10) > 0 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr-10)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr-=10;
		}
		tmp.clear();

		itr = p+10;
		while((itr/10) < 9 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr+10)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr+=10;
		}
		tmp.clear();

		itr = p-11;
		while((itr%10) > 0 && (itr/10) > 0 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr-11)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr-=11;
		}
		tmp.clear();

		itr = p-9;
		while((itr%10) < 9 && (itr/10) > 0 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr-9)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr-=9;
		}
		tmp.clear();

		itr = p+11;
		while((itr%10) < 9 && (itr/10) < 9 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr+11)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr+=11;
		}
		tmp.clear();

		itr = p+9;
		while((itr%10) > 0 && (itr/10) < 9 && opponent.contains(itr)) {
			tmp.add(itr);
			if(self.contains(itr+9)) {
				piecesToAdd.addAll(tmp);
				break;
			}
			itr+=9;
		}
		tmp.clear();

		opponent.removeAll(piecesToAdd);
		self.addAll(piecesToAdd);
		self.add(p);
	}
	
	public void print() {
		System.out.println("      1   2   3   4   5   6   7   8   ");
		System.out.println("    +---+---+---+---+---+---+---+---+");
		
		for(int i = 0; i < 8; i++) {
			System.out.print(" " + (i+1)+ "  |");
			for(int j = 0; j < 8; j++) {
				int tmp = (i+1)*10 + j+1;
				if(self.contains(tmp)) System.out.print(" \u001b[31mO\u001b[0m |");
				else if(opponent.contains(tmp)) System.out.print(" \u001b[36m@\u001b[0m |");
				else System.out.print("   |");
			}
			if(i==1) System.out.print("   Round " + (self.size() + opponent.size() - 3));
			if(i==2) System.out.print("   Player 1(\u001b[31mO\u001b[0m) score: " + self.size());
			if(i==3) System.out.print("   Player 2(\u001b[36m@\u001b[0m) score: " + opponent.size());
			System.out.println("\n    +---+---+---+---+---+---+---+---+");
		}
	}
	
	public int load(String filename) throws IOException {
		int startingTurn = 0;
		self.clear();
		opponent.clear();
		BufferedReader br = new BufferedReader(new FileReader(filename));
		try {
		    String line = br.readLine();
		    for(int i = 1, j = 1; i <= 10; i++, j = 1){
		        for(String n : line.split(" ")) {
		        	int k = Integer.parseInt(n);
		        	if(i == 9){
		        		startingTurn = k;
			    		break;
			    	}
		        	if(i == 10){
		        		timeLimit = k;
			    		break;
			    	}
		        	if(k == 1) self.add((i*10)+j);
		        	if(k == 2) opponent.add((i*10)+j);
		        	j++;
		        }       
		        line = br.readLine();
		    }
		} finally {
		    br.close();
		}
		return startingTurn;
	}
}

