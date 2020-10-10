#ifndef POOL_ADJACENT_VIOLATORS_ALGORITHM_H
#define POOL_ADJACENT_VIOLATORS_ALGORITHM_H

#include <openipDS/Vector.h>

namespace openip
{
  template<typename INPUT, typename OUTPUT>
  class PoolAdjacentViolatorsAlgorithm
  {
    public:
      PoolAdjacentViolatorsAlgorithm(int decreasing= 0, int fromright= 0);

      void apply(Vector<INPUT>& input, Vector<OUTPUT>& output, Vector<unsigned char>* roi= NULL, Vector<unsigned char>* support= NULL);
      
      void apply(Vector<INPUT>& input, Vector<INPUT> weights, Vector<OUTPUT>& output, Vector<unsigned char>* roi= NULL, Vector<unsigned char>* support= NULL);
      
      int decreasing;
      int fromright;
  };
  
  template<typename INPUT, typename OUTPUT>
  PoolAdjacentViolatorsAlgorithm<INPUT, OUTPUT>::PoolAdjacentViolatorsAlgorithm(int decreasing, int fromright)
  {
    this->decreasing= decreasing;
    this->fromright= fromright;
  }
  
  template<typename INPUT, typename OUTPUT>
  void PoolAdjacentViolatorsAlgorithm<INPUT, OUTPUT>::apply(Vector<INPUT>& input, Vector<OUTPUT>& output, Vector<unsigned char>* /*roi*/, Vector<unsigned char>* /*support*/)
  {
    //printf("pava: %d, %d\n", decreasing, fromright);
    double sum= 0;
    int n= 0;
    unsigned int j;
    output= input;
    if ( !fromright )
    {
      if ( !decreasing )
      {
	for ( unsigned int i= 0; i < input.size()-1; ++i )
	  if ( output(i) > input(i+1) )
	  {
	    sum= input(i+1);
	    n= 1;
	    j= i;
	    while ( output(j) > sum/n )
	    {
	      sum+= input(j);
	      n++;
	      --j;
	      if ( j < 0 )
		break;
	    }
	    ++j;
	    for ( ; j <= i+1; ++j )
	      output(j)= sum/n;
	  }
      }
      else
      {
	for ( unsigned int i= 0; i < input.size()-1; ++i )
	  if ( output(i) < input(i+1) )
	  {
	    sum= input(i+1);
	    n= 1;
	    j= i;
	    while ( output(j) < sum/n )
	    {
	      sum+= input(j);
	      n++;
	      --j;
	      if ( j < 0 )
		break;
	    }
	    ++j;
	    for ( ; j <= i+1; ++j )
	      output(j)= sum/n;
	  }
      }
    }
    else
    {
      if ( !decreasing )
      {
	for ( unsigned int i= input.size()-1; i > 0; --i )
	  if ( output(i) < input(i-1) )
	  {
	    sum= input(i-1);
	    n= 1;
	    j= i;
	    while ( output(j) < sum/n )
	    {
	      sum+= input(j);
	      n++;
	      ++j;
	      if ( j >= input.size() )
		break;
	    }
	    --j;
	    for ( ; j >= i-1; --j )
	      output(j)= sum/n;
	  }
      }
      else
      {
	for ( unsigned int i= input.size()-1; i > 0; --i )
	  if ( output(i) > input(i-1) )
	  {
	    sum= input(i-1);
	    n= 1;
	    j= i;
	    while ( output(j) > sum/n )
	    {
	      sum+= input(j);
	      n++;
	      ++j;
	      if ( j >= input.size() )
		break;
	    }
	    --j;
	    for ( ; j >= i-1; --j )
	      output(j)= sum/n;
	  }
      }
    }
  }
  
  template<typename INPUT, typename OUTPUT>
  void PoolAdjacentViolatorsAlgorithm<INPUT, OUTPUT>::apply(Vector<INPUT>& input, Vector<INPUT> weights, Vector<OUTPUT>& output, Vector<unsigned char>* /*roi*/, Vector<unsigned char>* /*support*/)
  {
    //printf("pava: %d, %d\n", decreasing, fromright);
    double sum= 0, wsum= 0;
    int n= 0, j;
    output= input;
    //int pool= 0;
    if ( !fromright )
    {
      if ( !decreasing )
      {
          Vector<INPUT> g, w;
          Vector<int> next, prev;
          g.resize(input.size());
          w.resize(input.size());
          next.resize(input.size());
          prev.resize(input.size());
          
          for ( unsigned int i= 0; i < input.size(); ++i )
          {
              g(i)= input(i);
              w(i)= weights(i);
              next(i)= i+1;
              prev(i)= i-1;
          }
          
          int i= 0;
          while ( next[i] < int(input.size()) )
          {
              if ( g(i) > g(next(i)) )
              {
                  g(i)= (g(i)*w(i) + g(next(i))*w(next(i)))/(w(i) + w(next(i)));
                  w(i)= w(i) + w(next(i));
                  
                  int j= i;
                  while ( prev(j) >= 0 && g(i) < g(prev(j)) )
                  {
                      j= prev(j);
                      g(i)= (g(i)*w(i) + g(j)*w(j))/(w(i) + w(j));
                      w(i)= w(i) + w(j);
                  }
                  
                  g(j)= g(i);
                  w(j)= w(i);
                  
                  next(j)= next(next(i));
                  if ( next(j) < int(input.size()) )
                    prev(next(j))= j;
                  else
                      break;
                  i= j;
              }
              else
              {
                  i= next(i);
              }
              
          }
          
          unsigned int n= 0;
          i= 0;
          for ( n= 0; n < input.size(); )
          {
              for ( int l= 0; l < next[i] - i; ++l )
              {
                  output(n++)= g(i);
              }
              i= next[i];
          }
      }
      else
      {
	for ( int i= 0; i < int(input.size())-1; ++i )
	  if ( output(i) < input(i+1) )
	  {
	    sum= input(i+1)*weights(i+1);
	    wsum= weights(i+1);
	    n= 1;
	    j= i;
	    while ( output(j) < sum/wsum )
	    {
	      sum+= input(j)*weights(j);
	      n++;
	      wsum+= weights(j);
	      --j;
	      if ( j < 0 )
		break;
	    }
	    ++j;
	    for ( ; j <= i+1; ++j )
	      output(j)= sum/wsum;
	  }
      }
    }
    else
    {
      if ( !decreasing )
      {
	for ( int i= input.size()-1; i > 0; --i )
	  if ( output(i) < input(i-1) )
	  {
	    sum= input(i-1)*weights(i-1);
	    wsum= weights(i-1);
	    n= 1;
	    j= i;
	    while ( output(j) < sum/wsum )
	    {
	      sum+= input(j);
	      wsum+= weights(j);
	      n++;
	      ++j;
	      if ( j >= int(input.size()) )
		break;
	    }
	    --j;
	    for ( ; j >= i-1; --j )
	      output(j)= sum/wsum;
	  }
      }
      else
      {
	for ( int i= input.size()-1; i > 0; --i )
	  if ( output(i) > input(i-1) )
	  {
	    sum= input(i-1)*weights(i-1);
	    wsum= weights(i-1);
	    n= 1;
	    j= i;
	    while ( output(j) > sum/wsum )
	    {
	      sum+= input(j);
	      wsum+= weights(j);
	      n++;
	      ++j;
	      if ( j >= int(input.size()) )
		break;
	    }
	    --j;
	    for ( ; j >= i-1; --j )
	      output(j)= sum/wsum;
	  }
      }
    }
  }
  
}

#endif