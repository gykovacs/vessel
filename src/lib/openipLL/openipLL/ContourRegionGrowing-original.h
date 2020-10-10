#ifndef CONTOURREGIONGROWING_H
#define CONTOURREGIONGROWING_H

#include <openipDS/PixelSet1.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/MatchedGaborFilter2.h>
#include <openipDS/PowerGaborFilter2.h>

#include <openipLL/RegionGrowing.h>
#include <openipLL/ComponentLabeling.h>
#include <openipLL/ConnectingLineOperator.h>
#include <openipLL/EqualSlopeAtBoundaries.h>
#include <openipLL/thinning.h>
#include <openipLL/Region2Operations.h>

#include <limits>

namespace openip
{
    
  
    template<typename INPUT>
    class ContourRegionGrowingTowardsEqualSlope: public RegionGrowing<INPUT>
    {
    public:
        ContourRegionGrowingTowardsEqualSlope(int maxit= 20, int se= 5, bool iTerm= true, bool mTerm= true, bool fTerm= true, bool sTerm= true, bool aTerm= true, bool lTerm= true,
                                                                        float ip= 0, float mp= 0, float fp= 0, float sp= 0, float ap= 0, float lp= 0, float circ= 0, float imult= 1, float omult= 1, float nw= 0.5, bool mf= false);

        ContourRegionGrowingTowardsEqualSlope(const ContourRegionGrowingTowardsEqualSlope& nrg);

        ~ContourRegionGrowingTowardsEqualSlope();

        virtual void init();

        virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL);

        virtual Vector<Region2> extractOrderedContours(Region2 object, Image<unsigned char>& mask);

        virtual int findPoints(Region2 contour, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, Image<unsigned char>* roi= NULL);

        virtual float contourSmoothness(Vector<float>& contour, int i);

        virtual void saveMask(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Vector<unsigned char>& mask);

        void intensityTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& label, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi= NULL);

        void morphologyTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi= NULL);

        void averageTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi= NULL);

        void filterTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi= NULL);
	
	void lineTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi= NULL);

        void slopeTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi= NULL);

        void circularityConstraint(Image<unsigned char>& output, PixelSet1& final);
	
	void fillHoles(Image<unsigned char>& input);
	
	int shrink(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi);
	
	int optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration);
	
	int optimize2(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration);
	
	int optimize3(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration);
	
	float energy(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int n, Image<unsigned char>& labels2, int flag= 0);
	
	void analyze(Image<INPUT>& input, Image<unsigned char>& labels);

        int isOuterContour4(Image<unsigned char>& image, int i);
        int isOuterContour8(Image<unsigned char>& image, int i);
        int isInnerContour4(Image<unsigned char>& image, int i);
        int isInnerContour8(Image<unsigned char>& image, int i);
	
	int connectivityTerm();

        int maxit;
        int se;
        Image<unsigned char> saveR;
        Image<unsigned char> saveG;
        Image<unsigned char> saveB;
	Image<unsigned char> already;
	Image<unsigned char> slopeMask;
	Image<float> filterResponses;
	Image<float> maxResponses;
	Image<int> filterIndices;
	Image<unsigned char> newLabels;
	Image<unsigned char> thin;
	
	Image<unsigned char> labels_original;
	Image<unsigned char> labels_original2;

        Image<unsigned char> tmpImage;

        Vector<MatchedCCorrelationPowerGaborFilterR2<float, float>* > filters;
	Vector<MatchedCorrelationPowerGaborFilterR2<float, float>* > cfilters;
	Vector<float> fth;
        Vector<MatchedCCorrelationPowerGaborFilterSimpleR2<float, float>* > pfilters;
	Vector<MatchedCorrelationPowerGaborFilterSimpleR2<float, float>* > pcfilters;
	Vector<float> pfth;

        bool iTerm;
        bool mTerm;
        bool fTerm;
        bool sTerm;
        bool aTerm;
	bool lTerm;
	bool mf;
        
        float ip;
        float mp;
        float fp;
        float sp;
        float ap;
	float lp;
	float imult;
	float omult;
	float nw;
	float maxint;

        float circ;

        int nTerms;
	
	ConnectingLineOperator2<float, float> *clo;
	EqualSlopeAtBoundaries<float> *esab;
	EqualSlopeAtBoundaries<float> *esab2;
	ExtractRegions er;
	Image<unsigned char> subimage;
	Vector<Region2> result;
	Vector<Vector<int> > neighborhoods;
    };

    template<typename INPUT>
    ContourRegionGrowingTowardsEqualSlope<INPUT>::ContourRegionGrowingTowardsEqualSlope(int maxit, int se, bool iTerm, bool mTerm, bool fTerm, bool sTerm, bool aTerm, bool lTerm,
                                                                                                           float ip, float mp, float fp, float sp, float ap, float lp, float circ, float imult, float omult, float nw, bool mf)
        : RegionGrowing<INPUT>()
    {
        this->maxit= maxit;
        this->se= se;
        
        this->iTerm= iTerm;
        this->mTerm= mTerm;
        this->fTerm= fTerm;
        this->sTerm= sTerm;
        this->aTerm= aTerm;
	this->lTerm= lTerm;

        this->ip= ip;
        this->mp= mp;
        this->fp= fp;
        this->sp= sp;
        this->ap= ap;
	this->lp= lp;
	
	this->circ= circ;
	this->imult= imult;
	this->omult= omult;
	this->nw= nw;
	
	this->mf= mf;
        
        //printf("intensity: %d morphology: %d filter: %d slope: %d\n", iTerm, mTerm, fTerm, sTerm);

	if ( fTerm )
	{
	  /*for ( float lambda= 4; lambda <= 6; lambda+= 0.5 )
	    for ( float gamma= 0.5; gamma <= 0.8; gamma+= 0.1 )
	      for ( float p= 0.5; p <= 2; p*= 2 )
	      {
		filters.push_back(new MatchedCCorrelationPowerGaborFilterR2<float, float>(lambda/1.5, 0, 3.14/90.0, 3.14, lambda, 3.14, gamma, p));
		pfilters.push_back(new MatchedCCorrelationPowerGaborFilterSimpleR2<float, float>(lambda/1.5, 0, 3.14/90.0, 3.14, lambda, 3.14, gamma, p));
	      }*/
	  
	  //Transform2Set<float, float> f= *(generateTransform2Set<float, float>(std::string("thin.fdf"), std::string("feature")));
	  /*Transform2Set<float, float> f= *(generateTransform2Set<float, float>(std::string("gabor-features-bfss.fdf"), std::string("feature")));
	  
	  
	  
	  for ( int i= 0; i < f.size(); ++i )
	    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i)) != NULL )
	    {
	      PowerGaborRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i));
	      //filters.push_back(tmp->mgf);
	      if ( tmp->lambda <= 5 && tmp->gamma < 0.8 )
	      {
	      filters.push_back((new PowerGaborRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, 3.14/90.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	      fth.push_back(tmp->th1* this->fp);
	      }

	    }
	    else if (dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i)) != NULL)
	    {
	      PowerGaborSimpleRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i));
	      //pfilters.push_back(tmp->mgf);
	      if ( tmp->lambda <= 5 && tmp->gamma < 0.8 )
	      {
	      pfilters.push_back((new PowerGaborSimpleRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, 3.14/90.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	      pfth.push_back(tmp->th1* this->fp);
	      }
	    }*/
	    printf("number of filters: %d %d\n", filters.size(), pfilters.size());
	}
	 
	//printf("number of filters: %d %d %d %d\n", filters.size(), pfilters.size(), fth.size(), pfth.size());

	clo= new ConnectingLineOperator2<float, float>(0, 0.2, 3.14, 3, 1, 9, 0, 1, 0, 0, 1, 0, lp);
	esab= new EqualSlopeAtBoundaries<float>(3, 17, 0.4, 1);
	esab2= new EqualSlopeAtBoundaries<float>(3, 21, 0.6, 9);
	subimage.resizeImage(3,3);
	
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	neighborhoods.push_back(Vector<int>());
	
	neighborhoods(0).push_back(1);
	neighborhoods(0).push_back(3);
	neighborhoods(0).push_back(4);
	
	neighborhoods(1).push_back(0);
	neighborhoods(1).push_back(2);
	neighborhoods(1).push_back(3);
	neighborhoods(1).push_back(4);
	neighborhoods(1).push_back(5);
	
	neighborhoods(2).push_back(1);
	neighborhoods(2).push_back(4);
	neighborhoods(2).push_back(5);
	
	neighborhoods(3).push_back(0);
	neighborhoods(3).push_back(1);
	neighborhoods(3).push_back(4);
	neighborhoods(3).push_back(6);
	neighborhoods(3).push_back(7);
	
	neighborhoods(4).push_back(0);
	neighborhoods(4).push_back(1);
	neighborhoods(4).push_back(2);
	neighborhoods(4).push_back(3);
	neighborhoods(4).push_back(5);
	neighborhoods(4).push_back(6);
	neighborhoods(4).push_back(7);
	neighborhoods(4).push_back(8);
	
	neighborhoods(5).push_back(1);
	neighborhoods(5).push_back(2);
	neighborhoods(5).push_back(4);
	neighborhoods(5).push_back(7);
	neighborhoods(5).push_back(8);
	
	neighborhoods(6).push_back(3);
	neighborhoods(6).push_back(4);
	neighborhoods(6).push_back(7);
	
	neighborhoods(7).push_back(3);
	neighborhoods(7).push_back(4);
	neighborhoods(7).push_back(5);
	neighborhoods(7).push_back(6);
	neighborhoods(7).push_back(8);
	
	neighborhoods(8).push_back(4);
	neighborhoods(8).push_back(5);
	neighborhoods(8).push_back(7);
    }

    template<typename INPUT>
    ContourRegionGrowingTowardsEqualSlope<INPUT>::ContourRegionGrowingTowardsEqualSlope(const ContourRegionGrowingTowardsEqualSlope& nrg)
        : RegionGrowing<INPUT>(nrg)
    {
        this->maxit= nrg.maxit;
        this->se= nrg.se;
        
        this->iTerm= nrg.iTerm;
        this->mTerm= nrg.mTerm;
        this->fTerm= nrg.fTerm;
        this->sTerm= nrg.sTerm;
        this->aTerm= nrg.aTerm;
	this->lTerm= nrg.lTerm;
        
        this->ip= nrg.ip;
        this->mp= nrg.mp;
        this->fp= nrg.fp;
        this->sp= nrg.sp;
        this->ap= nrg.ap;
	this->lp= nrg.lp;
	
	this->circ= nrg.circ;

        this->filters= nrg.filters;
    }

    template<typename INPUT>
    ContourRegionGrowingTowardsEqualSlope<INPUT>::~ContourRegionGrowingTowardsEqualSlope()
    {
    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::init()
    {

    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::fillHoles(Image<unsigned char>& input)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);

      Vector<int> final;
      int n= 0;

      while ( 1 )
      {
	  final.clear();
	  for ( int j= 2*input.columns; j < input.n - 2*input.columns; ++j )
	  {
	      if ( !input(j) )
	      {
		  n= 0;
		  for ( int i= 0; i < ses.size(); ++i )
		  {
		      if ( j + ses(i) >= 0 && j + ses(i) < input.n )
		      {
			  if ( input(j + ses(i)) )
			      ++n;
		      }
		  }
		  if ( n > 6 )
		      final.push_back(j);
		  if ( input(j + 1) == 255 && input(j - 1) == 255 )
		    final.push_back(j);
		  if ( input(j + input.columns) == 255 && input(j - input.columns) == 255 )
		    final.push_back(j);
		  if ( input(j + input.columns + 1) == 255 && input(j - input.columns - 1) == 255 )
		    final.push_back(j);
		  if ( input(j - input.columns + 1) == 255 && input(j + input.columns - 1) == 255 )
		    final.push_back(j);
	      }
	  }

	  for ( int i= 0; i < final.size(); ++i )
	      input(final(i))= 255;
	  if ( final.size() == 0 )
	      break;
      }
    }
    
    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::isOuterContour4(Image<unsigned char>& image, int p)
    {
        Vector<int> pos4;
        pos4.push_back(1);
        pos4.push_back(-1);
        pos4.push_back(image.columns);
        pos4.push_back(-image.columns);

        for ( int i= 0; i < pos4.size(); ++i )
            if ( p + pos4(i) >= 0 && p + pos4(i) < image.n && image(p + pos4(i)) == 255 && image(p) != 255 )
                return 1;
        return 0;
    }

    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::isOuterContour8(Image<unsigned char>& image, int p)
    {
        Vector<int> pos4;
        pos4.push_back(1);
        pos4.push_back(-1);
        pos4.push_back(image.columns);
        pos4.push_back(-image.columns);
        pos4.push_back(image.columns + 1);
        pos4.push_back(-image.columns + 1);
        pos4.push_back(image.columns - 1);
        pos4.push_back(-image.columns - 1);

        for ( int i= 0; i < pos4.size(); ++i )
            if ( p + pos4(i) >= 0 && p + pos4(i) < image.n && image(p + pos4(i)) == 255 && image(p) != 255 )
                return 1;
        return 0;
    }

    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::isInnerContour4(Image<unsigned char>& image, int p)
    {
        Vector<int> pos4;
        pos4.push_back(1);
        pos4.push_back(-1);
        pos4.push_back(image.columns);
        pos4.push_back(-image.columns);

        for ( int i= 0; i < pos4.size(); ++i )
            if ( p + pos4(i) >= 0 && p + pos4(i) < image.n && !image(p + pos4(i)) && image(p) )
                return 1;
        return 0;
    }

    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::isInnerContour8(Image<unsigned char>& image, int p)
    {
        Vector<int> pos4;
        pos4.push_back(1);
        pos4.push_back(-1);
        pos4.push_back(image.columns);
        pos4.push_back(-image.columns);
        pos4.push_back(image.columns + 1);
        pos4.push_back(-image.columns + 1);
        pos4.push_back(image.columns - 1);
        pos4.push_back(-image.columns - 1);

        for ( int i= 0; i < pos4.size(); ++i )
            if ( p + pos4(i) >= 0 && p + pos4(i) < image.n && !image(p + pos4(i)) && image(p) )
                return 1;
        return 0;
    }

    template<typename INPUT>
    float ContourRegionGrowingTowardsEqualSlope<INPUT>::contourSmoothness(Vector<float>& contour, int idx)
    {
        Vector<int> pos;
        pos.push_back(3);
        pos.push_back(contour.size()-3);
        pos.push_back(4);
        pos.push_back(contour.size()-4);


        float result= 0;
        for ( int i= 0; i < pos.size(); ++i )
            result+= (contour(idx) - contour((idx + pos(i))%contour.size()));

        return result;
    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::saveMask(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Vector<unsigned char>& mask)
    {
        for ( int i= 0; i < contour.size(); ++i )
        {
            saveR(contour(i))= 255;
            if ( mask(i) )
            {
                saveG(contour(i))= 255;
                saveR(contour(i))= saveB(contour(i))= 0;
            }
            else
                saveG(contour(i))= saveB(contour(i))= 0;
        }
        writeImage("candidates.bmp", saveR, saveG, saveB);
    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::intensityTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi)
    {
	Vector<float> dilated;
        Vector<float> difference;
        Vector<float> tmp;
        Vector<int> se;

	tmp.resize(contour.size());
	tmp= 0;
        int erode= 0;

        for ( int i= 0; i <= (this->se); ++i )
        {
            se.push_back(i);
            se.push_back(profile.size() - i);
        }

        dilated.resize(profile.size());
        difference.resize(profile.size());

        float max= 0;
	int half= 15;
	float th= ip;
	int flag= 0;
	float value1;
	float value2;
	StructuringElementSquare ses(3);
	ses.updateStride(input.columns);
	/*ses.clear();
	ses.push_back(0);*/
        for ( int i= 0; i < profile.size(); ++i )
        {
	    flag= 0;
	    for ( int j= 1; j < half && !flag; ++j )
	      for ( int k= 0; k < ses.size(); ++k )
	      {
		if ( /*(*roi)(contour((i + j ) % profile.size()) + ses(k)) &&*/ input(contour((i + j ) % profile.size()) + ses(k)) > profile(i) + th )
		{
		  ++flag;
		  value1= input(contour((i + j ) % profile.size()) + ses(k)) - profile(i);
		  break;
		}
	      }
	      
	    for ( int j= 1; j < half && flag == 1; ++j )
	      for ( int k= 0; k < ses.size(); ++k )
	      if ( /*(*roi)(contour((i + profile.size() - j)%profile.size()) + ses(k)) &&*/ input(contour((i + profile.size() - j)%profile.size()) + ses(k)) > profile(i) + th )
	      {
		value2= input(contour((i + profile.size() - j)%profile.size()) + ses(k)) - profile(i);
		++flag;
		break;
	      }
	    if ( flag == 2 )
	    {
	      tmp(i)= (value1 + value2)/2;
	    }
        }
	
	float maxvalue= tmp.getMax();
	for ( int i= 0; i < tmp.size(); ++i )
	  if ( tmp(i)  > 0 )
	    mask(i)++;
    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::morphologyTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi)
    {
        /*Vector<float> dilated;
        Vector<float> difference;
        Vector<float> tmp;
        Vector<int> se;
        float th;

        int erode= 0;

        for ( int i= 0; i <= (this->se); ++i )
        {
            se.push_back(i);
            se.push_back(profile.size() - i);
        }

        dilated.resize(profile.size());
        difference.resize(profile.size());

        float max= 0;
        for ( int i= 0; i < profile.size(); ++i )
        {
            if ( erode )
                max= 256;
            else
                max= -1000;
            for ( int j= 0; j < se.size(); ++j )
            {
                if ( erode )
                {
                    if ( profile((i + se(j)) % profile.size()) < max && (!roi || (*roi)(contour((i + se(j))%profile.size()))))
                        max= profile((i + se(j)) % profile.size());
                }
                else
                {
                    if ( profile((i + se(j)) % profile.size()) > max && (!roi || (*roi)(contour((i + se(j))%profile.size()))))
                        max= profile((i + se(j)) % profile.size());
                }

            }
            dilated(i)= max;
        }

        if ( erode )
        {
            for ( int i= 0; i < profile.size(); ++i )
                difference(i)= profile(i) - dilated(i);
        }
        else
        {
            for ( int i= 0; i < profile.size(); ++i )
                difference(i)= dilated(i) - profile(i);
        }

        tmp= difference;
        sort(tmp.begin(), tmp.end());

        th= tmp((difference.size()-1)*perc);

        for ( int i= 0; i < difference.size(); ++i )
            if ( difference(i) >= th && difference(i) > 2 )
                mask(i)++;*/
	
    }

    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::shrink(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi)
    {
	Vector<int> toRemove;
        for ( int i= 0; i < labels.n; ++i )
        {
	    if ( isInnerContour4(labels, i) )
	    {
	      if ( esab2->apply(labels, i, input, 1) )
		toRemove.push_back(i);
	    }
        }
        
        for ( int i= 0; i < toRemove.size(); ++i )
	  labels(toRemove(i))= 0;
	return toRemove.size();
    }
    
    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::averageTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi)
    {
        
    }
    
    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration)
    {
      Vector<int> pixels;
      
      float inner= 0.4;
      float outer= 0.6;
      float b= 0.3;
      int i1= 0;
      int o1= 0;
      
      for ( int i= 0; i < input.n; ++i )
      {
	//if ( iteration % 2 )
	{
	  if ( isInnerContour4(labels, i) && esab->swidth(labels, i, input) > i1 )
	    pixels.push_back(i);
	}
	//else
	{
	  if ( isOuterContour8(labels, i) && esab->swidth(labels, i, input) > o1)
	    pixels.push_back(i);
	}
      }
      
      float tmp, err1, err2;
      int n= 0;
      
      /*for ( int k= 0; k < 1; ++k )
      {
	Vector<int> toChange;
	
	for ( int i= 0; i < pixels.size(); ++i )
	{
	  tmp= esab->apply2(labels, pixels(i), input);
	  
	  if ( labels(pixels(i)) )
	    err1= fabs(inner - tmp);
	  else
	    err1= fabs(outer - tmp);
	  
	  labels(pixels(i))= (labels(pixels(i)) == 255) ? 0 : 255;
	  
	  tmp= esab->apply2(labels, pixels(i), input);
	  
	  if ( labels(pixels(i)) )
	    err2= fabs(inner - tmp);
	  else
	    err2= fabs(outer - tmp);
	  
	  //printf("%f ", err2); 
	  
	  if ( err2 < err1 )
	    toChange.push_back(pixels(i));
	  
	  labels(pixels(i))= (labels(pixels(i)) == 255) ? 0 : 255;
	}
	
	for ( int i= 0; i < toChange.size(); ++i )
	  labels(toChange(i))= (labels(toChange(i)) == 255) ? 0 : 255;
      }*/
      
      float tmp2;
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      
      srand(time(NULL));
      
      for ( int k= 0; k < 100000; ++k )
      {
	int r= rand()%pixels.size();
	
	tmp= esab->apply2(labels, pixels(r), input);
	if ( labels(pixels(r)) )
	  err1= fabs(inner - tmp);
	else
	  err1= fabs(outer - tmp);
	
	int n= 0;
	for ( int j= 0; j < ses.size(); ++j )
	  if ( ses(j) != 0 && labels(pixels(r) + ses(j)) == labels(pixels(r)) )
	    ++n;
	err1+= (1 - n/8.0)*b;
	
	labels(pixels(r))= (labels(pixels(r)) == 255) ? 0 : 255;
	
	if ( labels(pixels(r)) )
	    err2= fabs(inner - tmp);
	  else
	    err2= fabs(outer - tmp);
	  
	n= 0;
	for ( int j= 0; j < ses.size(); ++j )
	  if ( ses(j) != 0 && labels(pixels(r) + ses(j)) == labels(pixels(r)) )
	    ++n;
	err2+= (1 - n/8.0)*b;
	
	if ( err1 < err2 )
	  labels(pixels(r))= (labels(pixels(r)) == 255) ? 0 : 255;
      }
      
      return 1;
    }
    
    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::connectivityTerm()
    {
      Vector<int> q;
      
      int color= 0;
      for ( int i= 0; i < subimage.n; ++i )
      {
	if ( subimage(i) == 255 )
	{
	  ++color;
	  q.push_back(i);
	  subimage(i)= color;
	  while ( q.size() != 0 )
	  {
	    int p= q(0);
	    for ( int j= 0; j < neighborhoods(p).size(); ++j )
	    {
	      if ( subimage(neighborhoods(p)(j)) == 255 )
	      {
		q.push_back(neighborhoods(p)(j));
		subimage(neighborhoods(p)(j))= color;
	      }
	    }
	    q.erase(q.begin());
	  }
	}
      }

      for ( int i= 0; i < subimage.n; ++i )
	if ( subimage(i) )
	  subimage(i)= 255;
      
      return color;
    }
    
    template<typename INPUT>
    float ContourRegionGrowingTowardsEqualSlope<INPUT>::energy(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int n, Image<unsigned char>& labels2, int flag)
    {
      float energy= 0;
      
      float desiredSlopeInner= 0.5;
      float desiredSlopeOuter= 0.8;
      float imult= this->imult;
      float omult= this->omult;
      
      float sWeight= 1;
      float nWeight= nw;
      float cWeight= 0;
      
      float slope= 0;
      
      int width= esab->swidth(labels2, n, input);
      width+= flag;
      
      if ( width <= 2 )
	return 0;
      
      if ( width == 1 )
      {
	desiredSlopeInner= 0.16*imult;
	desiredSlopeOuter= 0.57*omult;
      }
      if ( width == 2 )
      {
	desiredSlopeInner= 0.24*imult;
	desiredSlopeOuter= 0.62*omult;
      }
      if ( width == 3 )
      {
	desiredSlopeInner= 0.38*imult;
	desiredSlopeOuter= 0.74*omult;
      }
      if ( width == 4 )
      {
	desiredSlopeInner= 0.48*imult;
	desiredSlopeOuter= 0.79*omult;
      }
      if ( width == 5 )
      {
	desiredSlopeInner= 0.55*imult;
	desiredSlopeOuter= 0.83*omult;
      }
      if ( width == 6 )
      {
	desiredSlopeInner= 0.59*imult;
	desiredSlopeOuter= 0.84*omult;
      }
      if ( width == 7 )
      {
	desiredSlopeInner= 0.63*imult;
	desiredSlopeOuter= 0.83*omult;
      }
      if ( width == 8 )
      {
	desiredSlopeInner= 0.65*imult;
	desiredSlopeOuter= 0.81*omult;
      }
      if ( width == 9 )
      {
	desiredSlopeInner= 0.64*imult;
	desiredSlopeOuter= 0.80*omult;
      }
      if ( width == 10 )
      {
	desiredSlopeInner= 0.63*imult;
	desiredSlopeOuter= 0.79*omult;
      }
      if ( width == 11 )
      {
	desiredSlopeInner= 0.61*imult;
	desiredSlopeOuter= 0.77*omult;
      }
      if ( width == 12 )
      {
	desiredSlopeInner= 0.62*imult;
	desiredSlopeOuter= 0.79*omult;
      }
      if ( width == 13 )
      {
	desiredSlopeInner= 0.65*imult;
	desiredSlopeOuter= 0.80*omult;
      }
      if ( width == 14 )
      {
	desiredSlopeInner= 0.64*imult;
	desiredSlopeOuter= 0.75*omult;
      }
      if ( width >= 15 )
      {
	desiredSlopeInner= 0.61*imult;
	desiredSlopeOuter= 0.71*omult;
      }
      
      
      if ( labels(n) )
      {
	float value= esab->apply2(labels2, n, input);
	//printf("%f ", value);
	if ( value < desiredSlopeInner )
	  slope= 0;
	/*else if ( value > desiredSlopeOuter )
	  slope= 1;*/
	else
	  slope= value - desiredSlopeInner;
      }
      else
      {
	float value= esab->apply2(labels2, n, input);
	if ( value >= desiredSlopeOuter )
	  slope= 0;
	/*else if ( value < desiredSlopeInner )
	  slope= 1;*/
	else
	  slope= desiredSlopeOuter - value;
      }
      
      float neighborhood= 0;
      float circularity= 0;
      
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      
      int minimum= 0;
      for ( int j= 0; j < ses.size(); ++j )
	if ( labels(n) && ses(j) != 0 && input(n) < input(n + ses(j)) )
	  ++minimum;
      /*if ( minimum == 8 )
	return 0;*/
      
      int nn= 0;
      for ( int j= 0; j < ses.size(); ++j )
	  if ( ses(j) != 0 && labels(n + ses(j)) == labels(n) )
	    ++nn;
      neighborhood= (1 - nn/8.0);
      
      /*StructuringElementDisk sed(5);
      sed.updateStride(input.columns);
      float area= 0;
      float perimeter= 0;
      
      for ( int j= 0; j < sed.size(); ++j )
	  if ( n + sed(j) >= 0 && n + sed(j) < input.n && labels(n + sed(j)) > 0 )
	  {
	      ++area;
	      if ( isInnerContour8(labels, n + sed(j)) )
		  ++perimeter;
	  }
      circularity= (perimeter*perimeter)/(4*M_PI*area);*/
      
      /*if ( width < 2 )
	nWeight= nw/2;*/
      
      energy= sWeight*slope + nWeight*neighborhood + cWeight*circularity;
      
      return energy;
    }
    
    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::optimize2(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration)
    {
      Vector<int> pixels;
      
      float inner= 0.5;
      float outer= 0.5;
      int i1= 0;
      int o1= 2;
      int i2= 20;
      int o2= 20;

      if ( iteration == 1 )
      {
	labels_original2.resizeImage(labels);
	labels_original2= labels;
      }
      
      labels_original.resizeImage(labels);
      labels_original= labels;
      
      StructuringElementDisk nh(3);
      nh.updateStride(labels.columns);
      
      for ( int i= 0; i < input.n; ++i )
      {
	//if ( iteration % 2 )
	
	/*int j;
	for ( j= 0; j < nh.size(); ++j )
	  if ( labels_original2(i + nh(j)) )
	    break;
	if ( j == nh.size() )
	  continue;*/
	
	{
	  if ( isInnerContour4(labels, i) )
	  {
	    int w= esab->swidth(labels_original, i, input);
	    if ( w > i1 && w <= i2 )
	      pixels.push_back(i);
	  }
	}
	//else
	{
	  if ( isOuterContour8(labels, i) )
	  {
	    int w= esab->swidth(labels_original, i, input);
	    if ( w > o1 && w <= o2 )
	      pixels.push_back(i);
	  }
	}
      }
      
      StructuringElementDisk sed(1);
      sed.updateStride(input.columns);
      
      srand(time(NULL));
      
      StructuringElementSquare ses(2);
      ses.updateStride(labels.columns);
      
      float temperature= 4;
      float annealing= 0.9;
      float target= 0;
      float oldTarget= 0;
      float newTarget= 0;
      
      //#pragma omp parallel for num_threads(2)
      for ( int k= 0; k < pixels.size(); ++k )
      {
	int r= k;
	int flag= 0;

	if ( roi && !(*roi)(pixels(r)) )
	  continue;
	if ( !slopeMask(pixels(r)) )
	  continue;

	int nn= 0;
	for ( int j= 0; j < ses.size(); ++j )
	    if ( ses(j) != 0 && labels(pixels(r) + ses(j)) == labels(pixels(r)) )
	      ++nn;
	if ( labels(pixels(r)) && nn <= 2 )
	  continue;
	
	int before, after;
	#pragma omp critical
	{
	  if ( labels(pixels(r)) )
	  {
	    subimage(0,0)= labels(pixels(r)-labels.columns-1);
	    subimage(0,1)= labels(pixels(r)-labels.columns);
	    subimage(0,2)= labels(pixels(r)-labels.columns+1);
	    subimage(1,0)= labels(pixels(r)-1);
	    subimage(1,1)= labels(pixels(r));
	    subimage(1,2)= labels(pixels(r)+1);
	    subimage(2,0)= labels(pixels(r)+labels.columns-1);
	    subimage(2,1)= labels(pixels(r)+labels.columns);
	    subimage(2,2)= labels(pixels(r)+labels.columns+1);
	    
	    before= connectivityTerm();
	    
	    subimage(4)= 0;
	    
	    after= connectivityTerm();
	  }
	}
	
	if ( before < after )
	      continue;
	
	int f= labels(pixels(r));
	
	float err1= 0;
	int nnn= 0;
	for ( int j= 0; j < sed.size(); ++j )
	  if ( sed(j) == 0 )
	  {
	    err1+= energy(input, labels, roi, pixels(r) + sed(j), labels_original);
	    ++nnn;
	  }

	if ( labels(pixels(r)) == 255 )
	{
	  labels(pixels(r))= 0;
	  flag= 0;
	}
	else
	{
	  labels(pixels(r))= 255;
	  flag= 0;
	}
	
	float err2= 0;
	nnn= 0;
	for ( int j= 0; j < sed.size(); ++j )
	  if ( sed(j) == 0 )
	  {
	    err2+= energy(input, labels, roi, pixels(r) + sed(j), labels_original, flag);
	    ++nnn;
	  }
	
	if ( err1 <= err2 )
	{
	  labels(pixels(r))= (labels(pixels(r)) == 255) ? 0 : 255;
	}
      }
      
      return 1;
    }
    
    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::optimize3(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration)
    {
      Vector<int> pixels;

      char filename[100];
      sprintf(filename, ".cache-%f.labels.bmp", fp);
      string cache0= input.filename + string(filename);
      sprintf(filename, ".cache-%f.filterResponses.float", fp);
      string cache1= input.filename + string(filename);
      sprintf(filename, ".cache-%f.maxResponses.float", fp);
      string cache2= input.filename + string(filename);
      sprintf(filename, ".cache-%f.newLabels.bmp", fp);
      string cache3= input.filename + string(filename);
      
      FILE* f;
      f= fopen(cache0.c_str(), "r");
      if ( f )
      {
	tprintf("reading cached images\n");
	readImage(cache0.c_str(), labels);
	readImage(cache1.c_str(), filterResponses);
	readImage(cache2.c_str(), maxResponses);
	readImage(cache3.c_str(), newLabels);
	tprintf("cached images read\n");
	return 0;
      }
      
	float bestCorr= -1;
        
        for ( int i= 0; i < filters.size(); ++i )
        {
            filters(i)->updateStride(input.columns);
            filters(i)->computeMinMax();
        }
        
        for ( int i= 0; i < pfilters.size(); ++i )
        {
          pfilters(i)->updateStride(input.columns);
          pfilters(i)->computeMinMax();
        }

        StructuringElementDisk sed(2);
	sed.updateStride(labels.columns);
        
	int z= 0;
	
	tprintf("applying filters...\n");
	#pragma omp parallel for reduction(+:z), num_threads(4)
	for ( int i= 0; i < labels.n; ++i )
	{
	  int num= 0;
	    float actCorr;
	    int flag= 0;
	    int flag2= 0;
	    filterResponses(i)= 0;
	    maxResponses(i)= 0;
	    for ( int j= 0; j < filters.size() && !flag; ++j )
	    {
	      if ( i + filters(j)->getMin() >= 0 && i + filters(j)->getMax() < input.n )
	      {
		actCorr= filters(j)->apply(input, i, roi);
//		printf("%f ", actCorr); fflush(stdout);
		if ( actCorr > fp && (!roi || (*roi)(i)))
		{
		    //flag++;
		    ++num;
		    labels(i)= 255;
		    //#pragma omp critical
		    {
		      filterResponses(i)+= actCorr;
		      /*if ( filterResponses(i) < actCorr )
			filterResponses(i)= actCorr;*/
		    }
		    filterIndices(i)= j;
		    newLabels(i)= 255;
		    if ( flag2 == 0 )
		    {
		      ++z;
		      flag2= 1;
		    }
		    //++z;
		    
		    if ( actCorr > maxResponses(i) )
		      maxResponses(i)= actCorr;
		}
		

	      }
	    }
	    
	    for ( int j= 0; j < pfilters.size() && !flag; ++j )
	    {
	      if ( i + pfilters(j)->getMin() >= 0 && i + pfilters(j)->getMax() < input.n )
	      {
		actCorr= pfilters(j)->apply(input, i, roi);
		if ( actCorr > fp && (!roi || (*roi)(i)))
		{
		    //flag++;
		    ++num;
		    labels(i)= 255;
		    //#pragma omp critical
		    {
		      filterResponses(i)+= actCorr;
		      /*if ( filterResponses(i) < actCorr )
			filterResponses(i)= actCorr;*/
		    }
		    filterIndices(i)= 1000 + j;
		    newLabels(i)= 255;
		    //++z;
		    if ( flag2 == 0 )
		    {
		      ++z;
		      flag2= 1;
		    }
		    
		    if ( actCorr > maxResponses(i) )
		      maxResponses(i)= actCorr;
		}

	      }
	    }
	    
	    if ( num > 0 )
		filterResponses(i)/= num;
	    else
		filterResponses(i)= 0;
//	    printf("%f ", filterResponses(i)); fflush(stdout);
	}
      
      	writeImage(cache0.c_str(), labels);
	writeImage(cache1.c_str(), filterResponses);
	writeImage(cache2.c_str(), maxResponses);
	writeImage(cache3.c_str(), newLabels);

      
      return z;
    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::analyze(Image<INPUT>& input, Image<unsigned char>& labels)
    {
      float inner= 0;
      int ni= 0;
      float outer= 0;
      int no= 0;
      float tmp;
      
      esab->updateStride(input.columns);
      
      Vector<int> iw;
      Vector<float> is;
      
      Vector<int> ow;
      Vector<float> os;
      
      iw.resize(20);
      is.resize(20);
      ow.resize(20);
      os.resize(20);
      
      for ( int i= 20000; i < labels.n - 20000; ++i )
      {
	if ( isInnerContour4(labels, i) )
	{
	  int width= esab->swidth(labels, i, input);
	  tmp= esab->apply2(labels, i, input);
	  iw(width)++;
	  is(width)+= tmp;
	}
	if ( isOuterContour8(labels, i) )
	{
	  int width= esab->swidth(labels, i, input);
	  tmp= esab->apply2(labels, i, input);
	  ow(width)++;
	  os(width)+= tmp;
	}
      }
      
      for ( int i= 0; i < iw.size(); ++i )
	printf("%d: %f %d\t", i, is(i)/iw(i), iw(i));
      
      printf("\t\t");
      
      for ( int i= 0; i < iw.size(); ++i )
	printf("%d: %f %d\t", i, os(i)/ow(i), ow(i));
      printf("\n");
    }
    
    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::filterTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi)
    {
        float bestCorr= -1;
        float actCorr;

	Image<unsigned char> thinned;
	thinned.resizeImage(labels);
	thinned= labels;
	
	thinningCWSI(labels, thinned);
	
	Vector<int> ends;
	StructuringElementSquare ses(3, thinned.columns);
	ses.updateStride(thinned.columns);
	
	for ( int i= -ses.getMin(); i < thinned.n - ses.getMax(); ++i )
	{
	  int n= 0;
	  if ( thinned(i) )
	  {
	    for ( int k= 0; k < ses.size(); ++k )
	      if ( ses(k) != 0 && thinned(i + ses(k)) )
		++n;
	    if ( n <= 1 )
	    {
	      /*StructuringElementDisk sed(5);
	      sed.updateStride(input.columns);

	      float area= 0;
	      float perimeter= 0;
	      for ( int j= 0; j < sed.size(); ++j )
		  if ( i + sed(j) >= 0 && i + sed(j) < input.n && labels(i + sed(j)) > 0 )
		  {
		      ++area;
		      if ( isInnerContour8(labels, i + sed(j)) )
			  ++perimeter;
		  }*/
	      //if ( (perimeter*perimeter)/(4*M_PI*area) <= circ )
	      {
		ends.push_back(i);
	      }
	    }
	  }
	}
	    
	
	//filters.updateStride(input.columns);
        for ( int i= 0; i < filters.size(); ++i )
        {
            filters(i)->updateStride(input.columns);
            filters(i)->computeMinMax();
        }
        
        for ( int i= 0; i < pfilters.size(); ++i )
        {
          pfilters(i)->updateStride(input.columns);
          pfilters(i)->computeMinMax();
        }

	for ( int i= 0; i < contour.size(); ++i )
	{
	  if ( !roi || (*roi)(contour(i)) )
	  {
	    int flag2= 0;
	    int x1, y1, x2, y2;
	    thinned.getCoordinate2D(contour(i), x1, y1);
	    for ( int j= 0; j < ends.size(); ++j )
	    {
	      thinned.getCoordinate2D(ends(j), x2, y2);
	      if ( sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)) <= 2 )
	      {
		flag2++;
		break;
	      }
	    }
	    
	    if ( !flag2 )
	      continue;
	    
	    int flag= 0;
	    for ( int j= 0; j < filters.size() && !flag; ++j )
	    {
	      if ( contour(i) + filters(j)->getMin() >= 0 && contour(i) + filters(j)->getMax() < input.n )
	      {
		actCorr= filters(j)->apply(input, contour(i), roi);
		if ( actCorr > fth(j) * perc )
		{
		    flag++;
		}
	      }
	    }
	    
	    for ( int j= 0; j < pfilters.size() && !flag; ++j )
	    {
	      if ( contour(i) + pfilters(j)->getMin() >= 0 && contour(i) + pfilters(j)->getMax() < input.n )
	      {
		actCorr= pfilters(j)->apply(input, contour(i), roi);
		if ( actCorr > pfth(j) * perc )
		{
		  flag++;
		}
	      }
	    }
	    
	    if ( flag && flag2 )
	      mask(i)++;
	  }
	}

    }
    
    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::lineTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi)
    {
        float bestCorr= -1;
        float actCorr;

	int n= 0;
        for ( int i= 0; i < contour.size(); ++i )
        {
            if ( clo->apply(input, contour(i), &labels) )
	    {
                mask(i)++;
		++n;
	    }
        }
        printf("lt: %d ", n);
    }
    
    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::slopeTerm(Region2& contour, Vector<float>& profile, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, float perc, Image<unsigned char>* roi)
    {
        /*StructuringElementDisk sed(se);
        sed.updateStride(input.columns);

        float minBackground;
        float meanForeground;
        int n;

	Vector<float> intensities;*/
        for ( int i= 0; i < contour.size(); ++i )
        {
	    /*intensities.clear();
            minBackground= FLT_MAX;
            meanForeground= 0;
            n= 0;
            for ( int j= 0; j < sed.size(); ++j )
            {
                if ( contour(i) + sed(j) >= 0 && contour(i) + sed(j) < labels.n && (!roi || (*roi)(contour(i) + sed(j))) )
                {
		    intensities.push_back(input(contour(i) + sed(j)));
                    if ( labels(contour(i)+ sed(j)) )
                        if ( input(contour(i) + sed(j)) < minBackground )
                            minBackground= input(contour(i) + sed(j));
                    if ( !labels(contour(i) + sed(j)) && contour.find(contour(i) + sed(j)) == contour.end() )
                    {
                        meanForeground+= input(contour(i) + sed(j));
                        ++n;
                    }
                }
            }
            //meanForeground/= n;
            //if ( profile(i) < meanForeground && fabs(profile(i) - meanForeground) >= sp*fabs(meanForeground - minBackground) )
            sort(intensities.begin(), intensities.end());
	    int length= 3;
	    //float meanForeground= 0;
	    float meanBackground= 0;
	    int nForeground= 0;
	    int nBackground= 0;
	    for ( int k= 0; k < length; ++k )
	    {
	      /*if ( intensities.size() - 1 - k >= 0 )
	      {
		meanForeground+= intensities(intensities.size()-1-k);
		nForeground++;
	      }*/
	      /*if ( k < intensities.size() )
	      {
		meanBackground+= intensities(k);
		nBackground++;
	      }
	    }
	    //meanForeground/= nForeground;
	    meanBackground/= nBackground;
	    /*if ( meanForeground > meanBackground + 2 && profile(i) < meanForeground && fabs(profile(i) - meanForeground) >= sp*fabs(meanForeground - meanBackground) )
                mask(i)++;*/
	    /*meanForeground/= n;
	    if ( meanForeground > meanBackground + 4 && profile(i) < meanForeground && fabs(profile(i) - meanForeground) >= sp*fabs(meanForeground - meanBackground) )
	      mask(i)++;*/
	    
	    if ( esab->apply(labels, contour(i), input) && (!roi || (*roi)(contour(i)) ) )
	      mask(i)++;
        }
    }

    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::circularityConstraint(Image<unsigned char>& output, PixelSet1& final)
    {
        PixelSet1 ps;
        Image<unsigned char> tmp;
        tmp.resizeImage(output);
        tmp= 0;

        StructuringElementDisk sed(5);
        sed.updateStride(output.columns);

        for ( int i= 0; i < final.size(); ++i )
            output(final(i))= 128;

        Vector<int> toRemove;

        for ( int i= 0; i < final.size(); ++i )
        {
            float area= 0;
            float perimeter= 0;
            for ( int j= 0; j < sed.size(); ++j )
                if ( final(i) + sed(j) >= 0 && final(i) + sed(j) < output.n && output(final(i) + sed(j)) > 0 )
                {
                    ++area;
                    if ( isInnerContour8(output, final(i) + sed(j)) )
                        ++perimeter;
                }
            if ( (perimeter*perimeter)/(4*M_PI*area) > circ )
	    {
	      output(final(i))= 0;
                toRemove.push_back(i);
	    }
        }

        int j;
        for ( int i= 0; i < final.size(); ++i )
        {
            for ( j= 0; j < toRemove.size(); ++j )
                if ( toRemove[j] == i )
                    break;
            if ( j == toRemove.size() )
                ps.push_back(final[i]);
        }

        for ( int i= 0; i < final.size(); ++i )
            output(final(i))= 0;

        final= ps;
        ps.stride= final.stride;
    }

    template<typename INPUT>
    int ContourRegionGrowingTowardsEqualSlope<INPUT>::findPoints(Region2 c, Image<INPUT>& input, Image<unsigned char>& labels, Vector<unsigned char>& mask, Image<unsigned char>* roi)
    {
        mask.resize(c.size());
        mask= 0;

        /*if ( c.size() < 2*(this->se) )
            return 0;*/

        Vector<float> contour;
        for ( int i= 0; i < c.size(); ++i )
            contour.push_back(input(c(i)));

        /*if ( iTerm )
          intensityTerm(c, contour, input, labels, mask, ip, roi);*/

        /*if ( mTerm )
          morphologyTerm(c, contour, input, mask, mp);*/

        if ( aTerm )
          averageTerm(c, contour, input, labels, mask, ap);

        if ( fTerm )
          filterTerm(c, contour, input, labels, mask, fp);

        if ( sTerm )
          slopeTerm(c, contour, input, labels, mask, 0);
	
	if ( lTerm )
	  lineTerm(c, contour, input, labels, mask, 0);

        saveMask(c, contour, input, mask);

        return 0;
    }

    template<typename INPUT>
    Vector<Region2> ContourRegionGrowingTowardsEqualSlope<INPUT>::extractOrderedContours(Region2 object, Image<unsigned char>& mask)
    {
        Vector<Region2> results;

        if ( object.size() == 1 )
        {
            Region2 r;
            r.push_back(object(0) + 1);
            r.push_back(object(0) + object.stride + 1);
            r.push_back(object(0) + object.stride);
            r.push_back(object(0) + object.stride - 1);
            r.push_back(object(0) - 1);
            r.push_back(object(0) - object.stride - 1);
            r.push_back(object(0) - object.stride);
            r.push_back(object(0) - object.stride + 1);
            results.push_back(r);
            return results;
        }

        Vector<int> pos;
        Vector<int> pos8;
        pos.push_back(1);
        pos.push_back(-mask.columns);

        pos.push_back(-1);
        pos.push_back(mask.columns);

        pos8.push_back(1);
        pos8.push_back(-mask.columns+1);
        pos8.push_back(-mask.columns);
        pos8.push_back(-mask.columns-1);
        pos8.push_back(-1);
        pos8.push_back(mask.columns-1);
        pos8.push_back(mask.columns);
        pos8.push_back(mask.columns+1);

        tmpImage= 0;

        for ( int i= 0; i < object.size(); ++i )
            tmpImage(object(i))= 255;

        int pm1= -1;

        while ( 1 )
        {
            Region2 result;
            int p;
            int dir;
            int start;
            int begin;

            p= -1;

            for ( int i= 0; i < tmpImage.n; ++i )
                if ( tmpImage(i) != 255 && isOuterContour4(tmpImage, i) && tmpImage(i) != 128 )
                {
                    p= i;
                    begin= i;
                    break;
                }

            if ( p == -1 )
                break;
            result.push_back(p);
            dir= 0;

            while ( 1 )
            {
                start= (dir+3)%4;

                int j;
                for ( j= 0; j < pos.size(); ++j )
                {
                    int pix= p + pos((start+j)%pos.size());
                    if ( pix >= 0 && pix < tmpImage.n && isOuterContour8(tmpImage, pix) )
                    {
                        pm1= p;
                        p= pix;
                        dir= (start + j)%pos.size();
                        break;
                    }
                }

                if ( j == pos.size() )
                {
                    result.push_back(p);
                    break;
                }

                if ( result.size() >= 2 && result(1) == p && result(0) == pm1 )
                    break;
                result.push_back(p);
            }

            for ( int i= 0; i < result.size(); ++i )
                tmpImage(result(i))= 128;

            if ( result.size() > 2 )
                results.push_back(result);
        }

        return results;
    }

    
    int hasHoles(Region2& region)
{
  ExtractRegions er(1);
  int rows= region.getHeight();
  int cols= region.getWidth();
  
  int r, c;
  int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
  for ( int i= 0; i < region.size(); ++i )
  {
    region.getCoordinate2D(region(i), r, c);
    if ( r < rmin )
      rmin= r;
    if ( r > rmax )
      rmax= r;
    if ( c < cmin )
      cmin= c;
    if ( c > cmax )
      cmax= c;
  }
  
  Image<unsigned char> tmp(rows, cols);
  tmp= 255;
  
  for ( int i= 0; i < region.size(); ++i )
  {
    region.getCoordinate2D(region(i), r, c);
    tmp(r - rmin, c - cmin)= 0;
  }
  
  /*writeImage("noholes-tmp.bmp", tmp);
  getchar();*/
  Region2Set rs;
  er.apply(tmp, rs);
  //printf("%d\n", rs.size());
  Region2Set result;
  for ( int i= 0; i < rs.size(); ++i )
  {
    int flag= 0;
    for ( int j= 0; j < rs(i).size() && flag == 0; ++j )
    {
      rs(i).getCoordinate2D(rs(i)(j), r, c);
      //printf("(%d %d) ", r, c);
      if ( r == 0 || r == tmp.rows-1 || c == 0 || c == tmp.columns-1 )
      {
	flag= 1;
      }
    }
    //printf("flag: %d\n", flag);
    if ( !flag )
      return 1;
  }
  
  return 0;
}
    
    template<typename INPUT>
    void ContourRegionGrowingTowardsEqualSlope<INPUT>::doRG(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> &seed, Image<unsigned char> *roi)
    {
      tprintf("aaa\n");
        clo->updateStride(input.columns);
	esab->updateStride(input.columns);
        tmpImage.resizeImage(seed);
	already.resizeImage(seed);
	filterResponses.resizeImage(seed);
	filterIndices.resizeImage(seed);
	newLabels.resizeImage(seed);
	slopeMask.resizeImage(seed);
	slopeMask= 0;
	already= 0;
	filterIndices= -1;
	filterResponses= -1;
	newLabels= 0;
	maxint= input.getMax();
	
	tprintf("bbb\n");
	GaussianFilter2<float, float> gf(3, 7);
	Image<float> tmpImage;
	Image<float> tmpImage2;
	tmpImage.resizeImage(input);
	tmpImage2.resizeImage(input);
	tmpImage= input;
	maxResponses.resizeImage(seed);
	maxResponses= 0;
	gf.apply(tmpImage, tmpImage2);
	tmpImage2.normalize(0, 255);
	tmpImage= seed;
	tprintf("debug\n");
	esab->debug(seed, seed, tmpImage);
	writeImage("tmpImage.bmp", tmpImage);
	tprintf("starting\n");

        nTerms= 0;
        if ( aTerm ) ++nTerms;
        if ( mTerm ) ++nTerms;
        if ( fTerm ) ++nTerms;
        if ( sTerm ) ++nTerms;
        if ( iTerm ) ++nTerms;
	if ( lTerm ) ++nTerms;

        PixelSet1 seeds;
        for ( int i= 0; i < seed.n; ++i )
            if ( seed[i] )
                seeds.push_back(i);

        PixelSet1 final;
        StructuringElementDisk ses(2, seed.columns);

        output= seed;
        bool flag;
        int point;

        Vector<Region2> results;
        ExtractRegions er;
        tprintf("starting iteration\n");
        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        Vector<unsigned char> mask;

        int iteration= 0;
        Vector<Region2> contours;
        while ( 1 )
        {
	    ++iteration;
            if ( maxit >= 0 && iteration > maxit )
                break;
	  
	    thin.resizeImage(output);
	    thin= output;
	    
	    thinningCWSI(output, thin);
	    
	    if ( ! aTerm && ! mTerm && ! iTerm)
	    {
	      final.clear();

	      results.clear();

	      er.apply(output, results);
	      tprintf("number of regions: %d\n", results.size());
	      /*for ( int i= 0; i < results.size(); ++i )
		  printf("%d ", results(i).size());
	      printf("\n");*/

	      tmp= 0;
	      saveR= output;
	      saveB= output;
	      saveG= output;

	      for ( int i= 0; i < results.size(); ++i )
	      {
		  /*if ( results(i).size() == 1 )
		  {
		      for ( int j= 0; j < results(i).size(); ++j )
			  tmp(results(i)(j))= 255;
		      continue;
		  }*/

		  Vector<Region2> contours= extractOrderedContours(results(i), output);
		  printf("noc: %d,%d ", contours.size(), contours(0).size());

		  for ( int i= 0; i < contours.size(); ++i )
		  {
		      findPoints(contours(i), input, output, mask, roi);

		      for ( int j= 0; j < mask.size(); ++j )
			  if ( mask(j) && (!roi || (*roi)(contours(i)(j))))
			      final.push_back(contours(i)(j));
		  }
		  
		  for ( int i= 0; i < contours.size(); ++i )
		    for ( int j= 0; j < contours(i).size(); ++j )
		      for ( int k= 0; k < contours.size(); ++k )
			if ( i != k && contours(k).find(contours(i)(j)) != contours(k).end() )
			  final.push_back(contours(i)(j));
		      

		  /*for ( int j= 0; j < contour.size(); ++j )
		      if ( contour(j) >= 0 && contour(j) < input.n && input(contour(j)) < input(contour(min)) && mask(j) )
			  min= j;*/

		  //final.push_back(orderedContour(point));
	      }
	    }


            tprintf("final.size()= %d\n", final.size());

	    int k;
	    if ( mTerm || iTerm )
	    { 
	      printf("aaa\n"); fflush(stdout);
	      optimize3(input, output, roi, iteration);
	      printf("bbb\n"); fflush(stdout);
	      break;
	      /*if ( (k= optimize3(input, output, roi, iteration)) && final.size() == 0 )
	      {
		printf("kk= %d\n", k);
		continue;
	      }*/
	    }
	    if ( aTerm )
	    {
	      Image<unsigned char> tmp;
	      tmp.resizeImage(output);
	      StructuringElementSquare sed(3);
	      StructuringElementSquare sed2(3);
	      sed.updateStride(output.columns);
	      sed2.updateStride(output.columns);
	      
	      /*Vector<StructuringElementSquare*> vses;
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      vses.push_back(new StructuringElementSquare(3));
	      
	      for ( int j= 0; j < vses.size(); ++j )
		vses(j)->updateStride(output.columns);
	      */
	      
	      if ( iteration == 1 )
	      {
		tmp= 1;
		grayscaleDilate(&output, &tmp, sed);
		for ( int i= 0; i < input.n; ++i )
		  if ( output(i) != tmp(i) )
		    slopeMask(i)= 255;
		tmp= 0;
		grayscaleErode(&output, &tmp, sed2);
		for ( int i= 0; i < input.n; ++i )
		  if ( output(i) != tmp(i) )
		    slopeMask(i)= 255;
		/*printf("---------"); fflush(stdout);
		slopeMask= 0;
		for ( int i= input.columns*30; i < input.n - input.columns*30; ++i )
		{
		  if ( isInnerContour4(output, i) || isOuterContour8(output,i) )
		  for ( int k= 0; k < sed.size(); ++k )
		  {
		    if ( output(i + sed(k)) )
		    {
		      int width= 3;
		      width= esab->swidth(output, i, input);
		      if ( width > 7 )
			width= 7;
		      if ( width < 3 )
			break;
		      for ( int j= 0; j < vses(width)->size(); ++j )
			slopeMask(i + (vses)(width)->operator()(j))= 255;
		      break;
		    }
		  }
		  
		}*/
		printf("finished\n"); fflush(stdout);
		//writeImage("slopeMask.bmp", slopeMask);
	      }
	      //slopeMask= 255;
	      if ( (k= optimize2(input, output, roi, iteration)) && final.size() == 0 )
	      {
		printf("k= %d\n", k);
		//writeImage("tmp-optimize2.bmp", output);
		continue;
	      }
	    }
	    
	    
            if ( final.size() == 0 )
                break;

            circularityConstraint(output, final);

            for ( unsigned int j= 0; j < final.size(); ++j )
                output(final(j))= 255;
	    
	    //fillHoles(output);

            //writeImage("tmp-rg.bmp", output);
       }
       
       if ( mTerm )
       {
	  Image<unsigned char> mask;
	  Image<float> weights;
	  Image<unsigned char> tmp;
	  Image<unsigned char> tmpb;
	  Image<unsigned char> tmpc;
	  Image<unsigned char> tmpd;
	  Image<unsigned char> result;
	  
	  mask= newLabels;
	  //weights= filterResponses;
	  if ( mf )
	    weights= maxResponses;
	  else
	    weights= filterResponses;
	  //weights.normalize(0, 1);
	  
	  tmp.resizeImage(mask);
	  tmpb.resizeImage(mask);
	  tmpc.resizeImage(mask);
	  tmpd.resizeImage(mask);
	  result.resizeImage(mask);
	  tmp= 0;
	  tmpb= 0;
	  tmpd= 0;
	  result= 0;
	  
	  int sizeThreshold= 20;
	  float meanThreshold= ap;
	  float stdevThreshold= mp;
	  int sizeThreshold0= maxit;
	  int sizeThreshold1= se;
	  
	  printf("size thresholds: %d %d\n", sizeThreshold0, sizeThreshold1);
	  
	  Image<unsigned char> inputImage;
	  inputImage= input;
	  output= seed;
	  
	  Region2Set seedRegions;
	  ExtractRegions ers;
	  ers.apply(seed, seedRegions);
	  Region2Set tmpSeedRegions;
	  
	  ExtractSegments es;
	  
	  tmpc= 0;
	  for ( float threshold= 0.4; threshold <= 1; threshold+= 0.01 )
	  {
	    printf("%f ", threshold); fflush(stdout);
	    for ( int i= 0; i < weights.n; ++i )
	      if ( weights(i) > threshold )
		tmpb(i)= 255;
	      else
		tmpb(i)= 0;
	      
	    //for ( float threshold2= threshold*0.8; threshold2 <= threshold; threshold2+= 0.05 )
	    {
	      //NeighborhoodRegionGrowing<float> ng(REGION_GROWING_HARD_THRESHOLD, threshold2);
	      //ng.doRG(weights, tmpd, tmpb);
	      //tmpb= tmpd;
	      
	      Region2Set regions;
	      ExtractRegions er;
	      Region2Set segments;
	      //segments= es.apply(tmpb);
	      
	      er.apply(tmpb, regions);
	      //regions= segments;
	      
	      printf("%d %d ", regions.size(), segments.size());
	      //regions.add(segments);
	      
	      
	      //tmpSeedRegions= seedRegions;
	      //tmpSeedRegions.add(regions);
	      
	      #pragma omp parallel for num_threads(4)
	      for ( int i= 0; i < regions.size(); ++i )
	      {
		if ( regions(i).size() > 150000 )
		  continue;
		
		Region2 tmpRegion;
		tmpRegion.stride= tmpc.columns;
		//reduceToMinimumPoints(regions(i), inputImage, tmpRegion);
		tmpRegion= regions(i);
		  
		float rmean= tmpRegion.getMean(weights);
		float rmin= tmpRegion.getMin(weights);
		float rstdev= tmpRegion.getStandardDeviation(weights);
		
		float regionSize= tmpRegion.size();
		float covered= 0;
		
		//printf("cover ratio: %f\n", covered/regionSize);
		
		if ( regionSize > sizeThreshold0 && regionSize < sizeThreshold1 && (/*rmean >= ip || */(rmean >= meanThreshold && rstdev <= stdevThreshold)) )
		{
		  //tmpRegion.set(tmpc, (unsigned char)255);
		  tmpRegion.set(output, (unsigned char)255);
		}
	      }
	    }
	  }
	  
	  Region2Set regions;
	  tmpd= 0;
	  tmpd= tmpc;
	  /*ExtractRegions er2;
	  er2.apply(tmpc, regions);
	  thinningCWSI(tmpc, tmp);
	  
	  output= seed;
	  for ( int i= 0; i < regions.size(); ++i )
	  {
	    Region2 thinnedRegion;
	    Region2 tmpRegion;
	    thinnedRegion.stride= tmpc.columns;
	    tmpRegion.stride= tmpc.columns;
	    if ( regions(i).size() <= 2 )
	      continue;

	    for ( int j= 0; j < regions(i).size(); ++j )
	      if ( tmp(regions(i)(j)) )
		thinnedRegion.push_back(regions(i)(j));

	    reduceToMinimumPoints(regions(i), inputImage, tmpRegion);
	    printf("%d %d ", regions(i).size(), tmpRegion.size());
	    tmpRegion.set(tmpd, (unsigned char)255);
	  }*/
	  
	  //writeImage("tmpd.bmp", tmpd);
	  //writeImage("seed.bmp", seed);
	  
	  
	  /*AddAndConnectRegions aacr(8, 0.8);
	  aacr.apply(seed, tmpd, inputImage, output);*/
	  
	  
	  if ( roi )
	  {
	    Image<unsigned char> tmp;
	    tmp.resizeImage(*roi);
	    tmp= 0;
	    StructuringElementDisk sed(5);
	    sed.updateStride(tmp.columns);
	    grayscaleErode(roi, &tmp, sed);
	    for ( int i= 0; i < tmp.n; ++i )
	      if ( tmp(i) != (*roi)(i) )
		output(i)= seed(i);
	  }
	  
	  /*writeImage("output.bmp", output);
	  getchar();*/
	  
       }
       if ( iTerm )
       {
	  Image<unsigned char> mask;
	  Image<float> weights;
	  Image<unsigned char> tmp;
	  Image<unsigned char> tmpb;
	  Image<unsigned char> tmpc;
	  Image<unsigned char> result;
	  
	  mask= newLabels;
	  //weights= filterResponses;
	  if ( mf )
	    weights= maxResponses;
	  else
	    weights= filterResponses;
	  
/*	  weights.normalize(0, 255);
	  writeImage("weights.bmp", weights);
	  writeImage("seed.bmp", seed);*/
	  
	  tmp.resizeImage(mask);
	  tmpb.resizeImage(mask);
	  tmpc.resizeImage(mask);
	  result.resizeImage(mask);
	  tmp= 0;
	  tmpb= 0;
	  result= 0;
	  
	  int sizeThreshold= 20;
	  float meanThreshold= ap;
	  float stdevThreshold= mp;
	  int sizeThreshold0= 20;
	  int sizeThreshold1= 500;
	  
	  Image<unsigned char> inputImage;
	  inputImage= input;
	  output= seed;
	  for ( float threshold= 0; threshold <= 1; threshold+= 0.01 )
	  {
	    printf("%f ", threshold); fflush(stdout);
	    for ( int i= 0; i < weights.n; ++i )
	      if ( weights(i) > threshold )
		tmpb(i)= 255;
	      else
		tmpb(i)= 0;
	    
	    //printf("aaa"); fflush(stdout);
	    thinningCWSI(tmpb, tmpc);  
	    //printf("bbb"); fflush(stdout);
	    
	    //writeImage("tmpb.bmp", tmpb);
	    //writeImage("tmpc.bmp", tmpc);
	    //getchar();
	    //for ( float threshold2= threshold*3/4; threshold2 <= threshold; threshold2+= 0.05 )
	    {
	      Vector<Region2> regions;
	      ExtractRegions er;
	      Vector<Region2> regionsThinned;
	      
	      er.apply(tmpb, regions);
	      er.apply(tmpc, regionsThinned);
	      //printf("ccc"); fflush(stdout);
	      //#pragma omp parallel for num_threads(4)
	      for ( int i= 0; i < regions.size(); ++i )
	      {
		//int n= regions(i).numberOfCrossingPixels(tmpc);
		float rmean= regions(i).getMean(weights);
		float rmin= regions(i).getMin(weights);
		float rstdev= regions(i).getStandardDeviation(weights);
		
		float regionSize= regions(i).size();
		float covered= 0;
		
		if ( regions(i).size() < 1000 && regions(i).size() > 1 )
		{
		  for ( int j= 0; j < regions(i).size(); ++j )
		    if ( seed(regions(i)(j)) )
		      covered= covered + 1;
		  //printf("cover ratio: %f\n", covered/regionSize); fflush(stdout);
		  if ( covered/regionSize > ip )
		  {
		    Region2 thinnedRegion;
		    thinnedRegion.stride= tmpc.columns;
		   
		    for ( int j= 0; j < regions(i).size(); ++j )
		      if ( tmpc(regions(i)(j)) )
			thinnedRegion.push_back(regions(i)(j));

		    //thinnedRegion= reduceToMinimumPoints(regions(i), inputImage, thinnedRegion);

		    thinnedRegion.set(output, (unsigned char)255);
		    printf("%d ", thinnedRegion.size());
		  }
		}
	      }
	    }
	  }
       }
    }
}
#endif // CONTOURREGIONGROWING_H


