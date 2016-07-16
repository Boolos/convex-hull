#include "test.hpp"

bool csce::test::run() const {
	std::cout << "running tests ..." << std::endl;
	bool correct = this->test_line_intersect();
	correct &= this->test_convex_polygon();
	correct &= this->test_contains_all_points();
	correct &= this->test_validate_convex_hull();
	
	std::cout << "tests complete ... " << (correct ? "correct" : "INCORRECT") << std::endl;
	return correct;
}

bool csce::test::test_convex_polygon() const {
	bool correct = true;
	
	std::cout << "Testing convex polygon ... " << std::flush;
	if(this->debug){
		std::cout << std::endl;
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 3));
		polygon.push_back(csce::point<int>(16, 10));
		polygon.push_back(csce::point<int>(11, 12));
		polygon.push_back(csce::point<int>(0, 0));
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool convex = csce::utility::is_convex(polygon, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << "ERROR: " << error << std::endl;
			}
		}
		
		correct &= (convex == expected_result);
		if(this->debug){
			std::cout << (convex == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(5, 5));
		polygon.push_back(csce::point<int>(0, 0));
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool convex = csce::utility::is_convex(polygon, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << "ERROR: " << error << std::endl;
			}
		}
		
		correct &= (convex == expected_result);
		if(this->debug){
			std::cout << (convex == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(5, 5));
		polygon.push_back(csce::point<int>(4, 1));
		polygon.push_back(csce::point<int>(0, 0));
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool convex = csce::utility::is_convex(polygon, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << "ERROR: " << error << std::endl;
			}
		}
		
		correct &= (convex == expected_result);
		if(this->debug){
			std::cout << (convex == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(5, 5));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(0, 0));
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool convex = csce::utility::is_convex(polygon, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << "ERROR: " << error << std::endl;
			}
		}
		
		correct &= (convex == expected_result);
		if(this->debug){
			std::cout << (convex == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	std::cout << (correct ? "correct" : "INCORRECT") << std::endl;
	return correct;
}

bool csce::test::test_line_intersect() const {
	bool correct = true;
	
	std::cout << "Testing line intersection ... " << std::flush;
	if(this->debug){
		std::cout << std::endl;
	}
	
	{
		csce::point<int> p1(1, 1);
		csce::point<int> q1(10, 1);
		
		csce::point<int> p2(1, 2);
		csce::point<int> q2(10, 2);
	 
		csce::line<int> p1q1(p1, q1);
		csce::line<int> p2q2(p2, q2);
		
		bool intersects = p1q1.intersects(p2q2);
		correct &= !intersects;
		if(this->debug){
			intersects ? std::cout << "Yes" : std::cout << "No (correct)";
			std::cout << std::endl;
		}
	}
	
	{
		csce::point<int> p1(10, 0);
		csce::point<int> q1(0, 10);
		
		csce::point<int> p2(0, 0);
		csce::point<int> q2(10, 10);
	 
		csce::line<int> p1q1(p1, q1);
		csce::line<int> p2q2(p2, q2);
		
		bool intersects = p1q1.intersects(p2q2);
		correct &= intersects;
		if(this->debug){
			intersects ? std::cout << "Yes (correct)" : std::cout << "No";
			std::cout << std::endl;
		}
	}
	
	{
		csce::point<int> p1(-5, -5);
		csce::point<int> q1(0, 0);
		
		csce::point<int> p2(1, 1);
		csce::point<int> q2(10, 10);
	 
		csce::line<int> p1q1(p1, q1);
		csce::line<int> p2q2(p2, q2);
		
		bool intersects = p1q1.intersects(p2q2);
		correct &= !intersects;
		if(this->debug){
			intersects ? std::cout << "Yes" : std::cout << "No (correct)";
			std::cout << std::endl;
		}
	}
	
	{
		csce::point<int> p1(-5, -5);
		csce::point<int> q1(1, 1);
		
		csce::point<int> p2(1, 1);
		csce::point<int> q2(10, 10);
	 
		csce::line<int> p1q1(p1, q1);
		csce::line<int> p2q2(p2, q2);
		
		bool intersects = p1q1.intersects(p2q2);
		correct &= intersects;
		if(this->debug){
			intersects ? std::cout << "Yes (correct)" : std::cout << "No";
			std::cout << std::endl;
		}
	}
	
	{
		csce::point<int> p1(0, 5);
		csce::point<int> q1(5, 0);
		
		csce::point<int> p2(0, 0);
		csce::point<int> q2(2, 3);
	 
		csce::line<int> p1q1(p1, q1);
		csce::line<int> p2q2(p2, q2);
		
		bool intersects = p1q1.intersects(p2q2);
		correct &= intersects;
		if(this->debug){
			intersects ? std::cout << "Yes (correct)" : std::cout << "No";
			std::cout << std::endl;
		}
	}
	
	{
		csce::point<int> p1(0, 5);
		csce::point<int> q1(5, 0);
		
		csce::point<int> p2(0, 0);
		csce::point<int> q2(2, 2);
	 
		csce::line<int> p1q1(p1, q1);
		csce::line<int> p2q2(p2, q2);
		
		bool intersects = p1q1.intersects(p2q2);
		correct &= !intersects;
		if(this->debug){
			intersects ? std::cout << "Yes" : std::cout << "No (correct)";
			std::cout << std::endl;
		}
	}
	
	std::cout << (correct ? "correct" : "INCORRECT") << std::endl;
	
	return correct;
}

bool csce::test::test_contains_all_points() const {
	bool correct = true;
	std::cout << "Testing contains all points ... " << std::flush;
	if(this->debug){
		std::cout << std::endl;
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		
		int max = 20;
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::contains_all_points(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(-1, 1)); //outside polygon
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::contains_all_points(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(11, 0)); //outside polygon
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::contains_all_points(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(10, 5)); //on border
		points.push_back(csce::point<int>(0, 5)); //on border
		points.push_back(csce::point<int>(5, 0)); //on border
		points.push_back(csce::point<int>(5, 10)); //on border
		
		int max = 20;
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::contains_all_points(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 15));
		polygon.push_back(csce::point<int>(10, 20));
		polygon.push_back(csce::point<int>(0, 20));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(5, 15)); //intersects with two segment end-points
		
		
		int max = 20;
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::contains_all_points(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 15));
		polygon.push_back(csce::point<int>(10, 20));
		polygon.push_back(csce::point<int>(0, 20));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(-5, 15)); //crosses one segment and intersects two end-points (outside polygon)
		
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::contains_all_points(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	std::cout << (correct ? "correct" : "INCORRECT") << std::endl;
	return correct;
}

bool csce::test::test_validate_convex_hull() const {
	bool correct = true;
	std::cout << "Testing validate convex hull ... " << std::flush;
	if(this->debug){
		std::cout << std::endl;
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		
		int max = 20;
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(-1, 1)); //outside polygon
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(11, 0)); //outside polygon
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(10, 10));
		polygon.push_back(csce::point<int>(0, 10));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(10, 5)); //on border
		points.push_back(csce::point<int>(0, 5)); //on border
		points.push_back(csce::point<int>(5, 0)); //on border
		points.push_back(csce::point<int>(5, 10)); //on border
		
		int max = 20;
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 15));
		polygon.push_back(csce::point<int>(10, 20));
		polygon.push_back(csce::point<int>(0, 20));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(5, 15)); //intersects with two segment end-points
		
		
		int max = 20;
		
		bool expected_result = true;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 15));
		polygon.push_back(csce::point<int>(10, 20));
		polygon.push_back(csce::point<int>(0, 20));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		points.push_back(csce::point<int>(-5, 15)); //crosses one segment and intersects two end-points (outside polygon)
		
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 15));
		polygon.push_back(csce::point<int>(10, 20));
		polygon.push_back(csce::point<int>(9, 19)); //the polygon is not convex
		polygon.push_back(csce::point<int>(8, 20));
		polygon.push_back(csce::point<int>(0, 20));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(3, 7));
		points.push_back(csce::point<int>(5, 3));
		points.push_back(csce::point<int>(4, 8));
		points.push_back(csce::point<int>(8, 2));
		points.push_back(csce::point<int>(9, 9));
		points.push_back(csce::point<int>(5, 5));
		
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	{
		std::vector<csce::point<int>> polygon;
		polygon.push_back(csce::point<int>(0, 0));
		polygon.push_back(csce::point<int>(10, 0));
		polygon.push_back(csce::point<int>(15, 15));
		polygon.push_back(csce::point<int>(10, 20));
		polygon.push_back(csce::point<int>(8, 20)); //the polygon is not convex
		polygon.push_back(csce::point<int>(0, 20));
		polygon.push_back(csce::point<int>(0, 0));
		
		std::vector<csce::point<int>> points;
		for(auto& point : polygon){
			points.push_back(point);
		}
		points.push_back(csce::point<int>(1, 1));
		points.push_back(csce::point<int>(5, 5));
		
		
		int max = 20;
		
		bool expected_result = false;
		
		std::vector<std::string> error_messages;
		bool result = csce::utility::validate(polygon, points, max, error_messages);
		if(this->debug){
			for(auto& error : error_messages){
				std::cout << error << std::endl;
			}
		}
		
		correct &= (result == expected_result);
		if(this->debug){
			std::cout << (result == expected_result ? "correct" : "INCORRECT") << std::endl;
		}
	}
	
	std::cout << (correct ? "correct" : "INCORRECT") << std::endl;
	return correct;
}