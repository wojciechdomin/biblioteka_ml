#ifndef SGDOptimizer_H
#define SGDOptimizer_H
#include "constants.hpp"

using namespace std;

struct SGDOptimizer{
	vector<ParameterContainer> vec;
	float learning_rate = 0.001;

	SGDOptimizer(float _learning_rate){
		learning_rate = _learning_rate;
	}

	void add(vector<ParameterContainer> _vec){
		for(auto x : _vec){
			vec.push_back(x);
		}
	}
	void set_learning_rate(float _learning_rate){
		learning_rate = _learning_rate;
	}	
	void update(){
		for(auto x : vec){
			for(int i = 0; i < x.N; i++){
				x.params[i] -= x.d_params[i] * learning_rate; 
			}
		}
	}
};
#endif
