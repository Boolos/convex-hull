#ifndef convex_hull_implementations_hpp
#define convex_hull_implementations_hpp

#include <vector>

#include "convex_hull_base.hpp"
#include "graham_scan.hpp"

namespace csce {
	namespace convex_hull_implementations {
		/**
		 * This defines the convex hull implementations to run. To add a new implementation,
		 * add an entry to the vector returned by this method.
		 */
		std::vector<csce::convex_hull_base*> list(int thread_count);
	}
}

#endif /* convex_hull_implementations_hpp */
