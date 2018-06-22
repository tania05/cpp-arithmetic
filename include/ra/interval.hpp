#ifndef interval_hpp
#define interval_hpp

#include <iostream>
#include <string>
#include <cfenv>

namespace ra
{
namespace math {

class rounding_mode_saver {
    public:
    // Save the rounding mode.
    rounding_mode_saver () {}
    // Restore the rounding mode to the value that was saved at
    // the time of construction.
    ~rounding_mode_saver ()
    {
      if (std::fesetround(old_mode)) {abort();}
    }
    // The type is neither movable nor copyable.
    
    void round_down()
    {
      if (std::fesetround(FE_DOWNWARD)) {abort();}
    }

    void round_up()
    {
      if (std::fesetround(FE_UPWARD)) {abort();}      
    }
    rounding_mode_saver ( rounding_mode_saver &&) = delete;
    rounding_mode_saver (const rounding_mode_saver &) = delete;
    rounding_mode_saver & operator=( rounding_mode_saver &&) = delete;
    rounding_mode_saver & operator=(const rounding_mode_saver &) = delete;

    private:
    int old_mode = std::fegetround();
};

struct indeterminate_result : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

// struct statistics {
// 	// The total number of indeterminate results encountered.
// 	unsigned long indeterminate_result_count;
// 	// The total number of interval arithmetic operations.
// 	unsigned long arithmetic_op_count;
// };

template <class T>
class interval{
  public:
    using real_type = T;

    struct statistics {
      // The total number of indeterminate results encountered.
      unsigned long indeterminate_result_count ;
      // The total number of interval arithmetic operations.
      unsigned long arithmetic_op_count ;
    };


    interval(real_type real_val = real_type(0)) : lower_bound(real_val), upper_bound(real_val) { }

    interval(interval&&) = default;
    interval& operator=(interval&&) = default;

    interval(const interval&) = default;
    interval& operator=(const interval&) = default;

    interval(real_type lower_, real_type upper_ ) : lower_bound(lower_), upper_bound(upper_) 
    { 
      if(lower_ > upper_)
      {
        //what does it mean by implementation defined?
        upper_bound = lower_;
        lower_bound = upper_;
      }
    }
    
    ~interval() = default;

    interval& operator+=(const interval& other)
    { 
      rounding_mode_saver rms;

      rms.round_down();
      lower_bound = lower_bound + other.lower_bound;
      
      rms.round_up();      
      upper_bound = upper_bound + other.upper_bound;
      ++stats_.arithmetic_op_count;
      return *this;
    }

    //TODO
    //remember to check what happens if we operate on itself
    interval& operator-=(const interval& other)
    {
      auto tmp = other.lower_bound;
      rounding_mode_saver rms;

      rms.round_down();

      lower_bound = lower_bound - other.upper_bound;

      rms.round_up();
      upper_bound = upper_bound - tmp;
      ++stats_.arithmetic_op_count;
      return *this;
    }

    //TODO
    //remember to check what happens if we operate on itself
    interval& operator*=(const interval& other)
    { 
      rounding_mode_saver rms;

      rms.round_down();
      const real_type arr1 [] = {lower_bound*other.lower_bound, lower_bound*other.upper_bound, upper_bound*other.lower_bound, upper_bound*other.upper_bound};      
      
      rms.round_up();
      const real_type arr2 [] = {lower_bound*other.lower_bound, lower_bound*other.upper_bound, upper_bound*other.lower_bound, upper_bound*other.upper_bound};            
      
      
      lower_bound = get_min(arr1);

      upper_bound = get_max(arr2);
      ++stats_.arithmetic_op_count;
      return *this;
    }

    real_type lower() const
    {
      return lower_bound;
    }

    real_type upper() const
    {
      return upper_bound;
    }

    bool is_singleton() const
    {
      return lower_bound == upper_bound;
    }

    int sign() const
    {
      if (lower_bound < 0 && upper_bound < 0)
      {
        return -1;
      }
      else if (lower_bound > 0 && upper_bound > 0)
      {
        return 1;
      }
      else if (lower_bound == 0 && upper_bound == 0)
      {
        return 0;
      }
      else
      {
        ++stats_.indeterminate_result_count;
        throw indeterminate_result("Could not determine the sign");
      }
    }

    static void clear_statistics()
    {
      stats_.indeterminate_result_count = 0;
      stats_.arithmetic_op_count = 0;
    }

    static void get_statistics(statistics& stats)
    {
      stats.indeterminate_result_count = stats_.indeterminate_result_count;
      stats.arithmetic_op_count = stats_.arithmetic_op_count;
    }

  private:
    real_type lower_bound;
    real_type upper_bound;

    static statistics stats_;
    
    real_type get_min(const real_type * mins)
    {
      auto min = mins[0];

      for(int i = 1; i < 4; ++i)
      {
        auto elem = mins[i];
        if(elem < min)
        {
          min = elem;
        }
      }
      return min;
    }

    real_type get_max(const real_type * maxs)
    {
      auto max = maxs[0];

      for(int i = 1; i < 4; ++i)
      {
        auto elem = maxs[i];
        if(elem > max)
        {
          max = elem;
        }
      }
      return max;
    }

    template <typename R>
    friend bool operator<(const interval<R>&a, const interval<R>& b);
};

  template<typename T>
  typename interval<T>::statistics interval<T>::stats_ = {0,0};

  template<typename T>
  interval<T> operator+(const interval<T>& a, const interval<T>& b)
  {   
      interval<T> tmp(a);
      tmp.operator+=(b);
      return tmp;
  }

  //binary minus
  template<typename T>
  interval<T> operator-(const interval<T>&a, const interval<T>& b)
  {
      interval<T> tmp(a);
      tmp.operator-=(b);
      return tmp;
  }

  //binary multiply
  template<typename T>
  interval<T> operator*(const interval<T>&a, const interval<T>& b)
  {
      interval<T> tmp(a);
      tmp.operator*=(b);
      return tmp;
  }
  
  //less than
  template<typename T>
  bool operator<(const interval<T>&a, const interval<T>& b)
  {
    if (a.lower() < b.lower() && a.upper() < b.upper() && a.upper() < b.lower() && a.lower() < b.upper())
    {
      return true;
    }
    else if (a.lower() >= b.lower() && a.upper() >= b.upper() && a.upper() >= b.lower() && a.lower() >= b.upper())
    {
      return false;
    }
    else
    {
      ++interval<T>::stats_.indeterminate_result_count;
      throw indeterminate_result("Could not determine the interval");
    }
  }

  // template <typename T> 
  // std::string tostring(const T& number)
  // {
  //   std::ostringstream os;
  //   os << number;
  //   return os.str();
  // }
  
  //stream inserter
  template<typename T>
  std::ostream& operator<<(std::ostream& os, const interval<T>& i)
  {
      os << "[" << std::to_string(i.lower()) << "," << std::to_string(i.upper()) << "]";
      return os;
  }
}
}
#endif