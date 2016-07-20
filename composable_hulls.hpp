#ifndef composable_hulls_hpp
#define composable_hulls_hpp

#include <vector>
#include <string>
#include <utility>
#include <omp.h>
#include <math.h>

#include "convex_hull_base.hpp"
#include "graham_scan.hpp"
#include "point.hpp"
#include "vector2d.hpp"

namespace csce
{
	template<typename T, typename U = graham_scan<T>>
	class composable_hulls : public csce::convex_hull_base<T>
	{
	public:
		composable_hulls(int _nthreads) : convex_hull_base<T>(_nthreads) {}
		~composable_hulls() {}
		
		std::string name() const
		{
			return std::string("Composable Hulls <").append(U(0).name()).append(">");
		}
		
		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>> &points) const
		{
			std::vector<std::vector<csce::point<T>>> sectors(this->nthreads);
			std::vector<std::vector<csce::point<T>>> hulls(this->nthreads);
			double secSize = M_PI / this->nthreads;
			size_t topMostPoint = 0;
			
			#pragma omp parallel num_threads(this->nthreads)
			{
				std::vector<std::vector<csce::point<T>>> localSectors(this->nthreads);
				
				// find top-most point
				#pragma omp for
				for(size_t i = 0; i < points.size(); i++)
				{
					if(points[i].y > points[topMostPoint].y)
					{
						topMostPoint = i;
					}
				}
				
				// split points up by sector
				#pragma omp for
				for(size_t i = 0; i < points.size(); i++)
				{
					double theta = atan2(points[topMostPoint].y - points[i].y, points[topMostPoint].x - points[i].x);
					size_t sector = theta / secSize;
					localSectors[sector].push_back(points[i]);
				}
				#pragma omp critical
				sectors.insert(sectors.end(), localSectors.begin(), localSectors.end());
				
				// local convex hull
				size_t id = omp_get_thread_num();
				// hulls[id] = U(this->nthreads).compute_hull(sectors[id]);
				
				// todo: compose the local hulls
			}
			
			std::vector<csce::point<T>> resultsOfShortestPath;
			/*
			int topMostPoint = 0;
			for(int i = 0; i < points.size(); i++){
				if(points[i].y > points[topMostPoint].y){
					topMostPoint = i;
				}
			}
			
			int tempPoint1 = topMostPoint;
			int tempPoint2 = 0;
			int orientationValue = 0;
			do{
				tempPoint2 = (tempPoint1 + 1) % points.size();
				for(int i = 0; i < points.size(); i++){
					orientationValue = operation(points[tempPoint1],points[i],points[tempPoint2]);
					if(orientationValue == 2){
						tempPoint2 = i;
					}
				}
				resultsOfShortestPath.push_back(points[tempPoint2]);
				tempPoint1 = tempPoint2;
			}
			while(tempPoint1 != topMostPoint);
			*/
			
			return resultsOfShortestPath;
		}
		
	private:
		int operation(csce::point<T> d, csce::point<T> e, csce::point<T> f) const
		{
			int resultsofSin = (e.y - d.y) * (f.x - e.x) - (e.x - d.x) * (f.y - e.y);
			if(resultsofSin == 0)
				return 0;
			else if(resultsofSin > 0)
				return 1;
			else
				return 2;
		}
	};
}

#endif 