#ifndef test_hpp
#define test_hpp

#include <iostream>
#include <vector>

#include "line.hpp"
#include "vector2d.hpp"
#include "point.hpp"
#include "utility.hpp"

namespace csce {
	class test {
	public:
		test(bool _debug = false) : debug(_debug) {}
		bool run() const;
		
	private:
		bool debug = false;
		bool test_line_intersect() const;
		bool test_convex_polygon() const;
		bool test_contains_all_points() const;
		bool test_validate_convex_hull() const;
	};
}

#endif /* test_hpp */
