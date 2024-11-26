#ifndef AdamOptimizer_H
#define AdamOptimizer_H
#include "constants.hpp"

using namespace std;

struct AdamOptimizer{
	vector<ParameterContainer> vec;
	vector<float*> first_moment;
	vector<float*> second_moment;
	float learning_rate = 0.001;
	float beta1 = 0.9;
	float beta2 = 0.999;
	float epsilon = 1e-8;
	int t = 0;

	AdamOptimizer(){
		t = 0;
	}

	AdamOptimizer(float _learning_rate){
		learning_rate = _learning_rate;
		t = 0;
	}

	AdamOptimizer(float _learning_rate, float _beta1, float _beta2){
		learning_rate = _learning_rate;
		beta1 = _beta1;
		beta2 = _beta2;
		t = 0;
	}	

	void add(vector<ParameterContainer> _vec){
		for(auto x : _vec){
			vec.push_back(x);
			float* pnt = new float[x.N];
			for(int i = 0; i < x.N; i++) pnt[i] = 0.0;
			first_moment.push_back(pnt);
			pnt = new float[x.N];
			for(int i = 0; i < x.N; i++) pnt[i] = 0.0;
			second_moment.push_back(pnt);
		}
	}

	void set_learning_rate(float _learning_rate){
		learning_rate = _learning_rate;
	}	

	void update(){
		t++;
		for(int i = 0; i < vec.size(); i++){
			for(int j = 0; j < vec[i].N; j++){
				first_moment[i][j] = beta1*first_moment[i][j]+(1.0-beta1)*vec[i].d_params[j];
				second_moment[i][j] = beta2*second_moment[i][j]+(1.0-beta2)*pow(vec[i].d_params[j],2.0);
			}
		}
		for(int i = 0; i < vec.size(); i++){
			for(int j = 0; j < vec[i].N; j++){
				//float first_moment_corrected = first_moment[i][j]/(1.0-pow(beta1,t*1.0));
				//float second_moment_corrected = second_moment[i][j]/(1.0-pow(beta2,t*1.0));
				vec[i].params[j] -= first_moment[i][j]/(1.0-pow(beta1,t*1.0))*learning_rate/(sqrt(second_moment[i][j]/(1.0-pow(beta2,t*1.0)))+epsilon);	
			}
		}
	}

	void save_weights(string filename){
		fstream f(filename,fstream::out);
		f<<fixed<<setprecision(10);
		for(int i = 0; i < vec.size(); i++){
			for(int j = 0; j < vec[i].N; j++){
				f<<vec[i].params[j]<<" ";
			}
		}
		f.flush();
		f.close();
	}

	void load_weights(string filename){
		fstream f(filename,fstream::in);
		for(int i = 0; i < vec.size(); i++){
			for(int j = 0; j < vec[i].N; j++){
				f>>vec[i].params[j];
			}
			if(i+1 < vec.size()) assert(f.good());
		}
		
		f.flush();
		f.close();
	}



};
#endif
