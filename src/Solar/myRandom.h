#ifndef __MY_RANDOM_H__
#define __MY_RANDOM_H__

#include <random>

std::default_random_engine e;

inline double randomFloat(double a, double b)
{
	std::uniform_real_distribution<double> u(a, b);
	return u(e);
}


#endif // !__MY_RANDOM_H__

