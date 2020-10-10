#include <openipLL/VolumeRegionOperations.h>
#include <openipLL/morphology3.h>
#include <openipLL/morphology.h>
#include <openipLL/convexHull.h>

#include <openipDS/Stopper.h>

namespace openip
{
  SubtractAndBoundVolumes::SubtractAndBoundVolumes()
  {
  }

  SubtractAndBoundVolumes::SubtractAndBoundVolumes(const SubtractAndBoundVolumes& /*f*/)
  {
  }

  SubtractAndBoundVolumes::~SubtractAndBoundVolumes()
  {
  }

  void SubtractAndBoundVolumes::apply(Volume<unsigned char>& input1, Volume<unsigned char>& input2, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < input1.n; ++i )
      if ( input1(i) )
      {
	if ( input2(i) )
	{
	  output(i)= 0;
	}
	else
	{
	  output(i)= 1;
	}
      }
      else
      {
	output(i)= 0;
      }
  }
  
  FilterComponentsBySize::FilterComponentsBySize(int a, int b)
  {
    this->a= a;
    this->b= b;
  }

  FilterComponentsBySize::FilterComponentsBySize(const FilterComponentsBySize& f)
  {
    this->a= f.a;
    this->b= f.b;
  }

  FilterComponentsBySize::~FilterComponentsBySize()
  {
  }
  
  void FilterComponentsBySize::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    ExtractRegions3 er;
    Region3Set results;
    er.apply(input, results);
    
    output= 0;
    
    if ( a == -1 )
      a= 0;
    if ( b == -1 )
      b= input.n;
    
    for ( unsigned int i= 0; i < results.size(); ++i )
      if ( int(results(i).size()) >= a && int(results(i).size()) < b )
	for ( unsigned int j= 0; j < results(i).size(); ++j )
	  output(results(i)(j))= 1;
  }
  
  

  KeepLargestComponent::KeepLargestComponent()
  {
  }

  KeepLargestComponent::KeepLargestComponent(const KeepLargestComponent& /*f*/)
  {
  }

  KeepLargestComponent::~KeepLargestComponent()
  {
  }

  void KeepLargestComponent::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    ExtractRegions3 er;
    Region3Set results;
    er.apply(input, results);
    
    output= 0;
    
    int maxIdx= 0;
    for ( unsigned int i= 0; i < results.size(); ++i )
      if ( results(i).size() > results(maxIdx).size() )
	maxIdx= i;
      
    for ( unsigned int i= 0; i < results(maxIdx).size(); ++i )
      output(results(maxIdx)(i))= 1;
  }
  
  
  
  KeepNLargestComponent::KeepNLargestComponent(int n)
  {
    this->n= n;
  }

  KeepNLargestComponent::KeepNLargestComponent(const KeepNLargestComponent& f)
  {
    this->n= f.n;
  }

  KeepNLargestComponent::~KeepNLargestComponent()
  {
  }

  void KeepNLargestComponent::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    ExtractRegions3 er;
    Region3Set results;
    er.apply(input, results);
    
    output= 0;
    
    int j= 0;
    while ( j < n )
    {
      int maxIdx= 0;
      for ( unsigned int i= 0; i < results.size(); ++i )
	if ( results(i).size() > results(maxIdx).size() )
	  maxIdx= i;
	
      for ( unsigned int i= 0; i < results(maxIdx).size(); ++i )
	output(results(maxIdx)(i))= 1;
      
      results.erase(results.begin() + maxIdx);
      ++j;
      if ( results.size() == 0 )
	break;
    }
  }
  
  
  
  CreateAxialMask::CreateAxialMask(int frontBorder, int backBorder, int begin, int end)
  {
    this->frontBorder= frontBorder;
    this->backBorder= backBorder;
    this->begin= begin;
    this->end= end;
  }
  
  CreateAxialMask::CreateAxialMask(const CreateAxialMask& f)
  {
    this->frontBorder= f.frontBorder;
    this->backBorder= f.backBorder;
    this->begin= f.begin;
    this->end= f.end;
  }

  CreateAxialMask::~CreateAxialMask()
  {
  }

  void CreateAxialMask::getFirstAndLastIndices(Volume<unsigned char>& input, int& first, int& last)
  {
    for ( unsigned int i= 0; i < input.slices; ++i )
      if ( input(i,0,0) )
      {
	first= i;
	break;
      }
    for ( int i= input.slices-1; i >= 0; --i )
      if ( input(i,0,0) )
      {
	last= i;
	break;
      }
  }
  
  void CreateAxialMask::getFirstAndLastIndicesCompleteSearch(Volume<unsigned char>& input, int& first, int& last)
  {
    first= -1;
    last= -1;
    for ( unsigned int i= 0; i < input.slices && first == -1; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( input(i,j,k) )
	  {
	    first= i;
	    break;
	  }
    for ( int i= int(input.slices)-1; i >= 0 && last == -1; --i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( input(i,j,k) )
	  {
	    last= i;
	    break;
	  }
  }
  
  void CreateAxialMask::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    for ( unsigned int i= 0; i < input.slices && minIdx == -1; ++i )
      for ( unsigned int j= 0; j < input.rows && minIdx == -1; ++j )
	for ( unsigned int k= 0; k < input.columns && minIdx == -1; ++k )
	  if ( input(i,j,k) )
	  {
	    minIdx= i;
	    break;
	  }
    for ( int i= int(input.slices) - 1; i >= 0 && maxIdx == -1; --i )
      for ( unsigned int j= 0; j < input.rows && maxIdx == -1; ++j )
	for ( unsigned int k= 0; k < input.columns && maxIdx == -1; ++k )
	  if ( input(i,j,k) )
	  {
	    maxIdx= i;
	    break;
	  }
    
    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(input.slices) )
      maxIdx= input.slices-1;
    
    for ( int i= minIdx; i <= maxIdx; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateAxialMask::apply(Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    output= 0;
    for ( int i= begin; i <= end; ++i )
      for ( unsigned int j= 0; j < output.rows; ++j )
	for ( unsigned int k= 0; k < output.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateAxialMask::apply(Region3& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    int s, r, c;
    
    for ( unsigned int i= 0; i < input.size(); ++i )
    {
      input.getCoordinate3D(input(i), s, r, c);
      if ( minIdx == -1 || s < minIdx )
	minIdx= s;
      if ( maxIdx == -1 || s > maxIdx )
	maxIdx= s;
    }

    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(output.slices) )
      maxIdx= output.slices-1;
    
    for ( int i= minIdx; i <= maxIdx; ++i )
      for ( unsigned int j= 0; j < output.rows; ++j )
	for ( unsigned int k= 0; k < output.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateAxialMask::apply(Region3Set& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    int s, r, c;
    
    for ( unsigned int i= 0; i < input.size(); ++i )
    {
      for ( unsigned int j= 0; j < input(i).size(); ++j )
      {
	input(i).getCoordinate3D(input(i)(j), s, r, c);
	if ( minIdx == -1 || s < minIdx )
	  minIdx= s;
	if ( maxIdx == -1 || s > maxIdx )
	  maxIdx= s;
      }
    }
    
    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(output.slices) )
      maxIdx= output.slices-1;
    
    for ( int i= minIdx; i <= maxIdx; ++i )
      for ( unsigned int j= 0; j < output.rows; ++j )
	for ( unsigned int k= 0; k < output.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  
  CreateSaggitalMask::CreateSaggitalMask(int frontBorder, int backBorder, int begin, int end)
  {
    this->frontBorder= frontBorder;
    this->backBorder= backBorder;
    this->begin= begin;
    this->end= end;
  }
  
  CreateSaggitalMask::CreateSaggitalMask(const CreateSaggitalMask& f)
  {
    this->frontBorder= f.frontBorder;
    this->backBorder= f.backBorder;
    this->begin= f.begin;
    this->end= f.end;
  }

  CreateSaggitalMask::~CreateSaggitalMask()
  {
  }

  void CreateSaggitalMask::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    for ( unsigned int k= 0; k < input.columns && minIdx == -1; ++k )
      for ( unsigned int i= 0; i < input.slices && minIdx == -1; ++i )
	for ( unsigned int j= 0; j < input.rows && minIdx == -1; ++j )
	  if ( input(i,j,k) )
	  {
	    minIdx= k;
	    break;
	  }
    for ( int k= int(input.columns) - 1; k >= 0 && maxIdx == -1; --k )	  
      for ( unsigned int i= 0; i < input.slices && maxIdx == -1; ++i )
	for ( unsigned int j= 0; j < input.rows && maxIdx == -1; ++j )
	  if ( input(i,j,k) )
	  {
	    maxIdx= k;
	    break;
	  }
    
    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(input.columns) )
      maxIdx= input.columns-1;
    
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( int k= minIdx; k <= maxIdx; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateSaggitalMask::apply(Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    output= 0;
    for ( unsigned int i= 0; i < output.slices; ++i )
      for ( unsigned int j= 0; j < output.rows; ++j )
	for ( int k= begin; k <= end; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateSaggitalMask::apply(Region3& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    int s, r, c;
    
    for ( unsigned int i= 0; i < input.size(); ++i )
    {
      input.getCoordinate3D(input(i), s, r, c);
      if ( minIdx == -1 || c < minIdx )
	minIdx= c;
      if ( maxIdx == -1 || c > maxIdx )
	maxIdx= c;
    }

    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(output.columns) )
      maxIdx= output.columns-1;
    
    for ( unsigned int i= 0; i < output.slices; ++i )
      for ( unsigned int j= 0; j < output.rows; ++j )
	for ( int k= minIdx; k <= maxIdx; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateSaggitalMask::apply(Region3Set& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    int s, r, c;
    
    for ( unsigned int i= 0; i < input.size(); ++i )
    {
      for ( unsigned int j= 0; j < input(i).size(); ++j )
      {
	input(i).getCoordinate3D(input(i)(j), s, r, c);
	if ( minIdx == -1 || c < minIdx )
	  minIdx= c;
	if ( maxIdx == -1 || c > maxIdx )
	  maxIdx= c;
      }
    }
    
    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(output.slices) )
      maxIdx= output.slices-1;
    
    for ( unsigned int i= 0; i < output.slices; ++i )
      for ( unsigned int j= 0; j < output.rows; ++j )
	for ( int k= minIdx; k <= maxIdx; ++k )
	  output(i,j,k)= 1;
  }
  
  
  
  
  CreateCoronalMask::CreateCoronalMask(int frontBorder, int backBorder, int begin, int end)
  {
    this->frontBorder= frontBorder;
    this->backBorder= backBorder;
    this->begin= begin;
    this->end= end;
  }
  
  CreateCoronalMask::CreateCoronalMask(const CreateCoronalMask& f)
  {
    this->frontBorder= f.frontBorder;
    this->backBorder= f.backBorder;
    this->begin= f.begin;
    this->end= f.end;
  }

  CreateCoronalMask::~CreateCoronalMask()
  {
  }

  void CreateCoronalMask::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    for ( unsigned int j= 0; j < input.rows && minIdx == -1; ++j )
      for ( unsigned int k= 0; k < input.columns && minIdx == -1; ++k )
	for ( unsigned int i= 0; i < input.slices && minIdx == -1; ++i )
	  if ( input(i,j,k) )
	  {
	    minIdx= j;
	    break;
	  }
    for ( unsigned int j= 0; j < input.rows && maxIdx == -1; ++j )	  
      for ( int k= int(input.columns) - 1; k >= 0 && maxIdx == -1; --k )	  
	for ( unsigned int i= 0; i < input.slices && maxIdx == -1; ++i )
	  if ( input(i,j,k) )
	  {
	    maxIdx= j;
	    break;
	  }
    
    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(input.rows) )
      maxIdx= input.rows-1;
    
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( int j= minIdx; j <= maxIdx; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateCoronalMask::apply(Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    output= 0;
    for ( unsigned int i= 0; i < output.slices; ++i )
      for ( int j= begin; j <= end; ++j )
	for ( unsigned int k= 0; k < output.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateCoronalMask::apply(Region3& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    int s, r, c;
    
    for ( unsigned int i= 0; i < input.size(); ++i )
    {
      input.getCoordinate3D(input(i), s, r, c);
      if ( minIdx == -1 || r < minIdx )
	minIdx= r;
      if ( maxIdx == -1 || r > maxIdx )
	maxIdx= r;
    }

    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(output.rows) )
      maxIdx= output.rows-1;
    
    for ( unsigned int i= 0; i < output.slices; ++i )
      for ( int j= minIdx; j <= maxIdx; ++j )
	for ( unsigned int k= 0; k < output.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  void CreateCoronalMask::apply(Region3Set& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1;
    int maxIdx= -1;
    
    output= 0;
    
    int s, r, c;
    
    for ( unsigned int i= 0; i < input.size(); ++i )
    {
      for ( unsigned int j= 0; j < input(i).size(); ++j )
      {
	input(i).getCoordinate3D(input(i)(j), s, r, c);
	if ( minIdx == -1 || r < minIdx )
	  minIdx= r;
	if ( maxIdx == -1 || r > maxIdx )
	  maxIdx= r;
      }
    }
    
    minIdx= minIdx - frontBorder;
    if ( minIdx < 0 )
      minIdx= 0;
    maxIdx= maxIdx + backBorder;
    if ( maxIdx >= int(output.rows) )
      maxIdx= output.rows-1;
    
    for ( unsigned int i= 0; i < output.slices; ++i )
      for ( int j= minIdx; j <= maxIdx; ++j )
	for ( unsigned int k= 0; k < output.columns; ++k )
	  output(i,j,k)= 1;
  }
  
  
  
  
  AxialCutAroundObject::AxialCutAroundObject(int radius0, int radius1)
  {
    this->radius0= radius0;
    this->radius1= radius1;
  }
    
  AxialCutAroundObject::AxialCutAroundObject(const AxialCutAroundObject& f)
  {
    this->radius0= f.radius0;
    this->radius1= f.radius1;
  }
  
  AxialCutAroundObject::~AxialCutAroundObject()
  {
  }
  
  void AxialCutAroundObject::apply(Volume<unsigned char>& input, Volume<unsigned char>& object, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    Volume<unsigned char> tmp0;
    Volume<unsigned char> tmp1;
    
    tmp0.resizeVolume(input);
    tmp1.resizeVolume(input);
    
    GrayscaleCSDilate<unsigned char, unsigned char> gd0(radius0);
    GrayscaleCSDilate<unsigned char, unsigned char> gd1(radius1);
    
    gd0.apply(object, tmp0);
    gd1.apply(object, tmp1);
    
    output= 0;
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( !tmp0(i) && tmp1(i) )
	output(i)= input(i);
  }
  
  
  
  ConnectClosestRegions::ConnectClosestRegions(float minDist, int resolution)
  {
    this->minDist= minDist;
    this->resolution= resolution;
  }
    
  ConnectClosestRegions::ConnectClosestRegions(const ConnectClosestRegions& f)
  {
    this->minDist= f.minDist;
    this->resolution= f.resolution;
  }
  
  ConnectClosestRegions::~ConnectClosestRegions()
  {
  }
  
  void ConnectClosestRegions::connectVoxels(int a, int b, Volume<unsigned char>& v)
  {
    int s, r, c, ss, rr, cc;
    v.getCoordinate3D(a, s, r, c);
    v.getCoordinate3D(b, ss, rr, cc);
    
    float ds= (ss - s)/float(resolution);
    float dr= (rr - r)/float(resolution);
    float dc= (cc - c)/float(resolution);
    
    for ( int i= 0; i < resolution; ++i )
      v(s + i*ds, r + i*dr, c + i*dc)= 1;
  }
  
  void ConnectClosestRegions::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    ExtractRegions3 er;
    Region3Set results;
    
    er.apply(input, results);
    
    output= 0;
    for ( unsigned int i= 0; i < results.size(); ++i )
      for ( unsigned int j= 0; j < results.size(); ++j )
	if ( i != j )
	{
	  if ( results(i).getMeanDistance(results(j)) < minDist )
	    for ( unsigned int k= 0; k < results(i).size(); ++k )
	    {
	      int l= results(i).getIndexOfClosestVoxel(results(i)(k), results(j));
	      connectVoxels(results(i)(k), results(j)(l), output);
	    }
	}
  }
  
  SetLabelsInVolume::SetLabelsInVolume()
  {
  }
    
  SetLabelsInVolume::SetLabelsInVolume(const SetLabelsInVolume& /*f*/)
  {
  }
  
  SetLabelsInVolume::~SetLabelsInVolume()
  {
  }
  
  void SetLabelsInVolume::apply(Volume<unsigned char>& input1, Volume<unsigned char>& input2, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < input1.slices; ++i )
      for ( unsigned int j= 0; j < input1.rows; ++j )
	for ( unsigned int k= 0; k < input1.columns; ++k )
	{
	  if ( input1(i,j,k) )
	  {
	    if ( input2(i,j,k) )
	      output(i,j,k)= input2(i,j,k);
	    else
	      output(i,j,k)= input1(i,j,k);
	  }
	  else
	    output(i,j,k)= input1(i,j,k);
	}
  }
  
  
  
  ExtendLabelingInVolume::ExtendLabelingInVolume()
  {
  }
    
  ExtendLabelingInVolume::ExtendLabelingInVolume(const ExtendLabelingInVolume& /*e*/)
  {
  }
  
  ExtendLabelingInVolume::~ExtendLabelingInVolume()
  {
  }
  
  void ExtendLabelingInVolume::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementCube sec(3, input.columns, input.sliceSize);
    
    output= input;
    int n;
    while ( 1 )
    {
      n= 0;
      for ( unsigned int i= 0; i < input.n; ++i )
	if ( output(i) == 1 && ( !roi || (*roi)(i)) )
	{
	  for ( unsigned int j= 0; j < sec.size(); ++j )
	    if ( output(i + sec(j)) > 1 )
	    {
	      output(i)= output(i + sec(j));
	      ++n;
	    }
	}
	
      if ( n == 0 )
	break;
    }
  }
  
  
  
  SetVoxelsToColor::SetVoxelsToColor(unsigned char label)
  {
    this->label= label;
  }
    
  SetVoxelsToColor::SetVoxelsToColor(const SetVoxelsToColor& e)
  {
    this->label= e.label;
  }
  
  SetVoxelsToColor::~SetVoxelsToColor()
  {
  }
  
  void SetVoxelsToColor::apply(Volume<unsigned char>& input1, Volume<unsigned char>& input2, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    output= input1;
    for ( unsigned int i= 0; i < input1.n; ++i )
      if ( input2(i) )
	output(i)= label;
  }
  
  
  
  
  AxialPropagationOfLabels::AxialPropagationOfLabels(int first, int last)
  {
    this->first= first;
    this->last= last;
  }
    
  AxialPropagationOfLabels::AxialPropagationOfLabels(const AxialPropagationOfLabels& a)
  {
    this->first= a.first;
    this->last= a.last;
  }
  
  AxialPropagationOfLabels::~AxialPropagationOfLabels()
  {
  }
  
  void AxialPropagationOfLabels::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    output= input;
    StructuringElementCube sec(3, input.columns, input.sliceSize);
    
    int n= 1;
    
    while ( n )
    {
      n= 0;
      for ( int i= first + 1; i < last; ++i )
      {
	for ( unsigned int j= 0; j < input.rows; ++j )
	  for ( unsigned int k= 0; k < input.columns; ++k )
	    if ( output(i,j,k) == 1 )
	    {
	      int p= i*input.sliceSize + j*input.columns + k;
	      for ( unsigned int l= 0; l < sec.size(); ++l )
		if ( output(p + sec(l)) > 1 )
		{
		  output(p)= output(p + sec(l));
		  ++n;
		  break;
		}
	    }
		
      }
    }
  }
  
  void AxialPropagationOfLabels::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, unsigned char color,  Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    output= input;
    StructuringElementCube sec(3, input.columns, input.sliceSize);
    
    int n= 1;
    
    while ( n )
    {
      n= 0;
      for ( int i= first + 1; i < last; ++i )
      {
	for ( unsigned int j= 0; j < input.rows; ++j )
	  for ( unsigned int k= 0; k < input.columns; ++k )
	    if ( output(i,j,k) == 1 )
	    {
	      int p= i*input.sliceSize + j*input.columns + k;
	      for ( unsigned int l= 0; l < sec.size(); ++l )
		if ( output(p + sec(l)) > 1 && output(p + sec(l)) == color )
		{
		  output(p)= output(p + sec(l));
		  ++n;
		  break;
		}
	    }
		
      }
    }
  }
  
  
  
  
  GrowRegionUntilLargeWidth::GrowRegionUntilLargeWidth(float a)
  {
    this->a= a;
  }
  
  GrowRegionUntilLargeWidth::GrowRegionUntilLargeWidth(const GrowRegionUntilLargeWidth& a)
  {
    this->a= a.a;
  }
  
  GrowRegionUntilLargeWidth::~GrowRegionUntilLargeWidth()
  {
  }
  
  void GrowRegionUntilLargeWidth::apply(Volume<unsigned char>& input, Volume<unsigned char>& object, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int minIdx= -1, maxIdx= -1;
    for ( unsigned int i= 0; i < object.slices && minIdx == -1; ++i )
    {
      for ( unsigned int j= 0; j < object.rows; ++j )
	for ( unsigned int k= 0; k < object.columns; ++k )
	  if ( object(i,j,k) )
	    minIdx= i;
    }
    for ( int i= int(object.slices)-1; i >= 0 && maxIdx == -1; --i )
    {
      for ( unsigned int j= 0; j < object.rows; ++j )
	for ( unsigned int k= 0; k < object.columns; ++k )
	  if ( object(i,j,k) )
	    maxIdx= i;
    }
    
    Image<unsigned char> tmp;
    object.getImage(tmp, 0, minIdx);
    ExtractRegions er;
    Region2Set results;
    er.apply(tmp, results);
    
    Region2 startRegion= results.getLargestRegion();
    
    int label= tmp(startRegion(0));
    
    output= object;
    
    for ( int i= minIdx - 1; i >= 0; --i )
    {
      input.getImage(tmp, 0, i);
      results.clear();
      er.apply(tmp, results);
      Region2Set intersectingRegions;
      intersectingRegions= results.getIntersectingRegions(startRegion);
      if ( intersectingRegions.size() == 0 )
	break;
      Region2 intersectingRegion= intersectingRegions.getLargestRegion();
      if ( intersectingRegion.size() > a * startRegion.size() )
	break;
      else
      {
	tmp= 0;
	intersectingRegion.set(tmp, (unsigned char)label);
	output.setImage(tmp, 0, i);
	startRegion= intersectingRegion;
      }
    }
    
    object.getImage(tmp, 0, maxIdx);
    results.clear();
    er.apply(tmp, results);
    
    startRegion= results.getLargestRegion();
    
    for ( unsigned int i= maxIdx + 1; i < input.slices; ++i )
    {
      input.getImage(tmp, 0, i);
      results.clear();
      er.apply(tmp, results);
      Region2Set intersectingRegions;
      intersectingRegions= results.getIntersectingRegions(startRegion);
      if ( intersectingRegions.size() == 0 )
	break;
      Region2 intersectingRegion= intersectingRegions.getLargestRegion();
      if ( intersectingRegion.size() > a * startRegion.size() )
	break;
      else
      {
	tmp= 0;
	intersectingRegion.set(tmp, (unsigned char)label);
	output.setImage(tmp, 0, i);
	startRegion= intersectingRegion;
      }
    }
  }
  
  
  ConnectRegionsInVolume::ConnectRegionsInVolume(int resolution)
  {
    this->resolution= resolution;
  }
    
  ConnectRegionsInVolume::ConnectRegionsInVolume(const ConnectRegionsInVolume& a)
  {
    this->resolution= a.resolution;
  }
  
  ConnectRegionsInVolume::~ConnectRegionsInVolume()
  {
  }
  
  void ConnectRegionsInVolume::connectVoxels(int s0, int r0, int c0, int s1, int r1, int c1, Volume<unsigned char>& v)
  {
    float ds= (s1 - s0)/float(resolution);
    float dr= (r1 - r0)/float(resolution);
    float dc= (c1 - c0)/float(resolution);
    
    for ( int i= 0; i < resolution; ++i )
    {
      int s= s0 + i*ds;
      int r= r0 + i*dr;
      int c= c0 + i*dc;
      if ( s >= 0 && s < int(v.slices) && r >= 0 && r < int(v.rows) && c >= 0 && c < int(v.columns) )
      {
	v(s0 + i*ds, r0 + i*dr, c0 + i*dc)= 1;
      }
    }
  }
  
  void ConnectRegionsInVolume::connectVoxels(int a, int b, Volume<unsigned char>& v)
  {
    int s, r, c, ss, rr, cc;
    v.getCoordinate3D(a, s, r, c);
    v.getCoordinate3D(b, ss, rr, cc);
    
    float ds= (ss - s)/float(resolution);
    float dr= (rr - r)/float(resolution);
    float dc= (cc - c)/float(resolution);
    
    for ( int i= 0; i < resolution; ++i )
    {
      int s0= s + i*ds;
      int r0= r + i*dr;
      int c0= c + i*dc;
      if ( s0 >= 0 && s0 < int(v.slices) && r0 >= 0 && r0 < int(v.rows) && c0 >= 0 && c0 < int(v.columns) )
	v(s + i*ds, r + i*dr, c + i*dc)= 1;
    }
  }
  
  void ConnectRegionsInVolume::connectPixels(int a, int b, Image<unsigned char>& img, int multiplier)
  {
    int r, c, rr, cc;
    img.getCoordinate2D(a, r, c);
    img.getCoordinate2D(b, rr, cc);
    
    float dr= (rr - r)/float(resolution);
    float dc= (cc - c)/float(resolution);
    
    int startIdxR= r;
    int startIdxC= c;
    
    int sign= (multiplier > 0) ? 1 : -1;
    
    int row, column;
    for ( int i= 0; i < abs(multiplier) * resolution; ++i )
    {
      row= startIdxR + sign*i*dr;
      column= startIdxC + sign*i*dc;
    
      if ( 0 <= row && row < img.rows && column >= 0 && column < img.columns )
	img(row, column)= 1;
    }
  }
  
  void ConnectRegionsInVolume::connectPixels(int r0, int c0, int r1, int c1, Image<unsigned char>& img, int multiplier)
  {
    float dr= (r1 - r0)/float(resolution);
    float dc= (c1 - c0)/float(resolution);
    
    int startIdxR= r0;
    int startIdxC= c0;
    
    int sign= (multiplier > 0) ? 1 : -1;
    
    int row, column;
    for ( int i= 0; i < abs(multiplier) * resolution; ++i )
    {
      row= startIdxR + sign*i*dr;
      column= startIdxC + sign*i*dc;
    
      if ( 0 <= row && row < img.rows && column >= 0 && column < img.columns )
	img(row, column)= 1;
    }
  }
  
  void ConnectRegionsInVolume::apply(Region3& input1, Region3& input2, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int tmp;
    for ( unsigned int i= 0; i < input1.size(); ++i )
    {
      tmp= input1.getIndexOfClosestVoxel(input1(i), input2);
      connectVoxels(input1(i), input2(tmp), output);
    }
    
    for ( unsigned int i= 0; i < input2.size(); ++i )
    {
      tmp= input2.getIndexOfClosestVoxel(input2(i), input1);
      connectVoxels(input2(i), input1(tmp), output);
    }
  }
  
    
  BandThreshold::BandThreshold(float a, float b)
  {
    this->a= a;
    this->b= b;
  }
    
  BandThreshold::BandThreshold(const BandThreshold& a)
  {
    this->a= a.a;
    this->b= a.b;
  }
  
  BandThreshold::~BandThreshold()
  {
  }
  
  void BandThreshold::apply(Volume<float>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( input(i) >= a && input(i) < b && (!roi || (*roi)(i)) )
	output(i)= 1;
      else
	output(i)= 0;
  }
  
  
  
  SimpleThreshold::SimpleThreshold(float a)
  {
    this->a= a;
  }
    
  SimpleThreshold::SimpleThreshold(const SimpleThreshold& a)
  {
    this->a= a.a;
  }
  
  SimpleThreshold::~SimpleThreshold()
  {
  }
  
  void SimpleThreshold::apply(Volume<float>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( input(i) > a && (!roi || (*roi)(i)) )
	output(i)= 1;
      else
	output(i)= 0;
  }
  
  
  
  
  ConvexHullInVolume::ConvexHullInVolume()
  {
  }
    
  ConvexHullInVolume::ConvexHullInVolume(const ConvexHullInVolume& /*c*/)
  {
  }
  
  ConvexHullInVolume::~ConvexHullInVolume()
  {
  }
  
  void ConvexHullInVolume::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support)*/)
  {
    Image<unsigned char> tmpImageBB;
    
    output= 0;
    for ( unsigned int i= 0; i < input.slices; ++i )
    {
      if ( !(*roi)(i, 0, 0) )
	continue;
      
      input.getImage(tmpImageBB, 0, i);
      
      Region2Set regions;
      ExtractRegions er;
      
      er.apply(tmpImageBB, regions);
      
      Region2 convexHull;
      ConvexHull ch(1);
      //printf("aaa0\n"); fflush(stdout);
      ch.apply(regions, convexHull);
      //printf("aaa1 %d\n", convexHull.size()); fflush(stdout);
      
      /*if ( convexHull.size() > 0 )
      {
	for ( int i= 0; i < tmpImageBB.size(); ++i )
	  if ( tmpImageBB(i) )
	    tmpImageBB(i)= 255;
	writeImage("before.bmp", tmpImageBB);
	for ( int i= 0; i < tmpImageBB.size(); ++i )
	  if ( tmpImageBB(i) )
	    tmpImageBB(i)= 1;
      }*/
      
      tmpImageBB= 0;
      
      for ( unsigned int j= 0; j < convexHull.size(); ++j )
	tmpImageBB(convexHull(j))= 1;
      
      /*if ( convexHull.size() > 0 )
      {
	for ( int i= 0; i < tmpImageBB.size(); ++i )
	  if ( tmpImageBB(i) )
	    tmpImageBB(i)= 255;
	writeImage("after.bmp", tmpImageBB);
	getchar();
	for ( int i= 0; i < tmpImageBB.size(); ++i )
	  if ( tmpImageBB(i) )
	    tmpImageBB(i)= 1;
      }*/
      
      output.setImage(tmpImageBB, 0, i);
    }
  }
  
    
  InnerAxialDarkReconstruction::InnerAxialDarkReconstruction(int radius)
  {
    this->radius= radius;
  }
    
  InnerAxialDarkReconstruction::InnerAxialDarkReconstruction(const InnerAxialDarkReconstruction& c)
  {
    this->radius= c.radius;
  }
  
  InnerAxialDarkReconstruction::~InnerAxialDarkReconstruction()
  {
  }
  
  void InnerAxialDarkReconstruction::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    
    tmp= input;
    
    int flag;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
      {
	flag= 1;
	for ( unsigned int k= 0; k < input.columns && flag; ++k )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 2;
	}
      }
	
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
      {
	flag= 1;
	for ( int k= int(input.columns)-1; k >= 0 && flag; --k )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 2;
	}
      }
	
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int k= 0; k < input.columns; ++k )
      {
	flag= 1;
	for ( unsigned int j= 0; j < input.rows && flag; ++j )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 2;
	}
      }
	
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int k= 0; k < input.columns; ++k )
      {
	flag= 1;
	for ( int j= int(input.rows) -1; j >= 0 && flag; --j )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 2;
	}
      }
	
    for ( unsigned int j= 0; j < input.rows; ++j )
      for ( unsigned int k= 0; k < input.columns && flag; ++k )
      {
	flag= 1;
	for ( unsigned int i= 0; i < input.slices && flag; ++i )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 2;
	}
      }
	
    for ( unsigned int j= 0; j < input.rows; ++j )
      for ( unsigned int k= 0; k < input.columns && flag; ++k )
      {
	flag= 1;
	for ( int i= int(input.slices) - 1; i >= 0 && flag; --i )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 2;
	}
      }

    GrayscaleAxialBrightRecon<unsigned char, unsigned char> gadr(radius);
    gadr.apply(tmp, output);
    
    for ( unsigned int i= 0; i < output.n; ++i )
    {
      if ( roi && !(*roi)(i) )
	output(i)= input(i);
      else if ( tmp(i) == 2 )
	output(i)= 0;
    }
  }
  
  
  OuterErodeAndReconstruct::OuterErodeAndReconstruct(int r)
  {
    this->radius= r;
  }
    
  OuterErodeAndReconstruct::OuterErodeAndReconstruct(const OuterErodeAndReconstruct& o)
  {
    this->radius= o.radius;
  }
  
  OuterErodeAndReconstruct::~OuterErodeAndReconstruct()
  {
  }
  
  void OuterErodeAndReconstruct::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    
    tmp= 0;
    
    int flag;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
      {
	flag= 1;
	for ( unsigned int k= 0; k < input.columns && flag; ++k )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 1;
	}
      }
	
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
      {
	flag= 1;
	for ( int k= int(input.columns)-1; k >= 0 && flag; --k )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 1;
	}
      }
	
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int k= 0; k < input.columns; ++k )
      {
	flag= 1;
	for ( unsigned int j= 0; j < input.rows && flag; ++j )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 1;
	}
      }
	
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int k= 0; k < input.columns; ++k )
      {
	flag= 1;
	for ( int j= int(input.rows) -1; j >= 0 && flag; --j )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 1;
	}
      }
	
    for ( unsigned int j= 0; j < input.rows; ++j )
      for ( unsigned int k= 0; k < input.columns && flag; ++k )
      {
	flag= 1;
	for ( unsigned int i= 0; i < input.slices && flag; ++i )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 1;
	}
      }
	
    for ( unsigned int j= 0; j < input.rows; ++j )
      for ( unsigned int k= 0; k < input.columns && flag; ++k )
      {
	flag= 1;
	for ( int i= int(input.slices) - 1; i >= 0 && flag; --i )
	{
	  if ( input(i,j,k) )
	    flag= 0;
	  else
	    tmp(i,j,k)= 1;
	}
      }
    
    output= tmp;
    /*GrayscaleCSDilate<unsigned char, unsigned char> gd(radius);
    gd.apply(tmp, output, roi);*/
    
/*    tmp= 0;

    FramedVolume<unsigned char>* a;
    FramedVolume<unsigned char>* b;
    
    for ( int i= 0; i < output.n; ++i )
      if ( !(output(i)) && (!roi || (*roi)(i)) )
	tmp(i)= input(i);
    
    output= 0;
    gd.apply(tmp, output, roi);
    
    for ( int i= 0; i < output.n; ++i )
      if ( output(i) && input(i) )
	output(i)= 1;
      else
	output(i)= 0;*/
  }
  
  
  
  
  AxialAxialCSReconstruct::AxialAxialCSReconstruct(int r1, int r2)
  {
    radius1= r1;
    radius2= r2;
  }
  
  AxialAxialCSReconstruct::AxialAxialCSReconstruct(const AxialAxialCSReconstruct& o)
  {
    radius1= o.radius1;
    radius2= o.radius2;
  }
  
  AxialAxialCSReconstruct::~AxialAxialCSReconstruct()
  {
  }
  
  void AxialAxialCSReconstruct::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    
    GrayscaleAxialErode<unsigned char, unsigned char> gae(radius1);
    GrayscaleAxialDilate<unsigned char, unsigned char> gad(radius1);
    GrayscaleCSDilate<unsigned char, unsigned char> gcs(radius2);
    KeepLargestComponent klc;
    
    gae.apply(input, output);
    /*klc.apply(output, tmp);
    output= tmp;*/
    gad.apply(output, tmp);
    klc.apply(tmp, output);
    tmp= output;
    gcs.apply(tmp, output);
    
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( input(i) && output(i) )
	output(i)= input(i);
      else
	output(i)= 0;
  }

  AxialLargestAxialReconstruct::AxialLargestAxialReconstruct(int r1)
  {
    radius1= r1;
  }
  
  AxialLargestAxialReconstruct::AxialLargestAxialReconstruct(const AxialLargestAxialReconstruct& o)
  {
    radius1= o.radius1;
  }
  
  AxialLargestAxialReconstruct::~AxialLargestAxialReconstruct()
  {
  }
  
  void AxialLargestAxialReconstruct::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    
    GrayscaleAxialErode<unsigned char, unsigned char> gae(radius1);
    GrayscaleAxialDilate<unsigned char, unsigned char> gad(radius1+2);
    KeepLargestComponent klc;
    
    ExtractRegions3 er;
    Region3Set regions;
    
    gae.apply(input, output);
    //klc.apply(output, tmp);
    er.apply(output, regions);
    Region3Set largest;
    largest= regions.getLargestNRegion(2);
    tmp= 0;
    if ( largest.size() == 1 )
    {
      largest(0).set(tmp, (unsigned char)1);
    }
    else if ( float(largest(0).size())/float(largest(1).size()) > 0.3 && float(largest(0).size())/float(largest(1).size()) < 3 )
    {
      largest(0).set(tmp, (unsigned char)1);
      largest(1).set(tmp, (unsigned char)1);
    }
    else
    {
      largest(0).set(tmp, (unsigned char)1);
    }
    
    gad.apply(tmp, output);
    
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( !roi || (*roi)(i) )
      {
	if ( input(i) && output(i) )
	  output(i)= input(i);
	else
	  output(i)= 0;
      }
      else
	output(i)= 0;
  }  

  
  CSLargestCSReconstruct::CSLargestCSReconstruct(int r1, int plus)
  {
    radius1= r1;
    this->plus= plus;
  }
  
  CSLargestCSReconstruct::CSLargestCSReconstruct(const CSLargestCSReconstruct& o)
  {
    radius1= o.radius1;
    this->plus= o.plus;
  }
  
  CSLargestCSReconstruct::~CSLargestCSReconstruct()
  {
  }
  
  void CSLargestCSReconstruct::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    
    GrayscaleCSErode<unsigned char, unsigned char> gae(radius1);
    GrayscaleCSDilate<unsigned char, unsigned char> gad(radius1+plus);
    KeepLargestComponent klc;
    ExtractRegions3 er;
    Region3Set regions;
    
    gae.apply(input, output, roi);
    //klc.apply(output, tmp);
    
    er.apply(output, regions);
    Region3Set largest;
    largest= regions.getLargestNRegion(2);
    tmp= 0;
    if ( largest.size() == 1 )
    {
      largest(0).set(tmp, (unsigned char)1);
    }
    else if ( float(largest(0).size())/float(largest(1).size()) > 0.3 && float(largest(0).size())/float(largest(1).size()) < 3 )
    {
      largest(0).set(tmp, (unsigned char)1);
      largest(1).set(tmp, (unsigned char)1);
    }
    else
    {
      largest(0).set(tmp, (unsigned char)1);
    }
    
    /*output= tmp;
    return;*/
    
    gad.apply(tmp, output, roi);
    
    
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( !roi || (*roi)(i) )
      {
	if ( input(i) && output(i) )
	  output(i)= input(i);
	else
	  output(i)= 0;
      }
      else
	output(i)= 0;
  }  

  
  AxialCoherence::AxialCoherence(int r)
  {
    this->radius= r;
  }
  
  AxialCoherence::AxialCoherence(const AxialCoherence& a)
  {
    this->radius= a.radius;
  }
  
  AxialCoherence::~AxialCoherence()
  {
  }
  
  void AxialCoherence::apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementCS sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step;
    if ( begin > end )
      step= -1;
    else
      step= 1;
    
    tmp= input;
    for ( unsigned int i= 0; i < input.rows; ++i )
      for ( unsigned int j= 0; j < input.columns; ++j )
	if ( tmp(begin, i, j) )
	  tmp(begin, i, j)= 2;
    
    int removed= 0;
    for ( int i= begin+step; (begin < end) ? (i < end) : (i > end); i+= step )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	{
	  if ( tmp(i,j,k) )
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp((i-step)*input.sliceSize + j*input.columns + k + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( int i= begin+step; (begin < end) ? (i < end) : (i > end); i+= step )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
  }
  
  
  SaggitalCoherence::SaggitalCoherence(int r)
  {
    this->radius= r;
  }
  
  SaggitalCoherence::SaggitalCoherence(const SaggitalCoherence& a)
  {
    this->radius= a.radius;
  }
  
  SaggitalCoherence::~SaggitalCoherence()
  {
  }
  
  void SaggitalCoherence::apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementAC sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step;
    if ( begin > end )
      step= -1;
    else
      step= 1;
    
    tmp= input;
    for ( unsigned int i= 0; i < input.rows; ++i )
      for ( unsigned int j= 0; j < input.slices; ++j )
	if ( tmp(j, i, begin) )
	  tmp(j, i, begin)= 2;

    int removed= 0;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( int k= begin+step; (begin < end) ? (k < end) : (k > end); k+= step )
	{
	  if ( tmp(i,j,k) )
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp((i)*input.sliceSize + j*input.columns + k - step + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( int k= begin+step; (begin < end) ? (k < end) : (k > end); k+= step )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
  }
  
  
  CoronalCoherence::CoronalCoherence(int r)
  {
    this->radius= r;
  }
  
  CoronalCoherence::CoronalCoherence(const CoronalCoherence& a)
  {
    this->radius= a.radius;
  }
  
  CoronalCoherence::~CoronalCoherence()
  {
  }
  
  void CoronalCoherence::apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementAS sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step;
    if ( begin > end )
      step= -1;
    else
      step= 1;
    
    tmp= input;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.columns; ++j )
	if ( tmp(i, begin, j) )
	  tmp(i, begin, j)= 2;
    
    int removed= 0;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( int j= begin+step; (begin < end) ? (j < end) : (j > end); j+= step )
	for ( unsigned int k= 0; k < input.columns; ++k )
	{
	  if ( tmp(i,j,k) )
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp(i*input.sliceSize + (j-step)*input.columns + k + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( int j= begin+step; (begin < end) ? (j < end) : (j > end); j+= step )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
  }
  
    
  AxialCoherenceBorder::AxialCoherenceBorder(int r, int reverse)
  {
    this->radius= r;
    this->reverse= reverse;
  }
  
  AxialCoherenceBorder::AxialCoherenceBorder(const AxialCoherenceBorder& a)
  {
    this->radius= a.radius;
    this->reverse= a.reverse;
  }
  
  AxialCoherenceBorder::~AxialCoherenceBorder()
  {
  }
  
  void AxialCoherenceBorder::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementCS sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step, begin, end;
    if ( reverse)
    {
      step= -1;
      begin= input.slices-1;
      end= 0;
    }
    else
    {
      step= 1;
      begin= 0;
      end= input.slices;
    }
    
    tmp= input;
    
    for ( unsigned int i= 0; i < input.rows; ++i )
      for ( unsigned int j= 0; j < input.columns; ++j )
	for ( int k= begin; (begin < end) ? (k < end) : (k > end); k+= step )
	{
	  if ( tmp(k,i,j) )
	  {
	    tmp(k,i,j)= 2;
	    break;
	  }
	}
	
    int removed= 0;
    for ( int i= begin; (begin < end) ? (i < end) : (i > end); i+= step )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	{
	  if ( tmp(i,j,k) && tmp(i,j,k) != 2 )
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp((i-step)*input.sliceSize + j*input.columns + k + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( int i= begin; (begin < end) ? (i < end) : (i > end); i+= step )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
  }
  
  
  SaggitalCoherenceBorder::SaggitalCoherenceBorder(int r, int reverse)
  {
    this->radius= r;
    this->reverse= reverse;
  }
  
  SaggitalCoherenceBorder::SaggitalCoherenceBorder(const SaggitalCoherenceBorder& a)
  {
    this->radius= a.radius;
    this->reverse= a.reverse;
  }
  
  SaggitalCoherenceBorder::~SaggitalCoherenceBorder()
  {
  }
  
  void SaggitalCoherenceBorder::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementAC sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step, begin, end;
    if ( reverse )
    {
      step= -1;
      begin= input.columns-1;
      end= 0;
    }
    else
    {
      step= 1;
      begin= 0;
      end= input.columns;
    }
    
    tmp= input;
    for ( unsigned int k= 0; k < input.slices; ++k )
      for ( unsigned int i= 0; i < input.rows; ++i )
	for ( int j= begin; (begin < end) ? (j < end) : (j > end); j+= step )
	  if ( tmp(k, i, j) )
	  {
	    tmp(k, i, j)= 2;
	    break;
	  }
    
    int removed= 0;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( int k= begin; (begin < end) ? (k < end) : (k > end); k+= step )
	{
	  if ( tmp(i,j,k) && tmp(i,j,k) != 2)
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp((i)*input.sliceSize + j*input.columns + k - step + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( int k= begin; (begin < end) ? (k < end) : (k > end); k+= step )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
  }
  
  
  CoronalCoherenceBorder::CoronalCoherenceBorder(int r, int reverse)
  {
    this->radius= r;
    this->reverse= reverse;
  }
  
  CoronalCoherenceBorder::CoronalCoherenceBorder(const CoronalCoherenceBorder& a)
  {
    this->radius= a.radius;
    this->reverse= a.reverse;
  }
  
  CoronalCoherenceBorder::~CoronalCoherenceBorder()
  {
  }
  
  void CoronalCoherenceBorder::apply(Volume<unsigned char>& input, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    StructuringElementAS sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step, begin, end;
    if ( reverse )
    {
      step= -1;
      begin= input.rows-1;
      end= 0;
    }
    else
    {
      step= 1;
      begin= 0;
      end= input.rows-1;
    }
    
    tmp= input;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( unsigned int j= 0; j < input.columns; ++j )
	for ( int k= begin; (begin < end) ? (k < end) : (k > end); k+= step )
	  if ( tmp(i, k, j) )
	  {
	    tmp(i, k, j)= 2;
	    break;
	  }
    
    int removed= 0;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( int j= begin; (begin < end) ? (j < end) : (j > end); j+= step )
	for ( unsigned int k= 0; k < input.columns; ++k )
	{
	  if ( tmp(i,j,k) && tmp(i,j,k) != 2 )
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp(i*input.sliceSize + (j-step)*input.columns + k + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( unsigned int i= 0; i < input.slices; ++i )
      for ( int j= begin; (begin < end) ? (j < end) : (j > end); j+= step )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
  }
    
    
  SuccessiveAxialCoherence::SuccessiveAxialCoherence(int r, int periods)
  {
    this->r= r;
    this->periods= periods;
  }
  
  SuccessiveAxialCoherence::SuccessiveAxialCoherence(const SuccessiveAxialCoherence& s)
  {
    this->r= s.r;
    this->periods= s.periods;
  }
  
  SuccessiveAxialCoherence::~SuccessiveAxialCoherence()
  {
  }
  
  void SuccessiveAxialCoherence::apply(Volume<unsigned char>& input, Volume<unsigned char>& mask, Volume<unsigned char>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
  {
    int startIdx= -1;
    int stopIdx= -1;
    
    for ( unsigned int i= 0; i < mask.slices; ++i )
      if ( mask(i,0,0) )
      {
	startIdx= i;
	break;
      }
    for ( int i= mask.slices-1; i >= 0; --i )
      if ( mask(i,0,0) )
      {
	stopIdx= i;
	break;
      }
      
    AxialCoherence ac(r);
    
    Volume<unsigned char> tmp0;
    tmp0.resizeVolume(input);
    Volume<unsigned char> tmp1;
    tmp1.resizeVolume(input);
    
    if ( periods == -1 )
    {
      ac.apply(input, startIdx, stopIdx, tmp0);
      ac.apply(tmp0, stopIdx, startIdx, output);
    }
  }
  
  CircularCoherence::CircularCoherence(int r)
  {
    this->r= r;
  }
    
  CircularCoherence::CircularCoherence(const CircularCoherence& c)
  {
    this->r= c.r;
  }
  
  CircularCoherence::~CircularCoherence()
  {
  }
  
  void CircularCoherence::apply(Image<unsigned char>& /*input*/, int /*r*/, int /*c*/, Image<unsigned char>& /*output*/)
  {
    /*output= 0;
    float dr, dc;
    float resolution= 1000;
    printf("%d %d\n", r, c);
    for ( int i= 0; i < input.rows; ++i )
      for ( int j= 0; j < input.columns; ++j )
      {
	if ( input(i,j) )
	{
	  dr= (i - r)/resolution;
	  dc= (j - c)/resolution;
	  
	  int k;
	  for ( k= 0; k < resolution; ++k )
	  {
	    if ( !input(r + k*dr, c + k*dc) )
	      break;
	  }
	  if ( k != resolution )
	    output(i, j)= 0;
	  else
	    output(i, j)= 1;
	}
      }*/
    
    /*Image<unsigned char> tmp0;
    Image<unsigned char> tmp1;
    Image<unsigned char> tmp2;
    Image<unsigned char> tmp3;
    Image<unsigned char> tmp4;
    
    tmp0.resizeImage(input);
    tmp1.resizeImage(input);
    tmp2.resizeImage(input);
    tmp3.resizeImage(input);
    tmp4.resizeImage(input);
    
    GrayscaleDilate<unsigned char, unsigned char> gd(1);
    
    tmp0(r,c)= 1;
    tmp1(r,c)= 1;
    tmp4= 0;
    tmp4(r,c)= 1;
    for ( int i= 0; i < 90; ++i )
    {
      gd.apply(tmp0, tmp2);
      
      for ( int i= 0; i < tmp0.n; ++i )
      {
	if ( tmp2(i) && input(i) && !tmp3(i) )
	  tmp4(i)= 1;
      }
      
      gd.apply(tmp1, tmp3);
      tmp0= tmp4;
      tmp1= tmp3;
    }
    output= tmp4;
    printf("."); fflush(stdout);*/
    /*input.normalize(0, 255);
    output.normalize(0, 255);
    writeImage("before.bmp", input);
    writeImage("after.bmp", output);
    getchar();*/
  }
  
  bool pairCompare(const std::pair<int, int>& firstElem, const std::pair<int, int>& secondElem)
  {
    return firstElem.first > secondElem.first;
  }
  
  
  RegionCleaningOperator3::RegionCleaningOperator3(int r)
  {
    this->radius= r;
  }
  
  RegionCleaningOperator3::RegionCleaningOperator3(const RegionCleaningOperator3& a)
  {
    this->radius= a.radius;
  }
  
  RegionCleaningOperator3::~RegionCleaningOperator3()
  {
  }
  
  void RegionCleaningOperator3::apply(Volume<unsigned char>& input, int begin, int end, Volume<unsigned char>& tokeep, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    this->changed= 0;
    StructuringElementCS sec(radius);
    sec.updateStride(input.columns, input.sliceSize);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    
    int step;
    if ( begin > end )
      step= -1;
    else
      step= 1;
    
    tmp= input;
    for ( unsigned int i= 0; i < input.rows; ++i )
      for ( unsigned int j= 0; j < input.columns; ++j )
	if ( tmp(begin, i, j) )
	  tmp(begin, i, j)= 2;
    
    int removed= 0;
    for ( int i= begin+step; (begin < end) ? (i <= end) : (i >= end); i+= step )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	{
	  if ( tokeep(i,j,k) )
	    tmp(i,j,k)= 2;
	  else if ( tmp(i,j,k) )
	  {
	    unsigned int m;
	    for ( m= 0; m < sec.size(); ++m )
	    {
	      if (tmp((i-step)*input.sliceSize + j*input.columns + k + sec(m)) == 2)
		break;
	    }
	    if ( m != sec.size() )
	      tmp(i,j,k)= 2;
	    else
	      removed++;
	  }
	}

    tprintf("removed: %d\n", removed);
    output= input;
    for ( int i= begin+step; (begin < end) ? (i <= end) : (i >= end); i+= step )
      for ( unsigned int j= 0; j < input.rows; ++j )
	for ( unsigned int k= 0; k < input.columns; ++k )
	  if ( !roi || (*roi)(i, j, k) )
          {
	    if ( tmp(i, j, k) == 2 )
	      output(i, j, k)= 1;
	    else
	      output(i, j, k)= 0;
          }
	    
    for ( unsigned int i= 0; i < input.n; ++i )
      if ( input(i) != output(i) )
	this->changed+= 1;
  }
  
  void RegionCleaningOperator3::apply(Volume<unsigned char>& input, Volume<unsigned char>& tokeep, Volume<unsigned char>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
  {
    int first= -1, last;
    if ( roi )
    {
      last= (*roi).slices;
      for ( unsigned int i= 0; i < (*roi).slices && first == -1; ++i )
	for ( unsigned int j= 0; j < (*roi).rows && first == -1; ++j )
	  for ( unsigned int k= 0; k < (*roi).columns && first == -1; ++k )
	    if ( (*roi)(i,j,k) )
	    {
	      first= i;
	      break;
	    }

      for ( int i= int((*roi).slices)-1; i >= 0; --i )
	for ( unsigned int j= 0; j < (*roi).rows && last == int((*roi).slices); ++j )
	  for ( unsigned int k= 0; k < (*roi).columns && last == int((*roi).slices); ++k )
	    if ( (*roi)(i,j,k) )
	    {
	      last= i;
	      break;
	    }
    }
    else
    {
      first= 0;
      last= input.slices-1;
    }
    
    int middle= (int)(first + (last - first)*double(rand())/double(RAND_MAX));
    Image<unsigned char> slice0, slice1, slice2;
    input.getImage(slice0, 0, first);
    input.getImage(slice1, 0, last);
    input.getImage(slice2, 0, middle);
    if ( slice0.getMax() == 0 || slice1.getMax() == 0 || slice2.getMax() == 0 )
    {
      output= input;
      this->changed= 0;
      return;
    }
    
    tprintf("%d %d %d\n", first, last, middle);
    
    Volume<unsigned char> tmp;
    tmp.resizeVolume(input);
    tmp= 0;
    int c= 0;
    apply(input, first, middle, tokeep, output, roi, support);
    c+= changed;
    apply(output, middle, last, tokeep, tmp, roi, support);
    c+= changed;
    apply(tmp, last, middle, tokeep, output, roi, support);
    c+= changed;
    apply(output, middle, first, tokeep, tmp, roi, support);
    c+= changed;
    
    output= tmp;
    this->changed= c;
  }
  
  RegionCleaningOperator4::RegionCleaningOperator4()
  {
  }
    
  RegionCleaningOperator4::RegionCleaningOperator4(const RegionCleaningOperator4& /*a*/)
  {
  }
  
  RegionCleaningOperator4::~RegionCleaningOperator4()
  {
  }
  
  void RegionCleaningOperator4::apply(Volume<unsigned char>& input, Volume<unsigned char>& tokeep, Volume<unsigned char>& output, FramedVolume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
  {
    Image<unsigned char> sliceInput, sliceToKeep, sliceRoi;
    
    output= 0;
    
    this->changed= 0;

    for ( unsigned int i= 0; i < input.slices; ++i )
    {
      if ( !roi )
      {
	tokeep.getImage(sliceToKeep, 0, i);
	
	ExtractRegions er;
	Region2Set inputRegions, toKeepRegions;
	
	er.apply(sliceToKeep, toKeepRegions);
	if ( toKeepRegions.size() == 0 )
	  continue;
	
	input.getImage(sliceInput, 0, i);
	er.apply(sliceInput, inputRegions);
	sliceInput= 0;
	
	int n0= 0, n1= 0;
	for ( unsigned int j= 0; j < inputRegions.size(); ++j )
	  n0+= inputRegions(j).size();
	for ( unsigned int j= 0; j < toKeepRegions.size(); ++j )
	{
	  Region2Set rs= inputRegions.getIntersectingRegions(toKeepRegions(j));
	  for ( unsigned int k= 0; k < rs.size(); ++k )
	  {
	    rs(k).set(sliceInput, 0, (unsigned char)1);
	    n1+= rs(k).size();
	  }
	}
	output.setImage(sliceInput, 0, i);
	changed+= n0 - n1;
      }
      else
      {
	roi->getImage(sliceRoi, 0, i);
	//printf("a%d ", i); fflush(stdout);
	ExtractRegions er;
	Region2Set inputRegions, toKeepRegions, roiRegions;

	if ( sliceRoi.getMax() == 0 )
	{
	  input.getImage(sliceInput, 0, i);
	  output.setImage(sliceInput, 0, i);
	  continue;
	}
	/*sliceRoi.normalize(0, 255);
	writeImage("roislice.bmp", sliceRoi);
	getchar();
	er.apply(sliceRoi, roiRegions);
	printf("%d, %d ", sliceRoi.getMax(), roiRegions.size());
	if ( roiRegions.size() == 0 )
	{
	  input.getImage(sliceInput, 0, i);
	  output.setImage(sliceInput, 0, i);
	  continue;
	}*/

	tokeep.getImage(sliceToKeep, 0, i);
	for ( unsigned int j= 0; j < sliceToKeep.n; ++j )
	  if ( sliceToKeep(j) )
	    sliceToKeep(j)= 255;

	//printf("b "); fflush(stdout);
	er.apply(sliceToKeep, toKeepRegions);

	if ( toKeepRegions.size() == 0 )
	  continue;

	input.getImage(sliceInput, 0, i);
	for ( unsigned int j= 0; j < sliceInput.n; ++j )
	  if ( sliceInput(j) )
	    sliceInput(j)= 255;

	er.apply(sliceInput, inputRegions);
	//printf("c "); fflush(stdout);
	sliceInput= 0;
	
	Region2Set rs= inputRegions.getIntersectingRegions(toKeepRegions);
	
	for ( unsigned int k= 0; k < rs.size(); ++k )
	  rs(k).set(sliceInput, 0, (unsigned char)1);
	
	//printf("d "); fflush(stdout);
	output.setImage(sliceInput, 0, i);

      }
    }
    
    for ( unsigned int i= 0; i < input.n; ++i )
    {
      if ( tokeep(i) )
	output(i)= input(i);
      if ( input(i) != output(i) )
	++changed;
    }
    tprintf("removed: %d\n", this->changed);      
  }
}
