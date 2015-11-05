#include "graph_node.hpp"
#include "graph_edge.hpp"
#include "adjacency_list.hpp"
#include "disjoint_sets.hpp"
#include "visual_graph.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <unordered_set>
#include <cassert>

template <typename T>
using edge_priq = std::priority_queue<graph_edge<T>,
                                      std::deque<graph_edge<T>>,
                                      std::greater<graph_edge<T>>> ;


template <typename T>
std::vector<graph_edge<T>> solve_kruskal(const adjacency_list<T>& adj_list)
{
  std::unordered_set<graph_edge<T>> edges = adj_list.get_edges();

  std::vector<graph_edge<T>> edge_vec;
  for (const auto& edge: edges) {
    auto pair = edge.get_vertices();
    edge_vec.push_back(edge);
  }

  std::sort(edge_vec.begin(), edge_vec.end());

  std::vector<graph_edge<T>> mst;
  disjoint_sets<graph_node<T>> vertex_sets;

  for (auto& edge: edge_vec) {
    auto pair = edge.get_vertices();

    auto* x1 = vertex_sets.find(*pair.first);
    auto* x2 = vertex_sets.find(*pair.second);

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

    x1 = vertex_sets.find(*pair.first);
    x2 = vertex_sets.find(*pair.second);

    if (vertex_sets.size() == adj_list.get_vertex_count()) {
      break;
    }
  }

  return mst;
}

template <typename T>
std::vector<graph_edge<T>> solve_prim(const adjacency_list<T>& adj_list)
{
  std::vector<graph_edge<T>> result;
  edge_priq<T> shortest_edges;
  std::unordered_set<graph_node<T>> visited;

  auto *vertex = adj_list.get_any_vertex();

  if (vertex == nullptr) {
    return result;
  }

  visited.insert(*vertex);
  std::unordered_set<graph_edge<T>> edges
      = adj_list.get_adjacent_edges(*vertex);

  std::for_each(edges.begin(), edges.end(),
                [&shortest_edges](auto& edge) {
                  shortest_edges.push(edge);
                });

  size_t max_edge_count = adj_list.get_vertex_count() - 1;
  while (result.size() < max_edge_count) {
    graph_edge<T> edge = shortest_edges.top();
    shortest_edges.pop();

    auto pair = edge.get_vertices();
    auto *next_vertex = pair.first;

    if (visited.find(*next_vertex) != visited.end()) {
      next_vertex = pair.second;

      if (visited.find(*next_vertex) != visited.end()) {
        continue;
      }
    }

    result.push_back(edge);
    visited.insert(*next_vertex);

    edges = adj_list.get_adjacent_edges(*next_vertex);
    std::for_each(edges.begin(), edges.end(),
                  [&shortest_edges](auto& edge) {
                    shortest_edges.push(edge);
                  });
  }

  return result;
}

template <typename T>
struct dijkstra_entry
{
  double min_distance;
  bool visited;
  const graph_node<T> *precedent;

  dijkstra_entry()
    : min_distance(std::numeric_limits<double>::max()),
      visited(false), precedent(nullptr) {}
};

template <typename T>
std::unordered_map<graph_node<T>, dijkstra_entry<T>> dijkstra_shortest_path(
                    const adjacency_list<T>& adj_list, const T& label)
{
  std::unordered_map<graph_node<T>, dijkstra_entry<T>> result;
  auto *start_vertex = adj_list.find_vertex(label);

  if (start_vertex == nullptr) {
    return result;
  }

  // initialize the the dijkstra table
  dijkstra_entry<T> de;
  de.visited = true;
  de.min_distance = 0;
  result[*start_vertex] = de;

  for (graph_node<T> vertex : adj_list.get_vertices()) {
    if (vertex != *start_vertex) {
      result[vertex] = dijkstra_entry<T>{};
    }
  }

  long visited_count = 1;

  edge_priq<T> shortest_edges;
  std::unordered_set<graph_edge<T>> adj_edges =
    adj_list.get_adjacent_edges(*start_vertex);

  std::for_each(adj_edges.begin(), adj_edges.end(),
      [&shortest_edges](auto& edge) {
        shortest_edges.push(edge);
      });

  while (!shortest_edges.empty()) {
    graph_edge<T> edge = shortest_edges.top();
    shortest_edges.pop();

    auto pair = edge.get_vertices();
    auto *v1 = pair.first, *v2 = pair.second;
    auto& de1 = result[*v1], &de2 = result[*v2];

    assert(de1.visited || de2.visited);

    if (de1.visited && de2.visited) {
      // ...
      auto dist2 = de1.min_distance + edge.get_weight();
      auto dist1 = de2.min_distance + edge.get_weight();

      if (dist2 < de2.min_distance) {
        de2.min_distance = dist2;
        de2.precedent = v1;
      } else if (dist1 < de1.min_distance) {
        de1.min_distance = dist1;
        de1.precedent = v2;
      }
    } else {
      auto& v_de = (!de1.visited) ? de2 : de1;
      auto v_vtx = (!de1.visited) ? v2 : v1;
      auto& uv_de = (!de1.visited) ? de1 : de2;
      auto uv_vtx = (!de1.visited) ? v1 : v2;

      uv_de.visited = true;
      uv_de.min_distance = std::min(uv_de.min_distance,
                                  v_de.min_distance + edge.get_weight());
      uv_de.precedent = v_vtx;
      visited_count++;

      adj_edges = adj_list.get_adjacent_edges(*uv_vtx);

      std::for_each(adj_edges.begin(), adj_edges.end(),
          [&shortest_edges](auto& edge) {
            shortest_edges.push(edge);
      });
    }
  }
}

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

  auto mst = solve_kruskal(adj_list);

  for (auto& edge: mst) {
    std::cout << edge.get_vertices().first->get_label()
              << "<->" << edge.get_vertices().second->get_label()
              << "|" << edge.get_weight() << '\n';
  }

  std::cout << "-----\n";

  adjacency_list<std::string> adj_list2;
  adj_list2.add_vertex(n1);
  adj_list2.add_vertex(n2);
  adj_list2.add_vertex(n3);
  adj_list2.add_vertex(n4);
  adj_list2.add_vertex(n5);
  adj_list2.add_vertex(n6);
  adj_list2.add_vertex(n7);

  adj_list2.add_edge(n1, n2, 2);
  adj_list2.add_edge(n1, n3, 4);
  adj_list2.add_edge(n1, n4, 1);
  adj_list2.add_edge(n2, n4, 3);
  adj_list2.add_edge(n2, n5, 10);
  adj_list2.add_edge(n3, n4, 2);
  adj_list2.add_edge(n3, n6, 5);
  adj_list2.add_edge(n4, n5, 7);
  adj_list2.add_edge(n4, n6, 8);
  adj_list2.add_edge(n4, n7, 4);
  adj_list2.add_edge(n5, n7, 6);
  adj_list2.add_edge(n6, n7, 1);

  auto mst2 = solve_prim(adj_list2);

  for (auto& edge: mst2) {
    std::cout << edge.get_vertices().first->get_label()
              << "<->" << edge.get_vertices().second->get_label()
              << "|" << edge.get_weight() << '\n';
  }

  to_dot(std::cout, adj_list2, "adj_list2");
  to_dot(std::cout, mst2, "mst");

  to_dot(std::cout, adj_list, "adj_list");
  std::string start_label = "1";
  std::unordered_map<graph_node<std::string>,
                     dijkstra_entry<std::string>> result
      = dijkstra_shortest_path(adj_list, start_label);

  std::cout << "------\n";
  for (auto& entry: result) {
    auto& vertex = entry.first;
    auto& info = entry.second;

    std::cout << vertex.get_label() << ": " << info.min_distance << '\n';
  }
}

