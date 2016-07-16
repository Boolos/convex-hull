#ifndef line_hpp
#define line_hpp

#include <cmath>

#include "point.hpp"
#include "vector2d.hpp"
#include "math_utility.hpp"

namespace csce {
	template<typename T>
	class line {
	public:
		csce::point<T> a;
		csce::point<T> b;
		line(const csce::point<T>& _a, const csce::point<T>& _b) : a(_a), b(_b) {}
		
		csce::vector2d<T> to_vector2d() const {
			return csce::vector2d<T>(a, b);
		}
		
		long double distance_to(const csce::point<T>& p) {
			csce::vector2d<T> ab = this->to_vector2d();
			csce::vector2d<T> bc(this->b, p);
			T dp = ab.dot(bc);
			if(dp > 0){
				return this->b.distance_to(p);
			}
			
			csce::vector2d<T> ac(this->a, p);
			csce::vector2d<T> ba(this->b, this->a);
			dp = ba.dot(ac);
			if(dp > 0){
				return this->a.distance_to(p);
			}
			
			return std::abs(static_cast<long double>(ab.cross(ac)) / static_cast<long double>(this->a.distance_to(b)));
		}
		
		bool contains_point(const csce::point<T>& p) const {
			//p has to be colinear with this line
			if(this->orientation(this->a, this->b, p) != 0)
				return false;
			
			return p.x <= std::max(this->a.x, this->b.x) && p.x >= std::min(this->a.x, this->b.x) && p.y <= std::max(this->a.y, this->b.y) && p.y >= std::min(this->a.y, this->b.y);
		}
		
		bool intersects(const csce::line<T>& other) const {
			int o1 = orientation(this->a, this->b, other.a);
			int o2 = orientation(this->a, this->b, other.b);
			int o3 = orientation(other.a, other.b, this->a);
			int o4 = orientation(other.a, other.b, this->b);
			
			//if the end points of the two lines lie on opposites sides of the other line
			if (o1 != o2 && o3 != o4)
				return true;
			
			if (o1 == 0 && this->contains_point(other.a)) return true;
			if (o2 == 0 && this->contains_point(other.b)) return true;
			if (o3 == 0 && other.contains_point(this->a)) return true;
			if (o4 == 0 && other.contains_point(this->b)) return true;
			
			return false;
		}
		
	private:
		/**
		 * Returns 0 if p->q is colinear with p->r, 1 if counter-clockwise, or -1 if clockwise.
		 */
		int orientation(const csce::point<T>& p, const csce::point<T>& q, const csce::point<T>& r) const {
			csce::vector2d<T> pq(p, q);
			csce::vector2d<T> pr(p, r);
			T cross_product = pq.cross(pr);
			
			if(csce::math_utility::equals_zero(cross_product))
				return 0;
			
			return (cross_product < 0) ? -1 : 1;
		}
	};
}

#endif /* line_hpp */
