#ifndef interval_hpp
#define interval_hpp

#include <iostream>

namespace ra::math {

template <class T>
class interval{
  public:
    using real_type = T;

    struct statistics statistics;

    interval(real_type real_val = real_type(0)) : lower_bound(real_val), upper_bound(real_val) { }

    interval(interval&&) = default;
    interval& operator=(interval&&) = default;

    interval(const interval&) = default;
    interval& operator=(interval&) = default;

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
      lower_bound = lower_bound + other.lower_bound;
      upper_bound = upper_bound + other.upper_bound;
      ++statistics.arithmetic_op_count;
      return *this;
    }

    //TODO
    interval& operator-=(const interval& other)
    {
      lower_bound = lower_bound - other.upper_bound;
      upper_bound = upper_bound - other.lower_bound;
      ++statistics.arithmetic_op_count;
      return *this;
    }

    //TODO
    //remember to check what happens if we operate on itself
    interval& operator*=(const interval& other)
    { 
      const real_type [] arr = {lower_bound*other.lower_bound, lower_bound*other.upper_bound, upper_bound*other.lower_bound, upper_bound*other.upper_bound};      
      lower_bound = get_min(arr);
      upper_bound = get_max(tmp);
      ++statistics.arithmetic_op_count;
      return *this;
    }

    real_type lower()
    {
      return lower_bound;
    }

    real_type upper()
    {
      return upper_bound;
    }

    bool is_singleton()
    {
      return lower_bound == upper_bound;
    }

    int sign()
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
        ++statistics.indeterminate_result_count;
        throw indeterminate_result("Sign contains range that's not trickly negative, positive or zero");
      }
    }

    static void clear_statistics()
    {
      statistics.indeterminate_result_count = 0;
      statistics.arithmetic_op_count = 0;
    }

    static void get_statistics(statistics& stats)
    {
      stats.indeterminate_result_count = statistics.indeterminate_result_count;
      stats.arithmetic_op_count = statistics.arithmetic_op_count;
    }

  private:
    real_type lower_bound;
    real_type upper_bound;
    
    real_type get_min(const real_type * mins)
    {
      auto min = mins[0];

      for(auto elem : mins)
      {
        if(elem < min)
        {
          min = elem;
        }
      }
      return min;
    }

    real_type get_max(const real_type * maxs)
    {
      real_type [] maxs = {a1*b1, a1*b2, a2*b1, a2*b2};
      auto max = maxs[0];

      for(auto elem : maxs)
      {
        if(elem > max)
        {
          max = elem;
        }
      }
      return max;
    }

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
};

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
  interval<T> operator<(const interval<T>&a, const interval<T>& b)
  {
    if (a.lower_bound < b.lower_bound && a.upper_bound < b.upper_bound)
    {
      return true;
    }
    else if (a.lower_bound >= b.lower_bound && a.upper_bound >= b.upper_bound)
    {
      return false;
    }
    else
    {
      throw indeterminate_result("First interval is not strictly less than or greater than or euqal to the second iterval");
    }
  }

  template <typename T> 
  string tostring(const T& number)
  {
    std::ostringstream os;
    os << number;
    return os.str();
  }
  
  //stream inserter
  template<typename T>
  std::ostream& operator<<(std::ostream& os, const interval<T>& i)
  {
      os << "[" << tostring(i.lower_bound) << "," << tostring(i.upper_bound) << "]";
      return os;
  }
}

#endif