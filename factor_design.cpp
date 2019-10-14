#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

struct Point {
protected:
  Point(const float x, const float y)
    : x_{x}, y_{} {}
private:
  float x_;
  float y_;
public:
  static Point NewCartesian(float x, float y)
  {
    return {x,y};
  }
  
  static Point NewPolar(float r, float theata)
  {
    return {r * sin(theata), r * cos(theata)};
  }
};