#include <iostream>

template <typename T, typename P>
double Max(T a, P b) {
    return (a > b) ? a : b;
}

int main() {
    std::cout << Max(50.1, 200.34) << std::endl;
    return 0;
}