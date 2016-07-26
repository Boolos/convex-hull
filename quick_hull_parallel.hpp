#ifndef quick_hull_parallel_hpp
#define quick_hull_parallel_hpp

#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <thread>
#include <mutex>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "line.hpp"
#include "vector2d.hpp"
#include "math_utility.hpp"

namespace csce {
	template<typename T>
	class quick_hull_parallel : public csce::convex_hull_base<T> {
	public:
		quick_hull_parallel(int _nthreads) : csce::convex_hull_base<T>(_nthreads) { }
		
		std::string name() const {
			return "Quick Hull (Parallel)";
		}

        std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
            auto left_most_point = get_left_most(points);
            this->hull.push_back(left_most_point);
            auto right_most_point = get_right_most(points);
            this->hull.push_back(right_most_point);
            
            line<T> left_to_right = { left_most_point, right_most_point };
            line<T> right_to_left = { right_most_point, left_most_point };
            auto partition = pp(points, left_to_right);

            std::thread l_thread(&csce::quick_hull_parallel<T>::get_hull, this, partition.first, left_to_right);
            std::thread r_thread(&csce::quick_hull_parallel<T>::get_hull, this, partition.second, right_to_left);
            
            l_thread.join();
            r_thread.join();

            return this->hull;
        }

        void get_hull(const std::vector<csce::point<T>>& points, const csce::line<T>& boundary) {
            if (points.size() == 0) {
                return;
            }

            auto hull_point = find_hull_point(points, boundary);
            this->mut.lock();
            this->hull.push_back(hull_point);
            this->mut.unlock();

            csce::line<T> a = { boundary.a, hull_point };
            auto left = pp(points, a).first;

            csce::line<T> b = { hull_point, boundary.b };
            auto right = pp(points, b).first;

            get_hull(left, a);
            get_hull(right, b);
        }

        // first has ccw for a -> b, second has cw for a -> b
        std::pair<std::vector<csce::point<T>>, std::vector<csce::point<T>>> pp(const std::vector<csce::point<T>>& points, const csce::line<T>& segment) const {
            std::pair<std::vector<csce::point<T>>, std::vector<csce::point<T>>> partition;
            partition.first = { };
            partition.second = { };

            for (int i = 0; i < points.size(); i++) {
                if (segment.to_vector2d().ccw(csce::vector2d<T>(segment.a, points[i]))) {
                    partition.first.push_back(points[i]);
                } else if (segment.to_vector2d().cw(csce::vector2d<T>(segment.a, points[i]))) {
                    partition.second.push_back(points[i]);
                }
            }

            return partition;
        }

        /** 
        * Has no concept of orientation.  Will grab the maximal distance point
        * on either side of the boundary.  Therefore, must pass a line segment
        * that is a boundary (i.e. all points are on one side of the line and
        * within the bounds of the boundary end points).
        */
        csce::point<T> find_hull_point(const std::vector<csce::point<T>>& points, const csce::line<T>& boundary) const {
            if (points.size() == 0) {
                throw "No points to find maximal distance!";
            }
            
            int index_of_maximal = 0;
            for (int i = 1; i < points.size(); i++) {
                auto dist = boundary.distance_to(points[i]);
                auto max = boundary.distance_to(points[index_of_maximal]);
                if (csce::math_utility::less_than(max, dist)) {
                    index_of_maximal = i;
                }
            }
            return points[index_of_maximal];
        }

        csce::point<T> get_left_most(const std::vector<csce::point<T>>& points) const {
            if (points.size() == 0) {
                throw "There are no points!";
            }

            csce::point<T> left_most = points[0];
            auto start = points.begin();
            auto end = points.end();
            for (auto current_point = start; current_point != end; current_point++) {
                if (left_most.x <= current_point->x) {
                    continue;
                }
                left_most = *current_point;
            }
            
            return left_most;
        } 

        csce::point<T> get_right_most(const std::vector<csce::point<T>>& points) const {
            if (points.size() == 0) {
                throw "There are no points!";
            }

            csce::point<T> right_most = points[0];
            auto start = points.begin();
            auto end = points.end();
            for (auto current_point = start; current_point != end; current_point++) {
                if (right_most.x >= current_point->x) {
                    continue;
                }
                right_most = *current_point;
            }
            
            return right_most;
        }

		
		
		~quick_hull_parallel() {}
		
		
	private:
        int nthread;
        std::vector<csce::point<T>> hull;
        std::mutex mut;
	};
}

#endif /* quick_hull_parallel_hpp */
