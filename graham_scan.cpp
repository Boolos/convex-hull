#include "graham_scan.hpp"

csce::graham_scan::graham_scan(int _nthreads) : convex_hull_base(_nthreads) {}
csce::graham_scan::~graham_scan() {}

std::string csce::graham_scan::name() const {
	return "Graham's Scan";
}

std::vector<csce::point> csce::graham_scan::compute_hull(const std::vector<csce::point>& points) const {
	return points;
}
