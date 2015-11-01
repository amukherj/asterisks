#include "graph_node.hpp"
#include "graph_edge.hpp"
#include "adjacency_list.hpp"
#include "disjoint_sets.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>

int main()
{
  graph_node<std::string> n0("0"), n1("1"), n2("2"), n3("3"),
                  n4("4"), n5("5"), n6("6"), n7("7"), n8("8");
  adjacency_list<std::string> adj_list;

  adj_list.add_vertex(n0);
  adj_list.add_vertex(n1);
  adj_list.add_vertex(n2);
  adj_list.add_vertex(n3);
  adj_list.add_vertex(n4);
  adj_list.add_vertex(n5);
  adj_list.add_vertex(n6);
  adj_list.add_vertex(n7);
  adj_list.add_vertex(n8);

  adj_list.add_edge(n0, n1, 4.0);
  adj_list.add_edge(n0, n7, 8.0);
  adj_list.add_edge(n1, n2, 8.0);
  adj_list.add_edge(n1, n7, 11.0);
  adj_list.add_edge(n2, n8, 2.0);
  adj_list.add_edge(n7, n8, 7.0);
  adj_list.add_edge(n6, n8, 6.0);
  adj_list.add_edge(n6, n7, 1.0);
  adj_list.add_edge(n2, n3, 7.0);
  adj_list.add_edge(n2, n5, 4.0);
  adj_list.add_edge(n6, n5, 2.0);
  adj_list.add_edge(n3, n5, 14.0);
  adj_list.add_edge(n3, n4, 9.0);
  adj_list.add_edge(n4, n5, 10.0);

  std::unordered_set<graph_edge<std::string>> edges = adj_list.get_edges();
  std::cout << edges.size() << '\n';

  std::vector<graph_edge<std::string>> edge_vec;
  for (const auto& edge: edges) {
    auto pair = edge.get_vertices();
    std::cout << pair.first->get_label() << "<->" << pair.second->get_label()
              << "|" << edge.get_weight() << '\n';
    edge_vec.push_back(edge);
    // std::cout << pair.first << ", " << pair.second << '\n';
  }
  std::cout << "----\n";

  for (auto& edge: edge_vec) {
    std::cout << edge.get_vertices().first->get_label()
              << "<->" << edge.get_vertices().second->get_label()
              << "|" << edge.get_weight() << '\n';
  }
  std::cout << "----\n";

  std::sort(edge_vec.begin(), edge_vec.end(),
           [] (graph_edge<std::string>& lhs, graph_edge<std::string>& rhs) {
             return (lhs.get_weight() < rhs.get_weight());
           });

  std::vector<graph_edge<std::string>> mst;
  disjoint_sets<graph_node<std::string>> vertex_sets;

  for (auto& edge: edge_vec) {
    auto pair = edge.get_vertices();
    std::cout << "Looking up vertex [" << pair.first->get_label()
              << "<->" << pair.second->get_label() << "]\n";

    auto* x1 = vertex_sets.find(*pair.first);
    std::cout << " - first vertex rep: " << x1 << '\n';
    auto* x2 = vertex_sets.find(*pair.second);
    std::cout << " - second vertex rep: " << x2 << '\n';

    if (x1 == nullptr) {
      vertex_sets.add(*pair.first);
    }

    if (x2 == nullptr) {
      vertex_sets.add(*pair.second);
    }

    if (x1 == nullptr || x2 == nullptr || x1 != x2) {
      mst.push_back(edge);
      vertex_sets.merge(*pair.first, *pair.second);
    }

    ////
    x1 = vertex_sets.find(*pair.first);
    x2 = vertex_sets.find(*pair.second);
    std::cout << " - [now] first vertex rep: " << x1 << '\n';
    std::cout << " - [now] second vertex rep: " << x2 << '\n';
    ////

    if (vertex_sets.size() == adj_list.get_vertex_count()) {
      break;
    }
  }

  for (auto& edge: mst) {
    std::cout << edge.get_vertices().first->get_label()
              << "<->" << edge.get_vertices().second->get_label()
              << "|" << edge.get_weight() << '\n';
  }
}

