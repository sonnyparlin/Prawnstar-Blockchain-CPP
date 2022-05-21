#include <iostream>
#include <iomanip>

//double myfunc(int x)
//{
//    return x += 100;
//}
//
//double myfunc1(int &x)
//{
//    return x += 100;
//}

int main()
{
    int myint = 35;
    int* pointer_to_int = &myint;

    std::cout << *pointer_to_int << std::endl;
//    std::cout << pointer_to_int << std::endl;
    // std::cout << &myint << std::endl;

    // const char* str = "Hello, world!";
    // cout << str; prints whole string due to special treatment,
    // it will automatically dereference the pointer till it enconters '\0' char
    // std::cout << str << std::endl;

//    time_t start, end;
//    time(&start);
//    for (int i=0;i<2000000000;++i)
//    {
//        myfunc(i);
//    }
//    time(&end);
//    auto time_taken = double(end - start);
//    std::cout << "Execution time : " << std::fixed << std::setprecision(5) << time_taken;
//    std::cout << " sec \n" << std::endl;
//
//    time(&start);
//    for (int i=0;i<2000000000;++i)
//    {
//        myfunc1(i);
//    }
//    time(&end);
//    time_taken = double(end - start);
//    std::cout << "Execution time : " << std::fixed << std::setprecision(5) << time_taken;
//    std::cout << " sec \n" << std::endl;

    return 0;
}