#ifndef NEIGHBORHOODREGIONGROWING_H
#define NEIGHBORHOODREGIONGROWING_H

#include <openipDS/PixelSet1.h>
#include <openipDS/StructuringElement2s.h>

#include <openipLL/RegionGrowing.h>

enum growingMode
{
    REGION_GROWING_LESS,
    REGION_GROWING_GREATER,
    REGION_GROWING_LESS_OR_EQUAL,
    REGION_GROWING_GREATER_OR_EQUAL,
    REGION_GROWING_EQUAL,
    REGION_GROWING_THRESHOLD,
    REGION_GROWING_HARD_THRESHOLD
};

template<typename INPUT>
class NeighborhoodRegionGrowing: public RegionGrowing<INPUT>
{
public:
    NeighborhoodRegionGrowing(int growingMode, float threshold= 0);

    NeighborhoodRegionGrowing(const NeighborhoodRegionGrowing& nrg);

    ~NeighborhoodRegionGrowing();

    virtual void init();

    virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL);

    int growingMode;

    Image<float> mode;

    float threshold;
};

template<typename INPUT>
NeighborhoodRegionGrowing<INPUT>::NeighborhoodRegionGrowing(int growingMode, float threshold)
    : RegionGrowing<INPUT>()
{
    this->growingMode= growingMode;
    this->threshold= threshold;
}

template<typename INPUT>
NeighborhoodRegionGrowing<INPUT>::NeighborhoodRegionGrowing(const NeighborhoodRegionGrowing& nrg)
    : RegionGrowing<INPUT>(nrg)
{
    this->growingMode= nrg.growingMode;
    this->threshold= nrg.threshold;
}

template<typename INPUT>
NeighborhoodRegionGrowing<INPUT>::~NeighborhoodRegionGrowing()
{
}

template<typename INPUT>
void NeighborhoodRegionGrowing<INPUT>::init()
{

}

template<typename INPUT>
void NeighborhoodRegionGrowing<INPUT>::doRG(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> &seed, Image<unsigned char> *roi)
{
    Image<float> mode;
    mode.resizeImage(input);
    mode= input;
    //float max= mode.getMax(roi);
    /*for ( int i= 0; i < mode.n; ++i )
      mode(i)= mode(i)/max;*/
    //mode.normalize(0,1);
    Image<unsigned char> tmp;
    tmp= seed;
    tmp= 0;

    PixelSet1 seeds;
    PixelSet1 seedsNew;
    for ( unsigned int i= 0; i < seed.n; ++i )
        if ( seed(i) )
            seeds.push_back(i);

    PixelSet1 final;
    StructuringElementSquare ses(3, seed.columns);

    output= seed;
    int total= 0;
    for ( unsigned int i= 0; i < seed.size(); ++i )
      if ( seed(i) )
	++total;

    if ( total > input.size()*0.2 )
    {
      output= 255;
      return;
    }
    
    //writeImage("seed.bmp", seed);
    //tprintf("starting iteration\n");
    unsigned int i, j, k, sum;
    while ( 1 )
    {
        final.clear();

	for ( i= 0; i < seeds.size(); ++i )
	{
	  if ( output(seeds(i)) && (!roi || (*roi)(seeds(i))) )
          {
                for ( j= 0; j < ses.size(); ++j )
		{
		    int actual= seeds(i) + ses(j);
                    if ( actual >= 0 && actual < int(output.n) && !output(actual) && !tmp(actual) )
                    {
			switch(growingMode)
			{
			    case REGION_GROWING_LESS:
			      if ( mode(actual) < mode(seeds(i)) && !tmp(actual) )
			      {
				final.push_back(actual);
				tmp(actual)= 1;
			      }
			      break;
			    case REGION_GROWING_GREATER:
			      if ( mode(actual) > mode(seeds(i)) && !tmp(actual) )
			      {
				final.push_back(actual);
				tmp(actual)= 1;
			      }
			      break;
			    case REGION_GROWING_LESS_OR_EQUAL:
			      if ( mode(actual) <= mode(seeds(i)) && !tmp(actual) )
			      {
				final.push_back(actual);
				tmp(actual)= 1;
			      }
			      break;
			    case REGION_GROWING_GREATER_OR_EQUAL:
				if ( mode(actual) >= mode(seeds(i)) && !tmp(actual) )
				{
				  final.push_back(actual);
				  tmp(actual)= 1;
				}
				break;
			    case REGION_GROWING_EQUAL:
				if ( mode(actual) == mode(seeds(i)) && !tmp(actual) )
				{
				  final.push_back(actual);
				  tmp(actual)= 1;
				}
				break;
			    case REGION_GROWING_THRESHOLD:
				if ( fabs(mode(actual) - mode(seeds(i))) < threshold && !tmp(actual) )
				{
				  final.push_back(actual);
				  tmp(actual)= 1;
				}
				break;
			    case REGION_GROWING_HARD_THRESHOLD:
				if ( mode(actual) > threshold && !tmp(actual) )
				{
				  final.push_back(actual);
				  tmp(actual)= 1;
				}
				break;
			}
			
			if ( !tmp(actual) )
			{
			  sum= 0;
			  for ( k= 0; k < ses.size(); ++k )
			    if ( actual + ses(k) >= 0 && actual + ses(k) < int(output.n) && output(actual + ses(k)) )
			      ++sum;
			  if ( sum >= 6 )
			  {
			    final.push_back(actual);
			    tmp(actual)= 1;
			  }
			}
                    }
		}
	}
	
        /*for ( int i= 0; i < output.n; ++i )
        {
            if ( !output(i) && (!roi || (*roi)(i)) )
            {
                flag= false;
                for ( unsigned int j= 0; j < ses.size(); ++j )
                    if ( i + ses(j) >= 0 && i + ses(j) < output.n )
                    {
                        if ( output(i + ses(j)) )
                        {
                            switch(growingMode)
                            {
                                case REGION_GROWING_LESS:
                                if ( mode(i) < mode(i + ses(j)) )

                                    final.push_back(i);
                                    flag= true;

                                    break;
                                case REGION_GROWING_GREATER:
                                if ( mode(i) > mode(i + ses(j)) )

                                    final.push_back(i);
                                    flag= true;

                                    break;
                                case REGION_GROWING_LESS_OR_EQUAL:
                                if ( mode(i) <= mode(i + ses(j)) )

                                    final.push_back(i);
                                    flag= true;

                                    break;
                                case REGION_GROWING_GREATER_OR_EQUAL:
                                if ( mode(i) >= mode(i + ses(j)) )

                                    final.push_back(i);
                                    flag= true;

                                    break;
                                case REGION_GROWING_EQUAL:
                                if ( mode(i) == mode(i + ses(j)) )

                                    final.push_back(i);
                                    flag= true;

                                    break;
                                case REGION_GROWING_THRESHOLD:
                                if ( fabs(mode(i) - mode(i + ses(j))) < threshold )

                                    final.push_back(i);
                                    flag= true;

                                    break;
                                case REGION_GROWING_HARD_THRESHOLD:
                                if ( mode(i) > threshold )
                                    final.push_back(i);
                                    flag= true;

                                    break;
                            }
                        }
                        if ( flag )
                            break;
                    }

                int sum= 0;
                for ( unsigned int j= 0; j < ses.size(); ++j )
                    if ( i + ses(j) >= 0 && i + ses(j) < output.n )
                        if ( output(i + ses(j)) )
                            ++sum;
                if ( sum >= 6 )
                    final.push_back(i);
            }*/
        }

        //printf("final.size()= %d\n", final.size());

        total+= final.size();
        
	if ( total > input.size()*0.2 )
	{
	  output= 255;
	  return;
	}
	
        if ( final.size() == 0 )
            break;

        for ( unsigned int j= 0; j < final.size(); ++j )
            output(final(j))= 255;
	seeds= final;
   }
   //mode.resizeImage(1, 1);
}

#endif // NEIGHBORHOODREGIONGROWING_H
