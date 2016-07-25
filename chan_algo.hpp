#ifndef chan_algo_hpp
#define chan_algo_hpp

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"
#include "graham_scan.hpp"
#include "jarvis_march.hpp"

namespace csce {
	template<typename T, typename U = graham_scan<T>, typename V = jarvis_march<T>>
	class chan_algo : public csce::convex_hull_base<T> {
	public:
		chan_algo(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}


		std::string name() const {
			return "Chan's Algorithm Sequential";
		}

		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) const {
			
			std::size_t m = points.size() / this->nthreads;
			
					std::vector<std::vector<csce::point<T>>> hulls;
			
					for(std::size_t i=0; i<points.size(); i=i+m){
					std::vector<csce::point<T>> chunk;
				
					if(points.begin()+i+m <= points.end())
					chunk.assign(points.begin()+i,points.begin()+i+m);
				
					else
					chunk.assign(points.begin()+i,points.end());
					
					U local_hulls(this->nthreads);
					
					hulls.push_back(local_hulls.compute_hull(chunk));
					
				}	
			
			std::vector<csce::point<T>> output;
			
				V global_hulls(this->nthreads);
				
					for(std::size_t i=0; i<this->nthreads; i++){
					    
						output.insert(output.end(), hulls[i].begin(), hulls[i].end());
						output = global_hulls.compute_hull(output);
					}
					
					return output;
			
		}
		
		~chan_algo() {}
	
	private:
	
		
		int operation(csce::point<T> d, csce::point<T> e, csce::point<T> f) const {
			csce::vector2d<T> a(e,d);
			csce::vector2d<T> b(e,f);
			bool is_counterclockwise = a.ccw(b);
			if(is_counterclockwise == true){
				return 2;
			}
			else {
				return 0;
			}
		}

	};
}


#endif /* chan_algo_hpp */
