#ifndef ADJACENCY_LIST_HPP
#define ADJACENCY_LIST_HPP

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "graph_node.hpp"
#include "graph_edge.hpp"

template <typename T>
class adjacency_list
{
public:
  void add_vertex(const graph_node<T>& vertex)
  {
    const auto& list = adj_list[vertex];
  }

  bool add_edge(const graph_edge<T>& edge)
  {
    auto pair = edge.get_vertices();
    return add_edge(pair.first, pair.second, edge.get_wait());
  }

  bool add_edge(const graph_node<T>& end1,
                const graph_node<T>& end2,
                double weight = 1.0)
  {
    auto it1 = adj_list.find(end1);
    auto it2 = adj_list.find(end2);

    if (it1 == adj_list.end()
        || it2 == adj_list.end()) {
      return false;
    }

    auto& list1 = it1->second;
    auto& list2 = it2->second;

    auto it3 = list1.find(relative_edge(end2));
    if (it3 == list1.end()) {
      list1.insert(relative_edge(end2, weight));
    } else if (it3->weight != weight) {
      // FIXME update the weight
    }

    auto it4 = list2.find(relative_edge(end1));
    if (it4 == list2.end()) {
      list2.insert(relative_edge(end1, weight));
    } else if (it4->weight != weight) {
      // FIXME update the weight
    }
  }

  std::unordered_set<graph_edge<T>> get_edges() const
  {
    std::unordered_set<graph_edge<T>> edge_set;

    for (auto& entry: adj_list) {
      const graph_node<T>& node = entry.first;
      // std::cout << "Found entry: " << node.get_label() << '\n';
      auto& neighbours = entry.second;

      std::for_each(neighbours.begin(), neighbours.end(),
                    [&edge_set, &node](auto& elem) {
                      // std::cout << ">>> Found edge: " << elem.other->get_label() << ", [" << elem.weight << "]\n";
                      auto new_edge = relative_edge::make_edge(node, elem);
                      auto it = edge_set.find(new_edge);
                      if (it == edge_set.end()) {
                        edge_set.insert(new_edge);
                      }
                    });
    }

    return edge_set;
  }

  std::unordered_set<graph_node<T>> get_vertices() const
  {
    std::unordered_set<graph_node<T>> result;

    for (const auto& entry: adj_list) {
      result.insert(entry.first);
    }

    return result;
  }

  const graph_node<T>* get_any_vertex() const
  {
    auto it = adj_list.begin();
    return (it != adj_list.end()) ? &(it->first) : nullptr;
  }

  size_t get_vertex_count() const
  {
    return adj_list.size();
  }

  const graph_node<T>* find_vertex(const T& label) const
  {
    graph_node<T> key(label);
    auto it = adj_list.find(key);

    if (it != adj_list.end()) {
      return &(it->first);
    } else {
      return nullptr;
    }
  }

  std::unordered_set<graph_edge<T>> get_adjacent_edges(const graph_node<T>& vertex) const
  {
    auto it = adj_list.find(vertex);
    std::unordered_set<graph_edge<T>> result;

    if (it != adj_list.end()) {
      auto relative_edges = it->second;
      std::for_each(relative_edges.begin(), relative_edges.end(),
                    [&result, &it](auto& redge) {
                      result.insert(relative_edge::make_edge(it->first,
                                              redge));
                    });
    }
    
    return result;
  }

private:
  struct relative_edge {
    const graph_node<T> *other;
    double weight;

    relative_edge(const graph_node<T>& other, double weight = 1.0)
      : other(&other), weight(weight)
    {}

    static graph_edge<T> make_edge(const graph_node<T>& first,
                                   const relative_edge& that)
    {
      graph_edge<T> edge(first, *(that.other), that.weight);
      return edge;
    }

    bool operator==(const relative_edge& that) const
    {
      return other == that.other;
    }
  };

  struct re_hasher
  {
    std::size_t operator()(const relative_edge& re) const
    {
      return make_hash(re.other);
    }
  };

  std::unordered_map<graph_node<T>,
                    std::unordered_set<relative_edge, re_hasher>> adj_list;
};

#endif /* ADJACENCY_LIST_HPP */
