#ifndef MAKE_HASH_HPP
#define MAKE_HASH_HPP

#include <functional>

template <typename T>
std::size_t make_hash(const T& object)
{
  std::hash<T> hash;
  return hash(object);
}

template <typename T1, typename... T>
std::size_t make_hash(T1 object, T... objects)
{
  std::size_t h1 = make_hash(object);
  std::size_t h2 = make_hash(objects...);

  // filched from boost::hash_combine
  h2 ^= h1 + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);

  return h2;
}

#endif /* MAKE_HASH_HPP */
