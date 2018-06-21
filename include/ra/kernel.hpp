#ifndef kernel_hpp
#define kernel_hpp

#include "interval.hpp"
#include <CGAL/MP_Float.h>
#include <CGAL/Cartesian.h>

namespace ra::geometry{

using namespace ra::math;

// A geometry kernel with robust predicates.
template <class R >
class Kernel
{
    public:
    // The type used to represent real numbers.
    using Real = R;
    // The type used to represent points in two dimensions.
    using Point = typename CGAL::Cartesian<R>::Point_2 ;
    // The type used to represent vectors in two dimensions.
    using Vector = typename CGAL::Cartesian<R>::Vector_2 ;
    // The possible outcomes of an orientation test.
    
    enum class Orientation : int {
    right_turn = -1,
    collinear = 0,
    left_turn = 1,
    };

    // The possible outcomes of an oriented-side-of test.
    enum class Oriented_side : int {
    on_negative_side = -1,
    on_boundary = 0,
    on_positive_side = 1,
    };
    // The set of statistics maintained by the kernel.
    struct Statistics {
    // The total number of orientation tests.
    std::size_t orientation_total_count ;
    // The number of orientation tests requiring exact
    // arithmetic.
    std::size_t orientation_exact_count ;
    // The total number of preferred-direction tests.
    std::size_t preferred_direction_total_count ;
    // The number of preferred-direction tests requiring
    // exact arithmetic.
    std::size_t preferred_direction_exact_count ;
    // The total number of side-of-oriented-circle tests.
    std::size_t side_of_oriented_circle_total_count ;
    // The number of side-of-oriented-circle tests
    // requiring exact arithmetic.
    std::size_t side_of_oriented_circle_exact_count ;
    };
    // Since a kernel object is stateless, construction and
    // destruction are trivial.
    Kernel() {}
    ~Kernel() = default;
    // The kernel type is both movable and copyable.
    // Since a kernel object is stateless, a copy/move operation
    // is trivial.
    Kernel(const Kernel&) = default;
    Kernel& operator=(const Kernel&) = default;
    Kernel (Kernel&&) = default;
    Kernel& operator=(Kernel&&)= default;
    // Determines how the point c is positioned relative to the
    // directed line through the points a and b (in that order).
    // Precondition: The points a and b have distinct values.
    Orientation orientation (const Point & a , const Point & b ,
    const Point & c )
    {
      ++(stats_.orientation_total_count);
      try
      {
        return orientation_calc<interval<Real>>(a,b,c);
      }
      catch(indeterminate_result& e)
      {
        ++(stats_.orientation_exact_count);
        return orientation_calc<CGAL::MP_Float>(a,b,c);
      }
    }
    // Determines how the point d is positioned relative to the
    // oriented circle passing through the points a, b, and c
    // (in that order).
    // Precondition: The points a, b, and c are not collinear.
    Oriented_side side_of_oriented_circle (const Point & a ,
    const Point & b , const Point & c , const Point & d )
    {
       ++(stats_.side_of_oriented_circle_total_count);
      try
      {
        return circle_side_calc<interval<Real>>(a,b,c,d);
      }
      catch(indeterminate_result& e)
      {
        // std::cout << "Youa re here" << std::endl;
        ++(stats_.side_of_oriented_circle_exact_count);
        return circle_side_calc<CGAL::MP_Float>(a,b,c,d);
      }
    }
    // Determines if, compared to the orientation of line
    // segment cd, the orientation of the line segment ab is
    // more close, equally close, or less close to the
    // orientation of the vector v.
    // The value returned is 1, 0, or -1 if, compared to the
    // orientation of cd, the orientation of ab is more close,
    // equally close, or less close to the orientation of v,
    // respectively.
    // Precondition: The points a and b have distinct values; the
    // points c and d have distinct values; the vector v is not
    // the zero vector.
    int preferred_direction (const Point & a , const Point & b ,
    const Point & c , const Point & d , const Vector & v )
    {
       ++(stats_.preferred_direction_total_count);
      try
      {
        return preferred_dir<interval<Real>>(a,b,c,d,v);
      }
      catch(indeterminate_result& e)
      {
        ++(stats_.preferred_direction_exact_count);
        return preferred_dir<CGAL::MP_Float>(a,b,c,d,v);
      }
    }
    // Tests if the quadrilateral with vertices a, b, c, and d
    // specified in CCW order is strictly convex.
    // Precondition: The vertices a, b, c, and d have distinct
    // values and are specified in CCW order.
    bool is_strictly_convex_quad (const Point & a , const Point & b ,
    const Point & c , const Point & d )
    {
      Point arr [] = {a, b, c, d, a};
      for(int i = 0; i<3; ++i)
      {
        if (orientation(arr[i],arr[i+1],arr[i+2]) != Orientation::left_turn)
        {
          return false;
        }
      }
      return true;
    }
    // Tests if the flippable edge, with endpoints a and c and
    // two incident faces abc and acd, is locally Delaunay.
    // Precondition: The points a, b, c, and d have distinct
    // values; the quadrilateral abcd must be strictly convex.
    bool is_locally_delaunay_edge (const Point & a , const Point & b ,
    const Point & c , const Point & d )
    {
      if(side_of_oriented_circle(a,b,c,d) == Oriented_side::on_positive_side)
      {
        return false;
      }
      return true;
    }
    // Tests if the flippable edge, with endpoints a and c and
    // two incident faces abc and acd, has the preferred-directions
    // locally-Delaunay property with respect to the first and
    // second directions u and v.
    // Precondition: The points a, b, c, and d have distinct values;
    // the vectors u and v are not zero vectors; the vectors u and
    // v are neither parallel nor orthogonal.
    bool is_locally_pd_delaunay_edge (const Point & a ,
    const Point & b , const Point & c , const Point & d ,
    const Vector & u , const Vector & v )
    {
      int val;
      if (side_of_oriented_circle(a,b,c,d) == Oriented_side::on_negative_side)
      {
        val = 1;
      } else if(side_of_oriented_circle(a,b,c,d) == Oriented_side::on_positive_side)
      {
        val = 0;
      } else{
        if(preferred_direction(b,c,a,d,u) > 0 || (preferred_direction(b,c,a,d,u) == 0 && preferred_direction(b,c,a,d,v) > 0))
        {
          val = 1;
        }
        else{
          val = 0;
        }
      }

      return val == 1;
    }
    // Clear (i.e., set to zero) all kernel statistics.
    static void clear_statistics ()
    {
      stats_.orientation_total_count = 0;
      stats_.orientation_exact_count = 0;
      stats_.preferred_direction_total_count = 0;
      stats_.preferred_direction_exact_count = 0;
      stats_.side_of_oriented_circle_total_count = 0;
      stats_.side_of_oriented_circle_exact_count = 0;
    }
    // Get the current values of the kernel statistics.
    static void get_statistics ( Statistics & statistics )
    {
      statistics.orientation_total_count = stats_.orientation_total_count;
      statistics.orientation_exact_count = stats_.orientation_exact_count;
      statistics.preferred_direction_total_count = stats_.preferred_direction_total_count;
      statistics.preferred_direction_exact_count = stats_.preferred_direction_exact_count;
      statistics.side_of_oriented_circle_total_count = stats_.side_of_oriented_circle_total_count;
      statistics.side_of_oriented_circle_exact_count = stats_.side_of_oriented_circle_exact_count;
    }

    private:
    static Statistics stats_;
    
    template<class T>
    Orientation orientation_calc(const Point &a, const Point &b, const Point &c)
    {
      T xa(a.x());
      T ya(a.y());
      T xb(b.x());
      T yb(b.y());
      T xc(c.x());
      T yc(c.y());

      T first(xa - xc);
      T second(xb - xc);
      T third(ya - yc);
      T fourth(yb - yc);

      T det = (first * fourth) - (second * third);
      return convert_orientation(det.sign());
      
    }

    Orientation convert_orientation(int s)
    {
      if(s < 0)
      {
        return Orientation::right_turn;
      }
      else if (s > 0)
      {
        return Orientation::left_turn;        
      }
      else
      {
        return Orientation::collinear;        
      }
    }

    Oriented_side convert_oriented_side(int s)
    {
      if(s < 0)
      {
        return Oriented_side::on_negative_side;
      }
      else if (s > 0)
      {
        return Oriented_side::on_positive_side;        
      }
      else
      {
        return Oriented_side::on_boundary;        
      }
    }

    template <class T>
    typename CGAL::Cartesian<T>::Point_3 make_3d_point(const Point& pt)
    {
      T x_(pt.x());
      T y_(pt.y());

      typename CGAL::Cartesian<T>::Point_3 p(x_, y_, (x_ * x_) + (y_ * y_));
      return p;
    }

    template<class T>
    Oriented_side circle_side_calc(const Point &a, const Point &b, const Point &c, const Point& d)
    {
      auto aa  = make_3d_point<T>(a);
      auto bb =  make_3d_point<T>(b);
      auto cc =  make_3d_point<T>(c);
      auto dd =  make_3d_point<T>(d);

      T e1(aa.x() - dd.x());
      T e2(bb.x() - dd.x());     
      T e3(cc.x() - dd.x());
      T e4(aa.y() - dd.y());
      T e5(bb.y() - dd.y());
      T e6(cc.y() - dd.y());
      T e7(aa.z() - dd.z());
      T e8(bb.z() - dd.z());
      T e9(cc.z() - dd.z());

      T det = determinants_3d(e1,e2,e3,e4,e5,e6,e7,e8,e9);
      std::cout << det << std::endl;
      std::cout << "****************" << std::endl;
      return convert_oriented_side(det.sign());
    }

    template <class T>
    T determinants_3d(T a, T b, T c, T d, T e, T f,T g,T h,T i)
    {
      T det = a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
      return det;
    }

    

    

    template <class T>
    T sqr(const typename CGAL::Cartesian<T>::Point_2 a)
    {
      T x(a.x());
      T y(a.y());
      T dist = x*x +  y*y;
      return dist;    
    }

    template <class T>
    typename CGAL::Cartesian<T>::Point_2 distance_sqrt(const Point &a, const Point& b)
    {
      T x(a.x() - b.x());
      T y(a.y() - b.y());
      typename CGAL::Cartesian<T>::Point_2 pt(x,y);
      return pt;
    }

    template <class T>
    int preferred_dir(const Point & a , const Point & b , const Point & c , const Point & d , const Vector & v )
    {
      auto b_a = distance_sqrt<T>(b,a);
      auto d_c = distance_sqrt<T>(d,c);

      T d_c_2 = sqr<T>(d_c); 
      T b_a_2 = sqr<T>(b_a);

      T b_a_x = b_a.x();
      T b_a_y = b_a.y();
      T d_c_x = d_c.x();
      T d_c_y = d_c.y();
      T v_x = v.x();
      T v_y = v.y();

      T b_a_v = (b_a_x * v_x) + (b_a_y * v_y);
      T d_c_v = (d_c_x * v_x) + (d_c_y * v_y);
            
      T dir = d_c_2*(b_a_v * b_a_v) - b_a_2*(d_c_v*d_c_v);
      return dir.sign();
    }

};
    template<typename T>
    typename Kernel<T>::Statistics Kernel<T>::stats_ = {0,0,0,0,0,0};
}

#endif