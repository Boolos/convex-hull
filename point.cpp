#include "point.hpp"

csce::point::point(int _x, int _y) : x(_x), y(_y) {}

std::string csce::point::str() const {
	std::stringstream output;
	output << "(" << this->x << ", " << this->y << ")";
	return output.str();
}

bool csce::point::operator==(const csce::point& other) const {
	return this->x == other.x && this->y == other.y;
}

bool csce::point::operator!=(const csce::point& other) const {
	return !(*this == other);
}

bool csce::point::operator<(const csce::point& other) const {
	if(this->y != other.y){
		return this->y < other.y;
	}
	return this->x < other.x;
}