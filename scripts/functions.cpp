#include <iostream>
#include <vector>
#include <algorithm>

int func1(int a, int b) {
    return a+b;
}

int main() {
//     std::cout << func1(3,5) << std::endl;
//
    std::vector<int> vec {1,1,1,1,1,1,1,1,1,1};
    int answer = 0;
    std::for_each(vec.begin(), vec.end(), [&answer](int x){
        answer += x;
    });

    std::cout << answer << std::endl;

    return 0;
}