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

using std::vector;
using std::ifstream;
using std::numeric_limits;

int negative_cycle(const vector<vector<int>> &adj, const vector<vector<int>> &cost) {
  // write your code here
  int n = adj.size();
  vector<int> dist(n, numeric_limits<int>::max());
  dist[0] = 0;

  for (int u = 1; u < n; ++u) {
    for (int j = 0; j < adj[u].size(); ++j) {
      int v = adj[u][j], alt = dist[u] + cost[u][j];
      if (alt < dist[v]) {
        dist[v] = alt;
      }
    }
  }

  for (int u = 0; u < n; ++u) {
    for (int j = 0; j < adj[u].size(); ++j) {
      int v = adj[u][j];
      if (dist[u] + cost[u][j] < dist[v]) {
        std::cout << u << ", " << v << std::endl;
        return 1;
      }
    }
  }

  return 0;
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
      std::cout << negative_cycle(adj, cost) << std::endl;
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
    fs.close();
  }
  return 0;
}
