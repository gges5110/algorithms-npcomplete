/*
In this assignment we will revisit an old friend, the traveling salesman problem (TSP). This week you will implement a
heuristic for the TSP, rather than an exact algorithm, and as a result will be able to handle much larger problem sizes.
Here is a data file describing a TSP instance (original source: http://www.math.uwaterloo.ca/tsp/world/bm33708.tsp).

The first line indicates the number of cities. Each city is a point in the plane, and each subsequent line indicates the
x- and y-coordinates of a single city.

The distance between two cities is defined as the Euclidean distance --- that is, two cities at locations (x,y) and (z,w)
have distance ((x−z)^2+(y−w)^2)^(1/2) between them.

You should implement the nearest neighbor heuristic:

1. Start the tour at the first city.
2. Repeatedly visit the closest city that the tour hasn't visited yet. In case of a tie, go to the closest city with the
   lowest index. For example, if both the third and fifth cities have the same distance from the first city (and are closer
   than any other city), then the tour should begin by going from the first city to the third city.
3. Once every city has been visited exactly once, return to the first city to complete the tour.

In the box below, enter the cost of the traveling salesman tour computed by the nearest neighbor heuristic for this instance,
rounded down to the nearest integer.
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <cmath> /*sqrt, pow, floor*/
#include <set>

using std::set;
using std::sqrt;
using std::pow;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
using std::numeric_limits;

template <typename T>
class City {
public:
  T x, y;
  int vertex;
  City(T _x, T _y, int _vertex): x(_x), y(_y), vertex(_vertex) {}
  City() {}

  bool operator < (const City &that) {
    return this->x < that.x;
  }
};

double Eucledian_distance(City<double> p_1, City<double> p_2) {
  return sqrt(pow(p_1.x - p_2.x, 2) + pow(p_1.y - p_2.y, 2));
}

class TSP {
private:
  vector<City<double>> _cities;
  int _node_size;

public:
  TSP(const vector<City<double>>& nodes): _cities(nodes), _node_size(_cities.size()) {}

  double minCost() {
    set<int> visited;
    // 1. Start the tour at the first city.
    int current_city, starting_city = 0;
    double sum = 0;

    // Optimize
    std::sort(_cities.begin(), _cities.end());
    for (int i = 0; i < _node_size; ++i) {
      if (_cities[i].vertex == 1) {
        starting_city = i;
        break;
      }
    }

    visited.insert(starting_city);
    current_city = starting_city;

    while (visited.size() < _node_size) {
      // 2. Repeatedly visit the closest city that the tour hasn't visited yet.
      double min = numeric_limits<double>::max(), next_city = -1;

      for (int u = 0; u < _node_size; ++u) {
        if (min < _cities[u].x - _cities[current_city].x) {
          break;
        } else {
          double alt = Eucledian_distance(_cities[u], _cities[current_city]);
          if ((visited.count(u) == 0 && alt <= min)) {
            min = alt;
            next_city = u;
          }
        }
      }

      visited.insert(next_city);
      current_city = next_city;
      sum += min;
    }

    // 3. Once every city has been visited exactly once, return to the first city to complete the tour.
    return std::floor(sum + Eucledian_distance(_cities[starting_city], _cities[current_city]));
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
      vector<City<double>> nodes(node_size, City<double>());
      int z;
      double x, y;

      for (int line = 0; line < node_size; ++line) {
        fs >> z >> x >> y;
        City<double> temp(x, y, z);
        nodes[line] = temp;
      }

      TSP tsp(nodes);
      cout << tsp.minCost() << endl;;

      fs.close();
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
  }
  return 0;
}
