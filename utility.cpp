#include "utility.hpp"

std::string csce::utility::duration_string(long long int duration){
	std::stringstream out;
	out << (duration / 1E9) << " seconds";
	return out.str();
}

std::vector<csce::point> csce::utility::random_points(int n, int min, int max){
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(min, max);
	
	std::vector<csce::point> points;
	points.reserve(n);
	for(int x=0; x<n; x++){
		points.push_back(csce::point(dist(mt), dist(mt)));
	}
	
	return points;
}

std::vector<csce::point> csce::utility::random_points(int n){
	int min = std::numeric_limits<int>::min();
	int max = std::numeric_limits<int>::max();
	return csce::utility::random_points(n, min, max);
}

std::vector<csce::point> csce::utility::points_from_file(int n, const std::string& file_path) {
	std::vector<csce::point> points;
	points.reserve(n);
	
	std::ifstream file(file_path, std::ios::in);
	if(file.is_open()){
		int x=0, y=0;
		while(file >> x >> y){
			points.push_back(csce::point(x, y));
			if(points.size() >= n){
				break;
			}
		}
		file.close();
	}
	
	return points;
}

void csce::utility::write_to_file(const std::vector<csce::point>& points, const std::string& file_path){
	std::ofstream file(file_path, std::ios_base::out | std::ios_base::trunc);
	if(file.is_open()){
		for(auto& point : points){
			file << point.x << " " << point.y << std::endl;
		}
		file.close();
	}
}

int csce::utility::validate(const std::vector<csce::point>& points, const std::vector<csce::point>& convex_hull, std::vector<std::string>& output_errors){
	int error_count = 0;
	
	//TODO: generate the convex hull points
	//TODO: validate that the convex_hull parameter and the generated convex hull contain the same points.
	
	/*for(int x=1; x<n; x++){
		if(array[x-1] > array[x]){
	 if(error_count < 10){
	 std::stringstream error;
	 error << "validate: array[" << (x-1) << "] = " << array[x-1] << " is not less than or equal to array[" << x << "] = " << array[x];
	 output_errors.push_back(error.str());
	 }
	 error_count++;
		}
	 }*/
	
	return error_count;
}