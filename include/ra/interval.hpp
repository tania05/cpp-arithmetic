#ifndef interval_hpp
#define interval_hpp

namespace ra::math {
struct indeterminate_result : public std::runtime_error
{
using std::runtime_error::runtime_error;
};

struct statistics {
// The total number of indeterminate results encountered.
unsigned long indeterminate_result_count ;
// The total number of interval arithmetic operations.
unsigned long arithmetic_op_count ;
};


}

#endif