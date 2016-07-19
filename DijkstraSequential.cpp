#include <stdio.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <utility>
#include <pthread.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>

using namespace std;

#define INF numeric_limits<size_t>::max()
#define MAXN 100000

int seed;
const size_t n = 4039; // number of nodes
const size_t m = 88234; //number of edges
size_t source; 
double p;

typedef pair<size_t, size_t> stst;


struct node
{
	size_t dist;
	vector<stst> adj;
};

node a[MAXN];

priority_queue<stst, vector<stst>, greater<stst> > Q;


double
timediff_in_s(const struct timespec &start,
              const struct timespec &end)
{
    struct timespec tmp;
    if (end.tv_nsec < start.tv_nsec) {
        tmp.tv_sec = end.tv_sec - start.tv_sec - 1;
        tmp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec - start.tv_sec;
        tmp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return tmp.tv_sec + (double)tmp.tv_nsec / 1000000000.0;
}


void FindDist()
{

	while (!Q.empty())
	{
		
		
		stst top = Q.top();
		Q.pop();
       	size_t u = top.second, d = top.first;
        
        if (d <= a[u].dist)
		{
				
		    for (size_t i = 0; i < a[u].adj.size(); i++)
		    {
				size_t v = a[u].adj[i].first;
				size_t w = a[u].adj[i].second;

				if (a[v].dist > a[u].dist + w)
				{

					a[v].dist = a[u].dist + w;
				
					Q.push(make_pair(a[v].dist, v)); 
				}
			}	
		} 
    }	
} 


void generate_graph(const size_t p)
{
	mt19937 rng;
    rng.seed(seed);
    uniform_real_distribution<float> rnd_f(0.0, 1.0);
    uniform_int_distribution<size_t> rnd_st(1, numeric_limits<int>::max());

    for (size_t i = 0; i < n; ++i) 
        for (size_t j = i + 1; j < n; ++j) 
            if (rnd_f(rng) < p) {
                size_t w = rnd_st(rng);
                a[i].adj.push_back(make_pair(j, w));
                a[j].adj.push_back(make_pair(i, w));
            }
}

void generate_graph()
{
	ifstream inf;
	inf.open("facebook_combined.txt");
    	
	mt19937 rng;
    rng.seed(seed);
    uniform_int_distribution<size_t> rnd_st(1, numeric_limits<int>::max());

	for (size_t i = 0; i < m; i++)
   	{
    	
    	size_t u, v, w = rnd_st(rng); // get weight randomly
    	inf >> u >> v;
    	a[u].adj.push_back(make_pair(v, w));
    	a[v].adj.push_back(make_pair(u, w));
  	}

  	inf.close();
}



int main()
{
	cin >> seed >> p;
    
    //set source to 0
	source = 0;
	
	for (size_t i = 0; i < n; i++) { a[i].dist = INF; a[i].adj.clear(); }
    a[source].dist = 0;
    	
    if (p <= 0) generate_graph(); else generate_graph(p);
    
  	struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

  	Q.push(make_pair(0, source));
  	
  	FindDist();

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    const double elapsed = timediff_in_s(start, end);
    fprintf(stdout, "%f\n", elapsed);
  	
  	return 0;
}
