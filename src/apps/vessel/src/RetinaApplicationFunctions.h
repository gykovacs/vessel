#ifndef RETINA_APPLICATION_FUNCTIONS_H
#define RETINA_APPLICATION_FUNCTIONS_H

EqualSlopeAtBoundaries<float>* gesab= NULL;
float calculateWidth(Image<unsigned char>& labels, Image<float>& input, int i)
{
  if ( gesab == NULL )
  {
    gesab= new EqualSlopeAtBoundaries<float>(3, 87, 0.4, 1);
    gesab->updateStride(labels.columns);
  }

  float width= gesab->swidthc(labels, i, input);
  
  return width;
}

float calculateRI(Image<unsigned char>& labels, Image<float>& input, int i)
{
  if ( gesab == NULL || gesab->stride != labels.columns )
  {
    gesab= new EqualSlopeAtBoundaries<float>(3, 87, 0.4, 1);
    gesab->updateStride(labels.columns);
  }

  float width, dyn, ri;
  gesab->computeDescriptors(labels, i, input, width, dyn, ri);
  
  return ri;
}

float roiRadius(Image<unsigned char>& roi)
{
  Vector<float> radii;
  
  float numroi0= roi.numberOfNonZeroElements();
    
  float r0= 0, c0= 0;
  for ( int i= 0; i < roi.rows; ++i )
    for ( int j= 0; j < roi.columns; ++j )
      if ( roi(i,j) )
      {
	r0+= i;
	c0+= j;
      }
  r0/= numroi0;
  c0/= numroi0;
    
  for ( int i= 1; i < roi.rows-1; ++i )
    for ( int j= 1; j < roi.columns-1; ++j )
      if ( roi(i,j) && Region2::isInnerContour8(roi, i*roi.columns+j) )
      {
	radii.push_back(sqrt((r0 - i)*(r0 - i) + (c0 - j)*(c0 - j)));
      }
      
  Vector<float> lradii;
  sort(radii.begin(), radii.end());
  for ( unsigned int i= radii.size()*0.9; i < radii.size(); ++i )
    lradii.push_back(radii(i));
  
  return lradii.getMedian();
}

void reduceROI(Image<float>& input, Image<unsigned char>& output)
{
  Image<unsigned char> roiOriginal;
  roiOriginal= output;

  Vector<StructuringElementDisk> vs;
  for ( int i= 5; i < 30; i+= 5 )
  {
    vs.push_back(StructuringElementDisk(i));
    vs(vs.size()-1).updateStride(input.columns);
  }
  
  Image<float> closed;
  closed.resizeImage(input);
  Image<float> opened;
  opened.resizeImage(input);
  
  Image<unsigned char> tmp;
  tmp.resizeImage(input);
  tmp= 0;
  for ( int i= vs.size()-1; i >= 0; --i )
  {
    tprintf("dilate\n");
    grayscaleDilate(&input, &closed, vs(i));
    tprintf("erode\n");
    grayscaleErode(&closed, &opened, vs(i));
    
    Vector<float> tosort;
    for ( unsigned int j= 0; j < closed.n; ++j )
      if ( output(j) && !tmp(j) )
	tosort.push_back(opened(j) - input(j));
    sort(tosort.begin(), tosort.end());
    float th= tosort(tosort.size()*0.9);
    for ( unsigned int j= 0; j < closed.n; ++j )
      if ( output(j) && opened(j) - input(j) > th )
	tmp(j)= 255;
  }

  output= tmp;
  //writeImage("rr0.bmp", output);
  
  ExtractRegions er;
  Region2Set regions;
  tprintf("extracting regions\n");
  er.apply(output, regions);
  tprintf("region extraction finished\n");
  
  for ( int i= input.n/2; i>= 2; i/= 1.5 )
  {
    Region2Set r= regions.getRegionsLargerThan(i);
    int size= 0;
    for ( unsigned int i= 0; i < r.size(); ++i )
      size+= r(i).size();
    //tprintf("%d %d %d %d\n", size, int(input.n*0.15), i, r.size());
    if ( size > input.n*0.15 )
    {
      output= 0;
      for ( unsigned int i= 0; i < r.size(); ++i )
	  r(i).set(output, 0, (unsigned char)255);
      break;
    }
  }
  
  //writeImage("rr1.bmp", output);
  
  StructuringElementDisk sed3(3);
  StructuringElementDisk sed2(3);
  sed3.updateStride(input.columns);
  sed2.updateStride(input.columns);
  tmp= 0;
  grayscaleDilate(&output, &tmp, sed3);
  output= tmp;
  grayscaleErode(&output, &tmp, sed2);
  
  regions.clear();
  er.apply(tmp, regions);
  
  for ( int i= input.n/2; i>= 2; i/= 1.5 )
  {
    Region2Set r= regions.getRegionsLargerThan(i);
    int size= 0;
    for ( unsigned int i= 0; i < r.size(); ++i )
	size+= r(i).size();
    //tprintf("%d %d %d %d\n", size, int(input.n*0.15), i, r.size());
    if ( size > input.n*0.15 )
    {
      output= 0;
      for ( unsigned int i= 0; i < r.size(); ++i )
	  r(i).set(output, 0, (unsigned char)255);
      break;
    }
  }
  
  //writeImage("rr2.bmp", output);
  
  StructuringElementDisk sed22(3);
  sed22.updateStride(input.columns);
  grayscaleDilate(&output, &tmp, sed2);
  output= tmp;
}

template<typename INPUT, typename OUTPUT>
void gaussianScaling(Image<INPUT>& input, Image<OUTPUT>& output)
{
  //tprintf("gaussian scaling started\n"); 
  Border2 ob= input.getBorder2();
  //tprintf("input border extracted\n");
  GaussianFilter2<INPUT, INPUT> filter(sqrt(float(input.rows)/output.rows));
  //tprintf("Gaussian filter generated\n");
  Border2 b= filter.getProposedBorder2();
  //tprintf("Gaussian border extracted\n");
  b.borderMode= BORDER_MODE_MIRRORED;
  //tprintf("proposed border: %d %d %d %d\n", b.leftBorder, b.rightBorder, b.topBorder, b.bottomBorder);
  
  input.resizeBorder(b);
  output.resizeBorder(b);
  
  Image<INPUT> filtered;
  filtered.resizeImage(input);
  
  filter.updateStride(input.columns);
  //tprintf("scaling: %d %d %d\n", input.columns, filtered.columns, output.columns);
  
  
  
  filter.apply(input, filtered);
  //tprintf("filter applied\n");
  //writeImage("filtered.bmp", filtered);
  
    float dr, dc;
    dc= float(input.columns - input.leftBorder - input.rightBorder)/(output.columns - output.leftBorder - output.rightBorder);
    dr= float(input.rows - input.topBorder - input.bottomBorder)/(output.rows - output.topBorder - output.bottomBorder);
    INPUT min, max;
    input.getMinMax(min, max);

    float tdr= (output.topBorder) * dr;
    float tdc= (output.leftBorder) * dc;

    //tprintf("mocking\n");
    //#pragma omp parallel for private(p, ca, cb, ra, rb, tdr, tdc), firstprivate(oldca)
    for ( int r= output.topBorder; r < output.rows - output.topBorder; ++r )
    {
	tdr= (r - output.topBorder) * dr;
	tdc= 0;
	for ( int c= output.leftBorder; c < output.columns - output.rightBorder; ++c )
	{
	    output(r, c)= filtered(filtered.topBorder + floor(tdr + 0.5), filtered.leftBorder + floor(tdc + 0.5));
	    tdc+= dc;
	}
    }
    //tprintf("mocking finished\n");

    output.fillBorder();
    
    input.removeBorder();
    output.removeBorder();
    ob.borderMode= BORDER_MODE_MIRRORED;
    input.resizeBorder(ob);
    output.resizeBorder(ob);
    
    //writeImage("o.bmp", output);
}

#endif
