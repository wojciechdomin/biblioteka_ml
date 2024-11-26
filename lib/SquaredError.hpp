#ifndef SquaredError_H
#define SquaredError_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;


struct SquaredError{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* ground[MAX_BATCH_SIZE];
	int N;
	float Loss;
	SquaredError(int _N){
		N = _N;
		for(int i = 0; i < MAX_BATCH_SIZE; i++){
			d_input[i] = new float[N];
		}
	}
	void accept_input(float* inp[MAX_BATCH_SIZE], int batch_size){
		assert(batch_size < MAX_BATCH_SIZE);
		for(int i = 0; i < batch_size; i++){
			input[i] = inp[i];
		}
	}
	void accept_ground(float* gro[MAX_BATCH_SIZE], int batch_size){
		assert(batch_size < MAX_BATCH_SIZE);
		for(int i = 0; i < batch_size; i++){
			ground[i] = gro[i];
		}
	}
	void calculate_loss(int batch_size){
		Loss = 0.0;
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < N; j++){			
			//	debug(Loss);
				Loss += (input[i][j]-ground[i][j])*(input[i][j]-ground[i][j]);			
			}
		}
	}

	void calculate_derivatives(int batch_size){
		for(int i = 0; i < batch_size; i++){
			for(int j = 0; j < N; j++){			
				d_input[i][j] = 2*(input[i][j]-ground[i][j]);			
			}
		}		
	}
};
#endif
