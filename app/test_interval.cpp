#include <iostream>
#include "ra/interval.hpp"
#include <string.h>

using namespace ra::math;
using namespace std;

template <class T> 
void constructor_tests()
{
    std::cout << "Testing constructors" << std::endl;

}
template <class T> void do_test()
{
    constructor_tests<T>();    
}

int main()
{

    std::cout << "Test with float" << std::endl;
    do_test<float>();
    std::cout << "All tests with float passes!" << std::endl;
    return 0;
}