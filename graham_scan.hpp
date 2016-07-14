#ifndef graham_scan_hpp
#define graham_scan_hpp

#include <vector>
#include <string>

#include "convex_hull_base.hpp"
#include "point.hpp"

namespace csce {
	template<typename T>
	class graham_scan : public csce::convex_hull_base<T> {
	public:
		graham_scan(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}
		
		
		std::string name() const {
			return "Graham's Scan";
		}
		
		
		std::vector<csce::point<T>> compute_hull(const std::vector<csce::point<T>>& points) const {
			return points;
		}
		
		
		~graham_scan() {}
	};
}

#endif /* graham_scan_hpp */
