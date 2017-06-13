/*
The first line indicates the number of vertices and edges, respectively.
Each subsequent line describes an edge (the first two numbers are its tail
and head, respectively) and its length (the third number). NOTE: some of the
edge lengths are negative. NOTE: These graphs may or may not have
negative-cost cycles.

Your task is to compute the "shortest shortest path". Precisely, you must
first identify which, if any, of the three graphs have no negative cycles.
For each such graph, you should compute all-pairs shortest paths and
remember the smallest one (i.e., compute minu,v∈Vd(u,v), where d(u,v)
denotes the shortest-path distance from u to v).

If each of the three graphs has a negative-cost cycle, then enter "NULL" in
the box below. If exactly one graph has no negative-cost cycles, then enter
the length of its shortest shortest path in the box below. If two or more
of the graphs have no negative-cost cycles, then enter the smallest of the
lengths of their shortest shortest paths in the box below.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>

using std::queue;
using std::priority_queue;
using std::vector;
using std::ifstream;
using std::numeric_limits;

class compareDist {
  const vector<int>& _dist;
public:
  compareDist(const vector<int>& dist): _dist(dist) {
  }

  bool operator()(int u, int v) {
    return _dist[u] < _dist[v];
  }
};

int negative_cycle(const vector<vector<int>> &adj, const vector<vector<int>> &cost) {
  // write your code here
  int n = adj.size();
  vector<int> dist(n, numeric_limits<int>::max());
  dist[0] = 0;

  for (int u = 0; u < n; ++u) {
    for (int j = 0; j < adj[u].size(); ++j) {
      int v = adj[u][j], alt = dist[u] + cost[u][j];
      if (dist[u] != numeric_limits<int>::max() && alt < dist[v]) {
        // std::cout << "relax" << std::endl;
        dist[v] = alt;
      }
    }
  }

  for (int u = 0; u < n; ++u) {
    for (int j = 0; j < adj[u].size(); ++j) {
      int v = adj[u][j], alt = dist[u] + cost[u][j];
      if (dist[u] != numeric_limits<int>::max() && alt < dist[v]) {
        // std::cout << alt << ", " << dist[v] << std::endl;
        // std::cout << u << ", " << v << std::endl;
        return 1;
      }
    }
  }

  return 0;
}

int distance(const vector<vector<int>> &adj, const vector<vector<int>> &cost, int s, int t) {
  int n = adj.size();
  vector<int> dist(n, numeric_limits<int>::max()); //, prev(n, -1); We don't need the path.
  priority_queue<int, vector<int>, compareDist> unvisited{compareDist(dist)};

  unvisited.push(s);
  dist[s] = 0;

  while (!unvisited.empty()) {
    int u = unvisited.top();
    unvisited.pop();

    for (int j = 0; j < adj[u].size(); ++j) {
      int v = adj[u][j];
      int alt = dist[u] + cost[u][j];
      if (alt < dist[v]) {
        dist[v] = alt;
        unvisited.push(v);
      }
    }
  }
  // std::cout << dist[t] << std::endl;
  return dist[t] == numeric_limits<int>::max() ? -1 : dist[t];
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    ifstream fs;
    fs.exceptions(ifstream::failbit | ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main
      int n, m;
      fs >> n >> m;
      vector<vector<int> > adj(n, vector<int>());
      vector<vector<int> > cost(n, vector<int>());
      for (int i = 0; i < m; i++) {
        int x, y, w;
        fs >> x >> y >> w;
        adj[x - 1].push_back(y - 1);
        cost[x - 1].push_back(w);
      }
      if (negative_cycle(adj, cost)) {
        std::cout << "NULL" << std::endl;
      } else {
        int min = numeric_limits<int>::max();
        for (int u = 0; u < n; ++u) {
          for (int v = 0; v < n; ++v) {
            if (u != v) {

              min = std::min(distance(adj, cost, u, v), min);
            }
          }
        }
        std::cout << min << std::endl;
      }
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
    fs.close();
  }
  return 0;
}
