#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

struct Point {
  float x, y;
  friend class PointFactory;

private:
  Point(float x, float y) : x(x), y(y) {}
};

struct PointFactory {
  static Point NewCartesian(float x, float y) 
    { return Point{x, y}; }

  static Point NewPolar(float r, float theata) {
    return Point{r * sin(theata), r * cos(theata)};
  }
};

int main() {
  auto myPoint = PointFactory::NewCartesian(3,4);
}