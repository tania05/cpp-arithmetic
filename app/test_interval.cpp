#include <iostream>
#include "ra/interval.hpp"
#include <string.h>
#include <cassert>

using namespace ra::math;
using namespace std;


template <class T>
void constructor_tests()
{
  cout << "Testing constructors" << endl;

  //constructor with one value
  interval<T> a1(1.3);
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

  //copy constructor
  interval<T> c1(t4);
  assert(c1.lower() == (T)-6.7);
  assert(c1.upper() == (T)-3.5);
  assert(t4.lower() == (T)-6.7);
  assert(t4.upper() == (T)-3.5);


  //move constructor
  interval<T> c2(std::move(c1));
  assert(c1.lower() == (T)-6.7);
  assert(c1.upper() == (T)-3.5);
  assert(c2.lower() == (T)-6.7);
  assert(c2.upper() == (T)-3.5);

  cout << "Done constructors" << endl << endl;
}

template <class T>
void add()
{
  interval<T> a1(-1.5, 7.5);
  interval<T> a2(3.5,9.0);

  a1 += a2;
  assert(a1.lower() == T(2.0));
  assert(a1.upper() == T(16.5));

  //adding to itself
  interval<T> self(2.5, 3.5);
  self += self;
  cout << self.lower() << endl;
  cout << self.upper() << endl;

  interval<T> z1(0, 0);
  z1 += z1;
  assert(z1.lower() == T(0));
  assert(z1.upper() == T(0));

  interval<T> z2(-1, 0);
  z2 += z2;
  assert(z2.lower() == T(-2));
  assert(z2.upper() == T(0));

}

template <class T>
void deduct()
{
  interval<T> a1(-1.5, 7.5);
  interval<T> a2(3.5,9.0);

  a1 -= a2;
  assert(a1.lower() == T(-10.5));
  assert(a1.upper() == T(4.0));

  //deducting from itself
  interval<T> self(2.5, 3.5);
  self -= self;
  assert(self.lower() == T(-1.0));
  assert(self.upper() == T(1.0));

  interval<T> z1(0, 0);
  z1 -= z1;
  assert(z1.lower() == T(0));
  assert(z1.upper() == T(0));

  interval<T> z2(-2, -1);
  z2 -= z2;
  assert(z2.lower() == T(-1));
  assert(z2.upper() == T(1));  
}

template <class T>
void multiply()
{
  interval<T> a1(-1.0, 7.0);
  interval<T> a2(3.5,9.0);

  a1 *= a2;
  assert(a1.lower() == T(-9.0));
  assert(a1.upper() == T(63.0));

  //multiplied by itself
  interval<T> self(2.0, 3.0);
  self *= self;
  cout << self.lower() << endl;
  cout << self.upper() << endl;

  assert(self.lower() == T(4.0));
  assert(self.upper() == T(9.0));

  interval<T> z1(0, 0);
  z1 *= z1;
  assert(z1.lower() == T(0));
  assert(z1.upper() == T(0));

  interval<T> z2(-2, -1);
  z2 *= z2;
  
  assert(z2.lower() == T(1));
  assert(z2.upper() == T(4));  
}


template <class T>
void compound_operators()
{
  cout << "Doing addition" << endl;
  add<T>();
  cout << "Doing deduction" << endl;
  deduct<T>();
  cout << "Doing multiplication" << endl;
  multiply<T>();
  cout << "Done compound operators" << endl << endl;
}

template <class T>
void check_singleton()
{
  cout << "Checking for singleton Operation" << endl;
  interval<T> a1(-1.0, 7.0);
  interval<T> a2(3.5,-9.0);
  interval<T> a3(0,0);
  interval<T> a4(-6.0,-6.0);
  assert(!a1.is_singleton());
  assert(!a2.is_singleton());
  assert(a3.is_singleton());
  assert(a4.is_singleton());
  cout << "Done checking singleton" << endl << endl;
}

template <class T>
void check_sign()
{
  cout << "Checking for Sign" << endl;
  interval<T> a1(-1.0, 7.0);
  interval<T> a2(3.5,9.0);
  interval<T> a3(0,0);
  interval<T> a4(-6.0,-6.0);
  try
  {
    a1.sign();
    assert(false);
  }
  catch(indeterminate_result & e)
  {  
  }

  assert(a2.sign() == 1);
  assert(a3.sign() == 0);
  assert(a4.sign() == -1);

  cout << "Done checking Sign" << endl << endl;
}

template <class T>
void binary_add()
{
  interval<T> a1(-1.5, 7.5);
  interval<T> a2(3.5,9.0);

  auto r1 = a1 + a2;
  assert(a1.lower() == T(-1.5));
  assert(a1.upper() == T(7.5));
  assert(a2.lower() == T(3.5));
  assert(a2.upper() == T(9.0));
  assert(r1.lower() == T(2.0));
  assert(r1.upper() == T(16.5));

  //adding to itself
  interval<T> self(2.5, 3.5);
  auto r2 = self + self;
  assert(r2.lower() == T(5.0));
  assert(r2.upper() == T(7.0));
  assert(self.lower() == T(2.5));
  assert(self.upper() == T(3.5));

  interval<T> z1(0, 0);
  auto r3 = z1 + z1;
  assert(r3.lower() == T(0));
  assert(r3.upper() == T(0));
  assert(z1.lower() == T(0));
  assert(z1.upper() == T(0));

  interval<T> z2(-1, 0);
  auto r4 = z2 + z2;
  assert(r4.lower() == T(-2));
  assert(r4.upper() == T(0));
  assert(z2.lower() == T(-1));
  assert(z2.upper() == T(0));

}

template <class T>
void binary_deduct()
{
  interval<T> a1(-1.5, 7.5);
  interval<T> a2(3.5,9.0);

  auto r1 = a1 - a2;
  assert(r1.lower() == T(-10.5));
  assert(r1.upper() == T(4.0));
  assert(a1.lower() == T(-1.5));
  assert(a1.upper() == T(7.5));
  assert(a2.lower() == T(3.5));
  assert(a2.upper() == T(9.0));


  //deducting from itself
  interval<T> self(2.5, 3.5);
  auto r2 = self - self;
  assert(r2.lower() == T(-1.0));
  assert(r2.upper() == T(1.0));
  assert(self.lower() == T(2.5));
  assert(self.upper() == T(3.5));


  interval<T> z1(0, 0);
  auto r3 = z1 - z1;
  assert(r3.lower() == T(0));
  assert(r3.upper() == T(0));
  assert(z1.lower() == T(0));
  assert(z1.upper() == T(0));

  interval<T> z2(-2, -1);
  auto r4 = z2 - z2;
  assert(r4.lower() == T(-1));
  assert(r4.upper() == T(1)); 
  assert(z2.lower() == T(-2));
  assert(z2.upper() == T(-1));  
}

template <class T>
void binary_multiply()
{
  interval<T> a1(-1.0, 7.0);
  interval<T> a2(3.5,9.0);

  auto r1 = a1 * a2;
  assert(r1.lower() == T(-9.0));
  assert(r1.upper() == T(63.0));
  assert(a1.lower() == T(-1.0));
  assert(a1.upper() == T(7.0));
  assert(a2.lower() == T(3.5));
  assert(a2.upper() == T(9.0));

  //multiplied by itself
  interval<T> self(2.0, 3.0);
  auto r2 = self * self;
  assert(r2.lower() == T(4.0));
  assert(r2.upper() == T(9.0));
  assert(self.lower() == T(2.0));
  assert(self.upper() == T(3.0));

  interval<T> z1(0, 0);
  auto r3 = z1 * z1;
  assert(r3.lower() == T(0));
  assert(r3.upper() == T(0));
  assert(z1.lower() == T(0));
  assert(z1.upper() == T(0));

  interval<T> z2(-2, -1);
  auto r4 = z2 * z2;
  assert(r4.lower() == T(1));
  assert(r4.upper() == T(4));
  assert(z2.lower() == T(-2));
  assert(z2.upper() == T(-1));  
}

template <class T>
void binary_operators()
{
  cout << "Doing binary addition" << endl;
  binary_add<T>();
  cout << "Doing binary deduction" << endl;
  binary_deduct<T>();
  cout << "Doing binary multiplication" << endl;
  binary_multiply<T>();
  cout << "Done binary operators" << endl << endl;
}

template <class T>
void less_than()
{
  cout << "Doing less than operation" << endl;

  interval<T> a1(-1.0, 7.0);
  interval<T> a2(3.5,9.0);
  interval<T> a3(-1.0, 7.0);
  interval<T> a4(3.5, 6.0);
  interval<T> a5(-7.0, -9.0);

  try
  {
    a1 < a4;
    assert(false);
  } catch (ra::math::indeterminate_result& e)
  {
  }

  assert(a1 < a2);
  assert((a1 < a3) == false);
  assert((a1 < a5) == false);
  cout << "Done less than operator" << endl << endl;  
  
}

template <class T>
void stream_inserter()
{
  interval<T> a5(-7.0, 9.0);
  cout << a5;
  cout << endl << "Done streamline inserter" << endl << endl;
}

template <class T> void do_test()
{
    constructor_tests<T>();
    compound_operators<T>();
    check_singleton<T>();
    check_sign<T>();
    binary_operators<T>();
    less_than<T>();
    stream_inserter<T>();
}

int main()
{
    std::cout << "!!!!!!!! Test with float !!!!!!!!!!!" << std::endl;
    do_test<float>();
    std::cout << "All tests with float passes!" << std::endl << std::endl << std::endl;
    std::cout << "!!!!!!!! Test with double !!!!!!!!!!!" << std::endl;
    do_test<double>();
    std::cout << "All tests with long double passes!" << std::endl << std::endl << std::endl;
    std::cout << "!!!!!!!! Test with double !!!!!!!!!!!" << std::endl;
    do_test<long double>();
    std::cout << "All tests with long double passes!" << std::endl << std::endl << std::endl;
    return 0;
}