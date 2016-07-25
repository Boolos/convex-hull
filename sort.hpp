#ifndef sort_hpp
#define sort_hpp

#include <cmath>
#include <utility>
#include <algorithm>
#include <iterator>

#include "point.hpp"

namespace csce {
	template<typename T>
	class sort {
	public:
		sort(const csce::point<T>& pivot_point) : p(pivot_point) {}
		
		void sort_array(typename std::vector<csce::point<T>>::iterator begin, typename std::vector<csce::point<T>>::iterator end) {
			std::sort(begin, end, [&](csce::point<T>& a, csce::point<T>& b) {
                                csce::vector2d<T> pa(this->p, a);
                                csce::vector2d<T> pb(this->p, b);
                                return pa.ccw(pb);
                        });
		}
		
	private:
		csce::point<T> p;
		
	};
}

#endif /* sort_hpp */
