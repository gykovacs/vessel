#ifndef RETINA_REGION_GROWING_H
#define RETINA_REGION_GROWING_H

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

namespace openip
{
    template<typename INPUT>
    class CorrectionOfEdgeBorders: public RegionGrowing<INPUT>
    {
    public:
	CorrectionOfEdgeBorders(int maxit= 20, float imult= 1, float omult= 1, float nw= 1.7, char* relativeIntensities= NULL, char* widthHistogram= NULL, float widthScaling= 1.0f, float vstage2dynth= 12, int errorMode= 1);

        CorrectionOfEdgeBorders(const CorrectionOfEdgeBorders& nrg);

        ~CorrectionOfEdgeBorders();

        virtual void init();

        virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL, float* error= NULL);
	
	int connectivityTerm();

	int optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration);
	
	float energy(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int n, Image<unsigned char>& labels2, int flag= 0);
	
	float error(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int iteration, int flag= 0);
	
	float error(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int n, Image<unsigned char>& labels2, int flag, float& weights);
	
	void fillCaches(Image<unsigned char>& labels, Image<INPUT>& input);
	
	void getDesiredSlopes(float width, float dyn, float& inner, float& outer);
	
	float getError(Image<INPUT>& input, Image<unsigned char>& image, Image<unsigned char>* roi, int flag= 0);

        int maxit;
	float imult, omult, nw;

	Image<unsigned char> thinned;
	Image<unsigned char> slopeMask;
	Image<unsigned char> modified;
	Image<unsigned char> lastImage;
	Image<unsigned char> labels_original;
	Image<float> meanWidths;

	EqualSlopeAtBoundaries<float> *esab;

	Image<unsigned char> subimage;
	Vector<Vector<int> > neighborhoods;
	
	Vector<float> innerMult;
	Vector<float> outerMult;
	
	Vector<float> widthHistogram;
	
	Vector<Vector<float> > relativeInner;
	Vector<Vector<float> > relativeOuter;
	
	Vector<Vector<float> > rierror;
	Vector<Vector<float> > roerror;
	Vector<Vector<float> > rin;
	Vector<Vector<float> > ron;
	Image<float> widthCache;
	Image<float> riCache;
	Image<float> dynCache;
	
	Vector<float> expectedDyn;
	Vector<float> dynstd;
	
	float widthScaling;
	int inner;
	float dynth;
	float dynthmod;
	float tableth;
	
	Vector<float> relativeInnerSum;
	Vector<float> relativeOuterSum;
	
	StructuringElementSquare ses3;
	StructuringElementSquare ses5;
	StructuringElementDisk sed10;
	
	Vector<float> errors;
	
	int errorMode;
	int useCache;
	
	Vector<float> wh;
	Vector<float> whi;
	Vector<float> who;
	
	float meanWidth;
	float nWidth;
    };

    template<typename INPUT>
    CorrectionOfEdgeBorders<INPUT>::CorrectionOfEdgeBorders(int maxit, float imult, float omult, float nw, char* relativeIntensities, char* /*widthHistogram*/, float widthScaling, float dynth, int errorMode)
        : RegionGrowing<INPUT>()
    {
	tprintf("correction of edge borders: maxit %d, imult: %f, omult: %f, nw: %f, ws: %f, dynth: %f\n", maxit, imult, omult, nw, widthScaling, dynth);
        this->maxit= maxit;
	this->imult= imult;
	this->omult= omult;
	this->nw= nw;
	this->widthScaling= widthScaling;
	this->dynth= dynth;
	this->errorMode= errorMode;
	this->useCache= 1;
        
	tprintf("instantiating equal slope at boundaries object\n");
	esab= new EqualSlopeAtBoundaries<float>(3, 87, 0.4, 1);
	subimage.resizeImage(3,3);
	tprintf("generating structuring elements\n");
	generateStructuringElementSquare(ses3, 3, 4000);
	generateStructuringElementSquare(ses5, 5, 4000);
	
	tprintf("caching local neighborhoods\n");
	for ( int i= 0; i < 9; ++i )
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
	
	/*if ( relativeIntensities != NULL && *relativeIntensities != '\0' )
	  tprintf("relative intensities: %s\n", relativeIntensities);
	
	if ( relativeIntensities != NULL && *relativeIntensities != '\0' )
	{
	  ifstream input;
	  input.open(relativeIntensities);
	  if ( input.good() )
	  {
	    int n;
	    float tmp;
	    input >> n;
	    for ( int i= 0; i < n; ++i )
	    {
	      input >> tmp;
	      if ( i < 16 )
		innerMult.push_back(tmp);
	    }
	    input >> n;
	    for ( int i= 0; i < n; ++i )
	    {
	      input >> tmp;
	      if ( i < 16 )
		outerMult.push_back(tmp);
	    }
	  }
	}
	else
	{
	  float im[]= {0, 0.16, 0.24, 0.38, 0.48, 0.55, 0.59, 0.63, 0.65, 0.64, 0.63, 0.61, 0.62, 0.65, 0.64, 0.61};
	  float om[]= {0, 0.57, 0.62, 0.74, 0.79, 0.83, 0.84, 0.83, 0.81, 0.80, 0.79, 0.77, 0.79, 0.80, 0.75, 0.71};
	  
	  for ( int i= 0; i < 16; ++i )
	  {
	    innerMult.push_back(im[i]);
	    outerMult.push_back(om[i]);
	  }
	}*/
	
	/*if ( widthHistogram != NULL && *widthHistogram != '\0')
	{
	  ifstream input;
	  input.open(widthHistogram);
	  int n;
	  float tmp;
	  input >> n;
	  for ( int i= 0; i < n; ++i )
	  {
	    input >> tmp;
	    this->widthHistogram.push_back(tmp);
	  }
	  input.close();
	}*/
	
	Vector<float> tmp(90);
	
	relativeInner.push_back(tmp);
	relativeOuter.push_back(tmp);
	
	for ( int i= 1; i < 70; ++i )
	{
	  char filename[1000];
	  sprintf(filename, "%s/relative-intensities-%d.txt", relativeIntensities, i);
          tprintf("reading: %s\n", filename);
	  ifstream input;
	  input.open(filename);
	  int n;
	  float tmp;
	  input >> n;
	  Vector<float> inner;
	  
	  for ( int i= 0; i < n; ++i )
	  {
	    input >> tmp;
	    inner.push_back(tmp);
	  }
	  input >> n;
	  Vector<float> outer;
	  for ( int i= 0; i < n; ++i )
	  {
	    input >> tmp;
	    outer.push_back(tmp);
	  }
	  
	  relativeInner.push_back(inner);
	  relativeOuter.push_back(outer);
	}
	relativeInner(0)= relativeInner(1);
	relativeOuter(0)= relativeOuter(1);
	
	for ( unsigned int i= 0; i < relativeInner.size(); ++i )
	{
	  for ( int j= relativeInner(i).size()-1; j >= 0; --j )
	  {
	    if ( relativeInner(i)(j) != -1)
	    {
	      for ( int k= relativeInner(i).size()-1; k >= j; --k )
		relativeInner(i)(k)= relativeInner(i)(j-2);
	      break;
	    }
	  }
	}
	
	for ( unsigned int i= 0; i < relativeOuter.size(); ++i )
	{
	  for ( int j= relativeOuter(i).size()-1; j >= 0; --j )
	  {
	    if ( relativeOuter(i)(j) != -1 )
	    {
	      for ( int k= relativeOuter(i).size()-1; k >= j; --k )
		relativeOuter(i)(k)= relativeOuter(i)(j-2);
	      break;
	    }
	  }
	}
	
	dynthmod= dynth;
	
	relativeInnerSum.resize(relativeInner(0).size());
	relativeOuterSum.resize(relativeInner(0).size());
	relativeInnerSum= 0;
	relativeOuterSum= 0;
	Vector<int> inn;
	Vector<int> outn;
	inn.resize(relativeInner(0).size());
	outn.resize(relativeInner(0).size());
	inn= 0;
	outn= 0;
	for ( int i= 60; i < 70; ++i )
	  for ( unsigned int j= 0; j < relativeInner(i).size(); ++j )
	  {
	    if ( relativeInner(i)(j) >= 0 )
	    {
	      relativeInnerSum(j)+= relativeInner(i)(j);
	      inn(j)++;
	    }
	    if ( relativeOuter(i)(j) >= 0 )
	    {
	      relativeOuterSum(j)+= relativeOuter(i)(j);
	      outn(j)++;
	    }
	  }
	
	for ( unsigned int i= 0; i < relativeInnerSum.size(); ++i )
	{
	  if ( inn(i) > 0 )
	    relativeInnerSum(i)/= inn(i);
	  else
	    relativeInnerSum(i)= -1;
	  if ( outn(i) > 0 )
	    relativeOuterSum(i)/= outn(i);
	  else
	    relativeOuterSum(i)= -1;
	}
	
	tableth= 50;
    }

    template<typename INPUT>
    CorrectionOfEdgeBorders<INPUT>::CorrectionOfEdgeBorders(const CorrectionOfEdgeBorders& nrg)
        : RegionGrowing<INPUT>(nrg)
    {
        this->maxit= nrg.maxit;
	this->imult= nrg.imult;
	this->omult= nrg.omult;
	this->nw= nrg.nw;
	this->neighborhoods= nrg.neighborhoods;
	
	esab= new EqualSlopeAtBoundaries<float>(3, 47, 0.4, 1);
	subimage.resizeImage(3,3);
	this->useCache= nrg.useCache;
    }

    template<typename INPUT>
    CorrectionOfEdgeBorders<INPUT>::~CorrectionOfEdgeBorders()
    {
    }

    template<typename INPUT>
    void CorrectionOfEdgeBorders<INPUT>::init()
    {
    }

    template<typename INPUT>
    int CorrectionOfEdgeBorders<INPUT>::connectivityTerm()
    {
      Vector<int> q;
      
      int color= 0;
      for ( unsigned int i= 0; i < subimage.n; ++i )
      {
	if ( subimage(i) == 255 )
	{
	  ++color;
	  q.push_back(i);
	  subimage(i)= color;
	  while ( q.size() != 0 )
	  {
	    int p= q(0);
	    for ( unsigned int j= 0; j < neighborhoods(p).size(); ++j )
	    {
	      if ( subimage(neighborhoods(p)[j]) == 255 )
	      {
		q.push_back(neighborhoods(p)[j]);
		subimage(neighborhoods(p)[j])= color;
	      }
	    }
	    q.erase(q.begin());
	  }
	}
      }

      for ( unsigned int i= 0; i < subimage.n; ++i )
	if ( subimage(i) )
	  subimage(i)= 255;
      
      return color;
    }
    
   
    template<typename INPUT>
    void CorrectionOfEdgeBorders<INPUT>::getDesiredSlopes(float width, float dyn, float& desiredSlopeInner, float& desiredSlopeOuter)
    {
      //int dyni= (int)floor(dyn+0.5);

      float wwa= width - floor(width);
      float wwb= ceil(width) - width;      
      float wwc= dyn - floor(dyn);
      float wwd= ceil(dyn) - dyn;

      int upperwidth= 90;
      int upperdyn= 90;
      
      if ( width < upperwidth )
      {
	if ( dyn < relativeInner.size()-1 )
	{
	  int idxa= (int)ceil(width);
	  int idxb= (int)floor(width);
	  int idxc= (int)ceil(dyn);
	  int idxd= (int)floor(dyn);
	  
	  if ( idxa == idxb )
	  {
	    wwa= 1;
	    wwb= 0;
	  }
	  if ( idxc == idxd )
	  {
	    wwc= 1;
	    wwd= 0;
	  }
	
	  if ( dyn < upperdyn )
	  {
	    if ( relativeInner(idxc)(idxa) == -1 || relativeInner(idxd)(idxa) == -1 || relativeInner(idxc)(idxb) == -1 || relativeInner(idxd)(idxb) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    if ( relativeOuter(idxc)(idxa) == -1 || relativeOuter(idxd)(idxa) == -1 || relativeOuter(idxc)(idxb) == -1 || relativeOuter(idxd)(idxb) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	    desiredSlopeInner= wwa*wwc*relativeInner(idxc)(idxa) + wwa*wwd*relativeInner(idxd)(idxa) + wwb*wwc*relativeInner(idxc)(idxb) + wwb*wwd*relativeInner(idxd)(idxb);
	    desiredSlopeOuter= wwa*wwc*relativeOuter(idxc)(idxa) + wwa*wwd*relativeOuter(idxd)(idxa) + wwb*wwc*relativeOuter(idxc)(idxb) + wwb*wwd*relativeOuter(idxd)(idxb);
	  }
	  else
	  {
	    if ( relativeInnerSum(idxa) == -1 || relativeInnerSum(idxb) == -1 || relativeOuterSum(idxa) == -1 || relativeOuterSum(idxb) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	      desiredSlopeInner= wwa*relativeInnerSum(idxa) + wwb*relativeInnerSum(idxb);
	      desiredSlopeOuter= wwa*relativeOuterSum(idxa) + wwb*relativeOuterSum(idxb);
	  }
	}
	else
	{
	  int idxa= (int)ceil(width);
	  int idxb= (int)floor(width);
	  
	  if ( idxa == idxb )
	  {
	    wwa= 1;
	    wwb= 0;
	  }
	  
	  if ( dyn < upperdyn )
	  {
	    if ( relativeInner(relativeInner.size()-1)(idxa) == -1 || relativeInner(relativeInner.size()-1)(idxb) == -1 ||relativeOuter(relativeOuter.size()-1)(idxa) == -1 || relativeOuter(relativeOuter.size()-1)(idxb) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	    desiredSlopeInner= wwa*relativeInner(relativeInner.size()-1)(idxa) + wwb*relativeInner(relativeInner.size()-1)(idxb);
	    desiredSlopeOuter= wwa*relativeOuter(relativeInner.size()-1)(idxa) + wwb*relativeOuter(relativeInner.size()-1)(idxb);
	  }
	  else
	  {
	    if ( relativeInnerSum(idxa) == -1 || relativeInnerSum(idxb) == -1 || relativeOuterSum(idxa) == -1 || relativeOuterSum(idxb) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	    desiredSlopeInner= wwa*relativeInnerSum(idxa) + wwb*relativeInnerSum(idxb);
	    desiredSlopeOuter= wwa*relativeOuterSum(idxa) + wwb*relativeOuterSum(idxb);
	  }
	}
      }
      else
      {
	if ( dyn < relativeInner.size()-1 )
	{
	  int idxa= (int)ceil(dyn);
	  int idxb= (int)floor(dyn);
	  
	  if ( idxa == idxb )
	  {
	    wwc= 1;
	    wwd= 0;
	  }

	  if ( dyn < tableth )
	  {
	    if ( relativeInner(idxa)(upperwidth-1) == -1 || relativeInner(idxb)(upperwidth-1) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    if ( relativeOuter(idxa)(upperwidth-1) == -1 || relativeOuter(idxb)(upperwidth-1) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	    desiredSlopeInner= wwc*relativeInner(idxa)(upperwidth-1) + wwd*relativeInner(idxb)(upperwidth-1);
	    desiredSlopeOuter= wwc*relativeOuter(idxa)(upperwidth-1) + wwd*relativeOuter(idxb)(upperwidth-1);
	  }
	  else
	  {
	    if ( relativeInnerSum(relativeInnerSum.size()-1) == -1 || relativeOuterSum(relativeOuterSum.size()-1) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	    desiredSlopeInner= relativeInnerSum(relativeInnerSum.size()-1);
	    desiredSlopeOuter= relativeOuterSum(relativeOuterSum.size()-1);
	  }
	}
	else
	{
	  if ( dyn < upperdyn )
	  {
	    if ( relativeInner(relativeInner.size()-1)(upperwidth-1) == -1 || relativeOuter(relativeOuter.size()-1)(upperwidth-1) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    desiredSlopeInner= relativeInner(relativeInner.size()-1)(upperwidth-1);
	    desiredSlopeOuter= relativeOuter(relativeInner.size()-1)(upperwidth-1);
	  }
	  else
	  {
	    if ( relativeInnerSum(relativeInnerSum.size()-1) == -1 || relativeOuterSum(relativeOuterSum.size()-1) == -1 )
	    {
	      desiredSlopeInner= -1;
	      return;
	    }
	    
	    desiredSlopeInner= relativeInnerSum(relativeInnerSum.size()-1);
	    desiredSlopeOuter= relativeOuterSum(relativeOuterSum.size()-1);
	  }
	}
      }
    }
 
     
 
    template<typename INPUT>
    float CorrectionOfEdgeBorders<INPUT>::energy(Image<INPUT>& /*input*/, Image<unsigned char>& labels, Image<unsigned char>* , int n, Image<unsigned char>& labels2, int /*flag*/)
    {
      float energy= 0;
      
      float desiredSlopeInner= 0.5;
      float desiredSlopeOuter= 0.8;
      
      float sWeight= 1;
      float nWeight= nw;
      
      float slope= 0;

      float width= widthCache(n);
      
      meanWidth+= width;
      nWidth+= 1.0f;
      
      //width= width/widthScaling;
      
      //int widthi= floor(width + 0.5);
      float dyn= dynCache(n);
      
      if ( dyn < dynth )
	return 0;
      
      /*if ( width < 2.5 && inner )
	return 0;*/
      
      getDesiredSlopes(width, dyn, desiredSlopeInner, desiredSlopeOuter);
      
      if ( desiredSlopeInner < 0 )
	return 0;

      //desiredSlopeInner+= (imult - 1)/2;
      //desiredSlopeOuter+= (omult - 1)/2;
      if ( imult < 1 )
      {
	desiredSlopeInner*= imult;
	desiredSlopeOuter*= omult;
      }
      else
      {
	desiredSlopeInner= 1 - (1 - desiredSlopeInner)/imult;
	desiredSlopeOuter= 1 - (1 - desiredSlopeOuter)/imult;
      }
      
      float value= riCache(n);
      
      if ( value == -1 )
	  return 0;

      if ( labels(n) )
      {
	if ( value < desiredSlopeInner )
	  slope= 0;
	else
	  /*slope= (value - desiredSlopeInner)/(desiredSlopeOuter - desiredSlopeInner);*/
	  //slope= (value - desiredSlopeInner)/(1 - desiredSlopeInner);
	  slope= (value - desiredSlopeInner)/(desiredSlopeOuter - desiredSlopeInner);
	  //slope= (value - desiredSlopeInner)/(desiredSlopeOuter - desiredSlopeInner);
      }
      else
      {
	if ( value >= desiredSlopeOuter )
	  slope= 0;
	else
	  slope= (desiredSlopeOuter - value)/(desiredSlopeOuter - desiredSlopeInner);
	  //slope= (desiredSlopeOuter - value)/(desiredSlopeOuter);
	  //slope= (desiredSlopeOuter - value) / (desiredSlopeOuter - desiredSlopeInner);
      }
      
      float neighborhood= 0;
      
      int nn= 0;
      for ( unsigned int j= 0; j < ses3.size(); ++j )
	  if ( ses3(j) != 0 && labels2(n + ses3(j)) == labels(n) )
	    ++nn;
      neighborhood= (1 - nn/8.0);
      
      if ( width < 2 && inner )
	neighborhood= 0;
      
      /*if ( fabs(slope) > 1 )
	slope= 1;*/
      
      energy= sWeight*slope + nWeight*neighborhood;
      
      return energy;
    }
    
    template<typename INPUT>
    int CorrectionOfEdgeBorders<INPUT>::optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int /*iteration*/)
    {
      meanWidth= 0;
      nWidth= 0;
      Vector<int> pixels;
      int changed= 0;

      labels_original.resizeImage(labels);
      labels_original= labels;
      
      for ( unsigned int i= 0; i < input.n; ++i )
	if ( slopeMask(i) )
	  pixels.push_back(i);
      
      //printf(" %d,", pixels.size()); fflush(stdout);
	
      #pragma omp parallel for num_threads(8)
      for ( unsigned int i= 0; i < pixels.size()*4; ++i )
      {
	int r= rand()%pixels.size();
	if ( (roi && !(*roi)(pixels(r))) || !slopeMask(pixels(r)) )
	  continue;

	int before= -1, after= -1;

	#pragma omp critical
	{
	  if ( labels(pixels(r)) && inner )
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
	
	float err1= 0;

	err1= energy(input, labels, roi, pixels(r), labels_original, 0);
	  
	if ( labels(pixels(r)) == 255 )
	  labels(pixels(r))= 0;
	else
	  labels(pixels(r))= 255;
	
	float err2= 0;
	err2= energy(input, labels, roi, pixels(r), labels_original, 1);
	
	if ( err1 <= err2 )
	  labels(pixels(r))= (labels(pixels(r)) == 255) ? 0 : 255;
	else
	{
	  ++changed;
	}
      }
      
      //printf("%d ", changed); fflush(stdout);
      return changed;
    }
    
    template<typename INPUT>
    float CorrectionOfEdgeBorders<INPUT>::error(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* , int n, Image<unsigned char>& labels2, int /*flag*/, float& /*weight*/)
    {
      float energy= 0;
      
      float desiredSlopeInner= 0.5;
      float desiredSlopeOuter= 0.8;
      
      float sWeight= 1;
      float nWeight= nw;
      
      float slope= 0;

      float width, dyn, value;
      
      if ( useCache )
      {
	width= widthCache(n);
	dyn= dynCache(n);
	value= riCache(n);
      }
      else
      {
	esab->computeDescriptors(labels, n, input, width, dyn, value);
      }

      //width= width/widthScaling;

      if ( dyn < dynth )
	return -10;
      
      getDesiredSlopes(width, dyn, desiredSlopeInner, desiredSlopeOuter);
      
      if ( desiredSlopeInner == -1 || desiredSlopeOuter == -1 )
	return -10;
      
      if ( imult < 1 )
      {
	desiredSlopeInner*= imult;
	desiredSlopeOuter*= omult;
      }
      else
      {
	desiredSlopeInner= 1 - (1 - desiredSlopeInner)/imult;
	desiredSlopeOuter= 1 - (1 - desiredSlopeOuter)/imult;
      }

      if ( value == -1 )
	return -10;

      {
	slope= -1;
	
	if ( labels(n) && slope == -1 )
	{
	  if ( value < desiredSlopeInner )
	    slope= 0;
	  else
	    slope= (value - desiredSlopeInner)/(desiredSlopeOuter - desiredSlopeInner);
	}
	else if ( slope == -1 )
	{
	  if ( value > desiredSlopeOuter )
	    slope= 0;
	  else
	    slope= (desiredSlopeOuter - value)/(desiredSlopeOuter - desiredSlopeInner);
	}
	
	float neighborhood= 0;
	
	int nn= 0;
	for ( unsigned int j= 0; j < ses3.size(); ++j )
	    if ( ses3(j) != 0 && labels2(n + ses3(j)) == labels(n) )
	      ++nn;
	neighborhood= (1 - nn/8.0);
	
	energy= sWeight*slope + nWeight*neighborhood;
      }
      
      return energy;
    }
    
    template<typename INPUT>
    float CorrectionOfEdgeBorders<INPUT>::error(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, int /*iteration*/, int flag)
    {
      Vector<int> pixels;

      labels_original.resizeImage(labels);
      labels_original= labels;
      
      for ( unsigned int i= 0; i < input.n; ++i )
	if ( slopeMask(i) )
	  pixels.push_back(i);

      float err= 0;
      float nerr= 0;
      float n= 0;
      float weight= 0;
      
      for ( unsigned int i= 0; i < pixels.size(); ++i )
      {
	int r= i;
	if ( (roi && !(*roi)(pixels(r))) || !slopeMask(pixels(r)) )
	  continue;

	++n;
	float tmp= error(input, labels, roi, pixels(r), labels_original, flag, weight);
	if ( tmp > -10 )
	{
	    err+= tmp;
	    nerr+= 1;
	}
      }

      if ( nerr > 0 )
      {
	return err/nerr;
      }
      else
	return FLT_MAX;
    }
    
    template<typename INPUT>
    float CorrectionOfEdgeBorders<INPUT>::getError(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* roi, int flag)
    {
      rierror.resize(relativeInner.size());
      roerror.resize(relativeInner.size());
      rin.resize(relativeInner.size());
      ron.resize(relativeInner.size());
      
      wh.resize(50);
      wh= 0;
      whi.resize(50);
      whi= 0;
      who.resize(50);
      who= 0;
      
      errors.clear();
      
      for ( unsigned int i= 0; i < rierror.size(); ++i )
      {
	rierror(i).resize(relativeInner(i).size());
	roerror(i).resize(rierror(i).size());
	rin(i).resize(rierror(i).size());
	ron(i).resize(rierror(i).size());
	
	rierror(i)= 0;
	roerror(i)= 0;
	rin(i)= 0;
	ron(i)= 0;
      }
      
      Image<unsigned char> tmp, tmp2;
      
      tmp.resizeImage(output);
      tmp2.resizeImage(tmp);
      
      slopeMask= 0;
      tmp= 0;
      tmp2= 0;
      
      grayscaleErode(&output, &tmp, ses3);

      for ( unsigned int i= 0; i < output.n; ++i )
      {
	if ( output(i) != tmp(i) && !thinned(i) )
	    slopeMask(i)= 255;
      }
      
      float err= 0;
      
      {
	fillCaches(output, input);
	
	err= error(input, output, roi, 0, flag);
	
	slopeMask= 0;
      }
      
      tmp= 1;
      
      grayscaleDilate(&output, &tmp, ses5);
      
      for ( unsigned int i= 0; i < input.n; ++i )
	if ( output(i) != tmp(i)  )
	  slopeMask(i)= 255;
	
      fillCaches(output, input);
      
      err+= error(input, output, roi, 0, flag);
      
      return fabs(err);
    }
    
    template<typename INPUT>
    void CorrectionOfEdgeBorders<INPUT>::fillCaches(Image<unsigned char>& labels, Image<INPUT>& input)
    {
      //tprintf("filling caches...");
      widthCache= -1;
      riCache= 0;
      dynCache= 0;
      
      float ri, width, dyn;
      
      meanWidths.resizeImage(input);
      meanWidths= -1;
      
      for ( unsigned int i= -ses3.getMin(); i < labels.n - ses3.getMax(); ++i )
      {
	if ( slopeMask(i) )
	{
	  esab->computeDescriptors(labels, i, input, width, dyn, ri);
	  widthCache(i)= width;
	  riCache(i)=  ri;
	  dynCache(i)= dyn;
	}
      }
      
      float tmp= 0;
      int n= 0;
      for ( unsigned int i= -ses5.getMin(); i < input.n - ses5.getMax(); ++i )
      {
	tmp= 0;
	n= 0;
	
	{
	  for ( unsigned int j= 0; j < ses5.size(); ++j )
	    if ( widthCache(i) >= 0 )
	    if ( esab->isInnerContour8(labels,i + ses5(j)) || esab->isOuterContour8(labels,i + ses5(j)) )
	      if ( widthCache(i + ses5(j)) >= 0 )
	      {
		tmp+= widthCache(i + ses5(j));
		++n;
	      }
	  
	  if ( n > 0 )
	  {
	    meanWidths(i)= tmp/n;
	  }
	  else
	    meanWidths(i)= 0;
	}
      }

      widthCache= meanWidths;
    }
     
    
     
    template<typename INPUT>
    void CorrectionOfEdgeBorders<INPUT>::doRG(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> &seed, Image<unsigned char> *roi, float* errorValue)
    {
	esab->updateStride(input.columns);
	ses3.updateStride(input.columns);
	ses5.updateStride(input.columns);
	sed10.updateStride(input.columns);
	slopeMask.resizeImage(output);
	modified.resizeImage(output);
	lastImage.resizeImage(output);
	slopeMask= 0;
	modified= 0;
	meanWidths.resizeImage(output);
	widthCache.resizeImage(output);
	riCache.resizeImage(output);
	dynCache.resizeImage(output);
	
	thinned= seed;
	thinned= 0;
	thinningCWSI(seed, thinned);

	tprintf("starting\n");

        output= seed;

        Vector<Region2> results;
        ExtractRegions er;

	Image<unsigned char> tmp, tmp2;
	tmp.resizeImage(output);
	tmp2.resizeImage(output);
	
	Image<unsigned char> bestResult;
	
	int n= 0;
	for ( unsigned int i= -ses3.getMin(); i < thinned.n - ses3.getMax(); ++i )
	{
	  if ( thinned(i) )
	  {
	    n= 0;
	    for ( unsigned int j= 0; j < ses3.size(); ++j )
	      if ( ses3(j) != 0 )
		if ( thinned(i + ses3(j)) )
		  ++n;
	    if ( n <= 1 )
	      slopeMask(i)= 255;
	    else
	      slopeMask(i)= 0;
	  }
	}
	
	thinned= 0;
	
	for ( unsigned int i= 0; i < slopeMask.n; ++i )
	  if ( slopeMask(i) )
	    for ( unsigned int j= 0; j < ses3.size(); ++j )
	      if ( i + ses3(j) >= 0 && i + ses3(j) < slopeMask.n )
		thinned(i + ses3(j))= 255;
		
	slopeMask= 0;
	
        tprintf("starting iteration\n");

        int iteration= 0;
	
	meanWidth= 1;
	nWidth= 1;
	
	//int innerIterations= (int)floor(widthScaling + 0.5);
	int innerIterations= 1;
	
	printf("inner: %d\n", innerIterations);
        while ( 1 )
        {
	  if ( (maxit >= 0 && iteration >= maxit) )
	  {
	      output= bestResult;
	      if ( errorValue != NULL )
	      {
		*errorValue= getError(input, output, roi, 0);
		printf("\nfinal error: %f, iterations: %d\n", *errorValue, iteration);
	      }
	      return;
	  }
	  
	  if ( iteration < innerIterations )
	  {
	    slopeMask= 0;
	    tmp= 0;
	    StructuringElementDisk ses3(1);
	    ses3.updateStride(output.columns);
	    grayscaleErode(&output, &tmp, ses3);
	    inner= 1;
	    for ( unsigned int i= 0; i < input.n; ++i )
	    {
	      if ( output(i) != tmp(i) && !thinned(i) && output.isRealImagePixel(i) )
		  slopeMask(i)= 255;
	    }
	    fillCaches(output, input);
	  }
	  else
	  {
	    slopeMask= 0;
	    inner= 0;
	    tmp= 1;
	    StructuringElementDisk sed3(1);
	    ses3.updateStride(output.columns);
	    grayscaleDilate(&output, &tmp, ses3);
	    
	    for ( unsigned int i= 0; i < input.n; ++i )
	      if ( output(i) != tmp(i) && output.isRealImagePixel(i) )
		slopeMask(i)= 255;
	      else
		slopeMask(i)= 0;
	    
	    fillCaches(output, input);
	  }
	  
	  int changed= 1;
	  int i;
	  
	  Image<unsigned char> last;
	  last= output;
	  
	  for ( i= 0; i < 40 && changed > 0; ++i )
	  {
	    changed= optimize(input, output, roi, iteration);
	    printf("%d ", changed); fflush(stdout);
	  }

	  if ( i == 1 )
	    iteration= maxit;
	  
	  bestResult= output;
	  
	  ++iteration;
	}

    }
    
    
    template<typename INPUT>
    class AdditionOfThinObjects: public RegionGrowing<INPUT>
    {
    public:
        AdditionOfThinObjects(float mParameter= 0.1, float aParameter= 0.6, float fParameter= 0.0, float cParameter= 0.5, int sizeThreshold0= 20, int sizeThreshold1= 200, float widthScaling= 1.0, float shift= 1.0, float dynth= 8, float nw= 2.5, float wa= 5.5, float wb= 7.5);

        AdditionOfThinObjects(const AdditionOfThinObjects& nrg);

        ~AdditionOfThinObjects();

        virtual void init();

        virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

	int optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, Image<unsigned char>* support, int iteration);

	Image<float> filterResponses;
	Image<float> maxResponses;
	Image<float> sumOfResults;
	Vector<Image<float> > frs;
	Vector<int> lambdas;
	
        Vector<MatchedCCorrelationPowerGaborFilterR2<float, float>* > filters;
        Vector<MatchedCCorrelationPowerGaborFilterSimpleR2<float, float>* > pfilters;
	
	Vector<PowerGaborRGLineSegmentTransform2<float, float>* > operators;
	Vector<PowerGaborSimpleRGLineSegmentTransform2<float, float>* > poperators;
        
	float mParameter, aParameter, fParameter, cParameter;
	int sizeThreshold0, sizeThreshold1;
	float widthScaling;
	float shift;
	float dynth;
	float nw;
	float dist;
	float wa, wb;
	float minth1, minth2, maxth1, maxth2;
	
	Vector<Image<float> > images;
    };

    template<typename INPUT>
    AdditionOfThinObjects<INPUT>::AdditionOfThinObjects(float mParameter, float aParameter, float fParameter, float cParameter, int sizeThreshold0, int sizeThreshold1, float widthScaling, float shift, float dynth, float nw, float wa, float wb)
        : RegionGrowing<INPUT>()
    {
      tprintf("width scaling: %f, aParameter: %f\n", widthScaling, aParameter);
      tprintf("wa: %f, wb: %f\n", wa, wb);
      this->mParameter= mParameter;
      this->aParameter= aParameter;
      this->fParameter= fParameter;
      this->cParameter= cParameter;
      this->sizeThreshold0= sizeThreshold0;
      this->sizeThreshold1= sizeThreshold1;
      this->widthScaling= widthScaling;
      this->shift= shift;
      this->dynth= dynth;
      this->nw= nw;
      this->wa= wa;
      this->wb= wb;
      
      Transform2Set<float, float> f= *(generateTransform2Set<float, float>(std::string("trained-features-selected-for-accuracy-iteration-2.fdf"), std::string("feature")));
      
      maxth1= maxth2= 0;
      minth1= minth2= FLT_MAX;
      
      int flag= 0;
      
      for ( unsigned int i= 0; i < f.size(); ++i )
      {
	if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i)) != NULL )
	{
	  PowerGaborRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i));
	  if ( tmp->lambda < wa*1.2 && tmp->lambda >= wa*0.8)
	  {
	    if ( tmp->th1 < minth1 )
	      minth1= tmp->th1;
	    if ( tmp->th2 < minth2 )
	      minth2= tmp->th2;
	    if ( tmp->th1 > maxth1 )
	      maxth1= tmp->th1;
	    if ( tmp->th2 > maxth2 )
	      maxth2= tmp->th2;
	    
	    operators.push_back(tmp);
	    
	    if ( flag == 0 )
	    {
	      tprintf("scale factor passed as parameter: %f\n", widthScaling);
	      ++flag;
	    }
	    tmp->scale= 1.0/widthScaling;
	    //tmp->step= M_PI/60.0;
	    tmp->regenerate();
	    
	    /*if ( input.rows < 1000 )
	      filters.push_back((new PowerGaborRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, 3.14/60.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	    else*/
	      //filters.push_back((new PowerGaborRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, M_PI/60.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	      filters.push_back(tmp->mgf);
	    
	    //tprintf("%f %f %f %f\n", tmp->sigma, tmp->lambda, tmp->gamma, tmp->powerFactor);
	    
	    lambdas.push_back(tmp->lambda);
	  }

	}
	else if (dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i)) != NULL)
	{
	  PowerGaborSimpleRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i));
	  if ( /*tmp->lambda * widthScaling <= minimumLambda &&*/ /*tmp->gamma <= 0.9*/ /*&& tmp->lambda >= 5.5*//* &&*/ tmp->lambda/**widthScaling*/ < wa*1.2 && tmp->lambda/**widthScaling*/ >= wa*0.8)
	  //if ( tmp->lambda == floor(wa + 0.5) )
	  {
	    poperators.push_back(tmp);
	    
	    if ( tmp->th1 < minth1 )
	      minth1= tmp->th1;
	    if ( tmp->th2 < minth2 )
	      minth2= tmp->th2;
	    if ( tmp->th1 > maxth1 )
	      maxth1= tmp->th1;
	    if ( tmp->th2 > maxth2 )
	      maxth2= tmp->th2;
	    
	    /*tmp->lambda/= widthScaling;
	    tmp->sigma/= widthScaling;*/
	    tmp->scale= 1.0/widthScaling;
	    //tmp->step=M_PI/60.0;
	    tmp->regenerate();
	    
	    /*if ( input.rows < 1000 )
	      pfilters.push_back((new PowerGaborSimpleRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, 3.14/60.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	    else*/
	      //pfilters.push_back((new PowerGaborSimpleRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, M_PI/60.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	      pfilters.push_back(tmp->mgf);
	    
	    lambdas.push_back(tmp->lambda);
	  }
	}
      }
      tprintf("number of filters: %zd %zd\n", filters.size(), pfilters.size());
      tprintf("minth1: %f, maxth1: %f, minth2: %f, maxth2: %f\n", minth1, maxth1, minth2, maxth2);
	
      if ( this->aParameter < minth2 )
        this->aParameter= minth2;
      tprintf("modified a parameter: %f\n", this->aParameter);
    }

    template<typename INPUT>
    AdditionOfThinObjects<INPUT>::AdditionOfThinObjects(const AdditionOfThinObjects& nrg)
        : RegionGrowing<INPUT>(nrg)
    {
        this->filters= nrg.filters;
	this->pfilters= nrg.pfilters;
	this->sizeThreshold0= nrg.sizeThreshold0;
	this->sizeThreshold1= nrg.sizeThreshold1;
	this->mParameter= nrg.mParameter;
	this->aParameter= nrg.aParameter;
	this->fParameter= nrg.fParameter;
	this->cParameter= nrg.cParameter;
    }

    template<typename INPUT>
    AdditionOfThinObjects<INPUT>::~AdditionOfThinObjects()
    {
    }

    template<typename INPUT>
    void AdditionOfThinObjects<INPUT>::init()
    {
    }
    
    template<typename INPUT>
    int AdditionOfThinObjects<INPUT>::optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, Image<unsigned char>* support, int /*iteration*/)
    {
      filterResponses= 0;
      maxResponses= 0;
      
      for ( unsigned int i= 0; i < filters.size(); ++i )
      {
	filters(i)->updateStride(input.columns);
	filters(i)->computeMinMax();
      }
      
      for ( unsigned int i= 0; i < pfilters.size(); ++i )
      {
	pfilters(i)->updateStride(input.columns);
	pfilters(i)->computeMinMax();
      }

      Vector<StandardDeviationFeature2<float, float>* > sdfs;
      Vector<TotalVariationFeature2<float, float>* > tvfs;
      sdfs.push_back(new StandardDeviationFeature2<float, float>(9));
      tvfs.push_back(new TotalVariationFeature2<float, float>(9));
      
      Image<float> sdImage;
      sdImage.resizeImage(input);
      Image<float> tvImage;
      tvImage.resizeImage(input);
      
      Image<unsigned char> reducedROI;
      reducedROI.resizeImage(input);
      reducedROI= *roi;

      Image<int> nums;
      nums.resizeImage(input);
      
      nums= 0;
      
      int caching= 1;
      
      #pragma omp parallel for num_threads(8), schedule(dynamic, 2)
      for ( unsigned int i= 0; i < filters.size(); ++i )
      {
	Image<float> tmp;
	Image<unsigned char> tmp2;
	tmp.resizeImage(input);
	tmp2.resizeImage(input);
	
	string featureName= operators(i)->getDescriptor('_');
	File file(input.filename);
	
	char filename[1000]/*, filename2[1000]*/;
	sprintf(filename, "cache4/%s-%s-%f.float", file.getFilename().c_str(), featureName.c_str(), widthScaling);
	
	FILE* fil= fopen(filename, "r");
	int exists= (fil != NULL);
	if ( exists )
	  fclose(fil);
	if ( exists && caching )
	{
	  readImage(filename, tmp, input.getBorder2());
	  //readImage(filename2, tmp2, input.getBorder2());
	  printf(","); fflush(stdout);
	}
	else if ( !exists || !caching )
	  filters(i)->apply(input, tmp, roi, support);
	if ( !exists && caching )
	  writeImage(filename, tmp);
	
	//images.push_back(tmp);
	
	#pragma omp critical
	{
	  for ( unsigned int j= 0; j < tmp.n; ++j )
	    if ( !roi || (*roi)(j) )
	    {
	      labels(j)= 255;
	      if ( tmp(j) > fParameter )
	      {
		nums(j)++;
		filterResponses(j)+= tmp(j);
	      }
	      if ( tmp(j) > maxResponses(j) )
		maxResponses(j)= tmp(j);
	    }
	  printf("."); fflush(stdout);
	}
      }
      
      
      //#pragma omp parallel for num_threads(4)
      //for ( int i= 0; i < poperators.size(); ++i )
      #pragma omp parallel for num_threads(4), schedule(dynamic, 2)
      for ( unsigned int i= 0; i < pfilters.size(); ++i )
      {
	Image<float> tmp;
	tmp.resizeImage(input);
	Image<float> tmp2;
	tmp2.resizeImage(input);
	
	string featureName= poperators(i)->getDescriptor('_');
	File file(input.filename);
	
	char filename[1000]/*, filename2[1000]*/;
	sprintf(filename, "cache4/%s-%s-%f.float", file.getFilename().c_str(), featureName.c_str(), widthScaling);
	//sprintf(filename2, "cache/%s-%s.bmp", file.getFilename().c_str(), featureName.c_str());
	
	FILE* fil= fopen(filename, "r");
	int exists= (fil != NULL);
	if ( exists )
	  fclose(fil);
	if ( exists && caching )
	{
	  readImage(filename, tmp, input.getBorder2());
	  printf(","); fflush(stdout);
	}
	else if ( !exists || !caching )
	  pfilters(i)->apply(input, tmp, roi, support);
	if ( !exists && caching )
	  writeImage(filename, tmp);
	
	//images.push_back(tmp);
	
	#pragma omp critical
	{
	  for ( unsigned int j= 0; j < tmp.n; ++j )
	    if ( !roi || (*roi)(j) )
	    {
	      labels(j)= 255;
	      if ( tmp(j) > fParameter )
	      //if ( tmp2(j) > fParameter )
	      {
		nums(j)++;
		filterResponses(j)+= tmp(j);
	      }
	      if ( tmp(j) > maxResponses(j) )
		maxResponses(j)= tmp(j);
	    }
	  printf("."); fflush(stdout);
	}
      }
      
      for ( unsigned int i= 0; i < nums.n; ++i )
	if ( nums(i) > 0 )
	  filterResponses(i)/= nums(i);
      
	
      //filterResponses= maxResponses;
      
      return 0;
    }

    void addThinVessels(Image<float> &input, Image<unsigned char>& output, float length, float th0)
    {
      Image<unsigned char> tmp;
      tmp.resizeImage(input);
      for ( float threshold= th0; threshold < 1; threshold+= 0.01 )
      {
	for ( unsigned int i= 0; i < input.n; ++i )
	  if ( input(i) > threshold )
	    tmp(i)= 255;
	  else
	    tmp(i)= 0;
	  
	ExtractRegions er;
	Region2Set rs;
	er.apply(tmp, rs);
	
	for ( unsigned int i= 0; i < rs.size(); ++i )
	  if ( rs(i).getDistanceOfFarestPixels() > length )
	    for ( unsigned int k= 0; k < rs(i).size(); ++k )
	      output(rs(i)(k))= 255;
      }
    }
    
    template<typename INPUT>
    void AdditionOfThinObjects<INPUT>::doRG(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> &seed, Image<unsigned char> *roi, Image<unsigned char>* support)
    {
	tprintf("addition of thin objects -- mp %f, ap %f, fp %f, cp %f, sizeth0 %d, sizeth1 %d, dynth %f, widthScaling %f, shift %f\n", mParameter, aParameter, fParameter, cParameter, sizeThreshold0, sizeThreshold1, dynth, widthScaling, shift);
	filterResponses.resizeImage(seed);
	filterResponses= -1;
	maxResponses.resizeImage(seed);
	maxResponses= -1;
	sumOfResults.resizeImage(seed);
	sumOfResults= 0;

	tprintf("starting\n");
        int iteration= 0;

	int regionsAdded= 0;
	Image<unsigned char> tmpimg;
	tmpimg.resizeImage(filterResponses);
	
	output= seed;
	
	regionsAdded= 0;
	optimize(input, tmpimg, roi, support, iteration);
	
	Image<unsigned char> tmpb, tmpc, tmpd;
	tmpb.resizeImage(filterResponses);
	tmpc.resizeImage(filterResponses);
	tmpd.resizeImage(filterResponses);
	tmpb= 0;
	tmpc= 0;
	tmpd= 0;
	
	float meanThreshold= aParameter;
	//float stdevThreshold= mParameter;

	Vector<Region2> regionsInserted;

	Image<int> numbers;
	numbers.resizeImage(filterResponses);
	numbers= 0;
	
	tprintf("thresholding...\n");
	tmpd= output;
	
	//float minError= FLT_MAX;
	//float minThreshold= meanThreshold;
	StructuringElementSquare ses3(3);
	ses3.updateStride(input.columns);
	
	nw= 0;
	CorrectionOfEdgeBorders<float> coeb(1, 1, 1, 2.8, NULL, NULL, widthScaling, dynth, shift);
	coeb.esab->updateStride(input.columns);
	coeb.ses3.updateStride(input.columns);
	coeb.ses5.updateStride(input.columns);
	coeb.useCache= 0;
	
	tprintf("starting\n");
	
	
	
	Image<float> tosave;
	tosave.resizeImage(filterResponses);
	tosave= filterResponses;
	tosave.normalize(0, 255);
	writeImage("filterResponses.bmp", tosave);
	
	for ( float threshold= 0.45; threshold <= 1; threshold+= 0.01 )
	{
	  printf("%f ", threshold); fflush(stdout);
	  for ( unsigned int i= 0; i < filterResponses.n; ++i )
	  {
	    if ( filterResponses(i) > threshold )
	      tmpb(i)= 255;
	    else
	      tmpb(i)= 0;
	  }
	    
	  Region2Set regions, regions2;
	  ExtractRegions er;
	  
	  er.apply(tmpb, regions);
	  
	  #pragma omp parallel for num_threads(4), schedule(dynamic, 2)
	  for ( unsigned int i= 0; i < regions.size(); ++i )
	  {
	    if ( regions(i).size() > 150000 )
	      continue;
	    if ( regions(i).size() <= 1)
	      continue;
	    
	    
	    Region2 tmpRegion;
	    tmpRegion.stride= tmpb.columns;

	    tmpRegion= regions(i);
	    
	    float regionSize= tmpRegion.size();
	    
	    if ( regionSize < sizeThreshold0 || regionSize > sizeThreshold1 )
	      continue;
	      
	    
	    //float rmin= tmpRegion.getMin(filterResponses);
	    //float rmean= tmpRegion.getMean(filterResponses);
	    //float rstdev= tmpRegion.getStandardDeviation(filterResponses);
	    float dofp= tmpRegion.getDistanceOfFarestPixels();
	    //float rect= tmpRegion.rectangularity();
	    
	    float rmean= 0;
	    int n= 0;
	    for ( unsigned int i= 0; i < tmpRegion.size(); ++i )
	      if ( !output(tmpRegion(i)) )
	      {
		rmean+= filterResponses(tmpRegion(i));
		++n;
	      }
	    rmean/= n;
	    
	    //printf("%f,%d ", dofp, tmpRegion.size());
	    //printf("%f,%f ", lcorr.getMean(), lcorr.getStandardDeviation());
	    
	    //printf("%d/%d ", i, regions.size()); fflush(stdout);
	    if ( dofp > sizeThreshold0  && rmean > meanThreshold/*&& -lcorr.getMean() > meanThreshold && flag*/ )
	    {
	      {
	      #pragma omp critical
	      {
		{
		  tmpRegion.set(output, 0, (unsigned char)255);
		  tmpRegion.set(tmpc, 0, (unsigned char)255);
		  regionsAdded++;
		}
	      }
	      }
	    }
	  }
	}
	
	tprintf("regions added: %d ", regionsAdded); fflush(stdout);
    }
    
    template<typename INPUT>
    class AdditionOfThinObjectsMTM: public RegionGrowing<INPUT>
    {
    public:
        AdditionOfThinObjectsMTM(float mParameter= 0.1, float aParameter= 0.6, float fParameter= 0.0, float cParameter= 0.5, int sizeThreshold0= 20, int sizeThreshold1= 200, float widthScaling= 1.0);

        AdditionOfThinObjectsMTM(const AdditionOfThinObjectsMTM& nrg);

        ~AdditionOfThinObjectsMTM();

        virtual void init();

        virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

	int optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, Image<unsigned char>* support, int iteration);

	Image<float> filterResponse;
	
        Vector<MatchedPWCMTMCCorrGaborFilterR2<float, float>* > filters;
        
	float mParameter, aParameter, fParameter, cParameter;
	int sizeThreshold0, sizeThreshold1;
	float widthScaling;
    };

    template<typename INPUT>
    AdditionOfThinObjectsMTM<INPUT>::AdditionOfThinObjectsMTM(float mParameter, float aParameter, float fParameter, float cParameter, int sizeThreshold0, int sizeThreshold1, float widthScaling)
        : RegionGrowing<INPUT>()
    {
      tprintf("width scaling: %f\n", widthScaling);
      this->mParameter= mParameter;
      this->aParameter= aParameter;
      this->fParameter= fParameter;
      this->cParameter= cParameter;
      this->sizeThreshold0= sizeThreshold0;
      this->sizeThreshold1= sizeThreshold1;
      this->widthScaling= widthScaling;
      
      Transform2Set<float, float> f= *(generateTransform2Set<float, float>(std::string("trained-features-selected-for-accuracy-iteration-2.fdf"), std::string("feature")));
      
      for ( int i= 0; i < f.size(); ++i )
	/*if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i)) != NULL )
	{
	  PowerGaborRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i));
	  if ( tmp->lambda <= minimumLambda && tmp->gamma <= 0.8 )
	  {
	    operators.push_back(tmp);
	    
	    tmp->lambda*= widthScaling;
	    tmp->sigma*= widthScaling;
	    tmp->regenerate();
	    
	    filters.push_back((new PowerGaborRGLineSegmentTransform2<float, float>(tmp->th1, tmp->th2, tmp->sigma, tmp->theta0, 3.14/60.0, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma, tmp->powerFactor))->mgf);
	    
	    lambdas.push_back(tmp->lambda);
	  }

	}
	else*/ if (dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i)) != NULL)
	{
	  PowerGaborSimpleRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i));
	  
	  Vector<float> lambdas;
	  Vector<float> gammas;
	  
	  if ( tmp->lambda == 6 && tmp->gamma <= 0.8 )
	  {
	    int flag= 0;
	    /*for ( int i= 0; i < lambdas.size(); ++i )
	      if ( lambdas(i) == tmp->lambda && gammas(i) == tmp->gamma )
	      {
		flag= 1;
		break;
	      }*/
	    if ( !flag )
	    {
	      filters.push_back(new MatchedPWCMTMCCorrGaborFilterR2<float, float>(tmp->sigma, tmp->theta0, 3.14/90, tmp->theta1, tmp->lambda, tmp->psi, tmp->gamma));
	      lambdas.push_back(tmp->lambda);
	      gammas.push_back(tmp->gamma);
	    }
	    //break;
	  }
	}
	printf("number of filters: %zd\n", filters.size());
    }

    template<typename INPUT>
    AdditionOfThinObjectsMTM<INPUT>::AdditionOfThinObjectsMTM(const AdditionOfThinObjectsMTM& nrg)
        : RegionGrowing<INPUT>(nrg)
    {
        this->filters= nrg.filters;
	this->sizeThreshold0= nrg.sizeThreshold0;
	this->sizeThreshold1= nrg.sizeThreshold1;
	this->mParameter= nrg.mParameter;
	this->aParameter= nrg.aParameter;
	this->fParameter= nrg.fParameter;
	this->cParameter= nrg.cParameter;
    }

    template<typename INPUT>
    AdditionOfThinObjectsMTM<INPUT>::~AdditionOfThinObjectsMTM()
    {
    }

    template<typename INPUT>
    void AdditionOfThinObjectsMTM<INPUT>::init()
    {
    }
    
    template<typename INPUT>
    int AdditionOfThinObjectsMTM<INPUT>::optimize(Image<INPUT>& input, Image<unsigned char>& labels, Image<unsigned char>* roi, Image<unsigned char>* support, int )
    {
      filterResponse= 0;
      
      for ( int i= 0; i < filters.size(); ++i )
      {
	filters(i)->updateStride(input.columns);
	filters(i)->computeMinMax();
      }

      tprintf("applying filters...\n");
      
      tprintf("filterResponses: %d %d\n", filterResponse.rows, filterResponse.columns);
      
      
      Image<int> nums;
      nums.resizeImage(input);
      
      nums= 0;
      
      #pragma omp parallel for num_threads(4)
      for ( int i= 0; i < filters.size(); ++i )
      {
	Image<float> tmp;
	tmp.resizeImage(input);
	
	filters(i)->apply(input, tmp, nums, roi, support);
	
	#pragma omp critical
	{
	  for ( int j= 0; j < tmp.n; ++j )
	    if ( !roi || (*roi)(j) )
	    {
	      labels(j)= 255;
	      if ( tmp(j) > fParameter )
	      {
		nums(j)++;
		filterResponse(j)+= tmp(j);
	      }
	    }
	}
      }
      
      for ( int i= 0; i < nums.n; ++i )
	if ( nums(i) > 0 )
	  filterResponse(i)/= nums(i);
      
      Image<float> tmpImg;
      tmpImg.resizeImage(filterResponse);
      tmpImg= filterResponse;
      tmpImg.normalize(0, 255);
      writeImage("filterResponse.bmp", tmpImg);
	
      return 0;
    }

    template<typename INPUT>
    void AdditionOfThinObjectsMTM<INPUT>::doRG(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> &seed, Image<unsigned char> *roi, Image<unsigned char>* support)
    {
	tprintf("addition of thin objects mtm -- mp %f, ap %f, fp %f, cp %f, sizeth0 %d, sizeth1 %d\n", mParameter, aParameter, fParameter, cParameter, sizeThreshold0, sizeThreshold1);
	filterResponse.resizeImage(seed);
	filterResponse= -1;
	
	optimize(input, output, roi, support, 0);
	
        // filtering the regions by the mean and standard deviation of intensities covered by it
	Image<unsigned char> tmpb, tmpc, tmpd;
	tmpb.resizeImage(filterResponse);
	tmpc.resizeImage(filterResponse);
	tmpd.resizeImage(filterResponse);
	tmpb= 0;
	tmpc= 0;
	tmpd= 0;

	float meanThreshold= aParameter;
	float stdevThreshold= mParameter;
	
	printf("size thresholds: %d %d\n", sizeThreshold0, sizeThreshold1);
	
	output= seed;

	Vector<Region2> regionsInserted;
	tprintf("adaptive thresholding\n");
	//for ( int l= 0; l < frs.size(); ++l )
	Image<int> numbers;
	numbers.resizeImage(filterResponse);
	numbers= 0;
	  
	for ( float threshold= 0.0; threshold <= 1; threshold+= 0.01 )
	{
	  for ( int i= 0; i < filterResponse.n; ++i )
	  {
	    if ( filterResponse(i) > threshold )
	    //if ( frs(l)(i) > threshold )
	      tmpb(i)= 255;
	    else
	      tmpb(i)= 0;
	  }
	    
	  Region2Set regions, regions2;
	  ExtractRegions er;
	  
	  er.apply(tmpb, regions);
	  
	  #pragma omp parallel for num_threads(4)
	  for ( int i= 0; i < regions.size(); ++i )
	  {
	    if ( regions(i).size() > 150000 )
	      continue;
	    if ( regions(i).size() <= 1)
	      continue;
	    
	    
	    Region2 tmpRegion;
	    tmpRegion.stride= tmpb.columns;

	    tmpRegion= regions(i);
	    
	    float regionSize= tmpRegion.size();
	    
	    if ( regionSize < sizeThreshold0 || regionSize > sizeThreshold1 )
	      continue;
	      
	    float rmean= tmpRegion.getMean(filterResponse);
	    float rmin= tmpRegion.getMin(filterResponse);
	    float rstdev= tmpRegion.getStandardDeviation(filterResponse);

	    
	    if ( ((rmean >= meanThreshold && rstdev <= stdevThreshold))  )
	    {
	      #pragma omp critical
	      {
	  	tmpRegion.set(output, 0, (unsigned char)255);
	      }
	    }
	  }
	}
	
	
	tprintf("regions inserted: %d, %d, %d\n", regionsInserted.size(), tmpd.getMin(), tmpd.getMax());
    }
}

#endif
