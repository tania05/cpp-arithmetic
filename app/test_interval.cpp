#include <iostream>
#include "ra/interval.hpp"
#include <string.h>
#include <cassert>

using namespace ra::math;
using namespace std;

template <class T> 
void constructor_tests()
{
    cout << "********Testing constructors...." << endl;
    
    //constructor with one value
    interval<T> a1(1.3);
    cout << a1.lower() << endl;
    cout << a1.upper() << endl;
    assert(a1.lower() == (T)1.3);
    assert(a1.upper() == (T)1.3);
    
    interval<T> a2(-1.3);
    assert(a2.lower() == (T)-1.3);
    assert(a2.upper() == (T)-1.3);
    
    interval<T> a3(0);
    assert(a3.lower() == (T)0);
    assert(a3.upper() == (T)0);

    //with two values
    interval<T> t1(1.3, 9.0);
    assert(t1.lower() == (T)1.3);
    assert(t1.upper() == (T)9.0);
    
    interval<T> t2(-1.3, 8.0);
    assert(t2.lower() == (T)-1.3);
    assert(t2.upper() == (T)8.0);
    
    interval<T> t3(0,0);
    assert(t3.lower() == (T)0);
    assert(t3.upper() == (T)0);
    
    interval<T> t4(-3.5,-6.7);
    assert(t4.lower() == (T)-6.7);
    assert(t4.upper() == (T)-3.5);    
    
    cout << "Done constructors" << endl;
}
template <class T> void do_test()
{
    constructor_tests<T>();   
}

int main()
{

    std::cout << "!!!!!!!! Test with float !!!!!!!!!!!" << std::endl;
    do_test<float>();
    std::cout << "All tests with float passes!" << std::endl;
    return 0;
}