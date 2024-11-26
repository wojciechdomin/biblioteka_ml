#ifndef Dense_H
#define Dense_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;


struct Dense{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* output[MAX_BATCH_SIZE];
	float* d_output[MAX_BATCH_SIZE];
	float* matrix;
	float* d_matrix;
	int N,M;

	Dense(int _N, int _M){
		N = _N; M = _M;
		matrix = new float[N*M];
		d_matrix = new float[N*M]; 
		for(int i = 0; i < N*M; i++){
			matrix[i] = float(rand())*0.001/float(RAND_MAX)-0.0005; 
		}
		for(int i = 0; i < MAX_BATCH_SIZE; i++){
			output[i] = new float[M];
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
		// debug("?");
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < M; j++){
				// debug("??");
				output[i][j] = 0.0;
				// debug("???");
				for(int k = 0; k < N; k++){
					output[i][j] += input[i][k]*matrix[k*M+j];
				}
			}
		}
		// debug("????");
	}

	void calculate_derivatives(int batch_size){
		for(int j = 0; j < M; j++){
			for(int k = 0; k < N; k++){
				d_matrix[k*M+j] = 0.0;
			}
		}
		for(int i = 0; i < batch_size; i++){
			for(int k = 0; k < N; k++){
				d_input[i][k] = 0.0;
			}
		}
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < M; j++){
				for(int k = 0; k < N; k++){
					d_matrix[k*M+j] += d_output[i][j]*input[i][k];
					d_input[i][k] += matrix[k*M+j]*d_output[i][j];
				}
			}
		}
	}

	/*void update(int batch_size, float lambda){
		for(int i = 0; i < N*M; i++){
			matrix[i] -= lambda*d_matrix[i];
		}
	}*/
	vector<ParameterContainer> get_params(){
		vector<ParameterContainer> params;
		ParameterContainer matrix_container(matrix,d_matrix,N*M);
		params.push_back(matrix_container);
		return params;
	}
	void statistics(){
		float mint = matrix[0];
		float maxt = matrix[0];
		for(int i = 0; i < N*M; i++){
			mint = min(mint, matrix[i]);
			maxt = max(maxt, matrix[i]);
		}
		cout<<"\n-----------------------\n";
		cout<<"Dense statistics\n";
		debug(N);
		debug(M);
		debug(mint);
		debug(maxt);
		cout<<"\n-----------------------\n";
	}
};
#endif
