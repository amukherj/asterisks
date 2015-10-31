#ifndef DISJOINT_SETS_HPP
#define DISJOINT_SETS_HPP

#include <unordered_map>
#include <vector>
#include <utility>

/**
 * A data structure that implements the Disjoint Sets
 *  data structure using the Union Find algorithm.
 *
 * You can store arbitrary types as long as they can be
 *  stored in an unordered container and compared for equality.
 *
 * It is a reference container and does not copy elements
 *  you add to it.
 */
template <typename T>
class DisjointSets
{
public:
  DisjointSets() {}
  ~DisjointSets() {}

  /**
   * @return true if the element was added, false if it was already present.
   */
  bool add(const T&);

  /**
   * @return non-null if the element is present, null otherwise.
   *         The returned value is the same for two elements that
   *         are part of the same subset.
   */
  const T *find(const T&);

  /**
   * Merges the two subsets that the specified elements are part
   *  of.
   *  @return Representative of the merged subset.
   */
  const T *merge(const T&, const T&);

private:
  struct DisjointSetEntry
  {
    const T *element;
    unsigned long parent;
    size_t size;

    DisjointSetEntry(const T* element, unsigned long parent)
      : element(element), parent(parent), size(1)
    {}
  };

  std::unordered_map<T, unsigned long> elem_lookup;
  std::vector<DisjointSetEntry> disjoint_set;

  unsigned long find_representative(DisjointSetEntry& entry, unsigned long index) {
    if (entry.parent == index) {
      return index;
    } else {
      auto parent = find_representative(disjoint_set[entry.parent], entry.parent);
      DisjointSetEntry& parent_entry = disjoint_set[parent];
      entry.parent = parent; // path compression

      return parent;
    }
  }

  std::pair<DisjointSetEntry*, unsigned long> find_entry(const T& elem) {
    auto it = elem_lookup.find(elem);
    if (it == elem_lookup.end()) {
      return std::make_pair(nullptr, ~(0UL));
    }

    DisjointSetEntry& entry = disjoint_set[it->second];
    auto parent = find_representative(entry, it->second);
    return std::make_pair(&disjoint_set[parent], parent);
  }
};

template <typename T>
const T *DisjointSets<T>::find(const T& elem)
{
  auto entry = find_entry(elem);
  return (entry.first) ? entry.first->element : nullptr;
}

template <typename T>
bool DisjointSets<T>::add(const T& elem)
{
  const T *entry = find(elem);

  if (entry == nullptr) {
    // FIXME thread-unsafe
    DisjointSetEntry entry(&elem, disjoint_set.size());
    disjoint_set.push_back(entry);
    return true;
  } else {
    return false;
  }

  return entry;
}

template <typename T>
const T *DisjointSets<T>::merge(const T& elem1, const T& elem2)
{
  auto find1 = find_entry(elem1);
  auto find2 = find_entry(elem2);

  auto *entry1 = find1.first;
  auto *entry2 = find2.first;

  auto index1 = find1.second;
  auto index2 = find2.second;

  if (entry1 != nullptr && entry2 != nullptr) {
    if (entry1->size > entry2->size) {
      entry2->parent = index1;
      entry1->size += entry2->size;
      return entry1->element;
    } else {
      entry1->parent = index2;
      entry2->size += entry1->size;
      return entry2->element;
    }
  }

  if (entry1 != nullptr) {
    return entry1->element;
  }

  return (entry2) ? entry2->element : nullptr;
}

#endif /* DISJOINT_SETS_HPP */
