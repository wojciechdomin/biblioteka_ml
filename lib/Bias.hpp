#ifndef Bias_H
#define Bias_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;


struct Bias{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* output[MAX_BATCH_SIZE];
	float* d_output[MAX_BATCH_SIZE];
	float* bias;
	float* d_bias;
	int N;

	Bias(int _N){
		N = _N;
		bias = new float[N];
		d_bias = new float[N]; 
		for(int i = 0; i < N; i++){
			bias[i] = float(rand())*0.001/float(RAND_MAX)-0.0005; 
		}
		for(int i = 0; i < MAX_BATCH_SIZE; i++){
			output[i] = new float[N];
			d_input[i] = new float[N];
		}
	}
	void accept_input(float* inp[MAX_BATCH_SIZE], int batch_size){
		assert(batch_size < MAX_BATCH_SIZE);
		for(int i = 0; i < batch_size; i++){
			input[i] = inp[i];
		}
	}
	void accept_derivatives(float* der[MAX_BATCH_SIZE], int batch_size){
		assert(batch_size < MAX_BATCH_SIZE);
		for(int i = 0; i < batch_size; i++){
			d_output[i] = der[i];
		}
	}
	void calculate_output(int batch_size){
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < N; j++){
				output[i][j] = input[i][j] + bias[j];
			}
		}
	
	}

	void calculate_derivatives(int batch_size){
		for(int i = 0; i < N; i++){
			d_bias[i] = 0.0;
		}
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < N; j++){
				d_bias[j] += d_output[i][j];
				d_input[i][j] = d_output[i][j];
			}
		}
	}

	/*void update(int batch_size, float lambda){
		for(int i = 0; i < N; i++){
			bias[i] -= lambda*d_bias[i];
		}
	}*/
	vector<ParameterContainer> get_params(){
		vector<ParameterContainer> params;
		ParameterContainer bias_container(bias,d_bias,N);
		params.push_back(bias_container);
		return params;
	}
	void statistics(){
		float mint = bias[0];
		float maxt = bias[0];
		for(int i = 0; i < N; i++){
			mint = min(mint, bias[i]);
			maxt = max(maxt, bias[i]);
		}
		cout<<"\n-----------------------\n";
		cout<<"Bias statistics\n";
		debug(N);
		debug(mint);
		debug(maxt);
		cout<<"\n-----------------------\n";
	}
};
#endif
