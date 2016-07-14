#ifndef line_hpp
#define line_hpp

#include "point.hpp"
#include "vector2d.hpp"

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
			
			return std::fabsl(static_cast<long double>(ab.cross(ac)) / static_cast<long double>(this->a.distance_to(b)));
		}
		
	private:
		
	};
}

#endif /* line_hpp */
