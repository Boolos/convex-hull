#ifndef utility_hpp
#define utility_hpp

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <random>

#include "point.hpp"

namespace csce {
	namespace utility {
		template<typename f, typename... args>
		long long int duration_function(f func, args&&... arg){
			std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
			func(std::forward<args>(arg)...);
			std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		}
		
		std::string duration_string(long long int duration);
		
		std::vector<csce::point> random_points(int n);
		std::vector<csce::point> random_points(int n, int min, int max);
		
		std::vector<csce::point> points_from_file(int n, const std::string& file_path);
		
		void write_to_file(const std::vector<csce::point>& points, const std::string& file_path);
		
		int validate(const std::vector<csce::point>& points, const std::vector<csce::point>& convex_hull, std::vector<std::string>& output_errors);
	}
}

#endif /* utility_hpp */
