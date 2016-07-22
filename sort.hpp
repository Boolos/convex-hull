#ifndef sort_hpp
#define sort_hpp

#include <thread>
#include <cmath>
#include <utility>
#include <algorithm>
#include <iterator>

#include "point.hpp"

namespace csce {
	template<typename T>
	class sort {
	public:
		sort(const csce::point<T>& pivot_point, int _nthreads) : p(pivot_point), nthreads(_nthreads) {}
		
		void sort_array(typename std::vector<csce::point<T>>::iterator begin, typename std::vector<csce::point<T>>::iterator end) {
			std::pair<int, int>* indices = new std::pair<int, int>[nthreads];
			std::thread* threads = new std::thread[nthreads]; //4 threads
			int n = static_cast<int>(std::distance(begin, end));
			int nelements = static_cast<int>(std::ceil(static_cast<double>(n) / static_cast<double>(this->nthreads)));
			
			int start = 0, stop = nelements;
			
			int segment_count = 0;
			
			//sort elements in each segment
			for(int x=0; x<nthreads && start < n; x++, start += nelements, stop += nelements){
				indices[x] = std::make_pair(start, std::min(stop, n));
				threads[x] = std::thread(&csce::sort<T>::sort_thread, this, begin + indices[x].first, begin + indices[x].second);
				segment_count++;
			}
			for(int x=0; x<segment_count; x++) {
				threads[x].join();
			}
			
			if(segment_count < 2){
				//no need to do an even-odd merge if there are not at least two segments to merge
				return;
			}
			
			//now do even-odd merge
			for(int start_index = 0, y=0; y<nthreads; start_index = (start_index + 1) % 2, y++){
				int thread_count = 0;
				for(int x=start_index, max = std::min(nthreads, segment_count) - 1; x < max; x+=2){
					thread_count++;
					threads[x>>1] = std::thread(&csce::sort<T>::merge_thread, this, begin + indices[x].first, begin + indices[x].second, begin + indices[x+1].second);
				}
				for(int x=0; x<thread_count; x++){
					threads[x].join();
				}
			}
			
			delete[] threads;
			delete[] indices;
		}
		
		
	private:
		void sort_thread(typename std::vector<csce::point<T>>::iterator begin, typename std::vector<csce::point<T>>::iterator end) {
			std::sort(begin, end, [&](csce::point<T>& a, csce::point<T>& b) {
				csce::vector2d<T> pa(this->p, a);
				csce::vector2d<T> pb(this->p, b);
				return pa.ccw(pb);
			});
		}
		
		
		void merge_thread(typename std::vector<csce::point<T>>::iterator begin, typename std::vector<csce::point<T>>::iterator mid, typename std::vector<csce::point<T>>::iterator end) {
			auto n = std::distance(begin, end);
			csce::point<T>* tmp = new csce::point<T>[n];
			
			typename std::vector<csce::point<T>>::iterator pa = begin;
			typename std::vector<csce::point<T>>::iterator pb = mid;
			int x = 0;
			while(pa != mid && pb != end){
				csce::point<T>& a = *pa;
				csce::point<T>& b = *pb;
				
				csce::vector2d<T> va(this->p, a);
				csce::vector2d<T> vb(this->p, b);
				
				if(va.ccw(vb)){
					pa++;
					tmp[x++] = a;
				} else {
					pb++;
					tmp[x++] = b;
				}
			}
			
			while(pa != mid){
				tmp[x++] = *pa;
				pa++;
			}
			
			while(pb != end){
				tmp[x++] = *pb;
				pb++;
			}
			
			std::copy(tmp, tmp + n, begin);
			delete[] tmp;
		}
		
		
	private:
		csce::point<T> p;
		int nthreads;
		
	};
}

#endif /* sort_hpp */
