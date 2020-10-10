#ifndef BINARY_IMAGE_OPERATIONS_H
#define BINARY_IMAGE_OPERATIONS_H

#include <openipDS/Image.h>
#include <openipDS/Region2.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/StructuringElement2s.h>

namespace openip
{
  PixelSet1 getEndPoints(Image<unsigned char>& input);
  
  int numberOf8Neighbors(Image<unsigned char>& input, int n);
}

#endif