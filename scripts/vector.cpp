#include <vector>
#include <iostream>

int main() {
  std::vector<int> vi{'a','b','c','d','e','f','g'};
  for(char i : vi) 
    std::cout << "i = " << i << std::endl;
  return 0;
}
