#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

static double sigmoid(const double &d);
static double sigprime(const double &d);

int main(){
	string init, train, result, tmp;
	int input_nodes, hidden_nodes, output_nodes;
	int sample, Ni, No;
	int epoch = 100;
	double rate = 0.05;
	ifstream ifs;
	ofstream ofs;

	do{
		cout << "Enter Initial Neural Network: ";
		cin >> init;
		ifs.open(init.c_str());
		if(ifs.is_open()) break;
		cerr << "DEBUG: File Not Found" << endl;
	}while(1);
		
	getline(ifs, tmp, ' ');
	input_nodes = stoi(tmp);
	getline(ifs, tmp, ' ');
	hidden_nodes = stoi(tmp);
	getline(ifs, tmp);
	output_nodes = stoi(tmp);
	
	double wih[hidden_nodes][input_nodes + 1];
	double who[output_nodes][hidden_nodes + 1];
	double wih2[input_nodes];
	double hi[hidden_nodes];
	double ha[hidden_nodes];
	double hd[hidden_nodes];
	double oi[output_nodes];
	double oa[output_nodes];
	double od[output_nodes];
	int boolean[output_nodes];
				

	for(int i = 0; i < hidden_nodes; i++){
		for(int j = 0; j < input_nodes + 1; j++){
			(j==input_nodes)?getline(ifs,tmp):getline(ifs, tmp, ' ');
			wih[i][j] = stod(tmp);
		}
	}

	//cout << "Edges from hidden nodes to output nodes" << endl;
	for(int i = 0; i < output_nodes; i++){
		for(int j = 0; j < hidden_nodes + 1; j++){
			(j==hidden_nodes)?getline(ifs,tmp):getline(ifs, tmp, ' ');
			who[i][j] = stod(tmp);
		}
	}

	cout << "***Loaded init file***" << endl;
	ifs.close();

	do{
		cout << "Enter train file: ";
		cin >> train;
		ifs.open(train.c_str());
		if(ifs.is_open()) break;
		cerr << "DEBUG: File Not Found" << endl;
	}while(1);

	//Read in train file
	do{
		cout << "Enter epoch: ";
		if(cin >> epoch) break;
		cout << "DEBUG: epoch has to be an integer" << endl;
	}while(1);

	do{
		cout << "Enter learning rate: ";
		if(cin >> rate) break;
		cout << "DEBUG: epoch has to be a double" << endl;
	}while(1);
	
	getline(ifs, tmp, ' ');
	sample = stoi(tmp);
	getline(ifs, tmp, ' ');
	Ni = stoi(tmp);

	if(Ni != input_nodes){
		cerr <<	"Fatal Error: Ni of train file does not match init file" << endl;
		exit(255);
	}
	getline(ifs, tmp);
	No = stoi(tmp);
	if(No != output_nodes) {
		cerr <<	"Fatal Error: No of train file does not match init file" << endl;
		exit(255);
	}

	ifs.close();
	//back propagation
	for(int i = 0; i < epoch; i++){
		ifs.open(train.c_str());
		getline(ifs, tmp);
		for(int j = 0; j < sample; j++){
			for(int k = 0; k < input_nodes; k++){
				getline(ifs,tmp, ' ');
				wih2[k] = stod(tmp);
			}

			for(int k = 0; k < output_nodes; k++){
				(k==output_nodes-1) ? getline(ifs,tmp):getline(ifs,tmp, ' ');
				boolean[k] = stoi(tmp);
			}

			for(int k = 0; k < hidden_nodes; k++){
				double sum = -wih[k][0];
				for(int l = 1; l < input_nodes + 1; l++){
					sum += wih[k][l] * wih2[l-1];
				}
				hi[k] = sum;
				ha[k] = sigmoid(sum);
			}

			for(int k = 0; k < output_nodes; k++){
				double sum = -who[k][0];
				for(int l = 1; l < hidden_nodes + 1; l++){
					sum += who[k][l] * ha[l-1];
				}
				oi[k] = sum;
				oa[k] = sigmoid(sum);
			}

			for(int k = 0; k < output_nodes; k++){
				od[k] = sigprime(oi[k]) * (boolean[k] - oa[k]);
			}

			for(int k = 0; k < hidden_nodes; k++){
				double sum2 = 0.0;
				for(int l = 0; l < output_nodes; l++){
					sum2 += who[l][k+1] * od[l];
				}
				hd[k] = sigprime(hi[k]) * sum2;
			}

			for(int k = 0; k < hidden_nodes; k++){
				wih[k][0] = wih[k][0] + rate * -1.0 * hd[k];
				for(int l = 1; l < input_nodes + 1; l++){
					wih[k][l] = wih[k][l] + rate * wih2[l-1] * hd[k];
				} 
			}

			for(int k = 0; k < output_nodes; k++){
				who[k][0] = who[k][0] + rate * -1.0 * od[k];
				for(int l = 1; l < hidden_nodes + 1; l++){
					who[k][l] = who[k][l] + rate * ha[l-1] * od[k]; 
				} 
			}
		}
		ifs.close();
	}

	cout << "***Finished Training***" << endl;

	do{
		cout << "Enter name for trained file: ";
		cin >> result;
		ofs.open(result.c_str());
		if(ofs.is_open()) break;
		cerr << "DEBUG: File Cannot be opened" << endl;
	}while(1);


	//output
	ofs << setprecision(3) << fixed;
	ofs << input_nodes << " " << hidden_nodes << " " << output_nodes << endl;

	for(int i = 0; i < hidden_nodes; i++){
		for(int j = 0; j < input_nodes; j++){
			ofs << wih[i][j] << " ";
		}
		ofs << wih[i][input_nodes] << endl;
	}

	for(int i = 0; i < output_nodes; i++){
		for(int j = 0; j < hidden_nodes; j++){
			ofs << who[i][j] << " ";
		}
		ofs << who[i][hidden_nodes] << endl;
	}
	ofs.close();
}

double sigmoid(const double &d){
	return 1/(1+exp(-1.0*d));
}

double sigprime(const double &d){
	return sigmoid(d)*(1-sigmoid(d));
}