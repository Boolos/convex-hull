#ifndef graham_scan_hpp
#define graham_scan_hpp

#include <vector>
#include <string>

#include "convex_hull_base.hpp"
#include "point.hpp"

namespace csce {
	class graham_scan : public convex_hull_base {
	public:
		graham_scan(int _nthreads);
		std::string name() const;
		std::vector<csce::point<long double>> compute_hull(const std::vector<csce::point<long double>>& points) const;
		~graham_scan();
	};
}

#endif /* graham_scan_hpp */
