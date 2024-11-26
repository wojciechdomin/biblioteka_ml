#ifndef BatchNormalization_H
#define BatchNormalization_H
extern const int MAX_BATCH_SIZE;
#include "constants.hpp"
using namespace std;


struct BatchNormalization{
	float* input[MAX_BATCH_SIZE];
	float* d_input[MAX_BATCH_SIZE];
	float* output[MAX_BATCH_SIZE];
	float* d_output[MAX_BATCH_SIZE];
	float* chi[MAX_BATCH_SIZE];
	float* d_chi[MAX_BATCH_SIZE];
	float* gamma;
	float* d_gamma;
	float* beta;
	float* d_beta;
	float* variance;
	float* d_variance;
	float* avg_variance;
	float* mean;
	float* d_mean;
	float* avg_mean;
	bool training;
	int channels, N, T;
	const float epsilon = 0.00001; 
	const float alpha = 0.9;

	BatchNormalization(int _channels, int _N, int _T){
		training = true;
		channels = _channels; N = _N; T = _T;
		gamma = new float[channels];
		d_gamma = new float[channels];
		beta = new float[channels];
		d_beta = new float[channels];
		variance = new float[channels];
		d_variance = new float[channels];
		avg_variance = new float[channels];
		mean = new float[channels];
		d_mean = new float[channels];
		avg_mean = new float[channels];
		for(int i = 0; i < channels; i++){
			gamma[i] = float(rand())/float(RAND_MAX)-0.5; 
			beta[i] = float(rand())/float(RAND_MAX)-0.5; 
			avg_mean[i] = 0.0;
			avg_variance[i] = 0.0;
		}
		for(int i = 0; i < MAX_BATCH_SIZE; i++){
			output[i] = new float[channels*N];
			chi[i] = new float[channels*N];
			d_chi[i] = new float[channels*N];
			d_input[i] = new float[channels*N];
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
		if(training){
			for(int i = 0; i < channels; i++){
				mean[i] = 0.0;
				variance[i] = 0.0;
				for(int t = 0; t < T; t++){
					for(int j = 0; j < N; j++){
						mean[i]+=input[t][i*N+j]/(N*T);
					}
				}

				for(int t = 0; t < T; t++){
					for(int j = 0; j < N; j++){
						variance[i]+=pow(input[t][i*N+j]-mean[i], 2.0)/(N*T);
					}
				}
				for(int t = 0; t < T; t++){
					for(int j = 0; j < N; j++){
						chi[t][i*N+j] = (input[t][i*N+j] - mean[i])/sqrt(variance[i]+epsilon);
						output[t][i*N+j] = gamma[i]*chi[t][i*N+j]+beta[i];
					}
				}
				avg_mean[i] = alpha*avg_mean[i]+(1.0-alpha)*mean[i];
				avg_variance[i] = alpha*avg_variance[i]+(1.0-alpha)*variance[i];
			}



		}else{
			for(int i = 0; i < channels; i++){
				for(int t = 0; t < batch_size; t++){
					for(int j = 0; j < N; j++){
						chi[t][i*N+j] = (input[t][i*N+j] - avg_mean[i])/sqrt(avg_variance[i]+epsilon);
						output[t][i*N+j] = gamma[i]*chi[t][i*N+j]+beta[i];
					}
				}
			}
		}
		
	}

	void calculate_derivatives(int batch_size){
		for(int i = 0; i < channels; i++){
			d_beta[i] = 0.0;
			for(int t = 0; t < T; t++){
				for(int j = 0; j < N; j++){
					d_beta[i]+=d_output[t][i*N+j];
				}
			}
			d_gamma[i] = 0.0;
			for(int t = 0; t < T; t++){
				for(int j = 0; j < N; j++){
					d_gamma[i]+=d_output[t][i*N+j]*chi[t][i*N+j];
					d_chi[t][i*N+j]=d_output[t][i*N+j]*gamma[i];
				}
			}
			d_variance[i] = 0.0;
			for(int t = 0; t < T; t++){
				for(int j = 0; j < N; j++){
					d_variance[i]+=d_chi[t][i*N+j]*(-0.5)*pow(variance[i]+epsilon,-1.5)*(input[t][i*N+j]-mean[i]);
				}
			}
			d_mean[i] = 0.0;
			for(int t = 0; t < T; t++){
				for(int j = 0; j < N; j++){
					d_mean[i]+=(-1.0)*pow(variance[i]+epsilon,-0.5)*d_chi[t][i*N+j];
				}
			}
			for(int t = 0; t < T; t++){
				for(int j = 0; j < N; j++){
					d_input[t][i*N+j] = (d_mean[i]+d_variance[i]*2.0*(input[t][i*N+j]-mean[i]))/(N*T)+pow(variance[i]+epsilon,-0.5)*d_chi[t][i*N+j];
				}
			}
		}
	}

/*	void update(int batch_size, float lambda){
		for(int i = 0; i < channels; i++){
			gamma[i] -= lambda*d_gamma[i];
			beta[i] -= lambda*d_beta[i];
		}
	}*/
	vector<ParameterContainer> get_params(){
		vector<ParameterContainer> params;
		ParameterContainer beta_container(beta,d_beta,channels);
		ParameterContainer gamma_container(gamma,d_gamma,channels);
		params.push_back(beta_container);
		params.push_back(gamma_container);
		return params;
	}


};
#endif