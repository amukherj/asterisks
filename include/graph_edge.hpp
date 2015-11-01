#ifndef GRAPH_EDGE_HPP
#define GRAPH_EDGE_HPP

#include <algorithm>
#include <iostream>
#include <utility>

#include "graph_node.hpp"
#include "utility/make_hash.hpp"

template <typename T>
class graph_edge
{
public:
  graph_edge(const graph_node<T>& end1, const graph_node<T>& end2)
    : end1(&end1), end2(&end2), weight(1.0)
  {}

  graph_edge(const graph_node<T>& end1, const graph_node<T>& end2,
             double weight)
    : end1(&end1), end2(&end2), weight(weight)
  {}

  /* graph_edge(const graph_edge& that)
    : end1(that.end1), end2(that.end2), weight(that.weight)
  {
    std::cout << "graph_edge copied: " << end1->get_label()
              << "<->" << end2->get_label() << '\n';
  }

  graph_edge& operator=(const graph_edge& that) {
    graph_edge junk(*this);
    graph_edge tmp(that);
    swap(tmp);

    std::cout << "graph_edge copied: " << end1->get_label()
              << "<->" << end2->get_label() << '\n';

    return *this;
  }

  void swap(graph_edge& that) {
    using std::swap;

    swap(end1, that.end1);
    swap(end2, that.end2);
    swap(weight, that.weight);
  } */

  double get_weight() const
  {
    return weight;
  }

  std::pair<const graph_node<T>*, const graph_node<T>*>
    get_vertices() const
  {
    return std::make_pair(end1, end2);
  }

private:
  const graph_node<T> *end1, *end2;
  double weight;
};

template <typename T>
bool operator==(const graph_edge<T>& left,
                const graph_edge<T>& right)
{
  if (&left == &right) {
    return true;
  }

  auto pair1 = left.get_vertices();
  auto pair2 = right.get_vertices();

  if (*pair1.first == *pair2.first
      && *pair1.second == *pair2.second) {
    return true;
  }

  if (*pair1.first == *pair2.second
      && *pair1.second == *pair2.first) {
    return true;
  }

  return false;
}

template <typename T>
bool operator<(const graph_edge<T>& left,
               const graph_edge<T>& right)
{
  return left.get_weight() < right.get_weight();
}

namespace std
{
  template <typename T>
  struct hash<graph_edge<T>>
  {
    std::size_t operator()(const graph_edge<T>& edge) const
    {
      auto pair = edge.get_vertices();
      auto result = make_symmetric_hash(*pair.first, *pair.second);

      return result;
    }
  };
}

#endif /* GRAPH_EDGE_HPP */
