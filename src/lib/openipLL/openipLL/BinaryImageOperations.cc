#include <openipLL/BinaryImageOperations.h>

namespace openip
{
  PixelSet1 getEndPoints(Image<unsigned char>& input)
  {
    StructuringElementSquare ses(3, input.columns);

    PixelSet1 p;
    p.stride= input.columns;

    int n= 0;

    for ( int i= 0; i < int(input.n); ++i )
    {
      if ( input(i) )
      {
	n= 0;
	for ( unsigned int j= 0; j < ses.size(); ++j )
	  if ( ses(j) != 0 && i + ses(j) >= 0 && i + ses(j) < int(input.n) && input(i + ses(j)) )
	    ++n;
	if ( n == 1 )
	  p.push_back(i);
      }
    }
    
    return p;
  }
  
  int numberOf8Neighbors(Image<unsigned char>& input, int n)
  {
    StructuringElementSquare ses(3, input.columns);
    
    int res= 0;
    for ( unsigned int j= 0; j < ses.size(); ++j )
      if ( ses(j) != 0 && n + ses(j) >= 0 && n + ses(j) < int(input.n) )
	if ( input(n + ses(j)) )
	  ++res;
	
    return res;
  }
}