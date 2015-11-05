#ifndef VISUAL_GRAPH_HPP
#define VISUAL_GRAPH_HPP

#include <sstream>
#include "adjacency_list.hpp"

template <typename E>
std::ostream& to_dot(std::ostream& out,
                   const adjacency_list<E>& adj_list,
                   const std::string& graph_name)
{
  to_dot(out, adj_list.get_edges(), graph_name);

  return out;
}

template <typename E, template <typename, typename = std::allocator<graph_edge<E>>> class L>
std::ostream& to_dot(std::ostream& out,
                   const L<graph_edge<E>>& edges,
                   const std::string& graph_name)
{
  out << "graph " << graph_name << " {\n";
  for (const auto& edge: edges) {
    auto pair = edge.get_vertices();
    out << "\t\"" << pair.first->get_label() << "\""
        << " -- \"" << pair.second->get_label() << "\" "
        << " [label=\"" << edge.get_weight() << "\"];\n";
  }
  out << "}\n";

  return out;
}

template <typename E, template <typename, typename = std::hash<graph_edge<E>>,
          typename = std::equal_to<graph_edge<E>>, typename = std::allocator<graph_edge<E>>> class L>
std::ostream& to_dot(std::ostream& out,
                   const L<graph_edge<E>>& edges,
                   const std::string& graph_name)
{
  out << "graph " << graph_name << " {\n";
  for (const auto& edge: edges) {
    auto pair = edge.get_vertices();
    out << "\t\"" << pair.first->get_label() << "\""
        << " -- \"" << pair.second->get_label() << "\" "
        << " [label=\"" << edge.get_weight() << "\"];\n";
  }
  out << "}\n";

  return out;
}

#endif /* VISUAL_GRAPH_HPP */
