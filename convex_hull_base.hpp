#ifndef convex_hull_base_hpp
#define convex_hull_base_hpp

#include <vector>
#include <string>

#include "point.hpp"

namespace csce {
	template<typename T>
	class convex_hull_base {
	public:
		virtual std::string name() const = 0;
		virtual std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) const = 0;
		
		virtual ~convex_hull_base() {}
		convex_hull_base(int _nthreads) : nthreads(_nthreads) {}
		
	protected:
		int nthreads = 0;
	};
}

#endif /* convex_hull_base_hpp */
