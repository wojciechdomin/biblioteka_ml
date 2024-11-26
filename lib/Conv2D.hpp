#ifndef Conv2D_H
#define Conv2D_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;

struct Conv2D{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* output[MAX_BATCH_SIZE];
	float* d_output[MAX_BATCH_SIZE];
	float* filter;
	float* d_filter;
	int N,M,K,n,m,fil,strides;
	Conv2D(int _N, int _M, int _K, int _n, int _m, int _fil, int _strides){
		N = _N; M = _M; K = _K; n = _n; m = _m; fil = _fil; strides = _strides;
		filter = new float[n*m*K*fil];
		d_filter = new float[n*m*K*fil]; 
		for(int i = 0; i < n*m*K*fil; i++){
			filter[i] = float(rand())/float(RAND_MAX)*0.1-0.05; 
		}
		for(int i = 0; i < MAX_BATCH_SIZE; i++){
			output[i] = new float[(N/strides)*(M/strides)*fil];
			d_input[i] = new float[N*M*K];
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
		
		for(int t = 0; t < batch_size; t++){
			for(int f = 0; f < fil; f++){

				for(int i = 0; i < N; i+=strides){
					for(int j = 0; j < M; j+=strides){
						
						
						//debug(temp);
						output[t][f*(N/strides)*(M/strides) + (i/strides)*(M/strides) + (j/strides)] = 0.0;
						for(int _i = (-n+1)/2; _i <= n/2; _i++){
							for(int _j = (-m+1)/2; _j <= m/2; _j++){
								for(int _k = 0; _k < K; _k++){
									if(i+_i >= 0 && i+_i < N && j+_j >= 0 && j+_j < M){
										output[t][f*(N/strides)*(M/strides) + (i/strides)*(M/strides) + (j/strides)]+=input[t][_k*N*M+(i+_i)*M+(j+_j)]*filter[f*K*n*m+_k*n*m+(_i-((-n+1)/2))*m+(_j-((-m+1)/2))];
									}
								}
							}
						}
						//debug(output[t][temp]);
					}
				}		
			}
		}	
	}

	void calculate_derivatives(int batch_size){
		// debug(1);
		// debug(K*N*M);
		for(int t = 0; t < batch_size; t++){
			for(int i = 0; i < K*N*M; i++){
				d_input[t][i] = 0.0;
			}
		}
		// debug(K*N*M);
		for(int i = 0; i < fil*K*n*m; i++){
			d_filter[i] = 0.0;
		}


		for(int t = 0; t < batch_size; t++){
			for(int f = 0; f < fil; f++){

				for(int i = 0; i < N; i+=strides){
					for(int j = 0; j < M; j+=strides){
						
						
						//debug(temp);
						//output[t][f*(N/strides)*(M/strides) + (i/strides)*(M/strides) + (j/strides)] = 0.0;
						for(int _i = (-n+1)/2; _i <= n/2; _i++){
							for(int _j = (-m+1)/2; _j <= m/2; _j++){
								for(int _k = 0; _k < K; _k++){
									if(i+_i >= 0 && i+_i < N && j+_j >= 0 && j+_j < M){
										d_input[t][_k*N*M+(i+_i)*M+(j+_j)]+=d_output[t][f*(N/strides)*(M/strides) + (i/strides)*(M/strides) + (j/strides)]*filter[f*K*n*m+_k*n*m+(_i-((-n+1)/2))*m+(_j-((-m+1)/2))];
										d_filter[f*K*n*m+_k*n*m+(_i-((-n+1)/2))*m+(_j-((-m+1)/2))]+=d_output[t][f*(N/strides)*(M/strides) + (i/strides)*(M/strides) + (j/strides)]*input[t][_k*N*M+(i+_i)*M+(j+_j)];
									}
								}
							}
						}
						//debug(output[t][temp]);
					}
				}		
			}
		}	
		// debug(2);
	}

	/*void update(int batch_size, float lambda){
		for(int i = 0; i < n*m*K*fil; i++){
			filter[i] -= lambda*d_filter[i];
		}
	}*/
	vector<ParameterContainer> get_params(){
		vector<ParameterContainer> params;
		ParameterContainer filter_container(filter,d_filter,n*m*K*fil);
		params.push_back(filter_container);
		return params;
	}
	void statistics(){
		float mint = filter[0];
		float maxt = filter[0];
		for(int i = 0; i < n*m*K*fil; i++){
			mint = min(mint, filter[i]);
			maxt = max(maxt, filter[i]);
		}
		cout<<"\n-----------------------\n";
		cout<<"Conv2D statistics\n";
		debug(N);
		debug(M);
		debug(K);
		debug(n);
		debug(m);
		debug(mint);
		debug(maxt);
		cout<<"\n-----------------------\n";
	}
};

#endif
