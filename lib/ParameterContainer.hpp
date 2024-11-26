#ifndef ParameterContainer_H
#define ParameterContainer_H
#include <bits/stdc++.h>
using namespace std;

struct ParameterContainer{
	float* params;
	float* d_params;
	int N;
	ParameterContainer(float* _params, float* _d_params, int _N){
		params = _params;
		d_params = _d_params;
		N = _N;
	}
};
#endif
