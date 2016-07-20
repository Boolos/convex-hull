#ifndef utility_hpp
#define utility_hpp

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <cmath>
#include <unordered_set>

#include "point.hpp"
#include "vector2d.hpp"
#include "line.hpp"

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
		
		
		template<typename T>
		std::vector<csce::point<T>> random_points(int n) {
			int min = std::numeric_limits<int>::min();
			int max = std::numeric_limits<int>::max();
			return csce::utility::random_points<T>(n, min, max);
		}
		
		
		template<typename T>
		using uniform_distribution = typename std::conditional<
			std::is_integral<T>::value, std::uniform_int_distribution<T>,
			typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>::type
		>::type;
		
		
		template<typename T>
		std::vector<csce::point<T>> random_points(int n, T min, T max) {
			std::random_device rd;
			std::mt19937 mt(rd());
			csce::utility::uniform_distribution<T> dist(min, max);
			
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
				T x=0, y=0;
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
		bool is_convex(const std::vector<csce::point<T>>& polygon, std::vector<std::string>& output_errors) {
			if(polygon.size() <= 3){
				return true; //triangles, line segments, and points are considered convex
			}

			//validate all points in the convex_hull input are either all clockwise or counterclockwise, but not both.
			bool ccw = true;
			for(std::size_t x=0; x<polygon.size()-2; x++){
				bool tmp_ccw = csce::vector2d<T>(polygon[x], polygon[x+1]).ccw(csce::vector2d<T>(polygon[x], polygon[x+2]));
				if(x == 0){
					ccw = tmp_ccw;
				} else {
					if(ccw != tmp_ccw){
						std::stringstream error;
						error << "The polygon has both clockwise and counter-clockwise angles starting at index " << x << ". {";
						error << polygon[x-1].str() << ", ";
						error << polygon[x].str() << ", ";
						error << polygon[x+1].str() << ", ";
						error << polygon[x+2].str() << "}";
						output_errors.push_back(error.str());
						return false;
					}
				}
			}
			
			return true;
		}
		
		
		template<typename T>
		bool contains_all_points(const std::vector<csce::point<T>>& convex_hull, const std::vector<csce::point<T>>& points, T max, std::vector<std::string>& output_errors) {
			std::unordered_set<csce::point<T>> point_set;
			for(auto& point : points){
				point_set.insert(point);
			}
			
			for(auto& point : convex_hull){
				point_set.erase(point);
			}
			
			for(auto& point : point_set){
				int point_intersections = 0;
				int intersections = 0;
				csce::line<T> ray(point, csce::point<T>(max + 1, point.y));
				for(std::size_t x=0; x<convex_hull.size() - 1; x++){
					csce::line<T> segment(convex_hull[x], convex_hull[x+1]);
					if(segment.contains_point(point)){
						//if the point is on the border of the polygon, then
						//the polygon "contains" the point.
						return true;
					}
					if(ray.contains_point(segment.a) || ray.contains_point(segment.b)){
						//if either segment end-point is on the ray, then two segments
						//will intersect with the ray (this segment and the segment sharing
						//this end-point)
						point_intersections++;
					} else if(ray.intersects(segment)){
						intersections++;
					}
				}
				intersections += (point_intersections >> 1);
				if(intersections % 2 == 0){
					std::stringstream error;
					error << "Point " << point.str() << " is not inside the convex hull.";
					output_errors.push_back(error.str());
					return false;
				}
			}
			
			return true;
		}
		
		
		template<typename T>
		bool validate(const std::vector<csce::point<T>>& convex_hull, const std::vector<csce::point<T>>& points, T max, std::vector<std::string>& output_errors) {
			if(convex_hull.empty()){
				return points.empty();
			}
			
			if(points.empty()){
				return false;
			}
			
			std::vector<csce::point<T>> polygon = convex_hull;
			if(polygon[0] != polygon[polygon.size() - 1]){
				//if the polygon is not closed, close the polygon
				polygon.push_back(polygon[0]);
			}
			
			if(!csce::utility::is_convex(polygon, output_errors)){
				return false;
			}
			
			if(!csce::utility::contains_all_points(polygon, points, max, output_errors)){
				return false;
			}
			
			return true;
		}
	}
}

#endif /* utility_hpp */
