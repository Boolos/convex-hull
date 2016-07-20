#ifndef vector2d_hpp
#define vector2d_hpp

#include <string>
#include <sstream>
#include <ostream>

#include "point.hpp"
#include "math_utility.hpp"

namespace csce {
	template<typename T>
	class vector2d {
	public:
		csce::point<T> p;
		
		vector2d(csce::point<T> p1, csce::point<T> p2) {
			this->p = p2 - p1;
		}
		
		T dot(const csce::vector2d<T>& other) const {
			return this->x * other.x + this->y * other.y;
		}
		
		T cross(const csce::vector2d<T>& other) const {
			return this->p.x * other.p.y - this->p.y * other.p.x;
		}
		
		bool ccw(const csce::vector2d<T>& other) const {
			return this->orientation(other) > 0;
		}
		
		bool cw(const csce::vector2d<T>& other) const {
			return this->orientation(other) < 0;
		}
		
		bool collinear(const csce::vector2d<T>& other) const {
			return this->orientation(other) == 0;
		}
		
		/**
		 * Returns 0 if the other vector is collinear with this vector,
		 * +1 if the other vector is counterclockwise to this vector,
		 * -1 if the other vector is clockwise to this vector.
		 */
		int orientation(const csce::vector2d<T>& other) const {
			T cross_product = this->cross(other);
			if(csce::math_utility::equals_zero(cross_product))
				return 0;
			
			return (cross_product < 0) ? -1 : 1;
		}
		
		std::string str() const {
			std::stringstream output;
			output << p.str();
			return output.str();
		}
		
		friend std::ostream& operator<<(std::ostream& stream, const csce::vector2d<T>& v) {
			stream << v.str();
			return stream;
		}
		
	private:
		
	};
}

#endif /* vector2d_hpp */
