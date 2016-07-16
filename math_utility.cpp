#include "math_utility.hpp"

bool csce::math_utility::equals(long double a, long double b){
	return std::abs(a - b) <= csce::math_utility::EPS;
}

bool csce::math_utility::equals(double a, double b){
	return std::abs(a - b) <= csce::math_utility::EPS;
}

bool csce::math_utility::equals(float a, float b){
	return std::abs(a - b) <= csce::math_utility::EPS;
}

bool csce::math_utility::equals(int a, int b){
	return a == b;
}

bool csce::math_utility::equals(long long int a, long long int b){
	return a == b;
}

bool csce::math_utility::equals(short a, short b) {
	return a == b;
}

bool csce::math_utility::equals_zero(long double a){
	return std::abs(a) <= csce::math_utility::EPS;
}

bool csce::math_utility::equals_zero(double a){
	return std::abs(a) <= csce::math_utility::EPS;
}

bool csce::math_utility::equals_zero(float a){
	return std::abs(a) <= csce::math_utility::EPS;
}

bool csce::math_utility::equals_zero(int a){
	return a == 0;
}

bool csce::math_utility::equals_zero(long long int a){
	return a == 0L;
}

bool csce::math_utility::equals_zero(short a) {
	return a == 0;
}

bool csce::math_utility::less_than(long double a, long double b) {
	return std::abs(a - b) > EPS && a < b;
}

bool csce::math_utility::less_than(double a, double b) {
	return std::abs(a - b) > EPS && a < b;
}

bool csce::math_utility::less_than(float a, float b) {
	return std::abs(a - b) > EPS && a < b;
}

bool csce::math_utility::less_than(int a, int b) {
	return a < b;
}

bool csce::math_utility::less_than(long long int a, long long int b) {
	return a < b;
}

bool csce::math_utility::less_than(short a, short b) {
	return a < b;
}