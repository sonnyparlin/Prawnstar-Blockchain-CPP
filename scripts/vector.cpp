#include <vector>
#include <iostream>

int main() {
  std::vector<int> chars{'a','b','c','d','e','f','g'};
  for(auto const &character : chars)
    std::cout << "char = " << character << std::endl;
  return 0;
}
