#ifndef constants_H
#define constants_H
#define float double
#include <bits/stdc++.h>
#include "ParameterContainer.hpp"
#define debug(x) cout<<#x<<" = "<<x<<"\n"
using namespace std;
const int MAX_BATCH_SIZE = 128;

float safe_exp(float x){
	if(x > 250.0){
		x = 250.0;
	}
	if(x < -250.0){
		x = -250.0;
	}
	return exp(x);
}

#endif
