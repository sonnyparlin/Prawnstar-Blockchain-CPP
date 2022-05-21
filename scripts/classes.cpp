#include <iostream>

class Person {
public:
    Person(std::string, int);
    ~Person()=default;

    std::string name;
    int age;

    bool operator<(const Person &rhs) const {
        return this->age < rhs.age;
    }

    bool operator>(const Person &rhs) const {
        return this->age > rhs.age;
    }
};

Person::Person(std::string _name, int _age)
        : name(std::move(_name)), age(_age){}

int main() {
    std::string name_ = "Sonnyjim";
    int age_ = 49;
    Person p1(name_, age_);
    Person p2("Kara", 46);
    auto* p3 = new Person("John Smith", 30);

    auto answer = (p1 > p2);
    std::cout << std::boolalpha << answer << std::endl;

//    std::cout << p1.name << " is " << p1.age << " years old." << std::endl;
//    std::cout << p2.name << " is " << p2.age << " years old." << std::endl;
//    std::cout << p3->name << " is " << p3->age << " years old." << std::endl;

    delete p3;
    return 0;
}