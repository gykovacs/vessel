#ifndef _VOLUME_REGION_OPERATIONS_H_
#define _VOLUME_REGION_OPERATIONS_H_

#include <openipDS/Volume.h>
#include <openipDS/FramedVolume.h>
#include <openipLL/ComponentLabeling.h>
#include <openipDS/StructuringElement3s.h>
#include <omp.h>
#include <climits>

namespace openip
{
  class SubtractAndBoundVolumes
  {
    public:
      SubtractAndBoundVolumes();

      SubtractAndBoundVolumes(const SubtractAndBoundVolumes& f);

      ~SubtractAndBoundVolumes();

      virtual void apply(Volume<unsigned char>& input1, Volume<unsigned char>& input2, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  template<typename T>
  class MinOperation
  {
    public:
      MinOperation();

      MinOperation(const MinOperation& f);

      ~MinOperation();

      virtual void apply(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  template<typename T>
  class MaxOperation
  {
    public:
      MaxOperation();

      MaxOperation(const MaxOperation& f);

      ~MaxOperation();

      virtual void apply(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  class FilterComponentsBySize
  {
    public:
      FilterComponentsBySize(int a, int b);

      FilterComponentsBySize(const FilterComponentsBySize& f);

      ~FilterComponentsBySize();

      virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

    int a;
    int b;
  };

  class KeepLargestComponent
  {
    public:
      KeepLargestComponent();

      KeepLargestComponent(const KeepLargestComponent& f);

      ~KeepLargestComponent();

      virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  class KeepNLargestComponent
  {
    public:
      KeepNLargestComponent(int n);

      KeepNLargestComponent(const KeepNLargestComponent& f);

      ~KeepNLargestComponent();

      virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      int n;
  };
  
  class CreateAxialMask
  {
  public:
    CreateAxialMask(int frontBorder= 0, int backBorder= 0, int begin= -1, int end= -1);
    
    CreateAxialMask(const CreateAxialMask& f);
    
    ~CreateAxialMask();
    
    void getFirstAndLastIndices(Volume<unsigned char>& input, int& first, int& last);
    
    void getFirstAndLastIndicesCompleteSearch(Volume<unsigned char>& input, int& first, int& last);
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Region3& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Region3Set& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int frontBorder;
    int backBorder;
    int begin;
    int end;
  };
  
  class CreateSaggitalMask
  {
  public:
    CreateSaggitalMask(int frontBorder= 0, int backBorder= 0, int begin= -1, int end= -1);
    
    CreateSaggitalMask(const CreateSaggitalMask& f);
    
    ~CreateSaggitalMask();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Region3& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Region3Set& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int frontBorder;
    int backBorder;
    int begin;
    int end;
  };
  
  class CreateCoronalMask
  {
  public:
    CreateCoronalMask(int frontBorder= 0, int backBorder= 0, int begin= -1, int end= -1);
    
    CreateCoronalMask(const CreateCoronalMask& f);
    
    ~CreateCoronalMask();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Region3& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Region3Set& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int frontBorder;
    int backBorder;
    int begin;
    int end;
  };
  
  class AxialCutAroundObject
  {
  public:
    AxialCutAroundObject(int radius0, int radius1);
    
    AxialCutAroundObject(const AxialCutAroundObject& f);
    
    ~AxialCutAroundObject();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& object, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius0;
    int radius1;
  };
  
  class ConnectClosestRegions
  {
  public:
    ConnectClosestRegions(float minDist, int resolution= 1000);
    
    ConnectClosestRegions(const ConnectClosestRegions& f);
    
    ~ConnectClosestRegions();
    
    void connectVoxels(int a, int b, Volume<unsigned char>& v);
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    float minDist;
    int resolution;
  };
  
  class SetLabelsInVolume
  {
  public:
    SetLabelsInVolume();
    
    SetLabelsInVolume(const SetLabelsInVolume& f);
    
    ~SetLabelsInVolume();
    
    virtual void apply(Volume<unsigned char>& input1, Volume<unsigned char>& input2, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  class ExtendLabelingInVolume
  {
  public:
    ExtendLabelingInVolume();
    
    ExtendLabelingInVolume(const ExtendLabelingInVolume& e);
    
    ~ExtendLabelingInVolume();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  class SetVoxelsToColor
  {
  public:
    SetVoxelsToColor(unsigned char label);
    
    SetVoxelsToColor(const SetVoxelsToColor& e);
    
    ~SetVoxelsToColor();
    
    virtual void apply(Volume<unsigned char>& input1, Volume<unsigned char>& input2, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    unsigned char label;
  };
  
  class AxialPropagationOfLabels
  {
  public:
    AxialPropagationOfLabels(int first, int last);
    
    AxialPropagationOfLabels(const AxialPropagationOfLabels& a);
    
    ~AxialPropagationOfLabels();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, unsigned char color, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int first;
    int last;
  };
  
  class GrowRegionUntilLargeWidth
  {
  public:
    GrowRegionUntilLargeWidth(float a);
    
    GrowRegionUntilLargeWidth(const GrowRegionUntilLargeWidth& a);
    
    ~GrowRegionUntilLargeWidth();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& object, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    float a;
  };
  
  class ConnectRegionsInVolume
  {
  public:
    ConnectRegionsInVolume(int resolution= 1000);
    
    ConnectRegionsInVolume(const ConnectRegionsInVolume& a);
    
    ~ConnectRegionsInVolume();
    
    virtual void connectVoxels(int a, int b, Volume<unsigned char>& v);
    
    virtual void connectPixels(int a, int b, Image<unsigned char>& img, int multiplier= 1);
    
    virtual void connectPixels(int r0, int c0, int r1, int c1, Image<unsigned char>& img, int multiplier= 1);
    
    virtual void connectVoxels(int s0, int r0, int c0, int s1, int r1, int c1, Volume<unsigned char>& v);
    
    virtual void apply(Region3& input1, Region3& input2, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int resolution;
  };
  
  class BandThreshold
  {
  public:
    BandThreshold(float a, float b);
    
    BandThreshold(const BandThreshold& a);
    
    ~BandThreshold();
    
    virtual void apply(Volume<float>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    float a;
    float b;
  };
  
  class SimpleThreshold
  {
  public:
    SimpleThreshold(float a);
    
    SimpleThreshold(const SimpleThreshold& a);
    
    ~SimpleThreshold();
    
    virtual void apply(Volume<float>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    float a;
  };
  
  class ConvexHullInVolume
  {
  public:
    ConvexHullInVolume();
    
    ConvexHullInVolume(const ConvexHullInVolume& c);
    
    ~ConvexHullInVolume();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  
  class InnerAxialDarkReconstruction
  {
  public:
    InnerAxialDarkReconstruction(int r);
    
    InnerAxialDarkReconstruction(const InnerAxialDarkReconstruction& c);
    
    ~InnerAxialDarkReconstruction();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
  };
  
  class OuterErodeAndReconstruct
  {
  public:
    OuterErodeAndReconstruct(int r);
    
    OuterErodeAndReconstruct(const OuterErodeAndReconstruct& o);
    
    ~OuterErodeAndReconstruct();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
  };
  
  class AxialAxialCSReconstruct
  {
  public:
    AxialAxialCSReconstruct(int r1, int r2);
    
    AxialAxialCSReconstruct(const AxialAxialCSReconstruct& o);
    
    ~AxialAxialCSReconstruct();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius1;
    int radius2;
  };
  
  class AxialLargestAxialReconstruct
  {
  public:
    AxialLargestAxialReconstruct(int r1);
    
    AxialLargestAxialReconstruct(const AxialLargestAxialReconstruct& o);
    
    ~AxialLargestAxialReconstruct();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius1;
  };
  
  class CSLargestCSReconstruct
  {
  public:
    CSLargestCSReconstruct(int r1, int plus= 0);
    
    CSLargestCSReconstruct(const CSLargestCSReconstruct& o);
    
    ~CSLargestCSReconstruct();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius1;
    int plus;
  };
  
  class AxialCoherence
  {
  public:
    AxialCoherence(int r);
    
    AxialCoherence(const AxialCoherence& a);
    
    ~AxialCoherence();
    
    virtual void apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
  };
  
  class AxialCoherenceBorder
  {
  public:
    AxialCoherenceBorder(int r, int reverse= 0);
    
    AxialCoherenceBorder(const AxialCoherenceBorder& a);
    
    ~AxialCoherenceBorder();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
    int reverse;
  };
  
  class SaggitalCoherence
  {
  public:
    SaggitalCoherence(int r);
    
    SaggitalCoherence(const SaggitalCoherence& a);
    
    ~SaggitalCoherence();
    
    virtual void apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
  };
  
  class SaggitalCoherenceBorder
  {
  public:
    SaggitalCoherenceBorder(int r, int reverse= 0);
    
    SaggitalCoherenceBorder(const SaggitalCoherenceBorder& a);
    
    ~SaggitalCoherenceBorder();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
    int reverse;
  };
  
  class CoronalCoherence
  {
  public:
    CoronalCoherence(int r);
    
    CoronalCoherence(const CoronalCoherence& a);
    
    ~CoronalCoherence();
    
    virtual void apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
  };
  
  class CoronalCoherenceBorder
  {
  public:
    CoronalCoherenceBorder(int r, int reverse= 0);
    
    CoronalCoherenceBorder(const CoronalCoherenceBorder& a);
    
    ~CoronalCoherenceBorder();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
    int reverse;
  };
  
  class SuccessiveAxialCoherence
  {
  public:
    SuccessiveAxialCoherence(int r, int periods);
    
    SuccessiveAxialCoherence(const SuccessiveAxialCoherence& s);
    
    ~SuccessiveAxialCoherence();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& mask, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int r;
    int periods;
  };
  
  class CircularCoherence
  {
  public:
    CircularCoherence(int r);
    
    CircularCoherence(const CircularCoherence& c);
    
    ~CircularCoherence();
    
    virtual void apply(Image<unsigned char>& input, int r, int c, Image<unsigned char>& output);
    
    int r;
  };
  
  template<typename T>
  MinOperation<T>::MinOperation()
  {
  }

  template<typename T>
  MinOperation<T>::MinOperation(const MinOperation& f)
  {
  }

  template<typename T>
  MinOperation<T>::~MinOperation()
  {
  }

  template<typename T>
  void MinOperation<T>::apply(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < input1.n; ++i )
      if ( !roi || (*roi)(i) )
      {
	if ( input1(i) && input2(i) )
	  output(i)= 1;
	else
	  output(i)= 0;
      }
      else
	output(i)= input1(i);
  }
  
  
  
  template<typename T>
  MaxOperation<T>::MaxOperation()
  {
  }

  template<typename T>
  MaxOperation<T>::MaxOperation(const MaxOperation& f)
  {
  }

  template<typename T>
  MaxOperation<T>::~MaxOperation()
  {
  }

  template<typename T>
  void MaxOperation<T>::apply(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < input1.n; ++i )
      if ( !roi || (*roi)(i) )
      {
	if ( input1(i) || input2(i) )
	  output(i)= 1;
	else
	  output(i)= 0;
      }
      else
	output(i)= input1(i);
  }
  
  template<typename T>
  class RemoveRegionsNotConnectedToARegion
  {
    public:
      RemoveRegionsNotConnectedToARegion();

      RemoveRegionsNotConnectedToARegion(const RemoveRegionsNotConnectedToARegion& f);

      ~RemoveRegionsNotConnectedToARegion();

      virtual void apply(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
  };
  
  template<typename T>
  RemoveRegionsNotConnectedToARegion<T>::RemoveRegionsNotConnectedToARegion()
  {
  }
  
  template<typename T>
  RemoveRegionsNotConnectedToARegion<T>::RemoveRegionsNotConnectedToARegion(const RemoveRegionsNotConnectedToARegion& r)
  {
  }
  
  template<typename T>
  RemoveRegionsNotConnectedToARegion<T>::~RemoveRegionsNotConnectedToARegion()
  {
  }
  
  template<typename T>
  void RemoveRegionsNotConnectedToARegion<T>::apply(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    tprintf("remove regions not connected to a region\n");
    CreateAxialMask cam;
    Volume<unsigned char> axialmask;
    axialmask.resizeVolume(input1);
    cam.apply(input2, axialmask);
    
    for ( unsigned int i= 0; i < input1.slices; ++i )
    {
      if ( axialmask(i,0,0) )
      {
	Image<unsigned char> slice;
	input1.getImage(slice, 0, i);
	ExtractRegions er;
	Region2Set regions;
	er.apply(slice, regions);
	
	Image<unsigned char> bb;
	input2.getImage(bb, 0, i);
	Region2Set backbone;
	er.apply(bb, backbone);
	Region2 backb= backbone.unifyToOneRegion();
	
	Region2Set intersecting= regions.getIntersectingRegions(backb);
	
	slice= 0;
	for ( unsigned int j= 0; j < intersecting.size(); ++j )
	  intersecting(j).set(slice, 0, (unsigned char)1);
	output.setImage(slice, 0, i);
      }
      else
      {
	Image<float> slice;
	input1.getImage(slice, 0, i);
	output.setImage(slice, 0, i);
      }
    }
    tprintf("remove regions not connected to a region finished\n");
  }
  
  template<typename T>
  class SmoothAxialConnectivity
  {
    public:
      SmoothAxialConnectivity(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold);

      SmoothAxialConnectivity(const SmoothAxialConnectivity& f);

      virtual ~SmoothAxialConnectivity();

      virtual void apply(Volume<T>& input1, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual void apply2(Volume<T>& input1, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual void apply3(Volume<T>& input1, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual Border3 getProposedBorder();
      
      virtual int getMin();
      
      virtual int getMax();
      
      virtual void makeStructuringElementsUnique();
      
      Vector<StructuringElementLineSegment> vsels;
      Vector<StructuringElementLineSegment> vsels2;
      Vector<StructuringElementLineSegment> vselsp;
      Vector<StructuringElementLineSegment> vselsm;
      float threshold;
      float overallThreshold;
      int changed;
      int length;
      
      Vector<Vector<StructuringElementLineSegment*> > neighbormap;
  };
  
  template<typename T>
  SmoothAxialConnectivity<T>::SmoothAxialConnectivity(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold)
  {
    for ( float a= alpha0; a <= alpha1; a+= alphastep )
      for ( float b= beta0; b <= beta1; b+= betastep )
      {
	vsels.push_back(StructuringElementLineSegment(length, a, b));
	vsels.push_back(StructuringElementLineSegment(-length, a, b));
      }
      
    /*for ( int l= length; l > 0; l-= 5 )
    {
      for ( float a= alpha0; a <= alpha1; a+= alphastep )
	for ( float b= beta0; b <= beta1; b+= betastep )
	{
	  vsels2.push_back(StructuringElementLineSegment(l, a, b));
	  vsels2.push_back(StructuringElementLineSegment(-l, a, b));
	}
    }*/
    
    
    for ( int l= length; l > 0; l-= length/10 )
    {
      for ( float a= alpha0; a <= alpha1; a+= alphastep )
	for ( float b= beta0; b <= beta1; b+= betastep )
	{
	  vselsp.push_back(StructuringElementLineSegment(l, a, b));
	}
    }
    
    for ( int l= length; l > 0; l-= length/10 )
    {
      for ( float a= alpha0; a <= alpha1; a+= alphastep )
	for ( float b= beta0; b <= beta1; b+= betastep )
	{
	  vselsm.push_back(StructuringElementLineSegment(-l, a, b));
	}
    }
      
    makeStructuringElementsUnique();
    
    this->threshold= threshold;
    this->overallThreshold= overallThreshold;
    this->changed= 1;
    this->length= length;
    
    tprintf("number of structuring elements: %d\n", vsels.size());
  }

  template<typename T>
  void SmoothAxialConnectivity<T>::makeStructuringElementsUnique()
  {
    tprintf("before unique: %d %d\n", vsels.size(), vselsm.size());
    for ( unsigned int i= 0; i < vsels.size(); ++i )
    {
      for ( unsigned int j= i+1; j < vsels.size(); ++j )
      {
	//int n= 0;
        unsigned int k;
	for ( k= 0; k < vsels(i).size() && k < vsels(j).size(); ++k )
	  if ( vsels(i)(k) != vsels(j)(k) )
	    break;
	if ( vsels(i).size() == vsels(j).size() && k == vsels(i).size() )
	  vsels.erase(vsels.begin()+j);
      }
    }
    
    /*for ( int i= 0; i < vsels2.size(); ++i )
    {
      for ( int j= i+1; j < vsels2.size(); ++j )
      {
	int n= 0, k;
	for ( k= 0; k < vsels2(i).size() && k < vsels2(j).size(); ++k )
	  if ( vsels2(i)(k) != vsels2(j)(k) )
	    break;
	if ( vsels2(i).size() == vsels2(j).size() && k == vsels2(i).size() )
	  vsels2.erase(vsels2.begin()+j);
      }
    }*/
    
    tprintf("a\n");
    for ( unsigned int i= 0; i < vselsm.size(); ++i )
    {
      for ( unsigned int j= i+1; j < vselsm.size(); ++j )
      {
	//int n= 0;
        unsigned int k;
	for ( k= 0; k < vselsm(i).size() && k < vselsm(j).size(); ++k )
	  if ( vselsm(i)(k) != vselsm(j)(k) )
	    break;
	if ( vselsm(i).size() == vselsm(j).size() && k == vselsm(i).size() )
	  vselsm.erase(vselsm.begin()+j);
      }
    }
    
    tprintf("b\n");
    for ( unsigned int i= 0; i < vselsp.size(); ++i )
    {
      for ( unsigned int j= i+1; j < vselsp.size(); ++j )
      {
	//int n= 0;
        unsigned int k;
	for ( k= 0; k < vselsp(i).size() && k < vselsp(j).size(); ++k )
	  if ( vselsp(i)(k) != vselsp(j)(k) )
	    break;
	if ( vselsp(i).size() == vselsp(j).size() && k == vselsp(i).size() )
	  vselsp.erase(vselsp.begin()+j);
      }
    }
    tprintf("after unique: %d %d %d %d\n", vsels.size(), vsels2.size(), vselsm.size(), vselsp.size());
  }
  
  template<typename T>
  SmoothAxialConnectivity<T>::SmoothAxialConnectivity(const SmoothAxialConnectivity& f)
  {
    this->vsels= f.vsels;
    this->vsels2= f.vsels2;
    this->vselsm= f.vselsm;
    this->vselsp= f.vselsp;
    this->threshold= f.threshold;
    this->overallThreshold= f.overallThreshold;
  }

  template<typename T>
  SmoothAxialConnectivity<T>::~SmoothAxialConnectivity()
  {
  }

  template<typename T>
  Border3 SmoothAxialConnectivity<T>::getProposedBorder()
  {
    Border3 bmax;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
    {
      Border3 b= vsels(i).getProposedBorder();
      bmax= maximizeBorders(bmax, b);
    }
    
    return bmax;
  }
  
  template<typename T>
  int SmoothAxialConnectivity<T>::getMin()
  {
    int min= INT_MAX;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      if ( vsels(i).getMin() < min )
	min= vsels(i).getMin();
    return min;
  }
  
  template<typename T>
  int SmoothAxialConnectivity<T>::getMax()
  {
    int max= -INT_MAX;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      if ( vsels(i).getMax() > max )
	max= vsels(i).getMax();
    return max;
  }
  
  bool pairCompare(const std::pair<int, int>& firstElem, const std::pair<int, int>& secondElem);
  
  template<typename T>
  void SmoothAxialConnectivity<T>::apply(Volume<T>& input, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      vsels(i).updateStride(input.columns, input.sliceSize);
    
    output= 0;
    
    //int j;
    int min= getMin(), max= getMax();
    changed= 0;
    //int lastIdx= 0;

    int changed= 0;
    int total= 0;
    int processed= 0;
    
    for ( int i= -min; i < int(input.n) - max; ++i )
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) )
	++total;
    
    #pragma omp parallel for schedule(dynamic, 10000)
    for ( int i= -min; i < int(input.n)-max; ++i )
    {
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) )
      {
	unsigned int j;
	int ok= 0;
	for ( j= 0; j < vsels.size(); ++j )
	{
	  int k;
	  int n= 0;
	  int size= vsels(j).size();
	  for ( k= 0; k < size; ++k )
	  {
	    n+= input(i + vsels(j)(k));
	    
	    if ( k - n > size*(1-threshold) )
	      break;
	  }
	  
	  if ( k == size && overallThreshold > 0 && n > size*threshold )
	    ++ok;
	  else if ( k == size && overallThreshold <= 0 && n > size*threshold )
	  {
	    output(i)= 1;
	    break;
	  }
	}
	
	if ( (j == vsels.size() && overallThreshold <= 0) || (overallThreshold > 0 && ok <= int(vsels.size() * overallThreshold)) )
	{
	  output(i)= 0;
	  #pragma omp critical
	  {
	    changed++;
	  }
	}
	else
	  output(i)= 1;
	
	#pragma omp critical
	  ++processed;
      }
      else if ( roi && !(*roi)(i) )
      {
	output(i)= input(i);
      }
      
      int t= omp_get_thread_num();
      if ( t == 0 && processed % 100 == 0)
	printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r%8d/%8d", processed, total);
	
    }

    this->changed= changed;
  }
  
  template<typename T>
  void SmoothAxialConnectivity<T>::apply2(Volume<T>& input, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      vsels2(i).updateStride(input.columns, input.sliceSize);
    
    output= 0;
    
    //int j;
    int min= getMin(), max= getMax();
    changed= 0;
    //int lastIdx= 0;

    int changed= 0;
    int total= 0;
    int processed= 0;
    
    for ( int i= -min; i < int(input.n) - max; ++i )
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) )
	++total;
    
    #pragma omp parallel for schedule(dynamic, 10000)
    for ( int i= -min; i < int(input.n)-max; ++i )
    {
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) )
      {
	unsigned int j;
	//int ok= 0;
	for ( j= 0; j < vsels2.size(); ++j )
	{
	  int k;
	  int n= 0, m= 0;
	  int size= vsels2(j).size();
	  for ( k= 0; k < size; ++k )
	  {
	    n+= input(i + vsels2(j)(k));
	    m+= input2(i + vsels2(j)(k));
	    
	    if ( k - n > size*(1-threshold) )
	      break;
	  }
	  
	  if ( k == size && overallThreshold <= 0 && n > size*threshold && m > 0 )
	  {
	    for ( unsigned int l= 0; l < vsels2(j).size(); ++l )
	      if ( input(i + vsels2(j)(l)) )
		output(i + vsels2(j)(l))= 1;
	  }
	}
	
	/*if ( (j == vsels.size() && overallThreshold <= 0) )
	{
	  //output(i)= 0;
	  #pragma omp critical
	  {
	    changed++;
	  }
	}
	else
	  output(i)= 1;*/
	#pragma omp critical
	  ++processed;
      }
      else if ( roi && !(*roi)(i) )
      {
	output(i)= input(i);
      }
      
      int t= omp_get_thread_num();
      if ( t == 0 && processed % 100 == 0)
	printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r%8d/%8d", processed, total);
	

    }

    for ( unsigned int j= 0; j < input.n; ++j )
      if ( input(j) != output(j) )
	++changed;
    
    this->changed= changed;
  }
  
  template<typename T>
  void SmoothAxialConnectivity<T>::apply3(Volume<T>& input, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      vsels(i).updateStride(input.columns, input.sliceSize);
    for ( unsigned int i= 0; i < vselsm.size(); ++i )
      vselsm(i).updateStride(input.columns, input.sliceSize);
    for ( unsigned int i= 0; i < vselsp.size(); ++i )
      vselsp(i).updateStride(input.columns, input.sliceSize);
    
    output= 0;
    
    //int j;
    int min= getMin(), max= getMax();
    changed= 0;
    //int lastIdx= 0;

    int changed= 0;
    int total= 0;
    int processed= 0;
    
    for ( int i= -min; i < int(input.n) - max; ++i )
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) )
	++total;
    
    #pragma omp parallel for schedule(dynamic, 10000)
    for ( int i= -min; i < int(input.n)-max; ++i )
    {
      int longestm= 0;
      int longestp= 0;
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) )
      {
	unsigned int j;
	//int ok= 0;
	
	for ( j= 0; j < vselsm.size(); ++j )
	{
	  if ( int(vselsm(j).size()) > longestm )
	  {
	    int k;
	    int n= 0;
	    int size= vselsm(j).size();
	    for ( k= 0; k < size; ++k )
	    {
	      n+= input(i + vselsm(j)(k));
	      
	      if ( k - n > size*(1-threshold) )
		break;
	    }
	    
	    if ( k == size && int(vselsm(j).size()) > longestm )
	      longestm= vselsm(j).size();
	  }
	}
	
	
	for ( j= 0; j < vselsp.size(); ++j )
	{
	  if ( int(vselsp(j).size()) > longestp )
	  {
	    int k;
	    int n= 0;
	    int size= vselsp(j).size();
	    for ( k= 0; k < size; ++k )
	    {
	      n+= input(i + vselsp(j)(k));
	      
	      if ( k - n > size*(1-threshold) )
		break;
	    }
	    
	    if ( k == size && int(vselsp(j).size()) > longestp )
	      longestp= vselsp(j).size();
	  }
	}
	
	if ( longestp + longestm > length )
	{
	  output(i)= 1;
	}
	else
	{
	  output(i)= 0;
	  #pragma omp critical
	  {
	    changed++;
	    ++processed;
	  }
	}
      }
      else if ( roi && !(*roi)(i) )
      {
	output(i)= input(i);
      }
      
      int t= omp_get_thread_num();
      if ( t == 0 && processed % 100 == 0)
	printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r%8d/%8d", processed, total);
    }

    this->changed= changed;
  }
  
  template<typename T>
  class IteratedSmoothAxialConnectivity
  {
    public:
      IteratedSmoothAxialConnectivity(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold, int iterations);

      IteratedSmoothAxialConnectivity(const IteratedSmoothAxialConnectivity& f);

      ~IteratedSmoothAxialConnectivity();

      virtual void apply(Volume<T>& input, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual void apply2(Volume<T>& input, Volume<T>& input2,  Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual void apply3(Volume<T>& input, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual Border3 getProposedBorder();
      
      SmoothAxialConnectivity<T>* sac;
      int iterations;
  };
  
  template<typename T>
  IteratedSmoothAxialConnectivity<T>::IteratedSmoothAxialConnectivity(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold, int iterations)
  {
    sac= new SmoothAxialConnectivity<T>(alpha0, alphastep, alpha1, beta0, betastep, beta1, length, threshold, overallThreshold);
    this->iterations= iterations;
  }

  template<typename T>
  IteratedSmoothAxialConnectivity<T>::IteratedSmoothAxialConnectivity(const IteratedSmoothAxialConnectivity& f)
  {
    sac= new SmoothAxialConnectivity<T>(*(f->sac));
    this->iterations= f.iterations;
  }

  template<typename T>
  IteratedSmoothAxialConnectivity<T>::~IteratedSmoothAxialConnectivity()
  {
    delete sac;
  }

  template<typename T>
  Border3 IteratedSmoothAxialConnectivity<T>::getProposedBorder()
  {
    return sac->getProposedBorder();
  }
  
  template<typename T>
  void IteratedSmoothAxialConnectivity<T>::apply(Volume<T>& input, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    tprintf("iterations: %d\n", iterations);
    Volume<T> tmp;
    tmp.resizeVolume(input);
    
    tmp= input;
    int total= 0;
    
    if ( iterations == -1 )
    {
      while ( sac->changed != 0 )
      {
	sac->apply(tmp, output, roi);
	tprintf("changed/total : %d/%d\n", sac->changed, total);
	total+= sac->changed;
	
	if ( sac->changed == 0 )
	{
	  tprintf("total number of voxels removed: %d\n", total);
	  return;
	}
	
	sac->apply(output, tmp, roi);
	tprintf("changed/total: %d/%d\n", sac->changed, total);
	total+= sac->changed;
      }
      output= tmp;
    }
    else
    {
      for ( int i= 0; i < iterations; ++i )
      {
	sac->apply(tmp, output);
	total+= sac->changed;
	if ( i == iterations - 1 )
	  return;
	sac->apply(output, tmp);
	++i;
      }
      output= tmp;
    }
    
    tprintf("total number of voxels removed: %d\n", total);
  }
  
  template<typename T>
  void IteratedSmoothAxialConnectivity<T>::apply2(Volume<T>& input, Volume<T>& input2,  Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    tprintf("iterations: %d\n", iterations);
    Volume<T> tmp;
    tmp.resizeVolume(input);
    
    tmp= input;
    int total= 0;
    
    if ( iterations == -1 )
    {
      while ( sac->changed != 0 )
      {
	sac->apply2(tmp, input2, output, roi);
	tprintf("changed/total : %d/%d\n", sac->changed, total);
	total+= sac->changed;
	
	if ( sac->changed == 0 )
	{
	  tprintf("total number of voxels removed: %d\n", total);
	  return;
	}
	
	sac->apply2(output, input2, tmp, roi);
	tprintf("changed/total: %d/%d\n", sac->changed, total);
	total+= sac->changed;
      }
      output= tmp;
    }
    else
    {
      for ( int i= 0; i < iterations; ++i )
      {
	sac->apply2(tmp, input2, output);
	total+= sac->changed;
	if ( i == iterations - 1 )
	  return;
	sac->apply2(output, input2, tmp);
	++i;
      }
      output= tmp;
    }
    
    tprintf("total number of voxels removed: %d\n", total);
  }
  
  template<typename T>
  void IteratedSmoothAxialConnectivity<T>::apply3(Volume<T>& input, Volume<T>& input2, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    tprintf("iterations: %d\n", iterations);
    Volume<T> tmp;
    tmp.resizeVolume(input);
    
    tmp= input;
    int total= 0;

    RemoveRegionsNotConnectedToARegion<unsigned char> rrnctar;    
    if ( iterations == -1 )
    {
      while ( sac->changed != 0 )
      {
	sac->apply3(tmp, output, roi);
	tmp= output;
	rrnctar.apply(tmp, input2, output);
	tprintf("changed/total : %d/%d\n", sac->changed, total);
	total+= sac->changed;
	
	if ( sac->changed == 0 )
	{
	  tprintf("total number of voxels removed: %d\n", total);
	  return;
	}
	
	sac->apply3(output, tmp, roi);
	rrnctar.apply(tmp, input2, output);
	tmp= output;
	tprintf("changed/total: %d/%d\n", sac->changed, total);
	total+= sac->changed;
      }
      output= tmp;
    }
    else
    {
      for ( int i= 0; i < iterations; ++i )
      {
	sac->apply3(tmp, output);
	tmp= output;
	rrnctar.apply(tmp, input2, output);
	total+= sac->changed;
	if ( i == iterations - 1 )
	  return;
	sac->apply3(output, tmp);
	rrnctar.apply(tmp, input2, output);
	++i;
      }
      output= tmp;
    }
    
    tprintf("total number of voxels removed: %d\n", total);
  }
  
  
  template<typename T>
  class RegionCleaningOperator1
  {
    public:
      RegionCleaningOperator1(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold);

      RegionCleaningOperator1(const RegionCleaningOperator1& f);

      ~RegionCleaningOperator1();

      virtual void apply(Volume<T>& input, Volume<T>& tokeep, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual Border3 getProposedBorder();
      
      void makeStructuringElementsUnique();
      
      int getMin();
      
      int getMax();
      
      float alpha0;
      float alphastep;
      float alpha1;
      float beta0;
      float betastep;
      float beta1;
      float length;
      float threshold;
      float overallThreshold;
      
      Vector<StructuringElementLineSegment> vsels;
      Vector<int> alphas;
      Vector<float> betas;
      Vector<float> lengths;
      Vector<Vector<int> > covered;
      
      int numAlphas;
      int changed;
      int originalNumber;
      int reducedNumber;
      
      StructuringElementCS prefilter;
  };
  
  template<typename T>
  RegionCleaningOperator1<T>::RegionCleaningOperator1(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold)
  {
    alpha0= alpha0/180*M_PI;
    alphastep= alphastep/180*M_PI;
    alpha1= alpha1/180*M_PI;
    beta0= beta0/180*M_PI;
    betastep= betastep/180*M_PI;
    beta1= beta1/180*M_PI;
    
    this->alpha0= alpha0;
    this->alphastep= alphastep;
    this->alpha1= alpha1;
    this->beta0= beta0;
    this->betastep= betastep;
    this->beta1= beta1;
    this->threshold= threshold;
    this->overallThreshold= overallThreshold;
    this->changed= 1;
    this->length= length;
    
    prefilter= StructuringElementCS(length);
    
    char cache[1000];
    sprintf(cache, "../rco1_%f_%f_%f_%f_%f_%f_%f.cache", alpha0, alphastep, alpha1, beta0, betastep, beta1, length);

    FILE* f;
    f= fopen(cache, "rt");
    if ( f == NULL )
    {
      reducedNumber= originalNumber= 1;
      
      numAlphas= 0;
      int totalAlphas= (alpha1 - alpha0)/alphastep;
      for ( float a= alpha0; a <= alpha1; a+= alphastep )
      {
	for ( float b= beta0; b <= beta1; b+= betastep )
	{
	  vsels.push_back(StructuringElementLineSegment(length, a, b));
	  vsels.push_back(StructuringElementLineSegment(-length, a, b));
	  alphas.push_back(numAlphas);
	  alphas.push_back((numAlphas + totalAlphas/2)%totalAlphas);
	  betas.push_back(b);
	  betas.push_back(b);
	  lengths.push_back(length);
	  lengths.push_back(-length);
	  Vector<int> tmp;
	  tmp.push_back(numAlphas);
	  covered.push_back(tmp);
	  tmp.clear();
	  tmp.push_back((numAlphas + totalAlphas/2)%totalAlphas);
	  covered.push_back(tmp);
	}
	
	numAlphas++;
      }
      
      tprintf("totalAlphas, numAlphas, covered.size(): %d,%d,%d\n", totalAlphas, numAlphas, covered.size()); 
      
      makeStructuringElementsUnique();
      
      tprintf("number of structuring elements: %d\n", vsels.size());
      
      tprintf("writing cache file\n");
      
      f= fopen(cache, "wt");
      fprintf(f, "%d\n", numAlphas);
      fprintf(f, "%d\n", originalNumber);
      fprintf(f, "%d\n", reducedNumber);
      fprintf(f, "%zd\n", alphas.size());
      for ( unsigned int i= 0; i < alphas.size(); ++i )
	fprintf(f, "%d ", alphas(i));
      fprintf(f, "\n");
      fprintf(f, "%zd\n", betas.size());
      for ( unsigned int i= 0; i < betas.size(); ++i )
	fprintf(f, "%f ", betas(i));
      fprintf(f, "\n");
      fprintf(f, "%zd\n", lengths.size());
      for ( unsigned int i= 0; i < lengths.size(); ++i )
	fprintf(f, "%f ", lengths(i));
      fprintf(f, "\n");
      fprintf(f, "%zd\n", covered.size());
      for ( unsigned int i= 0; i < covered.size(); ++i )
      {
	fprintf(f, "%zd\n", covered(i).size());
	for ( unsigned int j= 0; j < covered(i).size(); ++j )
	  fprintf(f, "%d ", covered(i)(j));
	fprintf(f, "\n");
      }
      fprintf(f, "%zd\n", vsels.size());
      for ( unsigned int i= 0; i < vsels.size(); ++i )
      {
	fprintf(f, "%zd %d %d\n", vsels(i).size(), vsels(i).stride, vsels(i).slicePitch);
	for ( unsigned int j= 0; j < vsels(i).size(); ++j )
	  fprintf(f, "%d ", vsels(i)(j));
	fprintf(f, "\n");
      }
      fclose(f);
      tprintf("structuring elements written\n");
    }
    else
    {
      tprintf("reading structuring elements\n");
      int ret= 0;
      ret= fscanf(f, "%d", &numAlphas); if ( !ret ) printf("error in fscanf\n");
      ret= fscanf(f, "%d", &originalNumber); if ( !ret ) printf("error in fscanf\n");
      ret= fscanf(f, "%d", &reducedNumber); if ( !ret ) printf("error in fscanf\n");
      int nalphas;
      ret= fscanf(f, "%d", &nalphas); if ( !ret ) printf("error in fscanf\n");
      alphas.resize(nalphas);
      for ( int i= 0; i < nalphas; ++i )
      {
	ret= fscanf(f, "%d", &(alphas(i))); if ( !ret ) printf("error in fscanf\n");
      }
      int nbetas;
      ret= fscanf(f, "%d", &nbetas); if ( !ret ) printf("error in fscanf\n");
      betas.resize(nbetas);
      for ( int i= 0; i < nbetas; ++i )
      {
	ret= fscanf(f, "%f", &(betas(i))); if ( !ret ) printf("error in fscanf\n");
      }
      int nlengths;
      ret= fscanf(f, "%d", &nlengths); if ( !ret ) printf("error in fscanf\n");
      lengths.resize(nlengths);
      for ( int i= 0; i < nlengths; ++i )
      {
	ret= fscanf(f, "%f", &(lengths(i))); if ( !ret ) printf("error in fscanf\n");
      }
      int ncovered;
      ret= fscanf(f, "%d", &ncovered); if ( !ret ) printf("error in fscanf\n");
      covered.resize(ncovered);
      for ( int i= 0; i < ncovered; ++i )
      {
	int ncoveredi;
	ret= fscanf(f, "%d", &ncoveredi); if ( !ret ) printf("error in fscanf\n");
	covered(i).resize(ncoveredi);
	for ( int j= 0; j < ncoveredi; ++j )
        {
	  ret= fscanf(f, "%d", &(covered(i)(j))); if ( !ret ) printf("error in fscanf\n");
        }
      }
      int nvsels;
      ret= fscanf(f, "%d", &nvsels); if ( !ret ) printf("error in fscanf\n");
      for ( int i= 0; i < nvsels; ++i )
      {
	int n, stride, slicePitch;
	ret= fscanf(f, "%d%d%d", &n, &stride, &slicePitch); if ( !ret ) printf("error in fscanf\n");
	Vector<int> tmp;
	tmp.resize(n);
	for ( int j= 0; j < n; ++j )
        {
	  ret= fscanf(f, "%d", &(tmp(j))); if ( !ret ) printf("error in fscanf\n");
        }
	vsels.push_back(StructuringElementLineSegment(tmp, stride, slicePitch));
      }
      fclose(f);
      tprintf("structuring elements read\n");
    }
  }

  template<typename T>
  void RegionCleaningOperator1<T>::makeStructuringElementsUnique()
  {
    originalNumber= vsels.size();
    tprintf("before unique: %d\n", vsels.size());
    
    Vector<int> toremove;
    int m= 0;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
    {
      toremove.clear();
      //#pragma omp parallel for num_threads(2), schedule(dynamic, 100)
      for ( unsigned int j= i+1; j < vsels.size(); ++j )
      {
	if ( vsels(i).size() == vsels(j).size() && abs(alphas(i) - alphas(j)) < 20 && vsels(i)(vsels(i).size()-1) == vsels(j)(vsels(j).size()-1) )
	{
	  ++m;
	  int k;
	  for ( k= vsels(i).size()-1; k >= 0; --k )
	    if ( vsels(i)(k) != vsels(j)(k) )
	      break;
	  if ( k == -1 )
	  {
	    //#pragma omp critical
	    {
	      toremove.push_back(j);
	    }
	  }
	}
      }
      
      sort(toremove.begin(), toremove.end());
      
      for ( int j= toremove.size() - 1; j >= 0; --j )
      {
	covered(i).push_back(alphas(toremove(j)));
	covered.erase(covered.begin()+toremove(j));
	vsels.erase(vsels.begin()+toremove(j));
	alphas.erase(alphas.begin()+toremove(j));
	betas.erase(betas.begin()+toremove(j));
	lengths.erase(lengths.begin()+toremove(j));
      }
    }
    
    tprintf("making unique covered map, m: %d\n", m);
    
    for ( unsigned int l= 0; l < covered.size(); ++l )
    {
      sort(covered(l).begin(), covered(l).end());
      Vector<int>::iterator it;
      it= std::unique(covered(l).begin(), covered(l).end());
      covered(l).erase(it, covered(l).end());
    }
    
    reducedNumber= vsels.size();
    tprintf("after unique: %d\n", vsels.size());
  }
  
  template<typename T>
  RegionCleaningOperator1<T>::RegionCleaningOperator1(const RegionCleaningOperator1& f)
  {
    this->vsels= f.vsels;
    this->alpha0= f.alpha0;
    this->alphastep= f.alphastep;
    this->alpha1= f.alpha1;
    this->beta0= f.beta0;
    this->betastep= f.betastep;
    this->beta1= f.beta1;
    this->threshold= f.threshold;
    this->overallThreshold= f.overallThreshold;
    this->changed= 1;
    this->length= f.length;
  }

  template<typename T>
  RegionCleaningOperator1<T>::~RegionCleaningOperator1()
  {
  }

  template<typename T>
  Border3 RegionCleaningOperator1<T>::getProposedBorder()
  {
    Border3 bmax;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
    {
      Border3 b= vsels(i).getProposedBorder();
      bmax= maximizeBorders(bmax, b);
    }
    
    return bmax;
  }
  
  template<typename T>
  int RegionCleaningOperator1<T>::getMin()
  {
    int min= INT_MAX;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      if ( vsels(i).getMin() < min )
	min= vsels(i).getMin();
    return min;
  }
  
  template<typename T>
  int RegionCleaningOperator1<T>::getMax()
  {
    int max= -INT_MAX;
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      if ( vsels(i).getMax() > max )
	max= vsels(i).getMax();
    return max;
  }
  
  template<typename T>
  void RegionCleaningOperator1<T>::apply(Volume<T>& input, Volume<T>& tokeep, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < vsels.size(); ++i )
      vsels(i).updateStride(input.columns, input.sliceSize);
    prefilter.updateStride(input.columns, input.sliceSize);
    
    output= 0;
    
    
    int min= getMin(), max= getMax();
    changed= 0;
    

    int changed= 0;
    int total= 0;
    int processed= 0;
    int outsideROI;
    
    for ( int i= -min; i < int(input.n) - max; ++i )
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) && !tokeep(i) )
	++total;
      
    Volume<unsigned char> mask;
    mask.resizeVolume(input);
    mask= 0;
    
    unsigned int l;
    for ( int i= -prefilter.getMin(); i < int(mask.n) - prefilter.getMax(); ++i )
    {
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) && !tokeep(i) )
      {
	for ( l= 0; l < prefilter.size(); ++l )
	  if ( !input(i + prefilter(l)) )
	    break;
	if ( l == prefilter.size() )
	  mask(i)= 1;
      }
    }
    
    #pragma omp parallel for schedule(dynamic, 10000)
    for ( int i= -min; i < int(input.n)-max; ++i )
    {
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) && !tokeep(i) && !mask(i) )
      {
	outsideROI= 0;
	Vector<int> a;
	a.resize(numAlphas);
	Vector<int> b;
	b.resize(vsels.size());
	a= 0;
	b= 0;
	unsigned int j;
	
	for ( j= 0; j < vsels.size(); ++j )
	{
	  int k;
	  int n= 0;
	  int size= vsels(j).size();
	  for ( k= 0; k < size; ++k )
	  {
	    n+= input(i + vsels(j)(k));
	    
	    if ( roi && !(*roi)(i + vsels(j)(k)) )
	    {
	      outsideROI++;
	      break;
	    }
	    
	    if ( (k+1) - n > size*(1-threshold) )
	      break;
	  }
	  
	  if ( k == size && n >= size*threshold )
	    b(j)= 1;
	    
	}
	
	for ( unsigned int m= 0; m < b.size(); ++m )
	  if ( b(m) )
	    for ( unsigned int l= 0; l < covered(m).size(); ++l )
		a(covered(m)(l))= 1;
	
	int maxLength= 0;
	int tmpLength= 0;
	for ( unsigned int k= 0; k < a.size(); ++k )
	{
	  if ( a(k) )
	  {
	    tmpLength= 1;
	    for ( unsigned int l= 1; l < a.size(); ++l )
	    {
	      if ( a((k + l)%a.size()) )
		++tmpLength;
	      else
	      {
		k+= l;
		break;
	      }
	    }
	    if ( tmpLength > maxLength )
	      maxLength= tmpLength;
	  }
	}

	/*for ( int i= 0; i < a.size(); ++i )
	  printf("%d ", a(i));
	printf("\n");
	getchar();*/
	//printf("%d,%d,%d ", a.size(), maxLength, outsideROI);
	float threshold= (a.size()*float(vsels.size() - outsideROI)/float(vsels.size())) * overallThreshold * float(reducedNumber)/float(originalNumber);
	//printf("%f ", threshold);
	if ( maxLength > threshold )
	  output(i)= 1;
	else
	{
	  output(i)= 0;
	  #pragma omp critical
	  {
	    changed++;
	  }
	}
	
	#pragma omp critical
	{
	  processed++;
	}
      }
      else if ( (!roi || (*roi)(i)) && tokeep(i) )
      {
	output(i)= input(i);
      }
      else if ( roi && !(*roi)(i) )
      {
	output(i)= input(i);
      }
      else if ( mask(i) )
      {
	output(i)= input(i);
      }
      
      int t= omp_get_thread_num();
      if ( t == 0 && processed % 100 == 0)
	printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r%8d/%8d", processed, total);
	
    }

    this->changed= changed;
    printf("\n");
    tprintf("removed: %d\n", changed);
  }
  
  
  template<typename T>
  class RegionCleaningOperator2
  {
    public:
      RegionCleaningOperator2(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold);

      RegionCleaningOperator2(const RegionCleaningOperator2& f);

      ~RegionCleaningOperator2();

      virtual void apply(Volume<T>& input, Volume<T>& tokeep, Volume<T>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
      
      virtual Border3 getProposedBorder();
      
      void makeStructuringElementsUnique();
      
      int getMin();
      
      int getMax();
      
      float alpha0;
      float alphastep;
      float alpha1;
      float beta0;
      float betastep;
      float beta1;
      float length;
      float threshold;
      float overallThreshold;
      
      Vector<StructuringElementLineSegment> vselsp;
      Vector<StructuringElementLineSegment> vselsm;
      Vector<float> alphas;
      Vector<float> betas;
      Vector<float> lengths;
      
      int changed;
  };
  
  template<typename T>
  RegionCleaningOperator2<T>::RegionCleaningOperator2(float alpha0, float alphastep, float alpha1, float beta0, float betastep, float beta1, float length, float threshold, float overallThreshold)
  {
    alpha0= alpha0/180*M_PI;
    alphastep= alphastep/180*M_PI;
    alpha1= alpha1/180*M_PI;
    beta0= beta0/180*M_PI;
    betastep= betastep/180*M_PI;
    beta1= beta1/180*M_PI;
    
    this->alpha0= alpha0;
    this->alphastep= alphastep;
    this->alpha1= alpha1;
    this->beta0= beta0;
    this->betastep= betastep;
    this->beta1= beta1;
    this->threshold= threshold;
    this->overallThreshold= overallThreshold;
    this->changed= 1;
    this->length= length;
    
    char cache[1000];
    sprintf(cache, "../rco2_%f_%f_%f_%f_%f_%f_%f.cache", alpha0, alphastep, alpha1, beta0, betastep, beta1, length);

    FILE* f;
    f= fopen(cache, "rt");
    if ( f == NULL )
    {
      for ( int l= length; l > 0; l-= 5 )
      {
	for ( float a= alpha0; a <= alpha1; a+= alphastep )
	{
	  for ( float b= beta0; b <= beta1; b+= betastep )
	  {
	    vselsp.push_back(StructuringElementLineSegment(l, a, b));
	    alphas.push_back(a);
	    betas.push_back(b);
	    lengths.push_back(l);
	  }
	}
      }
	
      makeStructuringElementsUnique();
      
      tprintf("number of structuring elements: %d\n", vselsm.size());
      
      tprintf("writing structuring elements\n");
      
      f= fopen(cache, "wt");
      fprintf(f, "%zd\n", alphas.size());
      for ( unsigned int i= 0; i < alphas.size(); ++i )
	fprintf(f, "%f ", alphas(i));
      fprintf(f, "\n");
      fprintf(f, "%zd\n", betas.size());
      for ( unsigned int i= 0; i < betas.size(); ++i )
	fprintf(f, "%f ", betas(i));
      fprintf(f, "\n");
      fprintf(f, "%zd\n", lengths.size());
      for ( unsigned int i= 0; i < lengths.size(); ++i )
	fprintf(f, "%f ", lengths(i));
      fprintf(f, "\n");
      fprintf(f, "%zd\n", vselsp.size());
      for ( unsigned int i= 0; i < vselsp.size(); ++i )
      {
	fprintf(f, "%zd %d %d\n", vselsp(i).size(), vselsp(i).stride, vselsp(i).slicePitch);
	for ( unsigned int j= 0; j < vselsp(i).size(); ++j )
	  fprintf(f, "%d ", vselsp(i)(j));
	fprintf(f, "\n");
      }
      fprintf(f, "%zd\n", vselsm.size());
      for ( unsigned int i= 0; i < vselsm.size(); ++i )
      {
	fprintf(f, "%zd %d %d\n", vselsm(i).size(), vselsm(i).stride, vselsm(i).slicePitch);
	for ( unsigned int j= 0; j < vselsm(i).size(); ++j )
	  fprintf(f, "%d ", vselsm(i)(j));
	fprintf(f, "\n");
      }
      fclose(f);
      
      tprintf("structuring elements written\n");
    }
    else
    {
      tprintf("reading structuring elements\n");
      
      int nalphas;
      int ret;
      ret= fscanf(f, "%d", &nalphas); if ( !ret ) printf("error in fscanf\n");
      alphas.resize(nalphas);
      for ( int i= 0; i < nalphas; ++i )
      {
	ret= fscanf(f, "%f", &(alphas(i))); if ( !ret ) printf("error in fscanf\n");
      }
      int nbetas;
      ret= fscanf(f, "%d", &nbetas); if ( !ret ) printf("error in fscanf\n");
      betas.resize(nbetas);
      for ( int i= 0; i < nbetas; ++i )
      {
	ret= fscanf(f, "%f", &(betas(i))); if ( !ret ) printf("error in fscanf\n");
      }
      int nlengths;
      ret= fscanf(f, "%d", &nlengths); if ( !ret ) printf("error in fscanf\n");
      lengths.resize(nlengths);
      for ( int i= 0; i < nlengths; ++i )
      {
	ret= fscanf(f, "%f", &(lengths(i))); if ( !ret ) printf("error in fscanf\n");
      }
      int nvselsp;
      ret= fscanf(f, "%d", &nvselsp); if ( !ret ) printf("error in fscanf\n");
      for ( int i= 0; i < nvselsp; ++i )
      {
	int n, stride, slicePitch;
	ret= fscanf(f, "%d%d%d", &n, &stride, &slicePitch); if ( !ret ) printf("error in fscanf\n");
	Vector<int> tmp;
	tmp.resize(n);
	for ( int j= 0; j < n; ++j )
        {
	  ret= fscanf(f, "%d", &(tmp(j))); if ( !ret ) printf("error in fscanf\n");
        }
	vselsp.push_back(StructuringElementLineSegment(tmp, stride, slicePitch));
      }
      int nvselsm;
      ret= fscanf(f, "%d", &nvselsm); if ( !ret ) printf("error in fscanf\n");
      for ( int i= 0; i < nvselsm; ++i )
      {
	int n, stride, slicePitch;
	ret= fscanf(f, "%d%d%d", &n, &stride, &slicePitch); if ( !ret ) printf("error in fscanf\n");
	Vector<int> tmp;
	tmp.resize(n);
	for ( int j= 0; j < n; ++j )
        {
	  ret= fscanf(f, "%d", &(tmp(j))); if ( !ret ) printf("error in fscanf\n");
        }
	vselsm.push_back(StructuringElementLineSegment(tmp, stride, slicePitch));
      }
      
      fclose(f);
      
      tprintf("structuring elements read\n");
    }
  }

  template<typename T>
  void RegionCleaningOperator2<T>::makeStructuringElementsUnique()
  {
    tprintf("before unique: %d\n", vselsp.size());
    /*for ( int i= 0; i < vselsm.size(); ++i )
    {
      Vector<int> toremove;
      //#pragma omp parallel for if ( vselsm.size() - i > 10000 ), num_threads(2), schedule(dynamic, 100)
      for ( int j= i+1; j < vselsm.size(); ++j )
      {
	if ( vselsm(i).size() == vselsm(j).size() )
	{
	  int n= 0, k;
	  for ( k= vselsm(i).size()-1; k >= 0; --k )
	    if ( vselsm(i)(k) != vselsm(j)(k) )
	      break;
	  if ( k == -1 )
	  {
	    //#pragma omp critical
	    {
	      toremove.push_back(j);
	    }
	  }
	}
      }
      
      sort(toremove.begin(), toremove.end());
      for ( int j= toremove.size()-1; j >= 0; --j )
      {
	vselsm.erase(vselsm.begin()+toremove(j));
      }
    }*/
    
    for ( unsigned int i= 0; i < vselsp.size(); ++i )
    {
      Vector<int> toremove;
      //#pragma omp parallel for if ( vselsp.size() - i > 10000 ), num_threads(2), schedule(dynamic, 100)
      for ( unsigned int j= i+1; j < vselsp.size(); ++j )
      {
	if ( vselsp(i).size() == vselsp(j).size() && vselsp(i)(vselsp(i).size()-1) == vselsp(j)(vselsp(j).size()-1) )
	{
	  int k;
	  for ( k= vselsp(i).size()-1; k >= 0; --k )
	    if ( vselsp(i)(k) != vselsp(j)(k) )
	      break;
	  if ( k == -1 )
	  {
	    //#pragma omp critical
	    {
	      toremove.push_back(j);
	    }
	  }
	}
      }
      
      sort(toremove.begin(), toremove.end());
      for ( int j= toremove.size()-1; j >= 0; --j )
      {
	vselsp.erase(vselsp.begin()+toremove(j));
	alphas.erase(alphas.begin()+toremove(j));
	betas.erase(betas.begin()+toremove(j));
	lengths.erase(lengths.begin()+toremove(j));
      }
    }
    
    for ( unsigned int i= 0; i < alphas.size(); ++i )
    {
      vselsm.push_back(StructuringElementLineSegment(-lengths(i), alphas(i), betas(i)));
    }
    
    tprintf("after unique: %d,%d, not proper: %d\n", vselsm.size(), vselsp.size(), 0);
  }
  
  template<typename T>
  RegionCleaningOperator2<T>::RegionCleaningOperator2(const RegionCleaningOperator2& f)
  {
    this->vselsm= f.vselsm;
    this->vselsp= f.vselsp;
    this->alpha0= f.alpha0;
    this->alphastep= f.alphastep;
    this->alpha1= f.alpha1;
    this->beta0= f.beta0;
    this->betastep= f.betastep;
    this->beta1= f.beta1;
    this->threshold= f.threshold;
    this->overallThreshold= f.overallThreshold;
    this->changed= 1;
    this->length= f.length;
  }

  template<typename T>
  RegionCleaningOperator2<T>::~RegionCleaningOperator2()
  {
  }

  template<typename T>
  Border3 RegionCleaningOperator2<T>::getProposedBorder()
  {
    Border3 bmax;
    for ( unsigned int i= 0; i < vselsm.size(); ++i )
    {
      Border3 b= vselsm(i).getProposedBorder();
      bmax= maximizeBorders(bmax, b);
      b= vselsp(i).getProposedBorder();
      bmax= maximizeBorders(bmax, b);
    }
    
    return bmax;
  }
  
  template<typename T>
  int RegionCleaningOperator2<T>::getMin()
  {
    int min= INT_MAX;
    for ( unsigned int i= 0; i < vselsm.size(); ++i )
      if ( vselsm(i).getMin() < min )
	min= vselsm(i).getMin();
    return min;
  }
  
  template<typename T>
  int RegionCleaningOperator2<T>::getMax()
  {
    int max= -INT_MAX;
    for ( unsigned int i= 0; i < vselsp.size(); ++i )
      if ( vselsp(i).getMax() > max )
	max= vselsp(i).getMax();
    return max;
  }
  
  template<typename T>
  void RegionCleaningOperator2<T>::apply(Volume<T>& input, Volume<T>& tokeep, Volume<T>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < vselsm.size(); ++i )
      vselsm(i).updateStride(input.columns, input.sliceSize);
    for ( unsigned int i= 0; i < vselsp.size(); ++i )
      vselsp(i).updateStride(input.columns, input.sliceSize);
    
    output= 0;
    
    
    int min= getMin(), max= getMax();
    changed= 0;
    

    int changed= 0;
    int total= 0;
    int processed= 0;
    
    for ( int i= -min; i < int(input.n) - max; ++i )
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) && !tokeep(i) )
	++total;
    
    #pragma omp parallel for schedule(dynamic, 10000)
    for ( int i= -min; i < int(input.n)-max; ++i )
    {
      if ( input(i) && input.isRealVolumeVoxel(i) && (!roi || (*roi)(i)) && !tokeep(i) )
      {
	unsigned int j;
	
	
	int lengthm= 0;
	int lengthp= 0;
	
	for ( j= 0; j < vselsm.size() || j < vselsp.size(); ++j )
	{
	  if ( j < vselsm.size() && int(vselsm(j).size()) > lengthm )
	  {
	    int k;
	    int n= 0;
	    int size= vselsm(j).size();
	    for ( k= 0; k < size; ++k )
	    {
	      n+= input(i + vselsm(j)(k));
	      
	      if ( k - n > size*(1-threshold) )
		break;
	    }
	    
	    if ( k == size && n > size*threshold )
	      if ( size > lengthm )
		lengthm= size;
	  }
	  
	  if ( j < vselsp.size() && int(vselsp(j).size()) > lengthp )
	  {
	    int k;
	    int n= 0;
	    int size= vselsp(j).size();
	    for ( k= 0; k < size; ++k )
	    {
	      n+= input(i + vselsp(j)(k));
	      
	      if ( k - n > size*(1-threshold) )
		break;
	    }
	    
	    if ( k == size && n > size*threshold )
	      if ( size > lengthp )
		lengthp= size;
	  }
	  
	  if ( lengthm + lengthp >= length )
	      break;
	}
	
	if ( lengthm + lengthp >= length )
	  output(i)= 1;
	else
	{
	  output(i)= 0;
	  #pragma omp critical
	  {
	    changed++;
	  }
	}
	
	#pragma omp critical
	{
	  processed++;
	}
      }
      else if ( roi && (*roi)(i) && tokeep(i) )
      {
	output(i)= input(i);
      }
      else if ( roi && !(*roi)(i) )
      {
	output(i)= input(i);
      }
      
      int t= omp_get_thread_num();
      if ( t == 0 && processed % 100 == 0)
	printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r%8d/%8d", processed, total);
	
    }

    this->changed= changed;
    
    printf("\n");
    tprintf("removed: %d\n", changed);
  }
  
  class RegionCleaningOperator3
  {
  public:
    RegionCleaningOperator3(int r);
    
    RegionCleaningOperator3(const RegionCleaningOperator3& a);
    
    ~RegionCleaningOperator3();
    
    virtual void apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& tokeep, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& tokeep, Volume<unsigned char>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int radius;
    int changed;
  };
  
  class RegionCleaningOperator4
  {
  public:
    RegionCleaningOperator4();
    
    RegionCleaningOperator4(const RegionCleaningOperator4& a);
    
    ~RegionCleaningOperator4();
    
    virtual void apply(Volume<unsigned char>& input, Volume<unsigned char>& tokeep, Volume<unsigned char>& output, FramedVolume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    
    int changed;
  };
}

#endif