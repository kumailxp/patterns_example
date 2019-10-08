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
    oss << "A circle of radius " << radius << " ";
    return oss.str();
  }
};

struct Square : Shape {
  float len;
  explicit Square(const float _len) : len{_len} {}
  void resize(float factor) { len *= factor;}

  std::string str() const override {
    std::ostringstream oss;
    oss << "A square of length and breath " << len << " ";
    return oss.str();
  }
};

// NOTE: The issue with this approach is that we can not 
// call member functions of shapes. So we need a new approach. 

// Create first decorator
struct ColouredShape : Shape {
  const Shape& shape;
  std::string colour;

  ColouredShape(const Shape& _shape, std::string _colour)
    : shape{_shape}, colour{_colour} {}

   std::string str() const override {
    std::ostringstream oss;
    oss << shape.str() << "has the colour " << colour << " ";
    return oss.str();
  }   
};

// Create second decorator
struct TransparentShape : Shape {
  const Shape& shape;
  int transperancy;

  TransparentShape(const Shape& _shape, int _transperancy)
    : shape{_shape}, transperancy{_transperancy} {}

   std::string str() const override {
    std::ostringstream oss;
    oss << shape.str() << "has transperancy of " 
    << transperancy << "% ";
    return oss.str();
  }   
};

int main() {
  Circle c1{10.0f};
  ColouredShape redCircle{c1, "red"};
  std::cout << redCircle.str() << std::endl;
  Square s1{20.0f};
  ColouredShape blueSquare{s1, "blue"};
  std::cout << blueSquare.str() << std::endl;

  // We can also mix up decorators to add
  // multiple properties
  TransparentShape lightgraySquare { 
    ColouredShape{Square{10},"gray"}, 
    50
  };
  std::cout << lightgraySquare.str() << std::endl;
  return 0;
}