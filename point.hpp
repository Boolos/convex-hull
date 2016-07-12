#ifndef point_hpp
#define point_hpp

#include <string>
#include <sstream>

namespace csce {
	class point {
	public:
		int x;
		int y;
		point(int _x, int _y);
		std::string str() const;
		
		bool operator==(const csce::point& other) const;
		bool operator!=(const csce::point& other) const;
		bool operator<(const csce::point& other) const;
	};
}

template <>
struct std::hash<csce::point>{
	std::size_t operator()(const csce::point& p) const{
		return (std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1)) >> 1;
	}
};

#endif /* point_hpp */
