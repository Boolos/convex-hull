#ifndef convex_hull_base_hpp
#define convex_hull_base_hpp

#include <vector>
#include <string>

#include "point.hpp"

namespace csce {
	class convex_hull_base {
	public:
		virtual std::string name() const = 0;
		virtual std::vector<csce::point<long double>> compute_hull(const std::vector<csce::point<long double>>& points) const = 0;
		virtual ~convex_hull_base();
		convex_hull_base(int _nthreads);
		
	private:
		int nthreads = 0;
	};
}

#endif /* convex_hull_base_hpp */
