#ifndef GRAPH_NODE_HPP
#define GRAPH_NODE_HPP

#include "utility/make_hash.hpp"

template <typename T>
class graph_node
{
public:
  graph_node(T label) : label(label), weight(1.0)
  {}

  graph_node(T label, double weight) : label(label), weight(weight)
  {}

  const T& get_label() const
  { return label; }

private:
  T label;
  mutable double weight;
};

template <typename T>
bool operator==(const graph_node<T>& left, 
                const graph_node<T>& right)
{
  return left.get_label() == right.get_label();
}

template <typename T>
bool operator!=(const graph_node<T>& left, 
                const graph_node<T>& right)
{
  return left.get_label() != right.get_label();
}


namespace std {
  template <typename T>
  struct hash<graph_node<T>>
  {
    std::size_t operator() (const graph_node<T>& element) const
    {
      return make_hash(element.get_label());
    }
  };
}

#endif /* GRAPH_NODE_HPP */
