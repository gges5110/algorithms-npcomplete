/*
In this assignment you will implement one or more algorithms for the traveling salesman problem,
such as the dynamic programming algorithm covered in the video lectures. Here is a data file
describing a TSP instance.

tsp.txt

The first line indicates the number of cities. Each city is a point in the plane, and each
subsequent line indicates the x- and y-coordinates of a single city.

The distance between two cities is defined as the Euclidean distance --- that is, two cities at
locations (x,y) and (z,w) have distance ((x−z)^2+(y−w)^2)^(1/2) between them.

In the box below, type in the minimum cost of a traveling salesman tour for this instance,
rounded down to the nearest integer.

OPTIONAL: If you want bigger data sets to play with, check out the TSP instances from around
the world here. The smallest data set (Western Sahara) has 29 cities, and most of the data sets
are much bigger than that. What's the largest of these data sets that you're able to solve ---
using dynamic programming or, if you like, a completely different method?

HINT: You might experiment with ways to reduce the data set size. For example, trying plotting the
points. Can you infer any structure of the optimal solution? Can you use that structure to speed
up your algorithm?
*/

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <limits>

#include <string>
#include <cmath> /*sqrt, pow, floor*/
#include <algorithm> /* std::next_permutation */
#include <unordered_map>

using std::unordered_map;
using std::sqrt;
using std::pow;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::next_permutation;
using std::vector;
using std::numeric_limits;

template <typename T>
class Coordinate {
public:
  T x, y;
  Coordinate(T _x, T _y): x(_x), y(_y) {}
  Coordinate() {}
};

double Eucledian_distance(Coordinate<double> p_1, Coordinate<double> p_2) {
  return sqrt(pow(p_1.x - p_2.x, 2) + pow(p_1.y - p_2.y, 2));
}

bool contains(int set, int i) {
  return (1 << i) & set;
}

int erase(int set, int i) {
  int ans = ~0 & (1 << i);
  return (~ans) & set;
}

class Set_vertex {
public:
  int vertex, set;
  Set_vertex(int vertex, int set): vertex(vertex), set(set) {}
  Set_vertex() {}

  bool operator==(const Set_vertex& that) const {
    return (vertex == that.vertex && set == that.set);
  }

  void print() {
    cout << "Print Set_vertex info: " << "vertex = " << vertex << ", set = " << set << endl;
  }
};

class KeyHasher {
public:
  std::size_t operator()(const Set_vertex& key) const {
    using std::size_t;
    using std::hash;

    // TODO: need to come up with a better hash function.
    int sum = 0;
    for (int i = 0; i < 32; ++i) {
      if (contains(key.set, i)) {
        sum = sum | (1 << i);
      }
    }
    sum |= (1 << (key.vertex));
    return sum;
    // return (31 * key.vertex + sum);
  }
};


class Cost_parent {
public:
  double cost;
  int parent;
  Cost_parent(double cost, int parent): cost(cost), parent(parent) {}
  Cost_parent() {}
  void print() {
    cout << "Print Cost_parent info: cost = " << cost << ", parent = " << parent << endl;
  }
};

class TSP {
private:
  int node_size;
  vector<Coordinate<double>> _nodes;
  vector<vector<double>> distance_matrix;

  void calculate_distances() {
    distance_matrix.resize(_nodes.size());
    for (int i = 0; i < node_size; ++i) {
      distance_matrix[i].resize(_nodes.size());
    }

    for (int i = 0; i < node_size; ++i) {
      distance_matrix[i][i] = 0;
      for (int j = i + 1; j < node_size; ++j) {
        double d = Eucledian_distance(_nodes[i], _nodes[j]);
        distance_matrix[i][j] = d;
        distance_matrix[j][i] = d;
      }
    }
  }

  vector<int> combinations(int k) {
    vector<int> combinations, answer;
    permutation(1, k, combinations, answer);
    return answer;
  }

  void permutation(int offset, int k, vector<int> combinations, vector<int> &answer) {
    if (k == 0) {
      int temp = 0;
      for (int i: combinations) {
        temp += 1 << i;
      }
      answer.push_back(temp);
      return;
    }
    for (int i = offset; i < node_size; ++i) {
       combinations.push_back(i);
       permutation(i + 1, k - 1, combinations, answer);
       combinations.pop_back();
    }
  }

public:
  TSP(const vector<Coordinate<double>>& nodes): _nodes(nodes) {
    node_size = _nodes.size();
    calculate_distances();
  }

  void minCost() {
    unordered_map<Set_vertex, Cost_parent, KeyHasher> map;

    for (int vertex = 1; vertex < node_size; ++vertex) {
      Set_vertex setVertex(vertex, 0);
      Cost_parent costParent(distance_matrix[vertex][0], 0);
      map.insert({setVertex, costParent});
    }

    unordered_map<Set_vertex, Cost_parent, KeyHasher> new_map;
    for (int set_size = 1; set_size < node_size - 1; ++set_size) {
      cout << "set_size = " << set_size << endl;
      vector<int> S = combinations(set_size);

      for (int set: S) {
        // cout << "set_size " << set_size << ", " << std::bitset<32>(set) << endl;
        for (int k = 1; k < node_size; ++k) {
          if (!contains(set, k)) {
            // cout << "set_size = " << set_size << endl;
            Cost_parent min_costParent = getMinCost(k, set, set_size, map);
            Set_vertex min_setVertex(k, set);
            new_map.insert({min_setVertex, min_costParent});
          }
        }
      }
      map = new_map;
      new_map.clear();
    }

    // [0, {1, 2, 3}]
    int set = ((~0) << 1) & (~((~0) << node_size));
    Cost_parent min_costParent = getMinCost(0, set, node_size, map);
    cout << std::floor(min_costParent.cost) << endl;
  }

  Cost_parent getMinCost(int k, int set, int set_size, unordered_map<Set_vertex, Cost_parent, KeyHasher> &map) {
    // Set_vertex min_setVertex;
    double min = numeric_limits<double>::max();
    int parent = -1;
    // cout << "Calculating k = " << k << " with set = " << std::bitset<32>(set) << endl;
    for (int m = 1; m < node_size; ++m) {
      if (contains(set, m)) {
        Cost_parent temp_costParent = map[Set_vertex(m, erase(set, m))];
        double cost = temp_costParent.cost + distance_matrix[k][m];
        if (cost < min) {
          min = cost;
          parent = m;
        }
      }
    }
    return Cost_parent(min, parent);
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    ifstream fs;
    fs.exceptions(ifstream::failbit | ifstream::badbit);
    try {
      fs.open(argv[1]);
      int node_size;
      fs >> node_size;
      vector<Coordinate<double>> nodes(node_size, Coordinate<double>());
      double x, y;

      for (int line = 0; line < node_size; ++line) {
        fs >> x >> y;
        Coordinate<double> temp(x, y);
        nodes[line] = temp;
      }

      TSP tsp(nodes);
      tsp.minCost();

      fs.close();
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
  }
  return 0;
}
