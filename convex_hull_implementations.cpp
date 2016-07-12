#include "convex_hull_implementations.hpp"

std::vector<csce::convex_hull_base*> csce::convex_hull_implementations::list(int thread_count){
	std::vector<csce::convex_hull_base*> algorithms;
	algorithms.push_back(new csce::graham_scan(thread_count));
	
	return algorithms;
}