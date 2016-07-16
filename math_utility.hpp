#ifndef math_utility_hpp
#define math_utility_hpp

#include <cmath>

namespace csce {
	namespace math_utility {
		const long double EPS = 1E-9;
		bool equals(long double a, long double b);
		bool equals(double a, double b);
		bool equals(float a, float b);
		bool equals(int a, int b);
		bool equals(long long int a, long long int b);
		bool equals(short a, short b);
		bool equals_zero(long double a);
		bool equals_zero(double a);
		bool equals_zero(float a);
		bool equals_zero(int a);
		bool equals_zero(long long int a);
		bool equals_zero(short a);
		
		bool less_than(long double a, long double b);
		bool less_than(double a, double b);
		bool less_than(float a, float b);
		bool less_than(int a, int b);
		bool less_than(long long int a, long long int b);
		bool less_than(short a, short b);
	}
}

#endif /* math_utility_hpp */
