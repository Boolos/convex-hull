#ifndef point_hpp
#define point_hpp

#include <string>
#include <sstream>
#include <ostream>
#include <cmath>

namespace csce {
	template<typename T>
	class vector2d;
	
	template<typename T>
	class point {
	public:
		T x = {};
		T y = {};
		point() {}
		point(T _x, T _y) : x(_x), y(_y) {}
		
		long double distance_to(const csce::point<T>& other) const {
			T dx = this->x - other.x;
			T dy = this->y - other.y;
			return std::sqrtl(dx * dx + dy * dy);
		}
		
		bool is_inside_polygon(const std::vector<csce::point<T>>& points){
			
		}
		
		static bool ccw(const csce::point<T>& a, const csce::point<T>& b, const csce::point<T>& c){
			return csce::vector2d<T>(a, b).ccw(csce::vector2d<T>(a, c));
		}
		
		std::string str() const {
			std::stringstream output;
			output << "(" << this->x << ", " << this->y << ")";
			return output.str();
		}
		
		bool operator==(const csce::point<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}
		
		bool operator!=(const csce::point<T>& other) const {
			return !(*this == other);
		}
		
		bool operator<(const csce::point<T>& other) const {
			if(this->y != other.y){
				return this->y < other.y;
			}
			return this->x < other.x;
		}
		
		friend std::ostream& operator<<(std::ostream& stream, const csce::point<T>& p){
			stream << p.str();
			return stream;
		}
		
		csce::point<T> operator-(const csce::point<T>& other) const {
			T dx = other.x - this->x;
			T dy = other.y - this->y;
			return csce::point<T>(dx, dy);
		}
		
		csce::point<T> operator+(const csce::point<T>& other) const {
			T nx = other.x + this->x;
			T ny = other.y + this->y;
			return csce::point<T>(nx, ny);
		}
		
		
	private:
		
	};
}

namespace std {
	template <typename T>
	struct hash< csce::point<T> >{
		std::size_t operator()(const csce::point<T>& p) const{
			return (std::hash<T>()(p.x) ^ (std::hash<T>()(p.y) << 1)) >> 1;
		}
	};
}

#endif /* point_hpp */