#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

inline static double sigmoid(const double &d);
inline double accuracy(const int *arr);
inline double precision(const int *arr);
inline double recall(const int *arr);

int main(){
	string init, test, result, tmp;
	int input_nodes, hidden_nodes, output_nodes;
	double pre, acc, rec, f1;
	int sample, Ni, No;
	int micro[4];
	double macro[3];
	ifstream ifs;
	ofstream ofs;

	do{
		cout << "Enter Trained Neural Network: ";
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
	double hiddenA[hidden_nodes];
	double outputA[output_nodes];
	int boolean[output_nodes];
	int result_vector[output_nodes][4];

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
		cout << "Enter test file: ";
		cin >> test;
		ifs.open(test.c_str());
		if(ifs.is_open()) break;
		cerr << "DEBUG: File Not Found" << endl;
	}while(1);

	getline(ifs, tmp, ' ');
	sample = stoi(tmp);
	getline(ifs, tmp, ' ');
	Ni = stoi(tmp);

	if(Ni != input_nodes){
		cerr <<	"Fatal Error: Ni of test file does not match init file" << endl;
		exit(255);
	}
	getline(ifs, tmp);
	No = stoi(tmp);
	if(No != output_nodes) {
		cerr <<	"Fatal Error: No of test file does not match init file" << endl;
		exit(255);
	}

	//back propagation
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
			hiddenA[k] = sigmoid(sum);
		}
		for(int k = 0; k < output_nodes; k++){
			double sum = -who[k][0];
			for(int l = 1; l < hidden_nodes + 1; l++){
				sum += who[k][l] * hiddenA[l-1];
			}
			outputA[k] = sigmoid(sum);
			int r = (outputA[k] >= 0.5) ? 1:0;
			if(r && boolean[k]) result_vector[k][0]++;
			else if(r && !boolean[k]) result_vector[k][1]++;
			else if(!r && boolean[k]) result_vector[k][2]++;
			else if(!r && !boolean[k]) result_vector[k][3]++;
		}
	}
	ifs.close();	

	cout << "***Finished Training***" << endl;

	do{
		cout << "Enter result file: ";
		cin >> result;
		ofs.open(result.c_str());
		if(ofs.is_open()) break;
		cerr << "DEBUG: File cannot be opened" << endl;
	}while(1);


	//output
	ofs << setprecision(3) << fixed;
	for(int i = 0; i < output_nodes; i++){
		acc = accuracy(result_vector[i]);
		pre = precision(result_vector[i]);
		rec = recall(result_vector[i]);
		f1 = 2*pre*rec/(pre+rec);

		micro[0] += result_vector[i][0];
		micro[1] += result_vector[i][1];
		micro[2] += result_vector[i][2];
		micro[3] += result_vector[i][3];
		macro[0] += acc;
		macro[1] += pre;
		macro[2] += rec;

		ofs << result_vector[i][0] << " " << result_vector[i][1] << " " << result_vector[i][2] << " " << result_vector[i][3] << " ";
		ofs << acc << " " << pre << " " << rec << " " << f1 <<endl;
	}

	acc = accuracy(micro);
	pre = precision(micro);
	rec = recall(micro);
	f1 = 2*pre*rec/(pre+rec);

	ofs << acc << " " << pre << " " << rec << " " << f1 <<endl;

	acc = macro[0]/output_nodes;
	pre = macro[1]/output_nodes;
	rec = macro[2]/output_nodes;
	f1 = 2*pre*rec/(pre+rec);

	ofs << acc << " " << pre << " " << rec << " " << f1 <<endl;

	ofs.close();
}

double sigmoid(const double &d) { return 1/(1+exp(-1.0*d)); }
double accuracy(const int *arr) { return (arr[0] + arr[3])/(arr[0] + arr[1] + arr[2] + arr[3]); }
double precision(const int *arr) { return arr[0]/(arr[0] + arr[1]); }
double recall(const int *arr) { return arr[0]/(arr[0] + arr[2]); }