#ifndef convex_hull_implementations_hpp
#define convex_hull_implementations_hpp

#include <vector>

#include "convex_hull_base.hpp"
#include "graham_scan.hpp"
#include "jarvis_march.hpp"
#include "graham_scan_parallel.hpp"
#include "jarvis_march_parallel.hpp"
#include "chan_algo.hpp"
#include "chan_algo_parallel.hpp"
#include "composable_hulls.hpp"
#include "quick_hull.hpp"
#include "quick_hull_parallel.hpp"

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
			algorithms.push_back(new csce::graham_scan_parallel<T>(thread_count));
			algorithms.push_back(new csce::jarvis_march<T>(thread_count));
			algorithms.push_back(new csce::jarvis_march_parallel<T>(thread_count));
			algorithms.push_back(new csce::chan_algo_parallel<T>(thread_count));
			algorithms.push_back(new csce::chan_algo<T>(thread_count));
			algorithms.push_back(new csce::quick_hull<T>(thread_count));
			algorithms.push_back(new csce::quick_hull_parallel<T>(thread_count));
			//algorithms.push_back(new csce::composable_hulls<T, csce::graham_scan_parallel<T>, csce::graham_scan_parallel<T>>(thread_count));
			//algorithms.push_back(new csce::composable_hulls<T, csce::graham_scan_parallel<T>, csce::jarvis_march_parallel<T>>(thread_count));
			//algorithms.push_back(new csce::composable_hulls<T, csce::jarvis_march_parallel<T>, csce::graham_scan_parallel<T>>(thread_count));
			//algorithms.push_back(new csce::composable_hulls<T, csce::jarvis_march_parallel<T>, csce::jarvis_march_parallel<T>>(thread_count));
			
			return algorithms;
		}
	}
}

#endif /* convex_hull_implementations_hpp */
