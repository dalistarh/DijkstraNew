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
#include <cstring>

#define INF numeric_limits<size_t>::max()
#define MAXN 100000

int seed;
const size_t n = 4039; // number of nodes
const size_t m = 88234; //number of edges
size_t source; 
double p;

struct edge_t {
    size_t target;
    size_t weight;
};

struct vertex_t {
    edge_t *edges;
    size_t num_edges;
    size_t distance;
};

struct task_t {
    vertex_t *v;
    const size_t distance;

	task_t(vertex_t *v,
           const size_t distance) : v(v), distance(distance)
    {

    }
};


template <class K, class V>
class PQueue
{
private:
    class entry_t
    {
    public:
        K key;
        V value;

        bool operator<(const entry_t &that) const
        {
            return this->key < that.key;
        }

        bool operator<=(const entry_t &that) const
        {
            return this->key <= that.key;
        }

        bool operator>=(const entry_t &that) const
        {
            return !operator<(that);
        }
    };

public:
    PQueue();
    virtual ~PQueue();

    void insert(const K &key, const V &value);
    bool delete_min(V &value);
    void clear();

  
private:
    void grow();
    void bubble_up(const size_t ix);
    void bubble_down(const size_t ix);

private:
    constexpr static size_t INITIAL_CAPACITY = 64;

    entry_t *m_array;
    size_t m_length;
    size_t m_capacity;
};

template <class K, class V>
PQueue<K, V>::PQueue() :
    m_array(new entry_t[INITIAL_CAPACITY]),
    m_length(0),
    m_capacity(INITIAL_CAPACITY)
{
}

template <class K, class V>
PQueue<K, V>::~PQueue()
{
    delete[] m_array;
}

template <class K, class V>
bool
PQueue<K, V>::delete_min(V &value)
{

    if (m_length == 0) {
        return false;
    }

    value = m_array[0].value;
    m_array[0] = m_array[m_length - 1];
    bubble_down(0);
    m_length--;

    return true;
}

template <class K, class V>
void
PQueue<K, V>::bubble_down(const size_t ix)
{
    size_t i = ix;
    size_t j = (i << 1) + 1;
    while (j < m_length) {
        const size_t k = j + 1;
        if (k < m_length && m_array[k] < m_array[j]) {
            j = k;
        }
        if (m_array[i] <= m_array[j]) {
            break;
        }
        std::swap(m_array[j], m_array[i]);
        i = j;
        j = (i << 1) + 1;
    }
}

template <class K, class V>
void
PQueue<K, V>::insert(const K &key,
                         const V &value)
{

    if (m_length == m_capacity) {
        grow();
    }

    m_array[m_length] = { key, value };
    bubble_up(m_length);
    m_length++;
}

template <class K, class V>
void
PQueue<K, V>::grow()
{
    const size_t new_capacity = m_capacity << 1;
    entry_t *new_array = new entry_t[new_capacity];

    memcpy(new_array, m_array, sizeof(m_array[0]) * m_capacity);
    delete[] m_array;

    m_array = new_array;
    m_capacity = new_capacity;
}

template <class K, class V>
void
PQueue<K, V>::bubble_up(const size_t ix)
{
    size_t i = ix;
    size_t j = (i - 1) >> 1;
    while (i > 0 && m_array[i] < m_array[j]) {
        std::swap(m_array[j], m_array[i]);
        i = j;
        j = (i - 1) >> 1;
    }
}

template <class K, class V>
void
PQueue<K, V>::clear()
{
    m_length = 0;
}



PQueue <size_t, task_t *> Q;
vertex_t *graph;



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




static vertex_t *
generate_graph(const size_t n,
               const int seed,
               const double p)
{
    vertex_t *data = new vertex_t[n];

    std::mt19937 rng;
    rng.seed(seed);
    std::uniform_real_distribution<float> rnd_f(0.0, 1.0);
    std::uniform_int_distribution<size_t> rnd_st(1, std::numeric_limits<int>::max());

    std::vector<edge_t> *edges = new std::vector<edge_t>[n];
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (rnd_f(rng) < p) {
                edge_t e;
                e.target = j;
                e.weight = rnd_st(rng);
                edges[i].push_back(e);
                e.target = i;
                edges[j].push_back(e);
            }
        }
        data[i].num_edges = edges[i].size();
        if (edges[i].size() > 0) {
            data[i].edges = new edge_t[edges[i].size()];
            for (size_t j = 0; j < edges[i].size(); ++j) {
                data[i].edges[j] = edges[i][j];
            }
        } else {
            data[i].edges = NULL;
        }
        data[i].distance = std::numeric_limits<size_t>::max();
    }

    data[0].distance = 0;
    delete[] edges;

    return data;
}

static vertex_t *
generate_graph(const size_t n,
               const int seed
               )
{
    vertex_t *data = new vertex_t[n];

    std::mt19937 rng;
    rng.seed(seed);
    std::uniform_int_distribution<size_t> rnd_st(1, std::numeric_limits<int>::max());

     
    std::ifstream inf;
	inf.open("facebook_combined.txt");
	
   
    std::vector<edge_t> *edges = new std::vector<edge_t>[n];
    for (size_t i = 0; i < m; i++) {
    	edge_t e;
        size_t u, v;
        inf >> u >> v; 
        e.target = u;
        e.weight = rnd_st(rng);
        edges[v].push_back(e);
        e.target = v;
        edges[u].push_back(e);
    }
    
    inf.close();
    for (size_t i = 0; i < n; i++) {    
        data[i].num_edges = edges[i].size();
        if (edges[i].size() > 0) {
            data[i].edges = new edge_t[edges[i].size()];
            for (size_t j = 0; j < edges[i].size(); ++j) {
                data[i].edges[j] = edges[i][j];
            }
        } else {
            data[i].edges = NULL;
        }
        data[i].distance = std::numeric_limits<size_t>::max();
    }

    data[0].distance = 0;
    delete[] edges;

    return data;
}

void FindDist()
{
   
   while (1) {
        
        task_t *task;
        if (!Q.delete_min(task)) break;

        const vertex_t *v = task->v;
        const size_t v_dist = v->distance;

        if (task->distance > v_dist) {
            delete task;
            continue;
        }

        for (size_t i = 0; i < v->num_edges; i++) {
            const edge_t *e = &v->edges[i];
            const size_t new_dist = v_dist + e->weight;

            vertex_t *w = &graph[e->target];
            size_t w_dist = w->distance;

            if (new_dist >= w_dist) {
                continue;
            }

           	w->distance = new_dist;
            Q.insert(new_dist, new task_t(w, new_dist));
        		
		}
		delete task;
	}
}



int main()
{
	std::cin >> seed >> p;
   
   	source = 0;
    
    if (p <= 0) graph = generate_graph(n, seed);
    else graph = generate_graph(n, seed, p);

    struct timespec start, end;
    
    Q.clear();
    
    clock_gettime(CLOCK_MONOTONIC, &start);

    Q.insert(0, new task_t(&graph[source], 0));
  	
  	FindDist();

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    const double elapsed = timediff_in_s(start, end);
    
    fprintf(stdout, "%f\n", elapsed);
  	
  	return 0;
}
