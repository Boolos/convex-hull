#ifndef chan_algo_hpp
#define chan_algo_hpp

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"
#include "graham_scan.hpp"
#include "jarvis_march.hpp"

namespace csce {
	template<typename T>
	class chan_algo : public csce::convex_hull_base<T> {
	public:
		chan_algo(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}


		std::string name() const {
			return "Chan's Algorithm Sequential";
		}

		virtual std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
			
			std::size_t m = points.size() / this->nthreads;
			
					std::vector<std::vector<csce::point<T>>> hulls;
			
					for(std::size_t i=0; i<points.size(); i=i+m){
					std::vector<csce::point<T>> chunk;
				
					if(points.begin()+i+m <= points.end())
					chunk.assign(points.begin()+i,points.begin()+i+m);
				
					else
					chunk.assign(points.begin()+i,points.end());
					
					hulls.push_back(this->graham_scan(chunk));
					
				}	
			
			std::vector<csce::point<T>> output;
				
					for(std::size_t i=0; i<this->nthreads; i++){
					    
						output.insert(output.end(), hulls[i].begin(), hulls[i].end());
						output = (this->jarvis_march(output));
					}
					
					return output;
			
		}
		
		~chan_algo() {}
	
	private:
	
		std::vector<csce::point<T>> graham_scan(std::vector<csce::point<T>>& points) const {
			if(points.size() <= 3){
				return points;
			}

			std::size_t index = this->pivot_index(points);
			if(index != 0){
				std::swap(points[0], points[index]);
			}
			
			//sort the points by angle around the pivot point (points[0])
			std::sort(points.begin() + 1, points.end(), [&points](csce::point<T>& a, csce::point<T>& b) {
				csce::vector2d<T> pa(points[0], a);
				csce::vector2d<T> pb(points[0], b);
				return pa.ccw(pb);
			});
			
			std::vector<csce::point<T>> s;
			s.reserve(points.size() + 1);
			
			s.push_back(points[points.size() - 1]);
			s.push_back(points[0]);
			s.push_back(points[1]);
			
			int x=2;
			while(x < points.size()){
				std::size_t y = s.size() - 1;
				csce::vector2d<T> a(s[y-1], s[y]);
				csce::vector2d<T> b(s[y-1], points[x]);
				if(a.ccw(b)){
					s.push_back(points[x++]);
				} else {
					s.pop_back();
				}
			}
			
			if(s.front() == s.back()){
				s.pop_back(); //the polygon is closed, so remove the last element to remove the duplicate point
			}

			return s;
		}
		
		std::size_t pivot_index(std::vector<csce::point<T>>& points) const {
			std::size_t index = 0;
			for(std::size_t x=0; x<points.size(); x++){
				csce::point<T>& p0 = points[index];
				csce::point<T>& cur = points[x];
				if(csce::math_utility::less_than(cur.y, p0.y)){
					index = x;
				} else if(csce::math_utility::equals(cur.y, p0.y)){
					if(csce::math_utility::less_than(cur.x, p0.x)){
						index = x;
					}
				}
			}
			return index;
		}
		
		
		
		std::vector<csce::point<T>> jarvis_march (const std::vector<csce::point<T>>& points) {
			std::vector<csce::point<T>> resultsOfShortestPath;
			int topMostPoint = 0;
			for(int i = 0; i < points.size(); i++){
				if(points[i].y > points[topMostPoint].y){
					topMostPoint = i;
				}
			}
			
			int tempPoint1 = topMostPoint;
			int tempPoint2 = 0;
			int orientationValue = 0;
			do{
				tempPoint2 = (tempPoint1 + 1) % points.size();
				for(int i = 0; i < points.size(); i++){
					orientationValue = operation(points[tempPoint1],points[i],points[tempPoint2]);
					if(orientationValue == 2){
						tempPoint2 = i;
					}
				}
				resultsOfShortestPath.push_back(points[tempPoint2]);
				tempPoint1 = tempPoint2;
			}
			while(tempPoint1 != topMostPoint);
			
			return resultsOfShortestPath;
		}
		
		int operation(csce::point<T> d, csce::point<T> e, csce::point<T> f) const {
			csce::vector2d<T> a(e,d);
			csce::vector2d<T> b(e,f);
			bool is_counterclockwise = a.ccw(b);
			if(is_counterclockwise == true){
				return 2;
			}
			else {
				return 0;
			}
		}

	};
}


#endif /* chan_algo_hpp */
