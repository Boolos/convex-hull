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
		template<typename T>
		std::vector<csce::convex_hull_base<T>*> list(int thread_count) {
			std::vector<csce::convex_hull_base<T>*> algorithms;
			algorithms.push_back(new csce::graham_scan<T>(thread_count));
			
			return algorithms;
		}
	}
}

#endif /* convex_hull_implementations_hpp */
