#include <iostream>
#include <string>
#include "disjoint_sets.hpp"

int main()
{
  DisjointSets<std::string> dsets;

  std::cout << "Debug1\n";
  dsets.add("Hello");
  std::cout << "Debug2\n";
  dsets.add("Hi");
  std::cout << "Debug3\n";
  dsets.add("Hola");
  std::cout << "Debug4\n";

  dsets.merge("Hello", "Hi");
  std::cout << "Debug5\n";

  auto *p1 = dsets.find("Hello");
  std::cout << "Debug6\n";
  auto *p2 = dsets.find("Hi");

  if (p1 == p2) {
    std::cout << "Same\n";
  } else {
    std::cout << "Different\n";
  }
}

