#ifndef graham_scan_hpp
#define graham_scan_hpp

#include <vector>
#include <string>
#include <utility>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"

namespace csce {
	template<typename T>
	class graham_scan : public csce::convex_hull_base<T> {
	public:
		graham_scan(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}
		
		
		std::string name() const {
			return "Graham's Scan";
		}
		
		
		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
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
			
			s.shrink_to_fit();
			return s;
		}
		
		
		~graham_scan() {}
		
		
	private:
		
		/**
		 * Finds the index of the point with minimum y coordinate.
		 * If there is a tie for minimum y coordinate, the index of the point with
		 * the minimum y coordinate and minimum x coordinate will be returned.
		 */
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
	};
}

#endif /* graham_scan_hpp */
