#ifndef jarvis_march_hpp
#define jarvis_march_hpp

#include <vector>
#include <string>
#include <utility>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"

namespace csce {
	template<typename T>
	class jarvis_march : public csce::convex_hull_base<T> {
	public:
		jarvis_march(int _nthreads) : convex_hull_base<T>(_nthreads){}
		
		
		std::string name() const {
			return "Jarvis' March";
		}
		
		
		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
			return this->performShortestPathCalculation(points);
		}
		
		
		~jarvis_march() {}
		
		
	private:
		std::vector<csce::point<T>> performShortestPathCalculation(const std::vector<csce::point<T>>& points) const {
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

#endif 
