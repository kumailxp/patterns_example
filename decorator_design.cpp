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

// Now we implement decorate pattern through 
// templates

template<typename T>
struct ColoredShape_t : T {
  std::string colour;

  template<typename ...Args>
  ColoredShape_t(const std::string _colour, Args ...args)
    : T(std::forward<Args>(args)...), colour(_colour)
    {}

  std::string str() const override {
    std::ostringstream oss;
    oss  << T::str() << "has the colour " << colour << " ";
    return oss.str();
  } 

};


template<typename T>
struct TransparentShape_t : T {
  int transperancy;

  template<typename ...Args>
  TransparentShape_t(const int _transperancy, Args ...args)
    : T(std::forward<Args>(args)...), transperancy(_transperancy)
    {}

   std::string str() const override {
    std::ostringstream oss;
    oss  << T::str() << "has transperancy of " << transperancy << "% ";
    return oss.str();
  } 

};

template<typename T>
struct BrightnessShape_t : T {
  int brightness;

  template<typename ...Args>
  BrightnessShape_t(const int _brightness, Args ...args)
    : T(std::forward<Args>(args)...), brightness(_brightness)
    {}

   std::string str() const override {
    std::ostringstream oss;
    oss  << T::str() << "has brightness of " << brightness << "% ";
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
  // The problem with the above approach is that we
  // cannot change the propertiy of square object.

  // Implementation using templates
  BrightnessShape_t<ColoredShape_t<TransparentShape_t<Square>>> sq1 = {20,"red", 51,5};
  std::cout << sq1.str() << std::endl;
  sq1.resize(100);
  std::cout << sq1.str() << std::endl;

  return 0;
}