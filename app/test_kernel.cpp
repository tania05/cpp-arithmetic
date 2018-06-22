#include "ra/kernel.hpp"
#include <CGAL/MP_Float.h>
#include <CGAL/Cartesian.h>

using namespace ra::geometry;
using namespace std;

template <class T>
void constructor_tests()
{
  cout << "Testing constructors" << endl;

  //constructor with one value
  Kernel<T> a1;
  Kernel<T> a2(a1);
  Kernel<T> a3(std::move(a2));
}

template<class T>
typename CGAL::Cartesian<T>::Vector_2 generate_vectors(T x, T y)
{
  return typename CGAL::Cartesian<T>::Vector_2(x, y);
}

template<class T>
typename CGAL::Cartesian<T>::Point_2 generate_points(T x, T y)
{
  return typename CGAL::Cartesian<T>::Point_2(x, y);
}

template <class T>
void test_orientation()
{
  cout << "Testing orientation" << endl;
  
  Kernel<T> orient;

  auto a = generate_points<T>(0, 0);
  auto b = generate_points<T>(2, 2);
  auto c = generate_points<T>(2, 0);
  auto d = generate_points<T>(1, 1);
  auto e = generate_points<T>(0, 2);

  assert(orient.orientation(a, b, c) == Kernel<T>::Orientation::right_turn);
  assert(orient.orientation(a, b, d) == Kernel<T>::Orientation::collinear);
  assert(orient.orientation(a, b, e) == Kernel<T>::Orientation::left_turn);
  
}

template <class T>
void test_strictly_convex()
{
  cout << "Testing strictly convect quad test" << endl;
  Kernel<T> scvx;
  auto b00 = generate_points<T>(0, 0);
  auto c20 = generate_points<T>(2, 0);
  auto d22 = generate_points<T>(2, 2);
  auto a02 = generate_points<T>(0, 2);
  auto e11 = generate_points<T>(1, 1);

  //colinear line
  assert((scvx.is_strictly_convex_quad(b00,e11,d22, generate_points<T>(3, 3))) == false);
  //sqr
  assert(scvx.is_strictly_convex_quad(a02,b00,c20,d22));
  //not strictly convex
  assert((scvx.is_strictly_convex_quad(a02,b00,e11, generate_points<T>(3, 0))) == false);    
}

template <class T>
void test_pd()
{
  cout << "Testing preferred direction" << endl;  
  auto a = generate_points<T>(0, 0);
  auto b = generate_points<T>(2, 2);
  auto c = generate_points<T>(2, 0);
  auto d = generate_points<T>(0, 2);
  auto v = generate_vectors<T>(1,0);
  auto w = generate_vectors<T>(-1, 2);
  auto u = generate_vectors<T>(2, 1);
  
  Kernel<T> pd;

  assert(pd.preferred_direction(a, b, c, d, u) == 1);
  assert(pd.preferred_direction(a, b, c, d, v) == 0);
  assert(pd.preferred_direction(a, b, c, d, w) == -1);
}
template <class T>
void test_local_pd_dl()
{
  cout << "Testing preferred-directions delauny " << endl;  
  Kernel<T> dl;
  auto a = generate_points<T>(0, 0);
  auto b = generate_points<T>(2, 0);
  auto c = generate_points<T>(0, 2);
  auto d = generate_points<T>(1, 1);
  auto e = generate_points<T>(2, 2);
  auto f = generate_points<T>(3, 3);

  auto u = generate_vectors<T>(1,1.5);
  auto v = generate_vectors<T>(1,0);
  auto w = generate_vectors<T>(-1,0.5);
  
  assert(dl.is_locally_pd_delaunay_edge(a, b, c, d, u, v) == false);
  assert(dl.is_locally_pd_delaunay_edge(c, a, b, e, u, v) == false);
  assert(dl.is_locally_pd_delaunay_edge(a, b, c, f, u, v));
}

template <class T>
void test_local_dl()
{
  cout << "Testing locally delauny edge" << endl;        
  Kernel<T> dl;
  auto a = generate_points<T>(0, 0);
  auto b = generate_points<T>(2, 0);
  auto c = generate_points<T>(0, 2);
  auto d = generate_points<T>(1, 1);
  auto e = generate_points<T>(2, 2);
  auto f = generate_points<T>(3, 3);

  assert(dl.is_locally_delaunay_edge(a, b, c, d) == false);
  assert(dl.is_locally_delaunay_edge(a, b, c, e));
  assert(dl.is_locally_delaunay_edge(a, b, c, f));
}

template <class T>
void test_side_of_circle()
{
  cout << "Testing side of oriented circle" << endl;  
  Kernel<T> circle;
  auto a = generate_points<T>(0, 0);
  auto b = generate_points<T>(2, 0);
  auto c = generate_points<T>(0, 2);
  auto d = generate_points<T>(1, 1);
  auto e = generate_points<T>(2, 2);
  auto f = generate_points<T>(3, 3);

  assert(circle.side_of_oriented_circle(a, b, c, d) == Kernel<T>::Oriented_side::on_positive_side);
  assert(circle.side_of_oriented_circle(a, b, c, e) == Kernel<T>::Oriented_side::on_boundary);
  assert(circle.side_of_oriented_circle(a, b, c, f) == Kernel<T>::Oriented_side::on_negative_side);
}

template <class T>
void do_test()
{
    constructor_tests<T>();
    test_orientation<T>();
    test_side_of_circle<T>();
    test_pd<T>();
    test_strictly_convex<T>();
    test_local_dl<T>();
    test_local_pd_dl<T>();
  
}

int main()
{
    do_test<float>();
    return 0;
}
