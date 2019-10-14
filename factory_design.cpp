#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <functional>

struct Point;

enum PrinterType {
  html,
  markdown
};

struct Point {
  float x, y;
  friend class PointFactory;

  std::string str() {
    return std::string("x= ") + std::to_string(x)
          + ", y=" + std::to_string(y) + "\n";
  }

private:
  Point(float x, float y) : x(x), y(y) {}
};

struct PointFactory {
  static Point NewCartesian(float x, float y) 
    { return Point{x, y}; }

  static Point NewPolar(float r, float theata) {
    return Point {r * sin(theata), r * cos(theata)};
  }
};


struct PrintingStrategy {
  virtual void output1(std::string str)= 0;
};

struct HtmlPrinting : PrintingStrategy {
  void output1(std::string str) override;
};

void HtmlPrinting::output1(std::string str) {
  std::cout << "html output: " << str;
}


struct MarkdownPrinting : PrintingStrategy {
  void output1(std::string str) override {
    std::cout << "markdown output: " << str;
  }
};

struct PointProcessor : std::vector<Point> {
private:
  PrintingStrategy* ps_;
public:
  PointProcessor(const std::initializer_list<value_type>& points,
    PrintingStrategy* ps) :
    std::vector<Point>(points),
    ps_(ps) {}
  void print() {
    for (Point &I : *this)
      ps_->output1(I.str());
  }

  ~PointProcessor() {
    delete ps_;
  }
};

int main() {
  auto pp = PointProcessor {{ 
        PointFactory::NewPolar(10,3.142),
        PointFactory::NewCartesian(34,52),
        PointFactory::NewCartesian(10,3100)
   },
    new HtmlPrinting()
   };
  pp.print();
}