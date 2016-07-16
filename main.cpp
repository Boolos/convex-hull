#include <iostream>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <unordered_set>
#include <set>
#include <cmath>
#include <algorithm>

#include "point.hpp"
#include "convex_hull_base.hpp"
#include "utility.hpp"
#include "convex_hull_implementations.hpp"

#include "test.hpp"

namespace csce {
	bool algorithm_tuple_comparator (const std::tuple<std::string, long long int, int>& a, const std::tuple<std::string, long long int, int>& b){
		return std::get<1>(a) < std::get<1>(b);
	}
}

template<typename T>
int run(int argc, char* argv[]) {
	int n = 320; //the default number of points. This can be changed by specifying the -n runtime argument.
	int thread_count = std::max(static_cast<unsigned int>(4), std::thread::hardware_concurrency()); //the default number of threads. This can be changed by the -t runtime argument.
	std::string input_file_path; //where to load data from, if anywhere. If this is not specified, the data will be generated at runtime.
	std::string output_file_path; //where to write out the generated data, if anywhere.
	int iterations = 1; //the number of times to sort the data
	long long int duration = 0;
	bool debug = false;
	bool test_mode = false;
	
	std::vector<csce::point<T>> points;
	std::vector<csce::point<T>> points_copy;
	std::vector<std::tuple<std::string, long long int, int>> algorithm_tuples;
	
	int min = -100;
	int max = 100;
	
	int c;
	while((c = getopt(argc, argv, ":dDf:m:M:n:o:r:t:")) != -1){
		switch(c){
			case 'd':
				debug = true;
				break;
				
			case 'D':
				test_mode = true;
				break;
				
			case 'f':
				if(optarg != NULL){
					input_file_path = std::string(optarg);
				}
				break;
				
			case 'm':
				if(optarg != NULL){
					min = std::stoi(optarg);
				}
				break;
				
			case 'M':
				if(optarg != NULL){
					max = std::stoi(optarg);
				}
				break;
				
			case 'n':
				if(optarg != NULL){
					n = std::stoi(optarg);
				}
				break;
				
			case 'o':
				if(optarg != NULL){
					output_file_path = std::string(optarg);
				}
				break;
				
			case 'r':
				if(optarg != NULL){
					iterations = std::stoi(optarg);
				}
				break;
				
			case 't':
				if(optarg != NULL){
					thread_count = std::stoi(optarg);
				}
				break;
				
			case '?':
				break;
		}
	}
	
	if(min > max){
		std::swap(min, max);
	}
	
	//
	// run unit tests
	//
	if(test_mode){
		csce::test(debug).run();
	}
	
	
	//
	//first - load the values into the array, either by populating it
	//        with random values or by reading the values from a file.
	//
	std::cout << "Populating array with " << n << " points with values between [" << min << ", " << max << "] ... " << std::flush;
	if(input_file_path.empty()){
		//no input file was specified, so populate the array with random numbers
		points = csce::utility::random_points<T>(n, min, max);
	} else {
		//load from the specified file
		points = csce::utility::points_from_file<T>(n, input_file_path);
	}
	std::cout << "done." << std::endl;
	
	
	//
	//second - if the output file path was specified, write the array to the output file
	//         so it can be used in subsequent runs of this process.
	//
	if(!output_file_path.empty()){
		std::cout << "Writing array to file (" << output_file_path << ") ... " << std::flush;
		csce::utility::write_to_file(points, output_file_path);
		std::cout << "done." << std::endl;
	}
	
	
	
	//if running in debug mode, print the values of the array
	if(debug){
		std::cout << "Running in debug mode ... printing array" << std::endl;
		std::cout << "-------" << std::endl;
		for(auto& point : points){
			std::cout << point.str() << std::endl;
		}
		std::cout << "-------" << std::endl;
		std::cout << "done printing array" << std::endl << std::endl;
	}
	
	
	
	if(iterations > 1){
		std::cout << "***********************************" << std::endl;
		std::cout << "** Running " << iterations << " iterations" << std::endl;
		std::cout << "***********************************" << std::endl;
		std::cout << std::endl << std::endl;
	}
	
	for(int iteration = 0; iteration < iterations; iteration++){
		if(iterations > 1){
			std::cout << "***********************************" << std::endl;
			std::cout << "** Starting iteration " << iteration << std::endl;
			std::cout << "***********************************" << std::endl;
		}
		
		std::vector<csce::convex_hull_base<T>*> algorithms = csce::convex_hull_implementations::list<T>(thread_count);
		if(iteration == 0){
			algorithm_tuples.reserve(algorithms.size());
			for(auto& algorithm : algorithms){
				algorithm_tuples.push_back(std::make_tuple(algorithm->name(), 0L, 0));
			}
		}
		
		for(std::size_t x=0; x<algorithms.size(); x++){
			points_copy = points;
			
			std::cout << "-------------------------------------------" << std::endl;
			std::cout << "Testing convex hull implementation (" << algorithms[x]->name() << ")" << std::endl;
			std::cout << "Computing convex hull ... " << std::flush;
			
			std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
			std::vector<csce::point<T>> hull_points = algorithms[x]->compute_hull(points_copy);
			std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
			std::get<1>(algorithm_tuples[x]) += duration;
			
			std::cout << "done in " << csce::utility::duration_string(duration) << std::endl;
			
			std::cout << "Validating convex hull ... " << std::flush;
			std::vector<std::string> error_messages;
			bool valid = csce::utility::validate<T>(hull_points, points, max, error_messages);
			std::cout << (valid ? "correct" : "INCORRECT") << std::endl;
			if(!valid){
				std::cout << "There were " << error_messages.size() << " errors.";
				std::cout << std::endl;
				for(std::string& message : error_messages){
					std::cout << "Error: " << message << std::endl;
				}
			} else {
				std::get<2>(algorithm_tuples[x])++;
			}
			std::cout << "-------------------------------------------" << std::endl;
		}
		
		//clean up the pointers
		for(auto& algorithm : algorithms){
			delete algorithm;
		}
		
		if(iterations > 1){
			std::cout << std::endl;
		}
	}
	
	if(iterations > 1){
		std::sort(algorithm_tuples.begin(), algorithm_tuples.end(), csce::algorithm_tuple_comparator);
		
		std::cout << std::endl;
		std::cout << "===================================" << std::endl;
		std::cout << "== Run statistics for " << iterations << " runs" << std::endl;
		std::cout << "===================================" << std::endl;
		
		for(std::size_t x=0; x<algorithm_tuples.size(); x++){
			std::string sort_name;
			long long int sort_duration = 0L;
			int sort_correct = 0;
			std::tie(sort_name, sort_duration, sort_correct) = algorithm_tuples[x];
			
			std::cout << "==" << std::endl;
			std::cout << "== " << sort_name << std::endl;
			std::cout << "==   Total time: " << csce::utility::duration_string(sort_duration) << std::endl;
			std::cout << "== Average time: " << csce::utility::duration_string(sort_duration / iterations) << std::endl;
			
			long double correct_percent = 100.0L * static_cast<long double>(sort_correct) / static_cast<long double>(iterations);
			std::cout << "==      Correct: " << correct_percent << "%  (" << sort_correct << " / " << iterations << ")" << std::endl;
			std::cout << "==" << std::endl;
		}
		
		std::cout << "===================================" << std::endl;
	}
	
	return 0;
}

int main(int argc, char* argv[]) {
	run<long double>(argc, argv);
}
