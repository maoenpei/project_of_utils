
#include "Naran.h"

#include <iostream>
#include <math.h>

using namespace Naran;

int g_k = 0;

class A
{
};

class B
{
};

int main(int argc, char **argv)
{	
	//double m0 = 50;
	//double s = 0.0655/12;
	double m0 = 30;
	double s = 0.045/12;
	for (int y = 1; y<31; y++){
		double n = y * 12;
		double m1 = pow(1+s, n);
		double equal_prin = (m1 * s * n / (m1 - 1)) * m0;
		double equal_prai = (1 + (n + 1) * s / 2) * m0;
		double first_prin = equal_prin / n;
		double first_prai = (s + 1 / n) * m0;
		std::cout<<""<<y<<"年 ";
		std::cout<<"等额本息:"<<equal_prin<<":"<<first_prin<<" ";
		std::cout<<"等额本金:"<<equal_prai<<":"<<first_prai<<" ";
		std::cout<<std::endl;
	}
	return 0;
}


