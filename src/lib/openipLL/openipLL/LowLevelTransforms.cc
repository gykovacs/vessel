#include <openipLL/LowLevelTransforms.h>

namespace openip
{
    float meanOrientation(Vector<float> orientations, Vector<float> magnitudes)
    {
      float d= M_PI/180;
      
      float dist;
      float minDist= FLT_MAX;
      float minOrientation= 0;
      
      float theta= -M_PI;
      float a, b, c;
      while ( theta < M_PI )
      {
	dist= 0;
	for ( unsigned int i= 0; i < orientations.size(); ++i )
	{
	  a= fabs(orientations(i) - theta);
	  b= fabs(orientations(i) + M_PI - theta);
	  c= fabs(orientations(i) - M_PI - theta);
	  
	  if ( a <= b && a <= c )
	    dist+= a*a;
	  else if ( b <= a && b <= c )
	    dist+= b*b;
	  else if ( c <= a && c <= b )
	    dist+= c*c;
	}
	
	if ( dist < minDist )
	{
	  minDist= dist;
	  minOrientation= theta;
	}
	
	theta+= d;
      }
      
      Vector<float> adjusted;
      theta= minOrientation;
      for ( unsigned int i= 0; i < orientations.size(); ++i )
      {
	a= fabs(orientations(i) - theta);
	b= fabs(orientations(i) + M_PI - theta);
	c= fabs(orientations(i) - M_PI - theta);
	
	if ( a <= b && a <= c )
	  adjusted.push_back(orientations(i));
	else if ( b <= a && b <= c )
	  adjusted.push_back(orientations(i)+M_PI);
	else if ( c <= a && c <= b )
	  adjusted.push_back(orientations(i)-M_PI);
      }
      
      float ats= 0, atc= 0, sum= 0;
      for ( unsigned int i= 0; i < adjusted.size(); ++i )
      {
	ats+= sin(adjusted(i))*magnitudes(i);
	atc+= cos(adjusted(i))*magnitudes(i);
	sum+= magnitudes(i);
      }
      
      ats/= sum;
      atc/= sum;
      
      //return minOrientation;
      return atan2(atc, ats);
    }
}