#ifndef jarvis_march_parallel_hpp
#define jarvis_march_parallel_hpp

#include <vector>
#include <string>
#include <utility>
#include <omp.h>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"

namespace csce {
	template<typename T>
	class jarvis_march_parallel : public csce::convex_hull_base<T> {
	public:
		jarvis_march_parallel(int _nthreads) : convex_hull_base<T>(_nthreads){}
		
		
		std::string name() const {
			return "Jarvis' March Parallel";
		}
		
		
		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
			return this->performShortestPathCalculation(points);
		}
		
		
		~jarvis_march_parallel() {}
		
		
	private:
		std::vector<csce::point<T>> performShortestPathCalculation(const std::vector<csce::point<T>>& points) const {
			std::vector<csce::point<T>> resultsOfShortestPath;
			int topMostPoint = 0;
			int leftMostPoint = 0;
			int rightMostPoint = 0;
			int bottomMostPoint = 0;
			int topLeft = 0;
			int topRight = 0;
			int bottomLeft = 0;
			int bottomRight = 0;
			#pragma omp parallel for num_threads(this->nthreads) 
			for(int i = 0; i < points.size(); i++){
				if(points[i].y > points[topMostPoint].y){
					topMostPoint = i;
				}
				if(points[i].y < points[bottomMostPoint].y) {
					bottomMostPoint = i;
				}
				if(points[i].x > points[rightMostPoint].x) { 
					rightMostPoint = i;
				}
				if(points[i].x < points[leftMostPoint].x) {
					leftMostPoint = i;
				}
			}
			#pragma omp parallel for num_threads(this->nthreads)
			for(int i = 0; i < points.size(); i++) {
				if(points[i] == points[topMostPoint] + points[leftMostPoint]){
					topLeft = i;
				}
				if(points[i] == points[topMostPoint] + points[rightMostPoint]) {
					topRight = i;
				}
				if(points[i] == points[bottomMostPoint] + points[leftMostPoint]) {
					bottomLeft = i;
				}
				if(points[i] == points[bottomMostPoint] + points[rightMostPoint]) {
					bottomRight = i;
				} 
			}
			int tempPoint1 = topMostPoint;
			int tempPoint2 = 0;
			int orientationValue = 0;
			do{
				tempPoint2 = (tempPoint1 + 1) % points.size();
				#pragma omp parallel for num_threads(this->nthreads)
				for(int i = 0; i < points.size(); i++){
					orientationValue = operation(points[tempPoint1],points[i],points[tempPoint2]);
					if(orientationValue == 2){
						tempPoint2 = i;
					}
				}
				
				resultsOfShortestPath.push_back(points[tempPoint2]);
				tempPoint1 = tempPoint2;
			}while(tempPoint1 != topMostPoint);

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

#endif 
