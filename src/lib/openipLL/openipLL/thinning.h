/**
 * @file thinning.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This file contains classes for iterative thinning methods.
 */

#ifndef _THINNING_H
#define	_THINNING_H

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>
#include <openipDS/BinaryMask2s.h>
#include <openipDS/BinaryMaskSet2.h>
#include <openipDS/BinaryMaskSystem2.h>
#include <openipDS/StructuringElement2s.h>
#include <openipLL/morphology.h>

namespace openip
{
    /**
     * CWSI87 method thinning mask #1
     */
    class CWSI87_1: public BinaryMask2
    {
    public:
        /**
         * constructor for CWSI87 method, thinning mask #1
         * @param stride stride of the images, on which this method will be applied
         * @param matches matches value
         * @param notMatches notMatches value
         */
        CWSI87_1(int stride= 2000, unsigned char matches= 255, unsigned char notMatches= 0);
    };

    /**
     * CWSI87 method mask thinning mask #2
     */
    class CWSI87_2: public BinaryMask2
    {
    public:
        /**
         * constructor for CWSI87 method, thinning mask #2
         * @param stride stride of the images on which this method will be applied
         * @param matches matches value
         * @param notMatches notMatches value
         */
        CWSI87_2(int stride= 2000, unsigned char matches= 255, unsigned char notMatches= 0);
    };

    /**
     * CWSI87 method restoring mask #1
     */
    class CWSI87_3: public BinaryMask2
    {
    public:
        /**
         * constructor for CWSI87 method, restoring mask #1
         * @param stride stride of the images on which this method will be applied
         * @param matches matches value
         * @param notMatches notMatches value
         */
        CWSI87_3(int stride= 2000, unsigned char matches= 255, unsigned char notMatches= 0);
    };

    /**
     * CWSI87 method restoring mask #2
     */
    class CWSI87_4: public BinaryMask2
    {
    public:
        /**
         * constructor for CWSI87 method, restoring mask #2
         * @param stride stride of the images on which thi method will be applied
         * @param matches matches value
         * @param notMatches notMatches value
         */
        CWSI87_4(int stride= 2000, unsigned char matches= 255, unsigned char notMatches= 0);
    };

    /**
    * matches if the number of foreground pixels is greater or equal to b in the 3x3 environment of the pixel
    */
    class BinaryMask2B: public BinaryMask2
    {
    public:
	/**
	* B mask constructor
	* @param stride stride of the image to use with
	* @param b integer to compare to
	*/
        BinaryMask2B(int stride, int b);

	/**
	* matches the filter in image Image at position n
	* @param image input image
	* @param n position to match in
	* @return matches value if matches, else notMatches value
	*/
        unsigned char match(Image<unsigned char>& image, int n);

	/**
	* matches the extracted enviroment
	* @param env environment to match
	* @return matches value if matches, else notMatches value
	*/
        unsigned char match(unsigned int env);

	/**
	* lower bound for matching
	*/
        int b;
    };

    /**
    * matches if the number of 0-1 changes in the 3x3 environment of the pixel is c
    */
    class BinaryMask2C: public BinaryMask2
    {
    public:
	/**
	* constructor for mask C
	* @param stride stride of the image to use with
	* @param c number of changes to match
	*/
        BinaryMask2C(int stride, int c);

	/**
	* matches the mask in image Image at position n
	* @param image to match the mask in
	* @param n position to match at
	* @return matches value if matches, otherwise notMatches value
	*/
        unsigned char match(Image<unsigned char>& image, int n);

	/**
	* matches the extracted environment
	* @param env extracted environment
	* @return matches value if matches, otherwise notMatches value
	*/
        unsigned char match(unsigned int env);

	/**
	* integer constant number of 0-1 changes to match
	*/
        int c;
    };

    /**
    * matches wether the number of foreground 4-neighbors is d
    */
    class BinaryMask2D: public BinaryMask2
    {
    public:
	/**
	* constructor for mask D
	* @param stride stride of the images to use with
	* @param d number of 4-connected foreground pixels to match
	*/
        BinaryMask2D(int stride, int d);

	/**
	* matches the mask at position n
	* @param image image to match in
	* @param n position to match at
	* @return matches value if matches, otherwise notMatches value
	*/
        unsigned char match(Image<unsigned char>& image, int n);

	/**
	* matches the extracted environment
	* @param env extracted environment
	* @return matches value if matches, otherwise notMatches value
	*/
        unsigned char match(unsigned int env);

	/**
	* integer constant, number of 4-connected foreground neigbors to match
	*/
        int d;
    };

    /**
     * CWSI thinning method
     * @param input input image
     * @param output output image
     */
    void thinningCWSI(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* roi= NULL);
    
    template<typename T>
    void thinningCWSIminimum(Image<unsigned char>& input, Image<T>& colors, Image<unsigned char>& output, Image<unsigned char>* roi= NULL)
    {
      BinaryMaskSet2 thinning(255, 0, BinaryMaskSet2::MATCH_ANY);
      BinaryMaskSet2 restore(255, 0, BinaryMaskSet2::MATCH_NONE);

      CWSI87_1 th11;
      CWSI87_1 th12(th11);
      th12.rotate90();
      CWSI87_1 th13(th12);
      th13.rotate90();
      CWSI87_1 th14(th13);
      th14.rotate90();
      CWSI87_2 th21;
      CWSI87_2 th22(th21);
      th22.rotate90();
      CWSI87_2 th23(th22);
      th23.rotate90();
      CWSI87_2 th24(th23);
      th24.rotate90();

      thinning.push_back(&th11);
      thinning.push_back(&th12);
      thinning.push_back(&th13);
      thinning.push_back(&th14);
      thinning.push_back(&th21);
      thinning.push_back(&th22);
      thinning.push_back(&th23);
      thinning.push_back(&th24);

      CWSI87_3 th3;
      CWSI87_4 th4;

      restore.push_back(&th3);
      restore.push_back(&th4);

      BinaryMaskSystem2 bms;
      bms.push_back(&thinning);
      bms.push_back(&restore);

      bms.generateLookUpTable();
      
      int tmp;

      PixelSet1 out;
      output= input;
      while(1)
      {
	  tmp= 0;
	  bms.match(output, out, &output);

	  if ( out.size() == 0 )
	      break;	  
	  
	  int maximum= 0;
	  for ( unsigned int j= 0; j < out.size(); ++j )
	    if ( colors(out(j)) > colors(out(maximum)) )
	      maximum= j;
	  //out.set(output, (unsigned char)0);
	  output(out(maximum))= 0;
	  
	  /*if ( roi )
	  {
	    PixelSet1 tmp;
	    for ( int i= 0; i < out.size(); ++i )
	      if ( (*roi)(out(i)) )
		tmp.push_back(out(i));
	    out= tmp;
	  }*/
      }
      
      Vector<int> pos;
      pos.push_back(1);
      pos.push_back(-input.columns + 1);
      pos.push_back(-input.columns);
      pos.push_back(-input.columns - 1);
      pos.push_back(-1);
      pos.push_back(input.columns - 1);
      pos.push_back(input.columns);
      pos.push_back(input.columns + 1);
      
      while (1)
      {
	int n= 0;
	int maximum= -1;
	int maxValue= 100000;
	for ( int i= input.columns + 1; i < input.n - input.columns-1; ++i )
	{
	  if ( output(i) && output(i + pos(0)) && !output(i + pos(1)) && output(i + pos(2)) && !output(i + pos(3)) && !output(i + pos(4)) && !output(i + pos(5)) && !output(i + pos(6)) && ! output(i + pos(7)) )
	  {
	    if ( colors(i) < maxValue )
	    {
	      maxValue= colors(i);
	      maximum= i;
	    }
	    ++n;
	  }
	  else if ( output(i) && !output(i + pos(0)) && !output(i + pos(1)) && output(i + pos(2)) && !output(i + pos(3)) && output(i + pos(4)) && !output(i + pos(5)) && !output(i + pos(6)) && ! output(i + pos(7)) )
	  {
	    if ( colors(i) < maxValue )
	    {
	      maxValue= colors(i);
	      maximum= i;
	    }

	    ++n;
	  }
	  else if ( output(i) && !output(i + pos(0)) && !output(i + pos(1)) && !output(i + pos(2)) && !output(i + pos(3)) && output(i + pos(4)) && !output(i + pos(5)) && output(i + pos(6)) && ! output(i + pos(7)) )
	  {
	    if ( colors(i) < maxValue )
	    {
	      maxValue= colors(i);
	      maximum= i;
	    }
	    ++n;
	  }
	  else if ( output(i) && output(i + pos(0)) && !output(i + pos(1)) && !output(i + pos(2)) && !output(i + pos(3)) && !output(i + pos(4)) && !output(i + pos(5)) && output(i + pos(6)) && ! output(i + pos(7)) )
	  {
	    if ( colors(i) < maxValue )
	    {
	      maxValue= colors(i);
	      maximum= i;
	    }
	    ++n;
	  }
	}

	if ( maximum == -1 )
	  break;
	output(maximum)= 0;
      }
    }
    
    void thinningCWSI1(Image<unsigned char>& input, Image<unsigned char>& output);

    void gradientThinningCWSI(Image<float>& image, Image<unsigned char>& input, Image<unsigned char>& output);

    void gradientThinningCWSI1(Image<float>& image, Image<unsigned char>& input, Image<unsigned char>& output);
}

#endif	/* _THINNING_H */

