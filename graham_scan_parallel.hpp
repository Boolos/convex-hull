#ifndef graham_scan_parallel_hpp
#define graham_scan_parallel_hpp

#include <vector>
#include <string>
#include <utility>
#include <thread>
#include <mutex>
#include <functional>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"
#include "sort.hpp"
#include "graham_scan.hpp"

namespace csce {
	template<typename T>
	class graham_scan_parallel : public csce::convex_hull_base<T> {
	public:
		graham_scan_parallel(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}
		
		
		std::string name() const {
			return "Graham's Scan Parallel";
		}
		
		
		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
			if(points.size() <= 1600){
				//if the input size is small enough, it is better to run the
				//sequential version of this algorithm. It also handles some
				//corner cases, such as sets of points that have three points or less.
				csce::graham_scan<T> sequential(this->nthreads);
				return sequential.compute_hull(points);
			}
	
			std::size_t index = this->pivot_index(points);
			if(index != 0){
				std::swap(points.front(), points[index]);
			}
			
			csce::sort<T> sorter(points.front(), this->nthreads);
			sorter.sort_array(points.begin() + 1, points.end());
		
			//the points are now sorted by angle with respect to the south-most point (points[0])
			//so divide the points into four different quadrants and process each quadrant in a
			//different thread.
	
			//the indices previously calculated are now invalidated after the sort
			this->pivot_index(points);

			//now npindex, epindex, spindex, wpindex represent the locations that the north,
			//east, south, and west points respectively are located
			
			return convex_hull(points);
		}
		
		
		~graham_scan_parallel() {}
		
		
	private:
		
		std::size_t spindex = 0;
		std::size_t npindex = 0;
		std::size_t epindex = 0;
		std::size_t wpindex = 0;
		std::mutex pivot_mutex;


		void convex_hull_thread(const std::vector<csce::point<T>>& points, const std::size_t begin, const std::size_t end, std::vector<csce::point<T>>& output){
			if(begin == end - 1){
				output.push_back(points[begin]);
				return;
			}

			output.push_back(begin == 0 ? points.back() : points[begin - 1]); //add the last point from the previous segment
                        output.push_back(points[begin]);
                        output.push_back(points[begin + 1]);

                        std::size_t x = begin + 2;
                        while(x < end){
                                std::size_t y = output.size() - 1;
                                csce::vector2d<T> a(output[y-1], output[y]);
                                csce::vector2d<T> b(output[y-1], points[x]);
                                if(a.ccw(b)){
                                        output.push_back(points[x++]);
                                } else {
                                        output.pop_back();
                                }
                        }
		}

		std::vector<csce::point<T>> convex_hull(const std::vector<csce::point<T>>& points){
			std::thread threads[4]; //there are four extreme points (N, E, S, W)
			std::vector<std::vector<csce::point<T>>> convex_hulls(4);

			threads[0] = std::thread(&csce::graham_scan_parallel<T>::convex_hull_thread, this, points, this->spindex, this->epindex, std::ref(convex_hulls[0]));
			threads[1] = std::thread(&csce::graham_scan_parallel<T>::convex_hull_thread, this, points, this->epindex, this->npindex, std::ref(convex_hulls[1]));
			threads[2] = std::thread(&csce::graham_scan_parallel<T>::convex_hull_thread, this, points, this->npindex, this->wpindex, std::ref(convex_hulls[2]));
			threads[3] = std::thread(&csce::graham_scan_parallel<T>::convex_hull_thread, this, points, this->wpindex, points.size(), std::ref(convex_hulls[3]));

			std::vector<csce::point<T>> output;
                        for(int x=0; x<4; x++){
                                threads[x].join();
				for(auto& point : convex_hulls[x]){
					output.push_back(point);
				}
                        }

			//the output contains a mostly correct convex hull (which should be a lot smaller than the input array),
			//so run the sequential algorithm to clean up an quadrant boundaries
			csce::graham_scan<T> sequential(this->nthreads);
                        return sequential.compute_hull(output);
		}
	
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
			return this->spindex;
		}
		
		void pivot_index_thread(const std::vector<csce::point<T>>& points, int start, int stop) {
			std::size_t sindex = start;
			std::size_t nindex = start;
			std::size_t eindex = start;
			std::size_t windex = start;
			for(std::size_t x=start; x<points.size() && x <= stop; x++){
				this->get_pivot_points(points, x, nindex, eindex, sindex, windex);
			}
			this->update_pivot_index(points, nindex, eindex, sindex, windex);
		}
	
		void update_pivot_index(const std::vector<csce::point<T>>& points, const std::size_t& n, const std::size_t& e, const std::size_t& s, const std::size_t& w){
			this->pivot_mutex.lock();
			this->get_pivot_points(points, n, this->npindex, this->epindex, this->spindex, this->wpindex);
			this->get_pivot_points(points, e, this->npindex, this->epindex, this->spindex, this->wpindex);
			this->get_pivot_points(points, s, this->npindex, this->epindex, this->spindex, this->wpindex);
			this->get_pivot_points(points, w, this->npindex, this->epindex, this->spindex, this->wpindex);
			this->pivot_mutex.unlock();
		}
	
		void get_pivot_points(const std::vector<csce::point<T>>& points, const std::size_t& x, std::size_t& n, std::size_t& e, std::size_t& s, std::size_t& w){
			const csce::point<T>& ps = points[s];
                        const csce::point<T>& pn = points[n];
                        const csce::point<T>& pe = points[e];
                        const csce::point<T>& pw = points[w];

                        const csce::point<T>& cur = points[x];
                        if(csce::math_utility::less_than(cur.y, ps.y)){
                                s = x;
                        } else if(csce::math_utility::equals(cur.y, ps.y)){
                                if(csce::math_utility::less_than(cur.x, ps.x)){
                                        s = x;
                                }
                        }
                                

                        if(csce::math_utility::less_than(pn.y, cur.y)){
                                n = x;
                        } else if(csce::math_utility::equals(cur.y, pn.y)){
                                if(csce::math_utility::less_than(cur.x, pn.x)){
                                        n = x;
                                }
                        }


                        if(csce::math_utility::less_than(cur.x, pw.x)){
                                w = x;
                        } else if(csce::math_utility::equals(cur.x, pw.x)){
                                if(csce::math_utility::less_than(cur.y, pw.y)){
                                        w = x;
                                }
                        }


                        if(csce::math_utility::less_than(pe.x, cur.x)){
                                e = x;
                        } else if(csce::math_utility::equals(cur.x, pe.x)){
                                if(csce::math_utility::less_than(cur.y, pe.y)){
                                        e = x;
                                }
                        }
		}
	};
}

#endif /* graham_scan_parallel_hpp */
