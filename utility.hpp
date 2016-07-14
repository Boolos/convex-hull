#ifndef utility_hpp
#define utility_hpp

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <cmath>

#include "point.hpp"

namespace csce {
	namespace utility {
		const long double EPS = 1E-9;
		
		template<typename f, typename... args>
		long long int duration_function(f func, args&&... arg){
			std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
			func(std::forward<args>(arg)...);
			std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		}
		
		
		std::string duration_string(long long int duration) {
			std::stringstream out;
			out << (duration / 1E9) << " seconds";
			return out.str();
		}
		
		
		bool equals(long double a, long double b){
			return std::abs(a - b) <= csce::utility::EPS;
		}
		
		
		template<typename T>
		std::vector<csce::point<T>> random_points(int n) {
			int min = std::numeric_limits<int>::min();
			int max = std::numeric_limits<int>::max();
			return csce::utility::random_points<T>(n, min, max);
		}
		
		
		template<typename T>
		std::vector<csce::point<T>> random_points(int n, int min, int max) {
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<T> dist(min, max);
			
			std::vector<csce::point<T>> points;
			points.reserve(n);
			for(int x=0; x<n; x++){
				points.push_back(csce::point<T>(dist(mt), dist(mt)));
			}
			
			return points;
		}
		
		
		template<typename T>
		std::vector<csce::point<T>> points_from_file(int n, const std::string& file_path) {
			std::vector<csce::point<T>> points;
			points.reserve(n);
			
			std::ifstream file(file_path, std::ios::in);
			if(file.is_open()){
				int x=0, y=0;
				while(file >> x >> y){
					points.push_back(csce::point<T>(x, y));
					if(points.size() >= n){
						break;
					}
				}
				file.close();
			}
			
			return points;
		}
		
		
		template<typename T>
		void write_to_file(const std::vector<csce::point<T>>& points, const std::string& file_path) {
			std::ofstream file(file_path, std::ios_base::out | std::ios_base::trunc);
			if(file.is_open()){
				for(auto& point : points){
					file << point.x << " " << point.y << std::endl;
				}
				file.close();
			}
		}
		
		
		template<typename T>
		int validate(const std::vector<csce::point<T>>& points, const std::vector<csce::point<T>>& convex_hull, std::vector<std::string>& output_errors) {
			int error_count = 0;
			
			//TODO: validate that all points in the convex hull are counter clock wise (or maybe all clockwise?)
			
			//TODO: then for all remaining points, validate that the remaining points are not outside the convex hull
			
			std::unordered_set<csce::point<T>> point_set;
			for(auto& point : points){
				point_set.insert(point);
			}
			
			
			
			//TODO: generate the convex hull points
			//TODO: validate that the convex_hull parameter and the generated convex hull contain the same points.
			
			/*for(int x=1; x<n; x++){
			 if(array[x-1] > array[x]){
			 if(error_count < 10){
			 std::stringstream error;
			 error << "validate: array[" << (x-1) << "] = " << array[x-1] << " is not less than or equal to array[" << x << "] = " << array[x];
			 output_errors.push_back(error.str());
			 }
			 error_count++;
			 }
			 }*/
			
			return error_count;
		}
	}
}

#endif /* utility_hpp */
