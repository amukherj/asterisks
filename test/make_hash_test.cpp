#include "utility/make_hash.hpp"
#include <iostream>
#include <string>

int main()
{
  std::string s1("Hello"), s2("World");

  std::cout << "hash[" << s1 << "] = " << std::hex << make_hash(s1) << '\n';
  std::cout << "hash[" << s2 << "] = " << std::hex << make_hash(s2) << '\n';
  std::cout << "hash[" << s1 << ", " << s2 << "] = "
            << std::hex << make_hash(s1, s2) << '\n';
}
