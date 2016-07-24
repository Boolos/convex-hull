#ifndef composable_hulls_hpp
#define composable_hulls_hpp

#include <vector>
#include <string>
#include <utility>
#include <omp.h>
#include <math.h>

#include "convex_hull_base.hpp"
#include "graham_scan_parallel.hpp"
#include "jarvis_march_parallel.hpp"
#include "point.hpp"
#include "vector2d.hpp"

namespace csce
{
	template<typename T, typename U = graham_scan_parallel<T>, typename V = jarvis_march_parallel<T>>
	class composable_hulls : public csce::convex_hull_base<T>
	{
	public:
		composable_hulls(int _nthreads) : convex_hull_base<T>(_nthreads) {}
		~composable_hulls() {}
		
		std::string name() const
		{
			return std::string("Composable Hulls <").append(U(0).name()).append(", ").append(V(0).name()).append(">");
		}
		
		virtual std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>> &points)
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
				for(size_t i = 0; i < this->nthreads; i++)
					sectors[i].insert(sectors[i].end(), localSectors[i].begin(), localSectors[i].end());
				
				#pragma omp barrier
				// local convex hull
				size_t id = omp_get_thread_num();
				hulls[id] = U(1).compute_hull(sectors[id]);
			}
			
			// composition of local hulls
			std::vector<csce::point<T>> resultsOfShortestPath = hulls[0];
			V combiner(this->nthreads);
			for(size_t i = 1; i < this->nthreads; i++)
			{
				resultsOfShortestPath.insert(resultsOfShortestPath.end(), hulls[i].begin(), hulls[i].end());
				resultsOfShortestPath = combiner.compute_hull(resultsOfShortestPath);
			}
			
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
