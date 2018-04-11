#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

static double sigmoid(const double &d);
static double sigprime(const double &d);

int main(){
	string init, train, result, tmp;
	vector<vector<double> > wih;
	vector<vector<double> > who;
	vector<double> wih2;
	vector<double> hiddenIn;
	vector<double> hiddenA;
	vector<double> hiddenD;
	vector<double> outputIn;
	vector<double> outputA;
	vector<double> outputD;
	vector<int> boolean;

	//cout << "Enter Initial Neural Network: ";
	//cin >> input;
	//set this to value for now
	init = "sample.NNGrades.init";
	train = "grades.train";
	result = "sample.NNGrades.result";
	
	ifstream ifs(init.c_str());
	
	int input_nodes, hidden_nodes, output_nodes;
	getline(ifs, tmp, ' ');
	input_nodes = stoi(tmp);
	getline(ifs, tmp, ' ');
	hidden_nodes = stoi(tmp);
	getline(ifs, tmp);
	output_nodes = stoi(tmp);

	int epoch = 100;
	double rate = 0.05;
	
	wih.resize(hidden_nodes, vector<double> (input_nodes + 1));
	who.resize(output_nodes, vector<double> (hidden_nodes + 1));

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

	ifs.close();

	//Read in train file
	ifs.open(train.c_str());
	int sample, Ni, No;
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

	wih2.resize(input_nodes);
	hiddenIn.resize(input_nodes);
	hiddenA.resize(input_nodes);
	hiddenD.resize(input_nodes);
	boolean.resize(output_nodes);
	outputIn.resize(output_nodes);
	outputA.resize(output_nodes);
	outputD.resize(output_nodes);

	/*double wih2[input_nodes];
	int boolean[output_nodes];
	double hiddenIn[input_nodes];
	double hiddenA[input_nodes];
	double hiddenD[input_nodes];
	double outputIn[output_nodes];
	double outputA[output_nodes];
	double outputD[output_nodes];
	*/			
	/*ofs << "#sample: " << sample << endl;
	ofs << "#input nodes: " << input_nodes << endl;
	ofs << "#output nodes: " << No << endl;
	*/
	ifs.close();

	for(int i = 0; i < 1; i++){
		ifs.open(train.c_str());
		getline(ifs, tmp);
		for(int j = 0; j < 1; j++){
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
				hiddenIn[k] = sum;
				hiddenA[k] = sigmoid(sum);
			}
			
			for(int k = 0; k < output_nodes; k++){
				double sum = -who[k][0];
				for(int l = 1; l < hidden_nodes + 1; l++){
					sum += who[k][l] * hiddenA[l-1];
				}
				outputIn[k] = sum;
				outputA[k] = sigmoid(sum);
			}

			for(int k = 0; k < output_nodes; k++){
				outputD[k] = sigprime(outputIn[k]) * (boolean[k] - outputA[k]);
			}
			for(int k = 0; k < output_nodes; k++){
				cout << outputD[k] << endl;
			}

			for(int k = 0; k < hidden_nodes; k++){
				double sum2 = 0.0;
				for(int l = 0; l < output_nodes; l++){
					sum2 += who[l][k+1] * outputD[l];
				}
				hiddenD[k] = sigprime(hiddenIn[k]) * sum2;
			}

			for(int k = 0; k < hidden_nodes; k++){
				wih[k][0] = wih[k][0] + rate * -1.0 * hiddenD[k];
				for(int l = 1; l < input_nodes + 1; l++){
					wih[k][l] = wih[k][l] + rate * wih2[l-1] * hiddenD[k];
				} 
			}

			for(int k = 0; k < output_nodes; k++){
				who[k][0] = who[k][0] + rate * -1.0 * outputD[k];
				for(int l = 1; l < hidden_nodes + 1; l++){
					who[k][l] = who[k][l] + rate * hiddenA[l-1] * outputD[k]; 
				} 
			}
		}
		ifs.close();

		ofstream ofs(result.c_str());
		ofs << setprecision(3) << fixed;
		ofs << input_nodes << " " << hidden_nodes << " " << output_nodes << endl;
		for(int i = 0; i < hidden_nodes; i++){
			for(int j = 0; j < input_nodes + 1; j++){
				ofs << wih[i][j] << " ";
			}
			ofs << endl;
		}

		for(int i = 0; i < output_nodes; i++){
			for(int j = 0; j < hidden_nodes + 1; j++){
				ofs << who[i][j] << " ";
			}
			ofs << endl;
		}
		ofs.close();

	}

}

double sigmoid(const double &d){
	return 1/(1+exp(-1.0*d));
}

double sigprime(const double &d){
	return sigmoid(d)*(1-sigmoid(d));
}