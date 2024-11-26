#ifndef Softmax_H
#define Softmax_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;




struct Softmax{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* output[MAX_BATCH_SIZE];
	float* d_output[MAX_BATCH_SIZE];
	float* sum;
	int N;

	Softmax(int _N){
		N = _N;
		sum = new float[MAX_BATCH_SIZE];
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
			float mint = input[i][0];
			for(int j = 0; j < N; j++){
				mint = min(mint,input[i][j]);
			}
			for(int j = 0; j < N; j++){
				input[i][j]-=mint;
			}
			sum[i] = 0.0;
			for(int j = 0; j < N; j++){
				sum[i] += safe_exp(input[i][j]);				
			}
			for(int j = 0; j < N; j++){
				output[i][j] = safe_exp(input[i][j])/sum[i];				
			}
		}
	}

	void calculate_derivatives(int batch_size){
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < N; j++){
				d_input[i][j] = d_output[i][j]*(1-output[i][j])*output[i][j];//ok
			}
			for(int j = 0; j < N; j++){
				for(int k = 0; k < N; k++){
					if(j != k){
						d_input[i][j] -= d_output[i][k]*(safe_exp(input[i][j])/sum[i])*(safe_exp(input[i][k])/sum[i]);
					}
				}
			}
		}
	}

	void update(int batch_size, float lambda){
		//nothing to update
	}
};
#endif
