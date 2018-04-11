#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

static double sigmoid(const double &d);

int main(){
	string init, test, result, tmp;
	int input_nodes, hidden_nodes, output_nodes;
	int sample, Ni, No;
	double micro[4];
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
	double ha[hidden_nodes];
	double oa[output_nodes];
	int boolean[output_nodes];
	int result_vector[output_nodes][4];

	for(int i = 0; i < output_nodes; i++){
		for(int j = 0; j < 4; j++) result_vector[i][j] = 0;
	}
	for(int i:micro) i = 0;
	for(double d:macro) d = 0;

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

	cout << "***Loaded trained file***" << endl;
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
	for(int i = 0; i < sample; i++){
		for(int j = 0; j < input_nodes; j++){
			getline(ifs,tmp, ' ');
			wih2[j] = stod(tmp);
		}
		for(int j = 0; j < output_nodes; j++){
			(j==output_nodes-1) ? getline(ifs,tmp):getline(ifs,tmp, ' ');
			boolean[j] = stoi(tmp);
		}

		for(int j = 0; j < hidden_nodes; j++){
			double sum = -wih[j][0];
			for(int k = 1; k < input_nodes + 1; k++){
				sum += wih[j][k] * wih2[k-1];
			}
			ha[j] = sigmoid(sum);
		}

		for(int j = 0; j < output_nodes; j++){
			double sum = -who[j][0];
			for(int k = 1; k < hidden_nodes + 1; k++){
				sum += who[j][k] * ha[k-1];
			}
			oa[j] = sigmoid(sum);
			int r = (oa[j] >= 0.5) ? 1:0;
			//cout << r << "," << boolean[j] << endl;
			if(r && boolean[j]) result_vector[j][0]++;
			else if(r && !boolean[j]) result_vector[j][1]++;
			else if(!r && boolean[j]) result_vector[j][2]++;
			else if(!r && !boolean[j]) result_vector[j][3]++;
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
		ofs << result_vector[i][0] << " " << result_vector[i][1] << " " << result_vector[i][2] << " " << result_vector[i][3] << " ";
		double accuracy = (double)(result_vector[i][0] + result_vector[i][3])/(double)(result_vector[i][0] + result_vector[i][1] + result_vector[i][2]+ result_vector[i][3]);
		double precision = (double)(result_vector[i][0])/(double)(result_vector[i][0] + result_vector[i][1]);
		double recall = (double)(result_vector[i][0])/(double)(result_vector[i][0] + result_vector[i][2]);
		double f1 = (2*precision*recall)/(precision+recall);
		micro[0] += result_vector[i][0];
		micro[1] += result_vector[i][1];
		micro[2] += result_vector[i][2];
		micro[3] += result_vector[i][3];
		macro[0] += accuracy;
		macro[1] += precision;
		macro[2] += recall;
		
		ofs << accuracy << " " << precision << " " << recall << " " << f1 <<endl;
	}

	double accuracy = (micro[0] + micro[3])/(micro[0] + micro[1] + micro[2]+ micro[3]);
	double precision = (micro[0])/(micro[0] + micro[1]);
	double recall = (micro[0])/(micro[0] + micro[2]);
	double f1 = 2*precision*recall/(precision+recall);

	ofs << accuracy << " " << precision << " " << recall << " " << f1 <<endl;

	accuracy = macro[0]/output_nodes;
	precision = macro[1]/output_nodes;
	recall = macro[2]/output_nodes;
	f1 = 2*precision*recall/(precision+recall);

	ofs << accuracy << " " << precision << " " << recall << " " << f1 <<endl;

	ofs.close();
}

double sigmoid(const double &d){
	return 1/(1+exp(-1.0*d));
}
