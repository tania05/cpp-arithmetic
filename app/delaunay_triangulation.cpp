#include "triangulation_2.hpp"
#include "ra/kernel.hpp"
#include <CGAL/Cartesian.h>

using Kernel = CGAL::Cartesian<double>;
using Triangulation = trilib::Triangulation_2<Kernel>;

template <class T>
void lop()
{
    std::cout<< "You are here" << std::endl;
}

int main()
{
    lop<double>();
    return 0;
}