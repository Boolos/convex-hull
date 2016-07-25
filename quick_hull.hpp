#ifndef quick_hull_hpp
#define quick_hull_hpp

#include <vector>
#include <string>
#include <iostream>
#include <utility>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "line.hpp"
#include "vector2d.hpp"
#include "math_utility.hpp"

namespace csce {
	template<typename T>
	class quick_hull : public csce::convex_hull_base<T> {
	public:
		quick_hull(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}
		
		std::string name() const {
			return "Quick Hull";
		}

		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) {
            std::cout << std::endl;
            std::vector<csce::point<T>> hull;

            auto left_most_point = get_left_most(points);
            hull.push_back(left_most_point);
            auto right_most_point = get_right_most(points);
            hull.push_back(right_most_point);
            
            line<T> left_to_right = { left_most_point, right_most_point };
            line<T> right_to_left = { right_most_point, left_most_point };
            auto p = pp(points, left_to_right);

            get_hull(p.first, left_to_right, hull);
            get_hull(p.second, right_to_left, hull);

            return hull;
        }

        // first has ccw for a -> b, second has cw for a -> b
        std::pair<std::vector<csce::point<T>>, std::vector<csce::point<T>>> pp(std::vector<csce::point<T>>& points, csce::line<T>& segment) {
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

        void get_hull(std::vector<csce::point<T>>& points, csce::line<T>& boundary, std::vector<csce::point<T>>& hull) {
            if (points.size() == 0) {
                return;
            }

            auto hull_point = find_hull_point(points, boundary);
            hull.push_back(hull_point);

            csce::line<T> a = { boundary.a, hull_point };
            auto left = pp(points, a).first;

            csce::line<T> b = { hull_point, boundary.b };
            auto right = pp(points, b).first;

            get_hull(left, a, hull);
            get_hull(right, b, hull);
        }

        /** 
        * Has no concept of orientation.  Will grab the maximal distance point
        * on either side of the boundary.  Therefore, must pass a line segment
        * that is a boundary (i.e. all points are on one side of the line and
        * within the bounds of the boundary end points).
        */
        csce::point<T> find_hull_point(std::vector<csce::point<T>>& points, csce::line<T>& boundary) {
            if (points.size() == 0) {
                throw "No points to find maximal distance!";
            }
            
            int index_of_maxial = 0;
            for (int i = 1; i < points.size(); i++) {
                auto dist = boundary.distance_to(points[i]);
                auto max = boundary.distance_to(points[index_of_maxial]);
                if (csce::math_utility::less_than(max, dist)) {
                    index_of_maxial = i;
                }
            }
            return points[index_of_maxial];
        }

        csce::point<T> get_left_most(std::vector<csce::point<T>>& points) {
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

        csce::point<T> get_right_most(std::vector<csce::point<T>>& points) {
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

		
		
		~quick_hull() {}
		
		
	private:
	};
}

#endif /* quick_hull_hpp */
