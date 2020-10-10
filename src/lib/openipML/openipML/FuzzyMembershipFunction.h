#ifndef FUZZY_MEMBERSHIP_FUNCTION_H
#define FUZZY_MEMBERSHIP_FUNCTION_H

namespace openip
{
  template<typename T>
  class FuzzyMembershipFunction
  {
    public:
      FuzzyMembershipFunction();

      FuzzyMembershipFunction(const FuzzyMembershipFunction& f);

      virtual ~FuzzyMembershipFunction();

      virtual T fuzzy(T v);
  };
  
  template<typename T>
  FuzzyMembershipFunction<T>::FuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  FuzzyMembershipFunction<T>::FuzzyMembershipFunction(const FuzzyMembershipFunction& f)
  {
  }
  
  template<typename T>
  FuzzyMembershipFunction<T>::~FuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T FuzzyMembershipFunction<T>::fuzzy(T /*v*/)
  {
    return 0;
  }
}

#endif