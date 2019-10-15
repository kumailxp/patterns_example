#include<iostream>

template <bool X>
struct notT {
  static constexpr bool value {!X};
};

template <typename T, typename U>
struct is_same_type {
  static constexpr bool value {false};
};

template <typename T>
struct is_same_type<T,T> {
  static constexpr bool value {true};
};

struct AA {

};

int main() {
  notT<true> A;
  std::cout << is_same_type<AA,AA>::value << std::endl;
  return 0;
}