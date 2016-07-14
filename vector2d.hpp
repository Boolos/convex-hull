#ifndef vector2d_hpp
#define vector2d_hpp

#include <string>
#include <sstream>
#include <ostream>

#include "point.hpp"

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
			return this->cross(other) > 0;
		}
		
		static bool ccw(const csce::vector2d<T>& a, const csce::vector2d<T>& b) {
			return a.ccw(b);
		}
		
		std::string str() const {
			std::stringstream output;
			output << p.str();
			return output.str();
		}
		
		friend std::ostream& operator<<(std::ostream& stream, const std::vector2d<T>& v) {
			stream << v.str();
			return stream;
		}
		
	private:
		
	};
}

#endif /* vector2d_hpp */