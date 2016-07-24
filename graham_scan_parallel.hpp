#ifndef graham_scan_parallel_hpp
#define graham_scan_parallel_hpp

#include <vector>
#include <string>
#include <utility>
#include <thread>
#include <mutex>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"
#include "sort.hpp"

namespace csce {
	template<typename T>
	class graham_scan_parallel : public csce::convex_hull_base<T> {
	public:
		graham_scan_parallel(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}
		
		
		std::string name() const {
			return "Graham's Scan Parallel";
		}
		
		
		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
			if(points.size() <= 3){
				return points;
			}
			
			std::size_t index = this->pivot_index(points);
			if(index != 0){
				std::swap(points[0], points[index]);
			}
			
			csce::sort<T> sorter(points[0], this->nthreads);
			sorter.sort_array(points.begin() + 1, points.end());
			
			std::vector<csce::point<T>> s;
			s.reserve(points.size() + 1);
			
			s.push_back(points[points.size() - 1]);
			s.push_back(points[0]);
			s.push_back(points[1]);
			
			int x=2;
			while(x < points.size()){
				std::size_t y = s.size() - 1;
				csce::vector2d<T> a(s[y-1], s[y]);
				csce::vector2d<T> b(s[y-1], points[x]);
				if(a.ccw(b)){
					s.push_back(points[x++]);
				} else {
					s.pop_back();
				}
			}
			
			s.shrink_to_fit();
			return s;
		}
		
		
		~graham_scan_parallel() {}
		
		
	private:
		
		std::size_t pindex = 0;
		std::mutex pivot_mutex;
		
		/**
		 * Finds the index of the point with minimum y coordinate.
		 * If there is a tie for minimum y coordinate, the index of the point with
		 * the minimum y coordinate and minimum x coordinate will be returned.
		 */
		std::size_t pivot_index(std::vector<csce::point<T>>& points) {
			std::thread* threads = new std::thread[this->nthreads];
			int nelements = static_cast<int>(std::ceil(static_cast<double>(points.size()) / static_cast<double>(this->nthreads)));
			int start = 0, stop = nelements;
			int segment_count = 0;
			
			for(int x=0; x<this->nthreads && start < points.size(); x++, start += nelements, stop += nelements){
				threads[x] = std::thread(&csce::graham_scan_parallel<T>::pivot_index_thread, this, points, start, stop);
				segment_count++;
			}
			
			for(int x=0; x<segment_count; x++){
				threads[x].join();
			}
			
			delete[] threads;
			return this->pindex;
		}
		
		void pivot_index_thread(const std::vector<csce::point<T>>& points, int start, int stop) {
			std::size_t index = start;
			for(std::size_t x=start; x<points.size() && x <= stop; x++){
				const csce::point<T>& p0 = points[index];
				const csce::point<T>& cur = points[x];
				if(csce::math_utility::less_than(cur.y, p0.y)){
					index = x;
				} else if(csce::math_utility::equals(cur.y, p0.y)){
					if(csce::math_utility::less_than(cur.x, p0.x)){
						index = x;
					}
				}
			}
			this->update_pivot_index(points, index);
		}
		
		void update_pivot_index(const std::vector<csce::point<T>>& points, const std::size_t& index) {
			this->pivot_mutex.lock();
			
			const csce::point<T>& p0 = points[this->pindex];
			const csce::point<T>& cur = points[index];
			if(csce::math_utility::less_than(cur.y, p0.y)){
				this->pindex = index;
			} else if(csce::math_utility::equals(cur.y, p0.y)){
				if(csce::math_utility::less_than(cur.x, p0.x)){
					this->pindex = index;
				}
			}
			
			this->pivot_mutex.unlock();
		}
	};
}

#endif /* graham_scan_parallel_hpp */
