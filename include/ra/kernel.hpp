#ifndef kernel_hpp
#define kernel_hpp

namespace ra :: geometry {
// A geometry kernel with robust predicates.
template <class R >
class Kernel
{
    public:
    // The type used to represent real numbers.
    using Real = R;
    // The type used to represent points in two dimensions.
    using Point = typename CGAL :: Cartesian <R >:: Point_2 ;
    // The type used to represent vectors in two dimensions.
    using Vector = typename CGAL :: Cartesian <R >:: Vector_2 ;
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
    std :: size_t orientation_total_count ;
    // The number of orientation tests requiring exact
    // arithmetic.
    std :: size_t orientation_exact_count ;
    // The total number of preferred-direction tests.
    std :: size_t preferred_direction_total_count ;
    // The number of preferred-direction tests requiring
    // exact arithmetic.
    std :: size_t preferred_direction_exact_count ;
    // The total number of side-of-oriented-circle tests.
    std :: size_t side_of_oriented_circle_total_count ;
    // The number of side-of-oriented-circle tests
    // requiring exact arithmetic.
    std :: size_t side_of_oriented_circle_exact_count ;
    };
    // Since a kernel object is stateless, construction and
    // destruction are trivial.
    Kernel ();
    ˜ Kernel ();
    // The kernel type is both movable and copyable.
    // Since a kernel object is stateless, a copy/move operation
    // is trivial.
    Kernel (const Kernel &);
    Kernel & operator=(const Kernel &);
    Kernel ( Kernel &&);
    Kernel & operator=( Kernel &&);
    // Determines how the point c is positioned relative to the
    // directed line through the points a and b (in that order).
    // Precondition: The points a and b have distinct values.
    Orientation orientation (const Point & a , const Point & b ,
    const Point & c );
    // Determines how the point d is positioned relative to the
    // oriented circle passing through the points a, b, and c
    // (in that order).
    // Precondition: The points a, b, and c are not collinear.
    Oriented_side side_of_oriented_circle (const Point & a ,
    const Point & b , const Point & c , const Point & d );
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
    const Point & c , const Point & d , const Vector & v );
    // Tests if the quadrilateral with vertices a, b, c, and d
    // specified in CCW order is strictly convex.
    // Precondition: The vertices a, b, c, and d have distinct
    // values and are specified in CCW order.
    bool is_strictly_convex_quad (const Point & a , const Point & b ,
    const Point & c , const Point & d );
    // Tests if the flippable edge, with endpoints a and c and
    // two incident faces abc and acd, is locally Delaunay.
    // Precondition: The points a, b, c, and d have distinct
    // values; the quadrilateral abcd must be strictly convex.
    bool is_locally_delaunay_edge (const Point & a , const Point & b ,
    const Point & c , const Point & d );
    // Tests if the flippable edge, with endpoints a and c and
    // two incident faces abc and acd, has the preferred-directions
    // locally-Delaunay property with respect to the first and
    // second directions u and v.
    // Precondition: The points a, b, c, and d have distinct values;
    // the vectors u and v are not zero vectors; the vectors u and
    // v are neither parallel nor orthogonal.
    bool is_locally_pd_delaunay_edge (const Point & a ,
    const Point & b , const Point & c , const Point & d ,
    const Vector & u , const Vector & v );
    // Clear (i.e., set to zero) all kernel statistics.
    static void clear_statistics ();
    // Get the current values of the kernel statistics.
    static void get_statistics ( Statistics & statistics );
};
}

#endif