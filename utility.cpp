#include "utility.hpp"

std::string csce::utility::duration_string(long long int duration) {
	std::stringstream out;
	out << (duration / 1E9) << " seconds";
	return out.str();
}