#include "graph_node.hpp"
#include "graph_edge.hpp"
#include <iostream>
#include <string>

int main()
{
  graph_node<std::string> n1("A"), n2("B"), n3("C");
  graph_edge<std::string> e1(n1, n2, 3.0), e2(n1, n3, 1.47), e3(n2, n1, 3.0);

  std::cout << std::hash<graph_edge<std::string>>()(e1) << '\n';
  std::cout << std::hash<graph_edge<std::string>>()(e3) << '\n';
  std::cout << std::hash<graph_edge<std::string>>()(e2) << '\n';

  if (e1 == e3) {
    std::cout << "e1 == e3\n";
  }
}
