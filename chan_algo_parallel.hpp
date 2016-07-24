#ifndef chan_algo_parallel_hpp
#define chan_algo_parallel_hpp

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "convex_hull_base.hpp"
#include "point.hpp"
#include "vector2d.hpp"

#define RIGHT_TURN -1  // CW
#define LEFT_TURN 1  // CCW
#define COLLINEAR 0  // Collinear


namespace csce {
	template<typename T>
	class chan_algo_parallel : public csce::convex_hull_base<T> {
	public:
		chan_algo_parallel(int _nthreads) : csce::convex_hull_base<T>(_nthreads) {}


		std::string name() const {
			return "Chan's Algorithm Parallel";
		}


		std::vector<csce::point<T>> compute_hull(std::vector<csce::point<T>>& points) const {
				 
		std::size_t m = (points.size() % this->nthreads == 0) ? points.size() / this->nthreads : ceil (double (points.size()) / double(this->nthreads));
				
			for(std::size_t t=0; t<points.size(); t++){
			
					std::vector<std::vector<csce::point<T>>> hulls;
							
					#pragma omp parallel for num_threads(this->nthreads) 
					for(std::size_t i=0; i<points.size(); i=i+m){
					std::vector<csce::point<T>> chunk;
				
					if(points.begin()+i+m <= points.end()){
					chunk.assign(points.begin()+i,points.begin()+i+m);
					}
					
					else {
					chunk.assign(points.begin()+i,points.end());  
					}
					
					hulls.push_back(this->graham_scan(chunk));
					}
					#pragma omp barrier
				
			std::vector<std::pair<int,int>> hull;
			
			hull.push_back(this->extreme_hullpt_pair(hulls));
			
				for(std::size_t i=0; i < m; ++i){
				std::pair<int,int> p = this->next_hullpt_pair(hulls,hull[hull.size()-1]);
				
				std::vector<csce::point<T>> output;
				
				if(p==hull[0]){
					
					for(std::size_t j=0; j<hull.size(); j++){
						output.push_back(hulls[hull[j].first][hull[j].second]);
					}
					
					return output;
				}
				
				hull.push_back(p);

				}
			
		}
		
		return points;
	}

		~chan_algo_parallel() {}
	
	private:
	
		std::pair<int,int> extreme_hullpt_pair(std::vector<std::vector<csce::point<T>>>& hulls) const {
			
		int h= 0, p= 0;
		
			for (std::size_t i = 0; i < hulls.size(); i++){
			int min_index = 0, min_y = hulls[i][0].y;
			
			for(std::size_t j = 1; j < hulls[i].size(); j++){
				
			if(hulls[i][j].y < min_y){
				min_y=hulls[i][j].y;
				min_index=j;
			}
		}
		
		if(hulls[i][min_index].y < hulls[h][p].y){
			h=i;
			p=min_index;
		}   
		}
		return std::make_pair(h,p);
		}
		
		std::pair<int,int> next_hullpt_pair(std::vector<std::vector<csce::point<T>>>& hulls, std::pair<int,int> lpoint) const {
		
		csce::point<T> &p = hulls[lpoint.first][lpoint.second];
		
		std::pair<int,int> next = std::make_pair(lpoint.first, (lpoint.second + 1) % hulls[lpoint.first].size());
		
		for (std::size_t h=0; h< hulls.size(); h++){
		
			if(h != lpoint.first){
				int s = tangent(hulls[h],p);
				csce::point<T> q= hulls[next.first][next.second];
				csce::point<T> r= hulls[h][s];
				int t= orientation(p,q,r);
			
			if( t== RIGHT_TURN || (t==COLLINEAR) && distance(p,r)> distance(p,q))
				next = std::make_pair(h,s);
		}
		}
		return next;
		}    
		
		int tangent(std::vector<csce::point<T>> v, csce::point<T> p) const {
			
		int L=0;
		int r= v.size();
		int L_before = orientation(p, v[0], v[v.size()-1]);
		int L_after = orientation(p, v[0], v[(L + 1) % v.size()]);
		
		while (L < r){
			int c = ((L + r)>>1);
			int c_before = orientation(p, v[c], v[(c - 1) % v.size()]);
			int c_after = orientation(p, v[c], v[(c + 1) % v.size()]);
			int c_side = orientation(p, v[L], v[c]);
		
		if (c_before != RIGHT_TURN and c_after != RIGHT_TURN)
			return c;
		
		else if ((c_side == LEFT_TURN) and (L_after == RIGHT_TURN or L_before == L_after) or (c_side == RIGHT_TURN and c_before == RIGHT_TURN))
			r = c;
		
		else
			L = c + 1 ;
			L_before = -c_after; 
			L_after = orientation(p, v[L], v[(L + 1) % v.size()]);
		}
		return L;
		}
		
		int distance(csce::point<T> p1, csce::point<T> p2) const {
			return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
		}
		
		int orientation(csce::point<T> p, csce::point<T> q, csce::point<T> r) const {
			int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
			if (val == 0) return 0;  // Collinear
			return (val > 0)? -1: 1; // CW: -1 or CCW: 1
		}
		
		//Alex's Graham Scan
		std::vector<csce::point<T>> graham_scan (std::vector<csce::point<T>>& points) const {
			if(points.size() <= 3){
				return points;
			}

			std::size_t index = this->pivot_index(points);
			if(index != 0){
				std::swap(points[0], points[index]);
			}
			
			//sort the points by angle around the pivot point (points[0])
			std::sort(points.begin() + 1, points.end(), [&points](csce::point<T>& a, csce::point<T>& b) {
				csce::vector2d<T> pa(points[0], a);
				csce::vector2d<T> pb(points[0], b);
				return pa.ccw(pb);
			});
			
			std::vector<csce::point<T>> s;
			s.reserve(points.size() + 1);
			
			s.push_back(points[points.size() - 1]);
			s.push_back(points[0]);
			s.push_back(points[1]);
			
			int x=2;
			while(x < points.size()){
				std::size_t y = s.size() - 1;
				csce::vector2d<T> a(s[y-1], s[y]);
				csce::vector2d<T> b(s[y-1], points[x]);
				if(a.ccw(b)){
					s.push_back(points[x++]);
				} else {
					s.pop_back();
				}
			}
			
			s.shrink_to_fit();
			return s;
		}
		
		std::size_t pivot_index(std::vector<csce::point<T>>& points) const {
			std::size_t index = 0;
			for(std::size_t x=0; x<points.size(); x++){
				csce::point<T>& p0 = points[index];
				csce::point<T>& cur = points[x];
				if(csce::math_utility::less_than(cur.y, p0.y)){
					index = x;
				} else if(csce::math_utility::equals(cur.y, p0.y)){
					if(csce::math_utility::less_than(cur.x, p0.x)){
						index = x;
					}
				}
			}
			return index;
		}

	};
}


#endif /* chan_algo_parallel_hpp */
