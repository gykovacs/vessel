#include <openipLL/imageIO.h>
#include <openipDS/Image.h>
#include <openipDS/ParameterFile.h>
#include <openipDS/OptionTable.h>
#include <openipDS/writing.h>
#include <openipLL/imageCorrection.h>
#include <openipSC/RandomNumberGenerator.h>
#include <openipDS/FileSystem.h>
#include <openipLL/ROIDetector.h>
#include <openipDS/DoubleRingFeature2s.h>
#include <openipLL/morphology.h>
#include <openipLL/HMRFSegmentation.h>
#include <openipDS/VisualWord.h>
#include <openipML/DecisionTreeClassifier.h>
#include <openipDS/ContinuousFeatureVector.h>
#include <openipML/KFoldCrossValidator.h>
#include <openipML/statisticalFunctions.h>
#include <openipML/StatisticalTests.h>
#include <openipML/Noises.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/Stopper.h>
#include <openipML/PrincipalComponentAnalysis.h>
#include <openipLL/LowLevelTransforms.h>
#include <openipLL/Transform2Chain.h>
#include <openipLL/Transform2Set.h>
#include <openipLL/Transform2Generator.h>
#include <openipLL/Scaling.h>
#include <openipLL/colorSpaces.h>
#include <openipDS/PowerGaborFilter2.h>
#include <openipLL/ConnectingLineOperator.h>
#include <openipLL/BinaryImageOperations.h>
#include <openipLL/Scaling.h>
#include <openipLL/convexHull.h>
#include <openipDS/Filter2s.h>

#include <RetinaRegionGrowing.h>
#include <RetinaApplicationFunctions.h>
#include <VesselApplicationFunctions.h>

#include <algorithm>

using namespace openip;

int erode;
int dilate;
int dilateImage;

int unknown= 0;
bool hscaling= false;
float vstage2dynth= 12;
float vstage1limit= 1;
float vstage2shift= 0;
float vstage2nweight= 2.5;
int vstage1mode0= 1;
int vstage1mode1= 6;
float vstage1th1multiplier= 1.0;
float vstage1th2multiplier= 1.0;
float vstage4wa= 5.5;
float vstage4wb= 7.5;
float vstage1roiradius= -1;
bool roc= 0;

int roiRadiusFunction(int /*argc*/, char** argv)
{
  Image<unsigned char> roi;
  readImage(argv[1], roi);
  
  printf("%f\n", roiRadius(roi));
  
  return 0;
}

/**
 * roi extraction
 * @param 
 * @param argv output file name
 * @return 
 */
int extractROI(int, char** argv)
{
  Image<unsigned char> input;

  Border2 b(35, 35, 35, 35, BORDER_MODE_ZERO);

  readImage(argv[1], input, b, READ_CHANNEL_RED);

  StructuringElementDisk sed(200, input.columns);
  sed.updateStride(input.columns);

  int i= input.rows/2;
  int j= input.columns/2;

  sed.translate(i * input.columns + j);

  float mean;
  mean= sed.getMean(input);

  Image<unsigned char> roi;
  roi.resizeImage(input);
  roi= 0;

  Image<unsigned char> roi2;
  roi2.resizeImage(input);

  float threshold= 0.2*mean;
  //threshold= 20;

  for ( unsigned int i= 0; i < input.n; ++i )
      if ( input(i) > threshold )
	  roi(i)= 255;
      else
	  roi(i)= 0;

  StructuringElementDisk sed2(5, input.columns);
  binaryDilate(&roi, &roi2, sed2);
  binaryErode(&roi2, &roi, sed2);
  binaryErode(&roi, &roi2, sed2);

  writeImage(argv[2], roi2);

  return 0;
}

int vstage0FunctionROI(Image<unsigned char>& input, Image<unsigned char>& roi)
{
  tprintf("extract ROI\n");
  
  //Border2 oldborder= input.getBorder2();
  Border2 b(35, 35, 35, 35, BORDER_MODE_ZERO);
  input.resizeBorder(b);
  roi.resizeImage(input);
  roi= 0;

  Image<float> tmp;
  tmp.resizeImage(input);
  tmp= 0;
  Image<float> tmp3;
  tmp3.resizeImage(input);
  tmp3= 0;
  
  StructuringElementDisk sed0(250);
  sed0.updateStride(input.columns);
  
  int i= input.rows/2;
  int j= input.columns/2;

  sed0.translate(i * input.columns + j);

  float mean= sed0.getMean(input);
  float threshold= mean;
  
  tprintf("threshold: %f\n", mean);
  
  for ( unsigned int i= 0; i < input.n; ++i )
    if ( input(i) > threshold )
      tmp(i)= 255;
    else
      tmp(i)= 0;
  
  StructuringElementDisk sed2(13);
  sed2.updateStride(input.columns);
  
  int changed= 1;
  
  while ( changed )
  {
    changed= 0;
    tmp3= tmp;
    for ( unsigned int i= 0; i < input.n; ++i )
    {
      if ( !tmp(i) && input.isRealImagePixel(i) )
      {
	float thlocal= 0;
	int n= 0;

	for ( unsigned int j= 0; j < sed2.size(); ++j )
	  if ( tmp(i + sed2(j)) )
	  {
	    thlocal+= input(i + sed2(j));
	    ++n;
	  }
	  
	//if ( n > 0.2*sed2.size() );
	{
	  thlocal/= n;
	  thlocal*= 0.75;
	  if ( input(i) > thlocal )
	  {
	    tmp3(i)= 255;
	    ++changed;
	  }
	}
      }
    }
    tmp= tmp3;
  } 

  roi= tmp;
  Image<unsigned char> tmp2;
  tmp2.resizeImage(roi);
  
  for ( unsigned int i= 0; i < roi.n; ++i )
    if ( roi(i) )
      tmp2(i)= 0;
    else
      tmp2(i)= 255;
  
  ExtractRegions er;
  Region2Set holes;
  er.apply(tmp2, holes);
  
  for ( unsigned int i= 0; i < holes.size(); ++i )
    if ( holes(i).size() < 1000 )
      for ( unsigned int j= 0; j < holes(i).size(); ++j )
	roi(holes(i)(j))= 255;
  tmp2= 0;
  
  float middler= 0, middlec= 0;
  int n= 0;
  for ( int i= 0; i < roi.rows; ++i )
      for ( int j= 0; j < roi.columns; ++j )
        if ( roi(i,j) )
        {
            middler+= i;
            middlec+= j;
            n++;
        }
    middler/= n;
    middlec/= n;
  middler= floor(middler);
  middlec= floor(middlec);
  tprintf("middle: %f %f\n", middler, middlec);
  
  for ( int i= 0; i < roi.rows; ++i )
      for ( int j= 0; j < roi.columns; ++j )
      {
          float dr= (middler - i)/4000.0;
          float dc= (middlec - j)/4000.0;
          if ( roi(i,j) )
              if ( i == 0 || i == roi.rows - 1 || j == 0 || j == roi.columns - 1 || (i-1 >= 0 && !roi(i-1,j)) || (i+1 < roi.rows && !roi(i+1,j)) || (j-1 >= 0 && !roi(i,j-1)) || (j+1 < roi.columns && !roi(i,j+1)) )
              {
                  for ( int k= 0; k < 4000; ++k )
                      roi(i + dr*k, j + dc*k)= 255;
              }
                  
      }
          
  
  //input.resizeBorder(oldborder);
  //roi.resizeBorder(oldborder);
  input.removeBorder();
  roi.removeBorder();
  
  return 0;
}

int vstage0FunctionExtend(Image<unsigned char>& input, Image<unsigned char>& roi, Image<unsigned char>& extended)
{
  tprintf("extend image\n");
  
  extended.resizeImage(input);
  Image<unsigned char> support;
  support.resizeImage(input);
  support= 255;
  
  tprintf("input.size: %d %d\n", input.rows, input.columns);
  tprintf("roi.size: %d %d\n", roi.rows, roi.columns);
  tprintf("extended.size: %d %d\n", extended.rows, extended.columns);
  
  FillSupport<unsigned char, unsigned char> fs;
  fs.apply(input, extended, &roi, &support);
  
  return 0;
}

int vstage0Function(int , char** argv)
{
  Image<unsigned char> red, green, roi, extended;
  readImage(argv[1], red, READ_CHANNEL_RED);
  readImage(argv[1], green, READ_CHANNEL_GREEN);
  
  vstage0FunctionROI(red, roi);
  
  writeImage(argv[5], roi);
  
  vstage0FunctionExtend(green, roi, extended);
  
  writeImage(argv[2], roi);
  
  writeImage(argv[3], extended);
  roi= 255;
  writeImage(argv[4], roi);
  
  return 0;
}

Vector<Image<float> > vstage1cache;

void updateThresholds(float& th1, float& th2, float minTh2, float th1mult, float th2mult)
{
  float tmpth1, tmpth2;
  
  if ( th1mult > 1 || th2mult > 1 )
  {
    if ( vstage1mode0 == 0 )
    {
      tmpth1= th1*th1mult;
      tmpth2= th2*th2mult;
    }
    else if ( vstage1mode0 == 1 )
    {
      tmpth1= 1 - (1 - th1)/th1mult;
      tmpth2= 1 - (1 - th2)/th2mult;
    }
    else if ( vstage1mode0 == 2 )
    {
      tmpth1= th1*th1mult;
      tmpth2= th2*th2mult;
    }
    else if ( vstage1mode0 == 3 )
    {
      tmpth1= 1 - (1 - th1)/th1mult;
      tmpth2= 1 - (1 - th2)/th2mult;
    }
    else
    {
      tmpth1= th1;
      tmpth2= th2;
    }
      
    th1= tmpth1;
    th2= tmpth2;
  }
  else
  {
    if ( vstage1mode0 == 0 )
    {
      tmpth1= th1*th1mult;
      tmpth2= th2*th2mult;
    }
    else if ( vstage1mode0 == 1 )
    {
      tmpth1= th1*th1mult;
      tmpth2= th2*th2mult;
    }
    else if ( vstage1mode0 == 2 )
    {
      tmpth1= 1 - (1 - th1)/th1mult;
      tmpth2= 1 - (1 - th2)/th2mult;
    }
    else if ( vstage1mode0 == 3 )
    {
      tmpth1= 1 - (1 - th1)/th1mult;
      tmpth2= 1 - (1 - th2)/th2mult;
    }
    else
    {
      tmpth1= th1;
      tmpth2= th2;
    }
    
    if ( !roc )
    {
      if ( tmpth2 < minTh2 )
      {
	if ( vstage1mode0 == 0 || vstage1mode0 == 1 )
	{
	  tmpth1= tmpth1 * minTh2/tmpth2;
	  tmpth2= minTh2;
	}
	if ( vstage1mode0 == 2 || vstage1mode0 == 3 )
	{
	  tmpth1= 1 - (1 - th1)/((th2 - 1)/(minTh2 - 1));
	  tmpth2= minTh2;
	}
      }
    }
    
    th1= tmpth1;
    th2= tmpth2;	
  }
}

Vector<float> maxPCC;
int maxpccinit= 0;

int checkOperator(PowerGaborRGLineSegmentTransform2<float,float>* op, int j)
{
  if ( !maxpccinit )
  {
    Image<float> image;
    image.resizeImage(1600,1600);
    image= 0;
    
    GaussianRandomFloatGenerator grfg(0, 1);
    for ( unsigned int i= 0; i < image.n; ++i )
      image(i)= grfg.generate();
    
    Image<float> output;
    Image<float> output2;
    output.resizeImage(image);
    output2.resizeImage(image);
    output= 0;
    output2= 0;
    
    op->applyGetFiltered(image, output, output2);
    
    Image<unsigned char> tmp;
    tmp.resizeImage(output);
    tmp= 0;
    
    float n= 0;
    float maxScore= 0;
    for ( unsigned int i= 0; i < output2.n; ++i )
    {
      if ( output2(i) > (op->th1 + op->th2)/2 )
	n+= 1;
      if ( maxScore < output2(i) )
	maxScore= output2(i);
    } 
    maxPCC(j)= maxScore;
    
    tprintf("%.15f\n", n/output2.n);
    if ( n > 0 )
      return 0;
    
    return 1;
  }
  else
  {
    if ( (op->th1 + op->th2)/2 < maxPCC(j) )
      return 0;
    else
      return 1;
  }
}

Vector<int> maskcache;
int maskinit= 0;

int checkOperator(PowerGaborSimpleRGLineSegmentTransform2<float,float>* op, int j)
{
  if ( !maxpccinit )
  {
    Image<float> image;
    image.resizeImage(1600,1600);
    image= 0;
    
    GaussianRandomFloatGenerator grfg(0, 1);
    for ( unsigned int i= 0; i < image.n; ++i )
      image(i)= grfg.generate();
    
    Image<float> output;
    Image<float> output2;
    output.resizeImage(image);
    output2.resizeImage(image);
    output= 0;
    output2= 0;
    
    op->applyGetFiltered(image, output, output2);
    
    Image<unsigned char> tmp;
    tmp.resizeImage(output);
    tmp= 0;
    
    float n= 0;
    float maxScore= 0;
    for ( unsigned int i= 0; i < output2.n; ++i )
    {
      if ( output2(i) > (op->th1 + op->th2)/2 )
	n+= 1;
      if ( maxScore < output2(i) )
	maxScore= output2(i);
    }
    tprintf("check operator %d against Gaussian noise: %f\n", j, maxScore);
    maxPCC(j)= maxScore;
    
    tprintf("%.15f\n", n/output2.n);
    if ( n > 0 )
      return 0;
    
    return 1;
  }
  else
  {
    if ( (op->th2 + op->th1)/2 < maxPCC(j) )
      return 0;
    else
      return 1;
  }
}


Vector<Image<float> > pyramid;
Vector<Image<unsigned char> > roipyramid;
Vector<Image<unsigned char> > supportpyramid;
Vector<float> scales;
Vector<float> factors;
int pyramidinit= 0;
Vector<Image<float> > wsocache;
int wsocacheinit= 0;

void applyFilter(int k, Image<float>& input, Image<unsigned char>& roi, Image<unsigned char>& support, Image<float>& res1, Image<float>& res2, Border2 b, PowerGaborRGLineSegmentTransform2<float,float>* op1= NULL, PowerGaborSimpleRGLineSegmentTransform2<float,float>* op2= NULL, int caching= 0, int usepyramid= 0)
{
  #pragma omp critical
  {
    //printf("in critical %d\n", omp_get_thread_num()); fflush(stdout);
    if ( pyramidinit == 0 )
    {
      tprintf("initializing pyramid %d\n", omp_get_thread_num()); fflush(stdout);
      scales.clear();
      factors.clear();
      pyramid.clear();
      roipyramid.clear();
      supportpyramid.clear();
      Border2 b= input.getBorder2();
      
      float scale= 1;
      if ( op1 )
	scale= op1->scale;
      else if ( op2 )
	scale= op2->scale;
      
      scales.push_back(scale);
      factors.push_back(1);
      pyramid.push_back(input);
      roipyramid.push_back(roi);
      supportpyramid.push_back(support);
      
      pyramid(0).removeBorder();
      roipyramid(0).removeBorder();
      supportpyramid(0).removeBorder();
      
      if ( usepyramid )
      {
        tprintf("usepyramid %d\n", omp_get_thread_num()); fflush(stdout);
	float sc= scale;
	float fact= 1;
	int columns= pyramid(0).columns;
	int rows= pyramid(0).rows;
	for ( int i= 0; i < 20; ++i )
	{
          tprintf("scale: %d\n", i); fflush(stdout);
	  sc*= 0.9;
	  fact*= 0.9;
	  Image<float> inp;
	  Image<unsigned char> ro, su;
	  inp.resizeImage(rows*fact, columns*fact);
	  ro.resizeImage(rows*fact, columns*fact);
	  su.resizeImage(rows*fact, columns*fact);
	  
          //printf("before gaussian scaling\n"); fflush(stdout);
	  gaussianScaling(pyramid(0), inp);
	  gaussianScaling(roipyramid(0), ro);
	  gaussianScaling(supportpyramid(0), su);
          //printf("after gaussian scaling\n"); fflush(stdout);
	  
	  for ( unsigned int i= 0; i < ro.n; ++i )
	  {
	    if ( ro(i) > 128 )
	      ro(i)= 255;
	    else
	      ro(i)= 0;
	    if ( su(i) > 128 )
	      su(i)= 255;
	    else
	      su(i)= 0;
	  }
	  
	  char filename[100];
	  sprintf(filename, "roi-%f.bmp", fact);
	  writeImage(filename, ro);
	  
	  pyramid.push_back(inp);
	  roipyramid.push_back(ro);
	  supportpyramid.push_back(su);
	  scales.push_back(sc);
	  factors.push_back(fact);
	  
	  tprintf("pyramid: %f %d %d, %d %d %d %d %d\n", sc, inp.columns, pyramid(pyramid.size()-1).columns, pyramid.size(), roipyramid.size(), supportpyramid.size(), scales.size(), factors.size());
	}
      }
      for ( unsigned int i= 0; i < pyramid.size(); ++i )
      {
	pyramid(i).setBorder(b);
	roipyramid(i).setBorder(b);
	supportpyramid(i).setBorder(b);
      }
      
      pyramidinit= 1;
    }
  }

  float size= 0;
  if ( op1 )
    size= op1->mgf->operator[](0)->size();
  else if ( op2 )
    size= op2->mgf->operator[](0)->size();
  
  int scaleIdx= 0;
  float minDist= FLT_MAX;
  for ( unsigned int i= 0; i < scales.size(); ++i )
  {
    float dist= fabs(size*factors(i)*factors(i) - 240);
    if ( dist < minDist && size*factors(i)*factors(i) > 240 )
    {
      scaleIdx= i;
      minDist= dist;
    }
  }
  
  if ( op1 )
  {
    op1->scale= scales(scaleIdx);
    op1->regenerate();
  }
  else if ( op2 )
  {
    op2->scale= scales(scaleIdx);
    op2->regenerate();
  }
  
  Image<float> result1;
  result1.resizeImage(pyramid(scaleIdx));
  Image<float> result2;
  result2.resizeImage(pyramid(scaleIdx));
  
  File file(input.filename);
  string filename= file.getFilename();
  string featureName;
  if ( op1 )
    featureName= op1->getDescriptor('_');
  else if ( op2 )
    featureName= op2->getDescriptor('_');

  char filename3[1000];
  sprintf(filename3, "cache/%s-%s.float", filename.c_str(), featureName.c_str());
  FILE* fil2= fopen(filename3, "r");
  int exists= (fil2 != NULL);
  
  if ( exists )
    fclose(fil2);
      
  if ( exists && caching )
  {
    printf("."); fflush(stdout);

    if ( !wsocacheinit || (wsocacheinit && wsocache(k).n <= 1) )
    {
      readImage(filename3, result2, b);
      wsocache(k)= result2;
    }
    else if ( wsocacheinit )
      result2= wsocache(k);
    
    bilinearScaling(result2, res2);
    
    float th1= 0, th2= 0;
    if ( op1 )
    {
      th1= op1->th1;
      th2= op1->th2;
    }
    else if ( op2 )
    {
      th1= op2->th1;
      th2= op2->th2;
    }
    
    PowerGaborSimpleRGLineSegmentTransform2<float, float> rg(th1, th2, 2, 2, 2, 2, 2, 2, 2, 2);
    result1= 0;
    rg.applyOnlyRG(res2, res1, &(roipyramid(0)), &(supportpyramid(0)));
  }
  else if ( !exists || !caching )
  {
    if ( op1 )
      op1->applyGetFiltered(pyramid(scaleIdx), result1, result2, &(roipyramid(scaleIdx)), &(supportpyramid(scaleIdx)));
    else if ( op2 )
      op2->applyGetFiltered(pyramid(scaleIdx), result1, result2, &(roipyramid(scaleIdx)), &(supportpyramid(scaleIdx)));
    
    bilinearScaling(result2, res2);
    
    float th1= 0, th2= 0;
    if ( op1 )
    {
      th1= op1->th1;
      th2= op1->th2;
    }
    else if ( op2 )
    {
      th1= op2->th1;
      th2= op2->th2;
    }
    
    PowerGaborSimpleRGLineSegmentTransform2<float, float> rg(th1, th2, 2, 2, 2, 2, 2, 2, 2, 2);
    result1= 0;
    rg.applyOnlyRG(res2, res1, &(roipyramid(0)), &(supportpyramid(0)));
    
    if ( wsocacheinit && wsocache(k).n <= 1 )
      wsocache(k)= result2;
  }
  
  if ( !exists && caching )
  {
    writeImage(filename3, result2);
  }
}

// applies a set of gabor filters -- region growing operators and fuses the results
int vstage1Function(char* featureFile, Image<float>& input, Image<unsigned char>& roi, Image<unsigned char>& support, Image<unsigned char>& output, int thresholdStart, float th1mult, float th2mult, float imageScale, int caching= 1, int lambdaThreshold= -1, Image<unsigned char>* imc= NULL, float* dist= NULL, Vector<int>* mask= NULL, int usepyramid= 0)
{
  tprintf("vessel segmentation stage1 simple, features: %s, thresholdStart: %d, scale: %f, caching: %d, thmult1: %f, thmult2: %f, lambdaThreshold: %d, etalon: %f mode0: %d, mode1: %d caching: %d, usepyramid: %d, imc: %p\n", featureFile, thresholdStart, imageScale, caching, th1mult, th2mult, lambdaThreshold, vstage1limit, vstage1mode0, vstage1mode1, caching, usepyramid, imc);

  Transform2Set<float, float>& t2set= *(generateTransform2Set<float, float>(std::string(featureFile), std::string("feature")));
  
  tprintf("t2set.size(): %d\n", t2set.size());
  
  Border2 b1(71, 71, 71, 71);
  Border2 b2= t2set.getProposedBorder();
  Border2 b(b1, b2);
  b.borderMode= BORDER_MODE_ZERO;
  tprintf("borders computed %d %d %d %d\n", b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder);
  
  Vector<int> mask2;
  mask2.resize(t2set.size());
  wsocacheinit= 1;
  wsocache.resize(t2set.size());
  mask2= 1;
  if ( maskinit == 0 )
    maskcache.resize(t2set.size());
  
  float minTh2= FLT_MAX;
  
  char fname[100];
  sprintf(fname, "maxPCC-%f.txt", imageScale);
  FILE* fmc= NULL;
  fmc= fopen(fname, "r");
  if ( fmc != NULL )
  {
    maxPCC.resize(t2set.size());
    maxpccinit= 1;
    float flag;
    for ( unsigned int i= 0; i < maxPCC.size(); ++i )
    {
      int rv= fscanf(fmc, "%f", &flag);
      if ( rv != 1 )
	break;
      maxPCC(i)= flag;
    }
    fclose(fmc);
  }
  else
    maxPCC.resize(t2set.size());
  
  for ( unsigned int i= 0; i < t2set.size(); ++i )
  {
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->th2 < minTh2 )
	minTh2= tmp->th2;
    }
    if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->th2 < minTh2 )
	minTh2= tmp->th2;
    }
  }
  
  int discarded= 0;
  
  #pragma omp parallel for
  for ( unsigned int i= 0; i < t2set.size(); ++i )
  {
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      tmp->scale= 1.0/imageScale;
      tmp->regenerate();
      
      if ( unknown )
      {
	mask2(i)= 1;
	if ( !roc )
	{
	  if ( tmp->mgf->operator[](0)->size() < 100 )
	    mask2(i)= checkOperator(tmp, i);
	}
      }
      
      updateThresholds(tmp->th1, tmp->th2, minTh2, th1mult, th2mult);
      tmp->regenerate2();
      
      if ( mask2(i) == 0 )
	++discarded;
    }
    if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      tmp->scale= 1.0/imageScale;
      tmp->regenerate();
      
      if ( unknown )
      {
	mask2(i)= 1;
	if ( !roc )
	{
	  if ( tmp->mgf->operator[](0)->size() < 100 )
	    mask2(i)= checkOperator(tmp, i);
	}
      }
      
      updateThresholds(tmp->th1, tmp->th2, minTh2, th1mult, th2mult);
      tmp->regenerate2();
      
      if ( mask2(i) == 0 )
	++discarded;
    }
  }
  
  tprintf("features generated\n");
  maxpccinit= 1;
  
  fmc= fopen(fname, "r");
  if ( fmc == NULL )
  {
    fmc= fopen(fname, "w");
    for ( unsigned int i= 0; i < maxPCC.size(); ++i )
      fprintf(fmc, "%f\n", maxPCC(i));
    fclose(fmc);
  }
  else
  {
    fclose(fmc);
  }
  
  tprintf("size of t2set after scaling: %d\n", mask2.numberOfNonZeroElements());

  tprintf("reading input images\n");
  Image<float> result;
  Image<unsigned char> img;
  Image<int> outputTmp;
  Image<unsigned char> roitmp;

  Border2 originalBorder= input.getBorder2();
  
  b.borderMode= BORDER_MODE_MIRRORED;
  input.resizeBorder(b);
  b.borderMode= BORDER_MODE_ZERO;
  roi.resizeBorder(b);
  support.resizeBorder(b);
  output.resizeImage(input);
  result.resizeImage(input);
  outputTmp.resizeImage(input);

  output= 0;
  outputTmp= 0;
  tprintf("applying the transforms\n");

  Image<unsigned char> reducedROI;
  reducedROI= roi;
  tprintf("roi size: %d\n", roi.numberOfNonZeroElements());
  
  tprintf("starting applying the transforms\n");
  int completed= 0;
  
  #pragma omp parallel for schedule(dynamic, 2)
  for ( unsigned int j= 0; j < t2set.size(); ++j )
  {
    if ( (mask && (*mask)(j) == 0) || !mask2(j) )
    {
      completed++;
      continue;
    }
    
    int threadnum= omp_get_thread_num();
    if ( threadnum == 0 )
    {
      printf("tcompleted/total: %d/%zd\n", completed, t2set.size());
      fflush(stdout);
    }
    
    Image<float> result(input), result2(input);
    Image<unsigned char> rtmp;
    rtmp.resizeImage(input);
    
    //tprintf("thread %d applying %d\n", omp_get_thread_num(), j); fflush(stdout);
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>*>(t2set(j)) != NULL )
      applyFilter(j, input, reducedROI, support, result, result2, b, dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(j)), NULL, caching, usepyramid);
    else if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(j)) != NULL )
      applyFilter(j, input, reducedROI, support, result, result2, b, NULL, dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(j)), caching, usepyramid);
    //tprintf("thread %d applied %d\n", omp_get_thread_num(), j); fflush(stdout);
    ExtractRegions er;
    Region2Set regions;
    
    rtmp= result;

    unsigned int nonzeroelements= rtmp.numberOfNonZeroElements();
    if ( nonzeroelements == rtmp.n || nonzeroelements == 0 )
      continue;
    
    er.apply(rtmp, regions);
    
    Vector<float> circularities;
    for ( unsigned int i= 0; i < regions.size(); ++i )
      circularities.push_back(circularity(regions(i), rtmp));
      
    #pragma omp critical
    {
      for ( unsigned int i= 0; i < regions.size(); ++i )
	if ( circularities(i) < 0.3 )
	  for ( unsigned int k= 0; k < regions(i).size(); ++k )
	    if ( (roi(regions(i)(k))) )
	      outputTmp(regions(i)(k))++;
      completed++;
    }
  }
  tprintf("starting adaptive thresholding, %d, %d, %d; %d, %d, %d\n", roi.columns, output.columns, input.columns, roi.leftBorder, output.leftBorder, input.leftBorder);
  
  for ( unsigned int i= 0; i < output.n; ++i )
    output(i)= int(outputTmp(i)) > 0 ? 255 : 0;
  
  //writeImage("output0.bmp", output);
  
  Vector<float> rii, rio;
  double riw= 0, row= 0;
  for ( unsigned int i= 0; i < output.n; ++i )
  {
      if ( roi.isRealImagePixel(i) && output(i) && Region2::isInnerContour4(output, i) )
      {
	float ri= calculateRI(output, input, i);
	float width= calculateWidth(output, input, i);
	rii.push_back(ri);
	riw+= width;
      }
      
      if ( roi.isRealImagePixel(i) && !output(i) && Region2::isOuterContour4(output, i) )
      {
	float ri= calculateRI(output, input, i);
	float width= calculateWidth(output, input, i);
	rio.push_back(ri);
	row+= width;
      }
  }
  
  tprintf("ri: %f %f %f %f\n", rii.getMean(), rio.getMean(), (rii.getMean() + rio.getMean())/2, fabs(rii.getMean() - rio.getMean()));
  
  if ( dist != NULL )
    *dist= 1 - fabs(rii.getMean() - rio.getMean());
    //*dist= fabs(0.5 - fabs(rii.getMean() + rio.getMean())/2);
  
  input.resizeBorder(originalBorder);
  support.resizeBorder(originalBorder);
  roi.resizeBorder(originalBorder);
  output.resizeBorder(originalBorder);
  
  return 0;
}

Vector<int> opmask;
int opmaskinit= 0;

int vstage1unknown2Function(char* featureFile, Image<float>& input, Image<unsigned char>& roi, Image<unsigned char>& support, Image<unsigned char>& output, int thresholdStart, float th1mult, float th2mult, float imageScale, int caching= 1, float* res= NULL, float* res2= NULL, float* res3= NULL, float* res4= NULL)
{
  tprintf("vessel segmentation stage1, features: %s, thresholdStart: %d, scale: %f, caching: %d, thmult1: %f, thmult2: %f\n", featureFile, thresholdStart, imageScale, caching, th1mult, th2mult);

  Transform2Set<float, float>& t2set= *(generateTransform2Set<float, float>(std::string(featureFile), std::string("feature")));
  
  tprintf("t2set.size(): %d\n", t2set.size());
  
  Vector<int> mask;
  mask.resize(t2set.size());
  mask= 1;

  if ( opmaskinit == 0 )
  {
    opmask.resize(t2set.size());
    opmask= 1;
    opmaskinit= 1;
  }
  
  Vector<int> maskcached;
  
  char fname[100];
  sprintf(fname, "mask-%f.txt", imageScale);
  
  FILE* fmc= NULL;
  fmc= fopen(fname, "r");
  if ( fmc != NULL )
  {
    maskcached.resize(t2set.size());
    maskcached= 1;
    int flag;
    for ( unsigned int i= 0; i < maskcached.size(); ++i )
    {
      int rv= fscanf(fmc, "%d", &flag);
      if ( rv != 1 )
	break;
      maskcached(i)= flag;
    }
    fclose(fmc);
  }
  
  float realImageScale= 1.0/imageScale;
  tprintf("real image scale: %f\n", realImageScale);
  int discarded= 0;
  
  float maxwavelength= 0;

  maxpccinit= 0;
  maxPCC.resize(t2set.size());
  for ( unsigned int i= 0; i < t2set.size(); ++i )
  {
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i));
      if ( tmp->lambda > maxwavelength )
	maxwavelength= tmp->lambda;
    }
    if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->lambda > maxwavelength )
	maxwavelength= tmp->lambda;
    }
  }
  
//#pragma omp parallel for
  for ( unsigned int i= 0; i < t2set.size(); ++i )
  {
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      tmp->scale= 1;
      tmp->regenerate();
      
      if ( tmp->lambda > maxwavelength*0.8 )
      {
	mask(i)= 1;
      }
      else
	mask(i)= 0;
      
      tmp->scale= 1.0/imageScale;
      tmp->regenerate();
      
      if ( mask(i) == 1 && tmp->mgf->operator[](0)->size() < 100 )
      {
	if ( maskcached.size() > 0 )
	  mask(i)= maskcached(i);
	else
	{
	  if ( !checkOperator(tmp, i) )
	  {
	    mask(i)= 0;
	    ++discarded;
	  }
	}
      }
    }
    if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      tmp->scale= 1;
      tmp->regenerate();
      
      if ( tmp->lambda > maxwavelength*0.8 )
      {
	mask(i)= 1;
      }
      else
	mask(i)= 0;
      
      tmp->scale= 1.0/imageScale;
      tmp->regenerate();
      
      if ( mask(i) == 1 && tmp->mgf->operator[](0)->size() < 100 )
      {
	if ( maskcached.size() > 0 )
	  mask(i)= maskcached(i);
	else
	{
	  if ( !checkOperator(tmp, i) )
	  {
	    mask(i)= 0;
	    ++discarded;
	  }
	}
      }
    }
  }
  tprintf("features generated\n");
  
  fmc= fopen(fname, "w");
  if ( fmc != NULL )
  {
    for ( unsigned int i= 0; i < mask.size(); ++i )
      fprintf(fmc, "%d\n", mask(i));
    fclose(fmc);
  }
  
  tprintf("size of t2set after scaling: %d %d\n", mask.numberOfNonZeroElements(), opmask.numberOfNonZeroElements());
  tprintf("discarded: %d\n", discarded);
  
  Border2 b1(75, 75, 75, 75);
  Border2 b2= t2set.getProposedBorder();
  Border2 b(b1, b2);
  b.borderMode= BORDER_MODE_ZERO;
  tprintf("borders computed %d %d %d %d\n", b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder);

  tprintf("reading input images\n");
  Image<float> result;
  Image<unsigned char> img;


  b.borderMode= BORDER_MODE_MIRRORED;
  input.resizeBorder(b);
  b.borderMode= BORDER_MODE_ZERO;
  roi.resizeBorder(b);
  support.resizeBorder(b);
  output.resizeImage(input);
  result.resizeImage(input);

  output= 0;

  
  tprintf("applying the transforms\n");

  Image<unsigned char> reducedROI;
  reducedROI.resizeImage(input);
  reducedROI= roi;

  float radius= roiRadius(roi);
    
  tprintf("roi radius: %f\n", radius);

  tprintf("starting applying the transforms\n");
  int completed= 0;
  
  tprintf("starting applying the transforms, %d\n", input.rows);
  
  //float detected= 0;
  Vector<float> opwidths;
  int lwdetected= 0;
  
  #pragma omp parallel for schedule(dynamic, 2)
  for ( unsigned int j= 0; j < t2set.size(); ++j )
  {
    if ( !mask(j) )
      continue;
    
    int threadnum= omp_get_thread_num();
    if ( threadnum == 0 )
      printf("\rcompleted/total: %d/%zd", completed, t2set.size());
    
    Image<float> result, result2;
    Image<unsigned char> rtmp;
    Image<unsigned char> rctmp;
    result.resizeImage(input);
    rtmp.resizeImage(input);
    rctmp.resizeImage(input);
    result2.resizeImage(input);
    
    //float lambda;
    string featureName= t2set(j)->getDescriptor('_');
    
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(j)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(j));
      
      //lambda= tmp->lambda;
      
      tmp->applyGetFiltered(input, result, result2, &roi, &support);
    }
    else if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(j)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(j));
      
      //lambda= tmp->lambda;
      
      tmp->applyGetFiltered(input, result, result2, &roi, &support);
    }
    
    ExtractRegions er;
    Region2Set regions;
    
    if ( (unsigned int)(rtmp.numberOfNonZeroElements()) == rtmp.n )
      continue;
    
    rtmp= result;
    er.apply(rtmp, regions);
    
    Vector<float> circularities;
    for ( unsigned int i= 0; i < regions.size(); ++i )
      circularities.push_back(circularity(regions(i), rtmp));
    
    rtmp= 0;
    
    #pragma omp critical
    {
      for ( unsigned int i= 0; i < regions.size(); ++i )
      {
	if ( circularities(i) < 0.3 )
	  {
	    rctmp= 0;
	    for ( unsigned int k= 0; k < regions(i).size(); ++k )
	    {
	      if ( roi(regions(i)(k)) )
	      {
		output(regions(i)(k))++;
		rtmp(regions(i)(k))= 255;
	      }
	    }
	  }

      }
      
      {
	Vector<float> wtmp;
	for ( unsigned int i= 0; i < rtmp.n; ++i )
	  if ( rtmp(i) && Region2::isInnerContour8(rtmp, i) )
	    wtmp.push_back(calculateWidth(rtmp, input, i));
	if ( wtmp.size() > 0 )
	{
	  opwidths.push_back(wtmp.getMean());
	  lwdetected++;
	}
      }
      
      completed++;
      
    }
  }
  
  for ( unsigned int i= 0; i < output.n; ++i )
    output(i)= int(output(i)) > 0 ? 255 : 0;
  
  Vector<float> wis;
  Vector<float> wos;
  Vector<float> wall;
  for ( unsigned int i= 0; i < output.n; ++i )
  {
      if ( Region2::isInnerContour8(output, i) )
      {
	float wi= calculateWidth(output, input, i);
	wis.push_back(wi);
	wall.push_back(wi);
      }
      
      if ( Region2::isOuterContour8(output, i) )
      {
	float wo= calculateWidth(output, input, i);
	wos.push_back(wo);
	wall.push_back(wo);
      }
  }

  tprintf("inner widths: %f %f %f outer widths: %f %f %f all widths: %f %f %f opwidths: %f %f %f detected: %d\n", wis.getMean(), wis.getMedian(), wis.getStandardDeviation(), wos.getMean(), wos.getMedian(), wos.getStandardDeviation(), wall.getMean(), wall.getMedian(), wall.getStandardDeviation(), opwidths.getMean(), opwidths.getMedian(), opwidths.getStandardDeviation(), lwdetected);
  
  *res= wall.getMedian();
  *res2= lwdetected;
  *res3= opwidths.getMedian();
  *res4= wall.getMean();

  tprintf("thresholding finished\n");
  
  return 0;
}

int vstage1Function(int , char** argv, int thStart, float th1mult, float th2mult, float imageScale)
{
  tprintf("fdf: %s\ninput: %s\nroi: %s\nsupport: %s\noutput:%s, th1mult: %f, th2mult: %f\n", argv[1], argv[2], argv[3], argv[4], argv[5], th1mult, th2mult);
  
  Image<float> input, inputOriginal;
  Image<unsigned char> roi, support, output, original;
  int usepyramid= 0;
  Border2 b(75, 75, 75, 75, BORDER_MODE_ZERO);
  
  readImage(argv[2], input);
  readImage(argv[3], roi);
  readImage(argv[4], support);

  inputOriginal.resizeImage(input);
  inputOriginal= input;
  
  if ( input.columns > 1000 )
  {
    reduceROI(input, roi);
    usepyramid= 1;
  }
  
  original.resizeImage(input);
  Border2 originalBorder= input.getBorder2();
  
  float roiradius= roiRadius(roi);
  roiradius= int(roiradius)/20;
  roiradius*= 20;
  float roiratio= vstage1roiradius/roiradius;	// STARE/DRIVE ~ 1.15
  
  tprintf("ROI radius, ratio: %f %f %f\n", roiradius, roiratio, imageScale);
  
  if ( unknown == 0 )
  {
    vstage1Function(argv[1], input, roi, support, output, thStart, th1mult, th2mult, imageScale, 0, -1, NULL, NULL, NULL, usepyramid);
  }
  else if ( unknown == 1 )
  {
    tprintf("dimensions: %d %d\n", input.rows, input.columns);

    float start= 0.5;
    float end= 2.0;
    float step= 1.1;
    
    if ( roiratio > 0.5 && roiratio < 2.0 )
    {
      start*= roiratio;
      end*= roiratio;
    }
    else
    {
      Image<float> inputscaled(input.rows*roiratio, input.columns*roiratio);
      inputscaled.setBorder(input.getBorder2());
      Image<unsigned char> roiscaled;
      Image<unsigned char> supportscaled;
      roiscaled.resizeImage(inputscaled);
      supportscaled.resizeImage(inputscaled);
      output.resizeImage(inputscaled);
      
      if ( inputscaled.rows < input.rows )
      {
	gaussianScaling(input, inputscaled);
	gaussianScaling(roi, roiscaled);
	gaussianScaling(support, supportscaled);
	for ( unsigned int i= 0; i < roiscaled.n; ++i )
	{
	  roiscaled(i)= roiscaled(i) > 128 ? 255 : 0;
	  supportscaled(i)= supportscaled(i) > 128 ? 255 : 0;
	}
      }
      
      input= inputscaled;
      roi= roiscaled;
      support= supportscaled;
    }
    
    float maxCorr= 0;
    float bestScale= 0;
    
    float res, res2, res3, res4, is= start;
    while ( 1 )
    {
      vstage1unknown2Function(argv[1], input, roi, support, output, thStart, th1mult, th2mult, is, 0, &res, &res2, &res3, &res4);
      if ( output.numberOfNonZeroElements() > output.n*0.0001 )
	break;
      is*= 1.1;
    }
    start= is;
    
    Image<unsigned char> mask;
    mask.resizeImage(output);
    mask= 0;

    StructuringElementDisk sed(input.columns/50);
    sed.updateStride(mask.columns);
    grayscaleDilate(&output, &mask, sed);
    
    //writeImage("base.bmp", output);
    //writeImage("dilated.bmp", mask);
    
    Vector<float> nums1;
    Vector<float> nums2;
    Vector<float> bs;
    Image<unsigned char> fakeoutput;
    fakeoutput.resizeImage(output);
      
    for ( float is2= start; is2 <= end; is2*= step )
    {
      vstage1unknown2Function(argv[1], input, mask, support, fakeoutput, thStart, th1mult, th2mult, is2, 0, &res, &res2, &res3, &res4);
      tprintf("MEANCORR: %f, SCALE2(IS2): %f, ROIRATIO: %f, IMGSCALE: %f\n", res4, is2, roiratio, imageScale);
      nums1.push_back(res2);
      nums2.push_back(res4);
      bs.push_back(is2);
      if ( res2 > maxCorr )
      {
	maxCorr= res2;
	bestScale= is2;
      }
    }
    
    Vector<float> rres;
    
    int bestIdx= -1;
    bestScale= -1;
    
    for ( unsigned int i= 0; i < nums1.size(); ++i )
      if ( nums1(i) == nums1.getMax() && (bestIdx == -1 || nums2(bestIdx) < nums2(i)) )
	bestIdx= i;
    for ( unsigned int i= 0; i < nums2.size(); ++i )
      if ( nums2(i) == nums2(bestIdx) )
	rres.push_back(bs(i));
    bestScale= rres.getGMean();

    if ( roiratio < 0.5 || roiratio > 2 )
      bestScale*= roiratio;
    
    fprintf(stderr, "%f\n", bestScale);
    
    return 0;
  }
  
  else if ( unknown == 2 )
  {
    float res, step= 0.025, mult= 0.75;
    int iter= 0;
    
    if ( input.columns > 1000 )
      step= 0.05;
    
    Vector<int> mask;
    mask.resize(384);
    mask= 1;
    
    float mind= FLT_MAX, minm= 1;

    tprintf("%d %d %d %d\n", input.columns, roi.columns, support.columns, output.columns);
    
    Image<unsigned char> originalROI;
    originalROI= roi;
    
    while ( 1 )
    {
      roi= originalROI;
      vstage1Function(argv[1], input, roi, support, output, thStart, mult, mult, imageScale, 1, -1, NULL, &res, &mask, usepyramid);
      
      float tmp= fabs(res);
      tprintf("distance: %f %f\n", res, tmp);
      
      if ( tmp < mind )
      {
	mind= tmp;
	minm= mult;
      }
      
      ++iter;
      
      mult+= step;
      if ( mult > 1.25 )
	break;
    }
    
    tprintf("%d %d %d %d\n", input.columns, roi.columns, support.columns, output.columns);
    
    vstage1Function(argv[1], input, roi, support, output, thStart, minm, minm, imageScale, 1, -1, NULL, &res, &mask, usepyramid);
    
    ofstream outputfile;
    outputfile.open("multiplier.txt");
    outputfile << minm;
    outputfile.close();
  }
  
  writeImage(argv[5], output);
  
  return 0;
}

int vstage2Function(Image<float>& input, Image<unsigned char>& seed, Image<unsigned char>& roi, int maxit, float nw, char* relativeIntensities, float widthScaling, Image<unsigned char>& output)
{
  tprintf("stage2: correction of edge borders, unknown: %d, ws: %f\n", unknown, widthScaling);

  Border2 originalBorder= input.getBorder2();
  Border2 b= Border2(97, 97, 97, 97, BORDER_MODE_MIRRORED);
  input.setBorder(b);
  b.borderMode= BORDER_MODE_ZERO;
  seed.setBorder(b);
  roi.setBorder(b);
  
  output.resizeImage(input);
  output= 0;
  
  tprintf("new ws: %f\n", widthScaling);
  
  if ( !unknown )
  {
    CorrectionOfEdgeBorders<float> rg(maxit, vstage2shift, vstage2shift, nw, relativeIntensities, NULL, 1.0/widthScaling, vstage2dynth);

    tprintf("start region growing\n");

    rg.doRG(input, output, seed, &roi);
  }
  else if ( unknown == 2 )
  {
    float bestshift= 0;
    float bestDist= FLT_MAX;
    float step= 0.05;
    int bestIdx= 0;
    
    if ( input.columns > 1000 )
      step= 0.1;

    Vector<Image<unsigned char> > results;
    int idx= 0;
    for ( float shift= 0.5; shift <= 1.5; shift+= step )
    {
      CorrectionOfEdgeBorders<float> rg(maxit, shift, shift, nw, relativeIntensities, NULL, 1.0/widthScaling, vstage2dynth, 1);
      
      float dist;
      rg.doRG(input, output, seed, &roi, &dist);
      
      results.push_back(output);
      
      if ( dist < bestDist )
      {
	bestDist= dist;
	bestshift= shift;
	bestIdx= idx;
      }
      
      printf("d: %f ", dist);

      ++idx;
      
      if ( dist > 4*bestDist )
	break;
    }

    printf("============== bestshift: %f\n", bestshift);
    fprintf(stderr, "%f\n", bestshift);
    
    output= results(bestIdx);
    
  }
  
  originalBorder.borderMode= BORDER_MODE_MIRRORED;
  input.setBorder(originalBorder);
  originalBorder.borderMode= BORDER_MODE_ZERO;
  seed.setBorder(originalBorder);
  output.setBorder(originalBorder);
  roi.setBorder(originalBorder);

  return 0;
}

int vstage2Function(int , char** argv, int maxit, float nw, char* relativeIntensities, float widthScaling)
{
  Image<float> input;
  Image<unsigned char> seed, roi, output;

  readImage(argv[1], input);
  readImage(argv[2], seed);
  readImage(argv[3], roi);
  
  tprintf("input: %s\n", argv[1]);
  tprintf("seed: %s\n", argv[2]);
  tprintf("roi: %s\n", argv[3]);
  tprintf("unknown: %d\n", unknown);
  tprintf("widthscaling: %f\n", widthScaling);
  tprintf("output: %s\n", argv[4]);
  vstage2Function(input, seed, roi, maxit, nw, relativeIntensities, widthScaling, output);
  
  tprintf("output: %s\n", argv[4]);
  writeImage(argv[4], output);
  
  return 0;
}

int vstage4bFunction(Image<float>& input, Image<unsigned char>& seed, Image<unsigned char>& roi, Image<unsigned char>& support, float mp, float ap, float /*fp*/, float /*cp*/, int sizeth0, int sizeth1, float widthScaling, Image<unsigned char>& output, int usepyramid= 0, char* featurefile= NULL)
{
  tprintf("stage4: addition of thin objects %f %f %d\n", mp, ap, usepyramid);
  tprintf("th1mult: %f, th2mult: %f\n", vstage1th1multiplier, vstage1th2multiplier);
  
  output.resizeImage(input);
  output= 0;

  float th1= ap;
  float th2= ap;
  updateThresholds(th1, th2, 0.2, vstage1th1multiplier, vstage1th2multiplier);
  ap= th1;
  tprintf("updated ap: %f\n", ap);
  
  tprintf("dimensions: %d %d, %d %d, %d %d, %d %d, %d %d\n", input.rows, input.columns, seed.rows, seed.columns, roi.rows, roi.columns, support.rows, support.columns, output.rows, output.columns);
  
  Transform2Set<float, float>& t2set= *(generateTransform2Set<float, float>(std::string(featurefile), std::string("feature")));
  
  tprintf("t2set.size(): %d\n", t2set.size());
  
  Border2 b1(71, 71, 71, 71);
  Border2 b2= t2set.getProposedBorder();
  Border2 b(b1, b2);
  b.borderMode= BORDER_MODE_ZERO;
  tprintf("borders computed %d %d %d %d\n", b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder);
  
  Vector<int> mask2;
  mask2.resize(t2set.size());
  wsocacheinit= 1;
  wsocache.resize(t2set.size());
  pyramidinit= 0;
  mask2= 1;
  if ( maskinit == 0 )
    maskcache.resize(t2set.size());
  
  float minTh2= FLT_MAX;
  
  char fname[100];
  sprintf(fname, "maxPCC-%f.txt", widthScaling);
  FILE* fmc= NULL;
  fmc= fopen(fname, "r");
  if ( fmc != NULL )
  {
    maxPCC.resize(t2set.size());
    maxpccinit= 1;
    float flag;
    for ( unsigned int i= 0; i < maxPCC.size(); ++i )
    {
      int rv= fscanf(fmc, "%f", &flag);
      if ( rv != 1 )
	break;
      maxPCC(i)= flag;
    }
    fclose(fmc);
  }
  else
    maxPCC.resize(t2set.size());
  
  for ( unsigned int i= 0; i < t2set.size(); ++i )
  {
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->th2 < minTh2 )
	minTh2= tmp->th2;
    }
    if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->th2 < minTh2 )
	minTh2= tmp->th2;
    }
  }
  
  int discarded= 0;
  
  #pragma omp parallel for
  for ( unsigned int i= 0; i < t2set.size(); ++i )
  {
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->lambda > 0.8*vstage4wa && tmp->lambda < 1.2*vstage4wa )
      {
	tmp->scale= 1.0/widthScaling;
	tmp->regenerate();
	if ( unknown )
	{
	  mask2(i)= 1;
	  if ( !roc )
	  {
	    if ( tmp->mgf->operator[](0)->size() < 100 )
	      mask2(i)= checkOperator(tmp, i);
	  }
	}
      }
      else
	mask2(i)= 0;
      
      if ( mask2(i) == 0 )
	++discarded;
    }
    if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i)) != NULL )
    {
      PowerGaborSimpleRGLineSegmentTransform2<float,float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(i));
      
      if ( tmp->lambda > 0.8*vstage4wa && tmp->lambda < 1.2*vstage4wa )
      {
	tmp->scale= 1.0/widthScaling;
	tmp->regenerate();
	if ( unknown )
	{
	  mask2(i)= 1;
	  if ( !roc )
	  {
	    if ( tmp->mgf->operator[](0)->size() < 100 )
	      mask2(i)= checkOperator(tmp, i);
	  }
	}
      }
      else
	mask2(i)= 0;
      
      if ( mask2(i) == 0 )
	++discarded;
    }
  }
  
  tprintf("features generated\n");
  maxpccinit= 1;
  
  fmc= fopen(fname, "r");
  if ( fmc == NULL )
  {
    fmc= fopen(fname, "w");
    for ( unsigned int i= 0; i < maxPCC.size(); ++i )
      fprintf(fmc, "%f\n", maxPCC(i));
    fclose(fmc);
  }
  else
  {
    fclose(fmc);
  }
  
  tprintf("size of t2set after scaling: %d\n", mask2.numberOfNonZeroElements());

  tprintf("reading input images\n");
  Image<float> result;
  Image<unsigned char> img;
  Image<float> outputTmp;
  Image<float> outputNum;
  Image<unsigned char> roitmp;

  Border2 originalBorder= input.getBorder2();
  
  b.borderMode= BORDER_MODE_MIRRORED;
  input.resizeBorder(b);
  b.borderMode= BORDER_MODE_ZERO;
  roi.resizeBorder(b);
  support.resizeBorder(b);
  output.resizeImage(input);
  result.resizeImage(input);
  outputTmp.resizeImage(input);
  outputNum.resizeImage(input);

  output= 0;
  outputTmp= 0;
  outputNum= 0;
  tprintf("applying the transforms\n");

  Image<unsigned char> reducedROI;
  reducedROI= roi;
  tprintf("roi size: %d\n", roi.numberOfNonZeroElements());
  
  tprintf("starting applying the transforms\n");
  int completed= 0;
  
  #pragma omp parallel for schedule(dynamic, 2)
  for ( unsigned int j= 0; j < t2set.size(); ++j )
  {
    if ( !mask2(j) )
    {
      completed++;
      continue;
    }
    
    int threadnum= omp_get_thread_num();
    if ( threadnum == 0 )
      tprintf("completed/total: %d/%zd\n", completed, t2set.size());
    
    Image<float> result(input), result2(input);
    Image<unsigned char> rtmp;
    rtmp.resizeImage(input);
    
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>*>(t2set(j)) != NULL )
      applyFilter(j, input, reducedROI, support, result, result2, b, dynamic_cast<PowerGaborRGLineSegmentTransform2<float,float>*>(t2set(j)), NULL, 0, usepyramid);
    else if ( dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(j)) != NULL )
      applyFilter(j, input, reducedROI, support, result, result2, b, NULL, dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float,float>*>(t2set(j)), 0, usepyramid);
    
    #pragma omp critical
    {
      for ( unsigned int i= 0; i < result2.n; ++i )
	if ( result2(i) > 0 )
	{
	  outputTmp(i)+= result2(i);
	  outputNum(i)+= 1.0;
	}
    }
  }

  for ( unsigned int i= 0; i < outputTmp.n; ++i )
    if ( outputNum(i) > 0 )
      outputTmp(i)/= outputNum(i);
    else
      outputTmp(i)= 0;
  
  Image<unsigned char> tmpb;
  tmpb.resizeImage(outputTmp);
  int regionsAdded= 0;
  
  for ( float threshold= 0.45; threshold <= 1; threshold+= 0.01 )
  {
    tprintf("thresholding at %f\n", threshold); fflush(stdout);
    for ( unsigned int i= 0; i < outputTmp.n; ++i )
    {
      if ( outputTmp(i) > threshold )
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
      
      if ( regionSize < sizeth0 || regionSize > sizeth1 )
	continue;

      
      float rmean= 0;
      int n= 0;
      for ( unsigned int i= 0; i < tmpRegion.size(); ++i )
	if ( !seed(tmpRegion(i)) )
	{
	  rmean+= outputTmp(tmpRegion(i));
	  ++n;
	}
      rmean/= n;

      if ( rmean < ap )
	continue;
      
      float dofp= tmpRegion.getDistanceOfFarestPixels();

      if ( dofp > sizeth0  )
      {
	#pragma omp critical
	{
	  tmpRegion.set(output, 0, (unsigned char)255);
	  regionsAdded++;
	}
      }
    }
  }
  
  tprintf("regions added: %d\n", regionsAdded);
    
  input.resizeBorder(originalBorder);
  support.resizeBorder(originalBorder);
  roi.resizeBorder(originalBorder);
  output.resizeBorder(originalBorder);
  
  //writeImage("output4.bmp", output);

  return 0;
}

int vstage4Function(Image<float>& input, Image<unsigned char>& seed, Image<unsigned char>& roi, Image<unsigned char>& support, float mp, float ap, float fp, float cp, int sizeth0, int sizeth1, float widthScaling, Image<unsigned char>& output, int usepyramid= 0)
{
  tprintf("stage4: addition of thin objects %f %f %d\n", mp, ap, usepyramid);
  tprintf("th1mult: %f, th2mult: %f\n", vstage1th1multiplier, vstage1th2multiplier);
  
  output.resizeImage(input);
  output= 0;

  float th1= ap;
  float th2= ap;
  updateThresholds(th1, th2, 0.2, vstage1th1multiplier, vstage1th2multiplier);
  ap= th1;
  tprintf("updated ap: %f\n", ap);
  
  tprintf("dimensions: %d %d, %d %d, %d %d, %d %d, %d %d\n", input.rows, input.columns, seed.rows, seed.columns, roi.rows, roi.columns, support.rows, support.columns, output.rows, output.columns);
  
  //AdditionOfThinObjects<float> rg(mp, ap, fp, cp, sizeth0, sizeth1, widthScaling, vstage2shift, vstage2dynth, vstage2nweight, vstage4wa, vstage4wb);
  AdditionOfThinObjects<float> rg(mp, ap, fp, cp, sizeth0, sizeth1, widthScaling, vstage2shift, vstage2dynth, vstage2nweight, vstage4wa, vstage1th1multiplier);

  rg.doRG(input, output, seed, &roi, &support);

  

  return 0;
}

int vstage4Function(int , char** argv, float mp, float ap, float fp, float cp, int sizeth0, int sizeth1, float widthScaling)
{
  Image<float> input;
  Image<unsigned char> seed, roi, output, support, original, backscaled;
  
  Border2 b(71, 71, 71, 71);
  b.borderMode=BORDER_MODE_ZERO;
  
  tprintf("input: %s\n", argv[1]);
  tprintf("seed: %s\n", argv[2]);
  tprintf("roi: %s\n", argv[3]);
  tprintf("support: %s\n", argv[4]);
  
  char* featurefile= argv[1];
  
  readImage(argv[2], input, b);
  readImage(argv[3], seed, b);
  readImage(argv[4], roi, b);
  readImage(argv[5], support, b);
  
  if ( input.columns > 1000 )
    reduceROI(input, roi);
  
  original.resizeImage(seed);
  original= seed;
  original.removeBorder();
  
  if ( unknown != 0 )
  {
    sizeth0/= widthScaling;
    
    output.resizeImage(input);
    output.setBorder(b);
    
    tprintf("original image: %d %d\n", input.rows, input.columns);
    
    tprintf("modified sizeth0: %d\n", sizeth0);
    tprintf("calling vstage4Function\n");
    tprintf("resized image: %d %d\n", input.rows, input.columns);
    tprintf("%d %d, %d %d, %d %d, %d %d, %d %d\n", input.rows, input.columns, seed.rows, seed.columns, roi.rows, roi.columns, support.rows, support.columns, output.rows, output.columns);
    tprintf("%d %d\n", input.leftBorder, input.topBorder);
    tprintf("%d %d\n", seed.leftBorder, seed.topBorder);
    tprintf("%d %d\n", roi.leftBorder, roi.topBorder);
    tprintf("%d %d\n", support.leftBorder, support.topBorder);
    tprintf("%d %d\n", output.leftBorder, output.topBorder);
    
    Vector<Image<unsigned char> > outputs;

    float sc= widthScaling; 
    float upperBound= 1;
    if ( input.rows < 1000 )
      upperBound= 0.5;
    do
    {
      vstage4bFunction(input, seed, roi, support, mp, ap, fp, cp, sizeth0, sizeth1, sc, output, 1, featurefile);
      outputs.push_back(output);
      sc*= 1.5;
      tprintf("sc: %f\n", sc);
    }
    while ( sc < upperBound );
    
    tprintf("vstage4Function called\n");
    tprintf("%d %d, %d %d, %d %d, %d %d, %d %d\n", input.rows, input.columns, seed.rows, seed.columns, roi.rows, roi.columns, support.rows, support.columns, output.rows, output.columns);
    
    for ( unsigned int i= 0; i < outputs.size(); ++i )
    {
      output= outputs(i);
      output.removeBorder();
      
      for ( unsigned int i= 0; i < output.n; ++i )
	if ( output(i) )
	  original(i)= 255;
    }
    
    output= original;
    
    tprintf("writing output image\n");
    writeImage(argv[6], output);
  }
  else
  {
    sizeth0/= widthScaling;
    
    tprintf("modified sizeth0: %d\n", sizeth0);
    tprintf("calling vstage4Function\n");
    tprintf("resized image: %d %d\n", input.rows, input.columns);
    tprintf("%d %d, %d %d, %d %d, %d %d, %d %d\n", input.rows, input.columns, seed.rows, seed.columns, roi.rows, roi.columns, support.rows, support.columns, output.rows, output.columns);
    tprintf("%d %d\n", input.leftBorder, input.topBorder);
    tprintf("%d %d\n", seed.leftBorder, seed.topBorder);
    tprintf("%d %d\n", roi.leftBorder, roi.topBorder);
    tprintf("%d %d\n", support.leftBorder, support.topBorder);
    tprintf("%d %d\n", output.leftBorder, output.topBorder);
    
    int pyramid= 0;
    if ( input.rows > 1000 )
        pyramid= 1;
    
    vstage4bFunction(input, seed, roi, support, mp, ap, fp, cp, sizeth0, sizeth1, widthScaling, output, pyramid, featurefile);
    
    for ( unsigned int i= 0; i < output.n; ++i )
      if ( output(i) )
	seed(i)= 255;
    output= seed;
    
    tprintf("writing output image\n");
    writeImage(argv[6], output);
  }
  
  return 0;
}

int vstageallFunction(int , char** argv, int vstage1thStart, float vstage1th1multiplier, float vstage1th2multiplier, float vstage1imgscale, int vstage2maxit, float vstage2nw, char* vstage2relint, float vstage2ws, float /*vstage3th0*/, float /*vstage3th1*/, float /*vstage4mp*/, float /*vstage4ap*/, float /*vstage4fp*/, float /*vstage4cp*/, int /*vstage4sizeth0*/, int /*vstage4sizeth1*/, float /*vstage4ws*/)
{
  Image<float> input;
  Image<unsigned char> roi, support, tmp, output;
  
  readImage(argv[2], input);
  readImage(argv[3], roi);
  readImage(argv[4], support);
  
  vstage1Function(argv[1], input, roi, support, output, vstage1thStart, vstage1th1multiplier, vstage1th2multiplier, vstage1imgscale);
  vstage2Function(input, output, roi, vstage2maxit, vstage2nw, vstage2relint, vstage2ws, tmp);
/*  vstage3Function(tmp, vstage3th0, vstage3th1, output);
  vstage4Function(input, output, roi, support, vstage4mp, vstage4ap, vstage4fp, vstage4cp, vstage4sizeth0, vstage4sizeth1, vstage4ws, tmp);*/

  output= tmp;
  
  writeImage(argv[6], output);
  return 0;
}

int vstageallcolorFunction(int , char** argv, int vstage1thStart, float vstage1th1multiplier, float vstage1th2multiplier, float vstage1imgscale, int vstage2maxit, float vstage2nw, char* vstage2relint, float vstage2vs, float /*vstage3th0*/, float /*vstage3th1*/, float /*vstage4mp*/, float /*vstage4ap*/, float /*vstage4fp*/, float /*vstage4cp*/, int /*vstage4sizeth0*/, int /*vstage4sizeth1*/, float /*vstage4ws*/)
{
  Image<float> input, originalGreen;
  Image<unsigned char> roi, support, tmp, output, red, green, extended;
  
  tprintf("reading color input image: %s\n", argv[2]);
  readImage(argv[2], red, READ_CHANNEL_RED);
  readImage(argv[2], green, READ_CHANNEL_GREEN);
  
  originalGreen= green;
  //int originalRows= red.rows, originalColumns= red.columns, scalingFlag= 0;
  
  tprintf("extracting the roi\n");
  vstage0FunctionROI(red, roi);
  
  tprintf("resizing if required\n");
  
  tprintf("extending the roi\n");
  vstage0FunctionExtend(green, roi, extended);
  
  tprintf("possible modification for drive\n");
  if ( red.filename.find("drive") != string::npos )
  {
    File f(red.filename);
    File filename= f.getFilename();
    string id= filename.substr(0, 2);
    string roifile;
    if ( atoi(id.c_str()) <= 20 )
      roifile= string("/home/gykovacs/openip/data/drscreen-vessel-database-drive/rois/") + id + ("_test_mask.gif.png");
    else
      roifile= string("/home/gykovacs/openip/data/drscreen-vessel-database-drive/rois/") + id + ("_training_mask.gif.png");
    FILE* roip;
    if ( (roip= fopen(roifile.c_str(), "r")) )
    {
      tprintf("reading outer roi: %s\n", roifile.c_str());
      fclose(roip);
      readImage(roifile.c_str(), roi);
    }
  }
  
  input= extended;
  support.resizeImage(roi);
  support= 255;
  
  input.removeBorder();
  support.removeBorder();
  roi.removeBorder();
  
  writeImage("tmpa.bmp", input);
  writeImage("tmpb.bmp", roi);
  writeImage("tmpc.bmp", support);
  
  tprintf("vstage1\n");
  vstage1Function(argv[1], input, roi, support, output, vstage1thStart, vstage1th1multiplier, vstage1th2multiplier, vstage1imgscale);
  
  tprintf("vstage2\n");
  vstage2Function(input, output, roi, vstage2maxit, vstage2nw, vstage2relint, vstage2vs, tmp);
  /*tprintf("vstage3\n");
  vstage3Function(tmp, vstage3th0, vstage3th1, output);*/
  tprintf("vstage4\n");
  //vstage4Function(input, output, roi, support, vstage4mp, vstage4ap, vstage4fp, vstage4cp, vstage4sizeth0, vstage4sizeth1, vstage4ws, tmp);
  
  
  tprintf("writing output image\n");
  writeImage(argv[4], output);
  return 0;
}


int calculateMeasuresFunction(int /*argc*/, char** argv)
{
  float acc= atof(argv[1]);
  float sens= atof(argv[2]);
  float spec= atof(argv[3]);
  float fg= atof(argv[4]);
  
  printf("acc: %f\n", acc);
  printf("sens: %f\n", sens);
  printf("spec: %f\n", spec);
  printf("fg: %f\n", fg);
  
  float tp= sens*fg;
  float fn= fg - tp;
  float fp= (tp*(acc-1) + acc*fn)/((spec)/(1-spec)*(1 - acc) - acc);
  float tn= (spec)/(1-spec)*fp;
  
  printf("tp: %f\n", tp);
  printf("fn: %f\n", fn);
  printf("fp: %f\n", fp);
  printf("tn: %f\n", tn);
  
  float all= tp + fn + fp + tn;
  
  printf("all: %f\n", tp + fn + fp + tn);
  printf("bg: %f\n", all - fg);
  float acc2= (tp + tn)/all;
  float sens2= tp/(tp + fp);
  float spec2= tn/(tn + fp);
  float ppv= tp/(tp + fp);
  float npv= tn/(tn + fn);
  float f1= (2*tp)/(2*tp + fn + fp);
  
  printf("acc: %f\n", acc2);
  printf("sens: %f\n", sens2);
  printf("spec: %f\n", spec2);
  printf("ppv: %f\n", ppv);
  printf("npv: %f\n", npv);
  printf("f1: %f\n", f1);
  
  printf("dice: %f\n", 2*tp/(fg + tp + fn));
  
  return 0;
}

int calculateForegroundFunction(int /*argc*/, char** argv)
{
  int fg= 0;
  Image<unsigned char> input;
  readImage(argv[1], input);
  for ( unsigned int i= 0; i < input.n; ++i )
    if ( input(i) )
      ++fg;
  printf("%d\n", fg);
  return 0;
}

int calculateSizeFunction(int /*argc*/, char** argv)
{
  Image<unsigned char> input;
  readImage(argv[1], input);
  printf("%d\n", input.rows*input.columns);
  return 0;
}

int pixeldiffFunction(int /*argc*/, char** argv)
{
  Image<unsigned char> a, b;
  readImage(argv[1], a);
  readImage(argv[2], b);
  
  int n= 0;
  for ( unsigned int i= 0; i < a.n; ++i )
    if ( a(i) != b(i) )
      ++n;
  printf("%d %s\n", n, argv[1]);
  
  return 0;
}

int scalingFunction(int /*argc*/, char** argv)
{
  Image<unsigned char> input;
  Image<unsigned char> output;
  float scalingFactor= atof(argv[2]);
  Border2 b(13, 13, 13, 13);
  readImage(argv[1], input, b);
  
  output.resizeImage(input.rows*scalingFactor, input.columns*scalingFactor);
  
  binearestNeighborScaling(input, output);
  
  writeImage(argv[3], output);
  
  return 0;
}

int gmeanFunction(int /*argc*/, char** argv)
{
  ifstream input;
  input.open(argv[1]);
  float mean= 1;
  int n= 0;
  Vector<float> a;
  while ( !input.eof() )
  {
    float tmp;
    input >> tmp;
    mean*= tmp;
    ++n;
    a.push_back(tmp);
  }
  
  printf("%f", pow(mean, 1.0/n));
  //printf("%f", a.getMedian());
  
  return 0;
}

int gmean2Function(int /*argc*/, char** argv)
{
  //ifstream input;
  //input.open(argv[1]);
  FILE* input= fopen(argv[1], "r");
  float mean= 1;
  int n= 0;
  Vector<float> a;
  int idx= atoi(argv[2]);
  
  float p0, p1;
  
  while ( fscanf(input, "%f%f", &p0, &p1) == 2 )
  {
    float tmp= 0;
    if ( idx == 1 )
      tmp= p1;
    if ( idx == 0 )
      tmp= p0;
    
    //printf("%f ", tmp);
    mean*= tmp;
    
    ++n;
    
  }
  
  printf("%f", pow(mean, 1.0/n));
  //printf("%f", a.getMedian());
  
  return 0;
}

int ameanFunction(int /*argc*/, char** argv)
{
  ifstream input;
  input.open(argv[1]);
  float mean= 0;
  int n= 0;
  Vector<float> a;
  while ( !input.eof() )
  {
    float tmp;
    input >> tmp;
    mean+= tmp;
    ++n;
    a.push_back(tmp);
  }
  
  printf("%f", mean/n);
  //printf("%f", a.getMedian());
  
  return 0;
}

int bestparametersFunction(int /*argc*/, char** argv)
{
  ifstream input;
  input.open(argv[1]);
  
  string parameters;
  string bestparameters;
  string line;
  
  float acc= 0;
  float bestAcc= 0;
  int flag= 0;
  while ( !input.eof() )
  {
    getline(input, line);
    if ( line.length() < 30 )
    {
      parameters= line;
      flag= 1;
    }
    else
    {
      if ( flag == 1 )
      {
	flag= 0;
	stringstream ss(line);
	string tmp;
	for ( int i= 0; i < 14; ++i )
	  ss >> tmp;
	ss >> acc;
	if ( acc > bestAcc )
	{
	  bestAcc= acc;
	  bestparameters= parameters;
	}
      }
    }
  }
  
  cout << bestAcc << " " << bestparameters << endl;
  
  return 0;
}

int ameanColsFunction(int /*argc*/, char** argv)
{
  ifstream input;
  input.open(argv[1]);
  
  string line;
  float number;
  Vector<float> means;
  Vector<float> nums;
  
  int i= 0;
  while ( input.good() )
  {
    getline(input, line);
    if ( line.length() < 2 )
      break;
    stringstream ss(line);
    int j= 0;
    while ( !ss.eof() )
    {
      ss >> number;
      if ( i == 0 )
      {
	means.push_back(number);
	if ( number > 0 )
	  nums.push_back(1);
	else
	  nums.push_back(0);
      }
      else if ( number > 0 )
      {
	means(j)+= number;
	nums(j)+= 1.0;
      }
      ++j;
    }
    ++i;
  }
  
  for ( unsigned int i= 0; i < means.size(); ++i )
    if ( nums(i) > 0 )
      means(i)/= nums(i);
  
  for ( unsigned int i= 0; i < means.size(); ++i )
    printf("%f ", means(i));
  
  return 0;
}

int ameanstdColsFunction(int /*argc*/, char** argv)
{
  ifstream input;
  input.open(argv[1]);
  
  string line;
  float number;
  Vector<float> means;
  Vector<float> means2;
  Vector<float> nums;
  
  int i= 0;
  while ( input.good() )
  {
    getline(input, line);
    stringstream ss(line);
    int j= 0;
    while ( !ss.eof() )
    {
      ss >> number;
      if ( i == 0 )
      {
	means.push_back(number);
	means2.push_back(number*number);
	if ( number > 0 )
	  nums.push_back(1);
	else
	  nums.push_back(0);
      }
      else if ( number > 0 )
      {
	means(j)+= number;
	means2(j)+= number*number;
	nums(j)+= 1.0;
      }
      ++j;
    }
    ++i;
  }
  
  for ( unsigned int i= 0; i < means.size(); ++i )
    if ( nums(i) > 0 )
    {
      means(i)/= nums(i);
      means2(i)= sqrt(means2(i)/nums(i) - means(i)*means(i));
    }
  
  printf("%zd\n", means.size());
  for ( unsigned int i= 0; i < means.size()-1; ++i )
    printf("%f ", means(i));
  printf("\n");
  for ( unsigned int i= 0; i < means2.size()-1; ++i )
    printf("%f ", means2(i));
  
  return 0;
}


int extractgreenFunction(int /*argc*/, char** argv)
{
  Image<unsigned char> green;
  readImage(argv[1], green, READ_CHANNEL_GREEN);
  
  writeImage(argv[2], green);
  return 0;
}

int smallestWavelengthFunction(int /*argc*/, char** argv)
{
  Transform2Set<float, float> f= *(generateTransform2Set<float, float>(std::string("trained-features-selected-for-accuracy-iteration-2.fdf"), std::string("feature")));
  
  Border2 b(77, 77, 77, 77, BORDER_MODE_MIRRORED);
  
  Image<float> input;
  readImage(argv[1], input, b);
  Image<unsigned char> mask;
  b.borderMode= BORDER_MODE_ZERO;
  readImage(argv[2], mask, b);
  Image<float> wid;
  wid.resizeImage(input);
  wid= 0;
  
  Vector<MatchedCCorrelationPowerGaborFilterR2<float, float>* > filters;
  Vector<MatchedCCorrelationPowerGaborFilterSimpleR2<float, float>* > pfilters;
  
  Vector<float> wavelengths;
  
  for ( unsigned int i= 0; i < f.size(); ++i )
    if ( dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i)) != NULL )
    {
      PowerGaborRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborRGLineSegmentTransform2<float, float>* >(f(i));
      if ( tmp->gamma < 0.5 )
      {
	filters.push_back(tmp->mgf);
	wavelengths.push_back(tmp->lambda);
	filters(filters.size()-1)->updateStride(input.columns);
      }
    }
    else if (dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i)) != NULL)
    {
      PowerGaborSimpleRGLineSegmentTransform2<float, float>* tmp= dynamic_cast<PowerGaborSimpleRGLineSegmentTransform2<float, float>* >(f(i));
      if ( tmp->gamma < 0.5 )
      {
	pfilters.push_back(tmp->mgf);
	wavelengths.push_back(tmp->lambda);
	pfilters(pfilters.size()-1)->updateStride(input.columns);
      }
    }
  tprintf("number of filters: %zd %zd\n", filters.size(), pfilters.size());
    
  Vector<float> ws;
  float tmp;
  float max= 0;
  float maxw= 0;
  Vector<float> widths;
  for ( unsigned int i= 0; i < mask.n; ++i )
    if ( Region2::isInnerContour8(mask, i) )
    {
      float w= calculateWidth(mask, input, i);
      if ( w > 0 )
      //printf("%f ", w); fflush(stdout);
      widths.push_back(w);
      wid(i)= w;
    }
  
  Image<float> wid2;
  wid2.resizeImage(wid);
  wid2= 0;
  StructuringElementSquare ses(3);
  ses.updateStride(wid.columns);
  for ( int k= 0; k < 10; ++k )
  {
    wid2= wid;
    for ( unsigned int i= 0; i < mask.n; ++i )
      if ( mask(i) && wid(i) == 0 )
      {
	Vector<float> tmp;
	for ( unsigned int k= 0; k < ses.size(); ++k )
	  if ( wid(i + ses(k)) > 0 )
	    tmp.push_back(wid(i + ses(k)));
	if ( tmp.size() > 0 )
	  wid2(i)= tmp.getMean();
      }
    wid= wid2;
  }
  
  tprintf("mean width: %f %d %f\n", widths.getMean(), widths.size(), widths(0));
  sort(widths.begin(), widths.end());
  tprintf("mean width: %f %d %f\n", widths.getMean(), widths.size(), widths(0));
  //float threshold= widths(int(float(widths.size())*0.005));
  float threshold= widths(widths.size()*0.01);
  tprintf("threshold: %f\n", threshold);
  
  for ( unsigned int i= 0; i < mask.n; ++i )
    if ( mask(i) && wid2(i) < threshold )
    {
      {
	max= 0;
	maxw= 0;
	for ( unsigned int j= 0; j < filters.size(); ++j )
	{
	  tmp= filters(j)->apply(input, i, NULL);
	  if ( tmp > max )
	  {
	    max= tmp;
	    maxw= wavelengths(j);
	  }
	}
	for ( unsigned int j= 0; j < pfilters.size(); ++j )
	{
	  tmp= pfilters(j)->apply(input, i, NULL);
	  if ( tmp > max )
	  {
	    max= tmp;
	    maxw= wavelengths(j + filters.size());
	  }
	}
	ws.push_back(maxw);
      }
    }
  
  tprintf("smallest wavelength: %f\n", ws.getMean());
  fprintf(stderr, "%f\n", ws.getMean());
  return 0;
}

int calculateMeanRIFunction(int /*argc*/, char** argv)
{
  Image<unsigned char> labels;
  Image<float> input;
  Border2 b(75, 75, 75, 75, BORDER_MODE_ZERO);
  readImage(argv[1], labels, b);
  b.borderMode= BORDER_MODE_MIRRORED;
  readImage(argv[2], input, b);
  
  Vector<float> ris;
  Vector<float> ros;
  
  for ( unsigned int i= 0; i < labels.n; ++i )
    if ( labels.isRealImagePixel(i) )
    {
      if ( Region2::isInnerContour8(labels, i) )
	ris.push_back(calculateRI(labels, input, i));
      else if ( Region2::isOuterContour8(labels, i) ) 
	ros.push_back(calculateRI(labels, input, i));
    }
    
  //printf("%f\n", (ris.getMean() + ros.getMean())/2);
  printf("%f\n", ros.getMean());
  
  return 0;
}

int visualizeROI0Function(int /*argc*/, char** argv)
{
  Image<unsigned char> input;
  Image<unsigned char> roi;
  
  readImage(argv[1], input);
  readImage(argv[2], roi);
  
  for ( unsigned int i= roi.columns+1; i < roi.n - roi.columns-1; ++i )
    if ( Region2::isInnerContour8(roi, i) || Region2::isOuterContour8(roi,i) )
      input(i)= 255;
    
  writeImage(argv[3], input);
  
  return 0;
}

int relativeIntensities(int , char** , bool ericolumns)
{
  Image<unsigned char> input;
  Image<float> image;

  char finput[1000];
  char fimage[1000];
  
  Vector<Vector<float> > inner;
  Vector<Vector<float> > outer;
  Vector<Vector<float> > stdinner;
  Vector<Vector<float> > stdouter;
  Vector<Vector<int> > ninner;
  Vector<Vector<int> > nouter;
  
  inner.resize(90);
  outer.resize(90);
  ninner.resize(90);
  nouter.resize(90);
  stdinner.resize(90);
  stdouter.resize(90);
  
  for ( unsigned int i= 0; i < inner.size(); ++i )
  {
    inner(i).resize(90);
    outer(i).resize(90);
    stdinner(i).resize(90);
    stdouter(i).resize(90);
    ninner(i).resize(90);
    nouter(i).resize(90);
    inner(i)= 0;
    outer(i)= 0;
    ninner(i)= 0;
    nouter(i)= 0;
    stdinner(i)= 0;
    stdouter(i)= 0;
  }
  
  //int i= 0;
  
  EqualSlopeAtBoundaries<float>* esab= new EqualSlopeAtBoundaries<float>(3, 87, 0.4, 1);
  
  Border2 b0(89, 89, 89, 89, BORDER_MODE_MIRRORED);
  Border2 b1(89, 89, 89, 89, BORDER_MODE_ZERO);
  
  float fwidth, fdyn, fri;
  
  int dynUpper= 90;
  int widthUpper= 90;
  
  while ( scanf("%s%s", finput, fimage) == 2 )
  {
    readImage(finput, input, b0);
    readImage(fimage, image, b1);
    
    fprintf(stderr, "%s %s\n", finput, fimage); fflush(stderr);
    esab->updateStride(image.columns);
    
    
    for ( int i= image.columns+1; i < int(image.n) - image.columns-1; ++i )
    {
      if ( image.isRealImagePixel(i) )
      {
	if ( Region2::isInnerContour4(input, i) )
	{
	  esab->computeDescriptors(input, i, image, fwidth, fdyn, fri);
	  int width= floor(esab->swidthc(input, i, image, 0)+0.5);
	  int dyn= floor(fdyn+0.5);
	  if ( width > 0 && width < int(inner.size()) && dyn < int(inner(0).size()) && !isnan(fri) )
	  {
	    inner(width)(dyn)+= fri;
	    ninner(width)(dyn)++;
	  }
	  if ( dyn > dynUpper )
	  {
	    for ( unsigned int j= dynUpper; j < inner(0).size(); ++j )
	    {
	      inner(width)(j)+= fri;
	      ninner(width)(j)++;
	    }
	  }
	  
	  if ( width > 1 && dyn > 1 && width < widthUpper-1 && dyn < dynUpper-1 )
	  {
	    inner(width-1)(dyn)+= fri;
	    ninner(width-1)(dyn)++;
	    inner(width+1)(dyn)+= fri;
	    ninner(width+1)(dyn)++;
	    inner(width)(dyn-1)+= fri;
	    ninner(width)(dyn-1)++;
	    inner(width)(dyn+1)+= fri;
	    ninner(width)(dyn+1)++;
	  }
	  if ( width > 2 && dyn > 2 && width < widthUpper-2 && dyn < dynUpper-2 )
	  {
	    inner(width-2)(dyn)+= fri;
	    ninner(width-2)(dyn)++;
	    inner(width+2)(dyn)+= fri;
	    ninner(width+2)(dyn)++;
	    inner(width)(dyn-2)+= fri;
	    ninner(width)(dyn-2)++;
	    inner(width)(dyn+2)+= fri;
	    ninner(width)(dyn+2)++;
	  }
	}
	else if ( Region2::isOuterContour4(input, i) )
	{
	  esab->computeDescriptors(input, i, image, fwidth, fdyn, fri);
	  int width= floor(esab->swidthc(input, i, image, 0)+0.5);
	  int dyn= floor(fdyn+0.5);
	  if ( width > 0 && width < int(outer.size()) && dyn < int(outer(0).size()) && !isnan(fri) )
	  {
	    outer(width)(dyn)+= fri;
	    nouter(width)(dyn)++;
	  }
	  if ( dyn > dynUpper )
	  {
	    for ( unsigned int j= dynUpper; j < outer(0).size(); ++j )
	    {
	      outer(width)(j)+= fri;
	      nouter(width)(j)++;
	    }
	  }
	  
	  if ( width > 1 && dyn > 1 && width < widthUpper-1 && dyn < dynUpper-1 )
	  {
	    outer(width-1)(dyn)+= fri;
	    nouter(width-1)(dyn)++;
	    outer(width+1)(dyn)+= fri;
	    nouter(width+1)(dyn)++;
	    outer(width)(dyn-1)+= fri;
	    nouter(width)(dyn-1)++;
	    outer(width)(dyn+1)+= fri;
	    nouter(width)(dyn+1)++;
	  }
	  
	  if ( width > 2 && dyn > 2 && width < widthUpper-2 && dyn < dynUpper-2 )
	  {
	    outer(width-2)(dyn)+= fri;
	    nouter(width-2)(dyn)++;
	    outer(width+2)(dyn)+= fri;
	    nouter(width+2)(dyn)++;
	    outer(width)(dyn-2)+= fri;
	    nouter(width)(dyn-2)++;
	    outer(width)(dyn+2)+= fri;
	    nouter(width)(dyn+2)++;
	  }
	}
      }
    }
    
  }

  tprintf("ris computed, computing the average\n");
  for ( unsigned int i= 0; i < inner.size(); ++i )
  {
    for ( unsigned int j= 0; j < inner(i).size(); ++j )
    {
      if ( ninner(i)(j) > 20 )
	inner(i)(j)/= ninner(i)(j);
      if ( nouter(i)(j) > 20 )
	outer(i)(j)/= nouter(i)(j);
    }
  }

  tprintf("writing to files\n");
  if ( !ericolumns)
  {
    for ( unsigned int i= 0; i < inner(0).size(); ++i )
    {
      ofstream file;
      char filename[100];
      sprintf(filename, "relative-intensities-%d.txt", i);
      file.open(filename);
      
      file << inner.size() << " ";
      for ( unsigned int j= 0; j < inner.size(); ++j )
      {
	if ( ninner(j)(i) > 20 )
	    file << inner(j)(i) << " ";
	  else
	    file << "-1 ";
      }
      file << endl;
      file << inner.size() << " ";
      for ( unsigned int j= 0; j < inner.size(); ++j )
      {
	if ( nouter(j)(i) > 20 )
	    file << outer(j)(i) << " ";
	  else
	    file << "-1 ";
      }
      file << endl;
      file.close();
    }
  }
  else
  {
    for ( unsigned int i= 0; i < inner(0).size(); ++i )
    {
      ofstream file;
      char filename[100];
      sprintf(filename, "relative-intensities-%d-columns.txt", i);
      file.open(filename);
      
      for ( unsigned int j= 0; j < inner.size(); ++j )
      {
	file << j << " ";
	if ( ninner(j)(i) > 20 )
	    file << inner(j)(i) << " ";
	  else
	    file << "-1 ";
	if ( nouter(j)(i) > 20 )
	    file << outer(j)(i) << " ";
	  else
	    file << "-1 ";  
	file << endl;
      }
      file.close();
    }
  }
  
  tprintf("deleting esab object\n");
  delete esab;

  return 0;
}

/**
 * main application
 * @param argc argument count
 * @param argv argument vector
 * @return 
 */
int main(int argc, char** argv)
{
    bool roi= false;
    bool vv= false;
    bool vv2= false;
    bool vstage0= false;
    bool vstage1= false;
    bool vstage2= false;
    //bool vstage3= false;
    bool vstage4= false;
    int vstage1thStart= 0;
    float vstage1imgscale= 1;
    int vstage2maxit= 30;
    float vstage2nw= 1.6;
    char vstage2relint[1000]= {'\0'};
    float vstage2vs= 1.0;
    float vstage3th0= 0.7;
    float vstage3th1= 0.17;
    float vstage4mp= 0.1;
    float vstage4ap= 0.6;
    float vstage4fp= 0.0;
    float vstage4cp= 0.5;
    int vstage4sizeth0= 20;
    int vstage4sizeth1= 200000;
    float vstage4ws= 1;
    bool vesselall= false;
    bool vesselallcolor= false;
    bool calculateMeasures= false;
    bool roiradius= false;
    //bool scale= false;
    //bool scale2= false;
    bool pixeldiff= false;
    bool scaling= false;
    bool geometricMean= false;
    bool geometricMean2= false;
    bool arithmeticMean= false;
    bool arithmeticMeanCols= false;
    bool arithmeticMeanStdCols= false;
    bool bestparameters= false;
    bool calculateSize= false;
    bool calculateForeground= false;
    bool calculateMeanRI= false;
    bool extractgreen= false;
    bool smallestwavelength= false;
    bool visroi0= false;
    bool eri= false;
    bool ericolumns= false;

    OptionTable ot;

    ot.addOption(string(""), OPTION_SEPARATOR, NULL, 0, string("Training"));
    
    
    //ot.addOption(string(""), OPTION_SEPARATOR, NULL, 0, string("ROI"));
    //ot.addOption(string("--roi"), OPTION_BOOL, (char*)&roi, 0, string("extract roi"));
    //ot.addOption(string("--roi.radius"), OPTION_BOOL, (char*)&roiradius, 0, string("roi radius"));
    
    //ot.addOption(string(""), OPTION_SEPARATOR, NULL, 0, string("Microaneurysm"));
    //ot.addOption(string("--abr"), OPTION_BOOL, (char*)&abr, 0, string("Abramoff database descriptor test"));
    //ot.addOption(string("--extr"), OPTION_BOOL, (char*)&extr, 0, string("extract aneurysms"));
    
    ot.addOption(string(""), OPTION_SEPARATOR, NULL, 0, string("Vessel"));
    ot.addOption(string("--unknown"), OPTION_INT, (char*)&unknown, 1, string("unknown image"));
    ot.addOption(string("--vessel.stage0"), OPTION_BOOL, (char*)&vstage0, 0, string("vessel extraction stage 0: extracting roi and extanding the image content"));
    ot.addUsage(string(argv[0]) + string(" --vessel.stage0 <colorinput> <roi> <extended> <support>"));
    ot.addOption(string("--vessel.stage1"), OPTION_BOOL, (char*)&vstage1, 0, string("vessel extraction stage 1"));
    ot.addOption(string("--vessel.stage1.th1mult"), OPTION_FLOAT, (char*)&vstage1th1multiplier, 1, string("threshold 1 multiplier"));
    ot.addOption(string("--vessel.stage1.th2mult"), OPTION_FLOAT, (char*)&vstage1th2multiplier, 1, string("threshold 2 multiplier"));
    //ot.addOption(string("--vessel.stage1.mode0"), OPTION_INT, (char*)&vstage1mode0, 1, string("mode 0"));
    //ot.addOption(string("--vessel.stage1.mode1"), OPTION_INT, (char*)&vstage1mode1, 1, string("mode 1"));
//    ot.addOption(string("--vessel.stage1.thStart"), OPTION_INT, (char*)&vstage1thStart, 1, string("start index of thresholding"));
    ot.addOption(string("--vessel.stage1.imgScale"), OPTION_FLOAT, (char*)&vstage1imgscale, 1, string("image scale"));
    ot.addOption(string("--vessel.stage1.limit"), OPTION_FLOAT, (char*)&vstage1limit, 1, string("image scale limit"));
    ot.addOption(string("--vessel.stage1.roiradius"), OPTION_FLOAT, (char*)&vstage1roiradius, 1, string("ROI radius"));
    ot.addUsage(string(argv[0]) + string(" --vessel.stage1 <feature.fdf> <input> <roi> <support> <threshold.descr> <output>"));
    ot.addOption(string("--vessel.stage2"), OPTION_BOOL, (char*)&vstage2, 0, string("vessel extraction stage 2"));
    ot.addOption(string("--vessel.stage2.maxit"), OPTION_INT, (char*)&vstage2maxit, 1, string("maximum iteration"));
    ot.addOption(string("--vessel.stage2.nw"), OPTION_FLOAT, (char*)&vstage2nw, 1, string("neighborhood weight"));
    ot.addOption(string("--vessel.stage2.ws"), OPTION_FLOAT, (char*)&vstage2vs, 1, string("width scaling"));
    ot.addOption(string("--vessel.stage2.dynth"), OPTION_FLOAT, (char*)&vstage2dynth, 1, string("dynamic range threshold"));
    ot.addOption(string("--vessel.stage2.shift"), OPTION_FLOAT, (char*)&vstage2shift, 1, string("shifting of the measured surface"));
    ot.addOption(string("--vessel.stage2.relint"), OPTION_CHAR, (char*)&vstage2relint, 1, string("directory of relative intensities"));
    ot.addUsage(string(argv[0]) + string(" --vessel.stage2 <input> <seed> <roi> <output>"));
    ot.addOption(string("--vessel.stage4"), OPTION_BOOL, (char*)&vstage4, 0, string("vessel extraction stage 4"));
    //ot.addOption(string("--vessel.stage4.mp"), OPTION_FLOAT, (char*)&vstage4mp, 1, string("m parameter"));
    ot.addOption(string("--vessel.stage4.ap"), OPTION_FLOAT, (char*)&vstage4ap, 1, string("a parameter"));
    ot.addOption(string("--vessel.stage4.sizeth0"), OPTION_INT, (char*)&vstage4sizeth0, 1, string("size lower bound"));
//    ot.addOption(string("--vessel.stage4.sizeth1"), OPTION_INT, (char*)&vstage4sizeth1, 1, string("size upper bound"));
    ot.addOption(string("--vessel.stage4.ws"), OPTION_FLOAT, (char*)&vstage4ws, 1, string("vessel stage4 width scaling"));
    ot.addOption(string("--vessel.stage4.wa"), OPTION_FLOAT, (char*)&vstage4wa, 1, string("wavelength"));
    //ot.addOption(string("--vessel.stage4.wb"), OPTION_FLOAT, (char*)&vstage4wb, 1, string("upper wavelength bound"));
    ot.addUsage(string(argv[0]) + string(" --vessel.stage4 <input> <seed> <roi> <support> <output>"));
    /*ot.addOption(string("--vessel.stage5"), OPTION_BOOL, (char*)&vstage5, 0, string("vessel extraction stage 5"));
    ot.addOption(string("--vessel.stage5.tl"), OPTION_FLOAT, (char*)&vstage5tl, 1, string("translation lower limit"));
    ot.addOption(string("--vessel.stage5.tu"), OPTION_FLOAT, (char*)&vstage5tu, 1, string("translation upper limit"));
    ot.addOption(string("--vessel.stage5.ra"), OPTION_FLOAT, (char*)&vstage5ra, 1, string("rotation angle"));
    ot.addOption(string("--vessel.all"), OPTION_BOOL, (char*)&vesselall, 0, string("vessel all stages"));
    ot.addUsage(string(argv[0]) + string(" --vessel.all <feature.fdf> <input> <roi> <support> <threshold.descr> <output>"));*/
    //ot.addOption(string("--vessel.allcolor"), OPTION_BOOL, (char*)&vesselallcolor, 0, string("vessel all stages from color image"));
    //ot.addUsage(string(argv[0]) + string(" --vessel.allcolor <feature.fdf> <colorinput> <threshold.dscr> <output>"));
    ot.addOption(string("--vv"), OPTION_BOOL, (char*)&vv, 0, string("validate vessels"));
    ot.addOption(string("--vv2"), OPTION_BOOL, (char*)&vv2, 0, string("validate vessels 2"));
    //ot.addOption(string(""), OPTION_SEPARATOR, NULL, 0, string("Microaneurysm"));
    //ot.addOption(string("--ma0"), OPTION_BOOL, (char*)&ma0, 0, string("microaneurysm detection with matched Gaussian filters"));
    //ot.addOption(string("--checkMA"), OPTION_BOOL, (char*)&checkMA, 0, string("check MA results"));
    
    ot.addOption(string(""), OPTION_SEPARATOR, NULL, 0, string("Others"));
    ot.addOption(string("--calculate.measures"), OPTION_BOOL, (char*)&calculateMeasures, 0, string("calculate measures from acc, sens, spec and fg"));
    ot.addOption(string("--calculate.fg"), OPTION_BOOL, (char*)&calculateForeground, 0, string("calculate number of foreground pixels"));
    ot.addOption(string("--calculate.size"), OPTION_BOOL, (char*)&calculateSize, 0, string("calculate size of image"));
    //ot.addOption(string("--calculate.roc"), OPTION_BOOL, (char*)&calculateROC, 0, string("calculate ROC curve"));
    //ot.addOption(string("--calculate.roc.ch"), OPTION_BOOL, (char*)&calculateROCch, 0, string("calculate ROC by convex hull"));
    //ot.addOption(string("--pixeldiff"), OPTION_BOOL, (char*)&pixeldiff, 0, string("pixel difference"));
    ot.addOption(string("--scaling"), OPTION_BOOL, (char*)&scaling, 0, string("scaling"));
//    ot.addOption(string("--nohscaling"), OPTION_BOOL, (char*)&hscaling, 0, string("no histogram scaling"));
    ot.addOption(string("--gmean"), OPTION_BOOL, (char*)&geometricMean, 0, string("geometric mean"));
    ot.addOption(string("--gmean2"), OPTION_BOOL, (char*)&geometricMean2, 0, string("geometric mean 2 columns"));
    ot.addOption(string("--amean"), OPTION_BOOL, (char*)&arithmeticMean, 0, string("arithmetic mean"));
    //ot.addOption(string("--amean.cols"), OPTION_BOOL, (char*)&arithmeticMeanCols, 0, string("arithmetic mean for columns"));
    //ot.addOption(string("--ameanstd.cols"), OPTION_BOOL, (char*)&arithmeticMeanStdCols, 0, string("arithmetic mean and std for columns"));
    //ot.addOption(string("--bestparameters"), OPTION_BOOL, (char*)&bestparameters, 0, string("best parameters"));
    //ot.addOption(string("--extract.green"), OPTION_BOOL, (char*)&extractgreen, 0, string("extract green channel"));
    //ot.addOption(string("--smallestwavelength"), OPTION_BOOL, (char*)&smallestwavelength, 0, string("determine smallest wavelength"));
    //ot.addOption(string("--calculate.mean.ri"), OPTION_BOOL, (char*)&calculateMeanRI, 0, string("calculate mean relative intensity"));
    //ot.addOption(string("--visualize.roi.0"), OPTION_BOOL, (char*)&visroi0, 0, string("visualize roi 0"));
    //ot.addOption(string("--roc"), OPTION_BOOL, (char*)&roc, 0, string("disable lower thresholds for ROC"));
    //ot.addOption(string("--extract.ri"), OPTION_BOOL, (char*)&eri, 0, string("extract relative intensities"));
    //ot.addOption(string("--extract.ri.columns"), OPTION_BOOL, (char*)&ericolumns, 0, string("print in columns"));
    

    if ( ot.processArgs(&argc, argv) )
        return 1;

    vstage2nweight= vstage2nw;
    
    if ( geometricMean )
      return gmeanFunction(argc, argv);
    else if ( geometricMean2 )
      return gmean2Function(argc, argv);
    else if ( arithmeticMean )
      return ameanFunction(argc, argv);
    else if ( arithmeticMeanCols )
      return ameanColsFunction(argc, argv);
    else if ( arithmeticMeanStdCols )
      return ameanstdColsFunction(argc, argv);
    else if ( roi )
        return extractROI(argc, argv);
    else if ( vv )
        return validateVessel(argc, argv);
    else if ( vv2 )
        return validateVessel2(argc, argv);
    else if ( vstage0 )
      return vstage0Function(argc, argv);
    else if ( vstage1 )
      return vstage1Function(argc, argv, vstage1thStart, vstage1th1multiplier, vstage1th2multiplier, vstage1imgscale);
    else if ( vstage2 )
      return vstage2Function(argc, argv, vstage2maxit, vstage2nw, vstage2relint, vstage2vs);
    else if ( vstage4 )
      return vstage4Function(argc, argv, vstage4mp, vstage4ap, vstage4fp, vstage4cp, vstage4sizeth0, vstage4sizeth1, vstage4ws);
    else if ( vesselall )
      return vstageallFunction(argc, argv, vstage1thStart, vstage1th1multiplier, vstage1th2multiplier, vstage1imgscale, vstage2maxit, vstage2nw, vstage2relint, vstage2vs, vstage3th0, vstage3th1, vstage4mp, vstage4ap, vstage4fp, vstage4cp, vstage4sizeth0, vstage4sizeth1, vstage4ws);
    else if ( vesselallcolor )
      return vstageallcolorFunction(argc, argv, vstage1thStart, vstage1th1multiplier, vstage1th2multiplier, vstage1imgscale, vstage2maxit, vstage2nw, vstage2relint, vstage2vs, vstage3th0, vstage3th1, vstage4mp, vstage4ap, vstage4fp, vstage4cp, vstage4sizeth0, vstage4sizeth1, vstage4ws);
    else if ( calculateMeasures )
      return calculateMeasuresFunction(argc, argv);
    else if ( calculateForeground )
      return calculateForegroundFunction(argc, argv);
    else if ( calculateSize )
      return calculateSizeFunction(argc, argv);
    else if ( roiradius )
      return roiRadiusFunction(argc, argv);
    else if ( pixeldiff )
      return pixeldiffFunction(argc, argv);
    else if ( scaling )
      return scalingFunction(argc, argv);
    else if ( bestparameters )
      return bestparametersFunction(argc, argv);
    else if ( extractgreen )
      return extractgreenFunction(argc, argv);
    else if ( smallestwavelength )
      return smallestWavelengthFunction(argc, argv);
    else if ( calculateMeanRI )
      return calculateMeanRIFunction(argc, argv);
    else if ( visroi0 )
      return visualizeROI0Function(argc, argv);
    else if ( eri )
      return relativeIntensities(argc, argv, ericolumns);

    return 0;
}


