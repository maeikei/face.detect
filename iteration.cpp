#include <iostream>
#include <cmath>
using namespace std;


#define DUMP(x) \
cout << #x << ":=<" << x << ">" << endl;



int main()
{
	double base_lr = 0.01;
	double gamma = 0.8;
	int stepsize = 100;
	for(int iter_ = 1 ; iter_ < 1000*10 ;iter_++ )
	{
		int current_step_ = iter_ /stepsize;
		auto rate = base_lr * pow(gamma, current_step_);
		if(0 == iter_%100)
		{
			DUMP(iter_);
			DUMP(rate);
		}
	}
	return 0;
}
