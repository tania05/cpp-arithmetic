#ifndef interval_hpp
#define interval_hpp

#include <iostream>
#include <string>

namespace ra::math {


struct indeterminate_result : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};


template <class T>
class interval{
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
      lower_bound = lower_bound + other.lower_bound;
      upper_bound = upper_bound + other.upper_bound;
      ++stats_.arithmetic_op_count;
      return *this;
    }

    //TODO
    interval& operator-=(const interval& other)
    {
      lower_bound = lower_bound - other.upper_bound;
      upper_bound = upper_bound - other.lower_bound;
      ++stats_.arithmetic_op_count;
      return *this;
    }

    //TODO
    //remember to check what happens if we operate on itself
    interval& operator*=(const interval& other)
    { 
      const real_type arr [] = {lower_bound*other.lower_bound, lower_bound*other.upper_bound, upper_bound*other.lower_bound, upper_bound*other.upper_bound};      
      lower_bound = get_min(arr);
      upper_bound = get_max(arr);
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
    if (a.lower() < b.lower() && a.upper() < b.upper())
    {
      return true;
    }
    else if (a.lower() >= b.lower() && a.upper() >= b.upper())
    {
      return false;
    }
    else
    {
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

#endif