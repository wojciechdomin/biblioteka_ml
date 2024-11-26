#ifndef ReLU_H
#define ReLU_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;


struct ReLU{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* output[MAX_BATCH_SIZE];
	float* d_output[MAX_BATCH_SIZE];
	int N;

	ReLU(int _N){
		N = _N;
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
				if(input[i][j] < 0.0){
					output[i][j] = 0.0;
				}else{
					output[i][j] = input[i][j];
				}
			}
		}

	}

	void calculate_derivatives(int batch_size){
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < N; j++){
				if(input[i][j] < 0.0){
					d_input[i][j] = 0.0;
				}else{
					d_input[i][j] = d_output[i][j];
				}
			}
		}
	}

	void update(int batch_size, float lambda){
		//nothing to update
	}
};
#endif