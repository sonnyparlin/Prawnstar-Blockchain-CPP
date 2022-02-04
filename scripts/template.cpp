#include <iostream>

template <class T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

struct Person {
    std::string name;
    int age;
    bool operator<(const Person &rhs) const {
        return this->age < rhs.age;
    }
};

int main() {

    Person p1,p2;
    p1.name="Sonnyjim";
    p1.age=49;

    p2.name="Kara";
    p2.age=46;

    bool answer = (p1 < p2);
    std::cout << max<int>(100,200) << std::endl;
    std::cout << std::boolalpha << answer << std::endl;

    return 0;
}
