#include <iostream>
#include <string>

#include <boost/pfr.hpp>

struct some_person {
    std::string name;
    unsigned birth_year;
};

int main() {
    some_person val{"Edgar Allan Poe", 1809};

    std::cout << boost::pfr::get<0>(val)
        << " was born in " << boost::pfr::get<1>(val) << '\n';

    std::cout << boost::pfr::io(val) << '\n';
    std::cin >> boost::pfr::io(val);
    std::cout << boost::pfr::io(val) << '\n';
}
