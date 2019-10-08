#include<iostream>
#include<string>
#include<sstream>

struct Shape {
  virtual std::string str() const = 0;
};

struct Circle : Shape {
  float radius;
  explicit Circle(const float _radius) : radius{_radius} {}
  void resize(float factor) { radius *= factor;}

  std::string str() const override {
    std::ostringstream oss;
    oss << "A circle of radius " << radius;
    return oss.str();
  }
};

struct Square : Shape {
  float len;
  explicit Square(const float _len) : len{_len} {}
  void resize(float factor) { len *= factor;}

  std::string str() const override {
    std::ostringstream oss;
    oss << "A square of length and breath " << len;
    return oss.str();
  }
};

int main() {

  std::cout << "Hello" << std::endl;
  return 0;
}