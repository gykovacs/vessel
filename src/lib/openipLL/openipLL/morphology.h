/**
 * @file morphology.h
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
 * Some common morphological operators.
 */

#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_

#include <vector>
#include <algorithm>
#include <iostream>

#include <openipDS/Image.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/PixelSequence1.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/PriorityQueue.h>
#include <openipDS/Volume.h>

#include <openipLL/ComponentLabeling.h>

#include <limits.h>
using namespace std;

namespace openip
{
    /**
    * dilates the pixel sequence input with structuring element se
    * @param input input pixel sequence
    * @param se structuring element
    * @return dilated pixel sequence
    */
    PixelSequence1 binaryDilate(PixelSequence1& input, StructuringElement2& se);
    
    /**
    * dilates the pixel set input with structuring element se
    * @param input input pixel set
    * @param se structuring element
    * @return dilated pixel set
    */
    PixelSet1 binaryDilate(PixelSet1& input, StructuringElement2& se);

    /**
    * erodes the pixel sequence input with structuring element se
    * @param input input pixel sequence
    * @param se structuring element
    * @return eroded pixel sequence
    */
    PixelSequence1 binaryErode(PixelSequence1& input, StructuringElement2& se);
    
    /**
    * erodes the pixel set input with structuring element se
    * @param input input pixel set
    * @param se structuring element
    * @return eroded pixel set
    */
    PixelSet1 binaryErode(PixelSet1& input, StructuringElement2& se);

    /**
    * erodes the binary image input into binary image output using structuring element se
    * in the foreground (non 0) region of the mask 
    * @param input input binary image
    * @param output output binary image
    * @param se structuring element
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    void binaryErode(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, Image<unsigned char>* mask= NULL);

    /**
    * erodes the binary image input into binary image output using structuring element se
    * in the foreground (non 0) region of the mask
    * @param input input binary image
    * @param output output binary image
    * @param se structuring element
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */

    /**
     * erodes the binary image input
     * @param input input binary image
     * @param se structuring element
     */
    void binaryErode(Image<unsigned char>& input, StructuringElement2& se);

    /**
     * binary erosion and reconstruction
     * @param input input image
     * @param output output image
     * @param se structuring element
     * @param mask the operation is performed in the foreground (non-zero) region of the mask
     */
    void binaryErodeAndReconstruct(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, Image<unsigned char>* mask= NULL);
    
    /**
    * dilates the binary image input into binary image output using structuring element se
    * in the foreground (non 0) region of the mask
    * @param input input binary image
    * @param output output binary image
    * @param se structuring element
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    void binaryDilate(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, Image<unsigned char>* mask= NULL);

    /**
    * erodes the grayscale image input into grayscale image output using structuring element se
    * in the foreground (non 0) region of the mask
    * @param input input grayscale image
    * @param output output grayscale image
    * @param se structuring element
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template<typename INPUT, typename OUTPUT>
    void grayscaleErode(Image<INPUT>* input, Image<OUTPUT>* output, StructuringElement2& se, Image<unsigned char>* mask= NULL)
    {
    int start= - se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            (*output)(i)= se.getMin(*input, i);
        }
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                INPUT min= numeric_limits<INPUT>::max();
                for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*mask)(i + *sit) > 0 && (*input)(i + *sit) < min )
                        min= (*input)(i + *sit);
                (*output)(i)= min;
            }
    }
    }


    
    /**
    * dilates the grayscale image input into grayscale image output using structuring element se
    * in the foreground (non 0) region of the mask
    * @param input input grayscale image
    * @param output output grayscale image
    * @param se structuring element
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template<typename INPUT, typename OUTPUT>
    void grayscaleDilate(Image<INPUT>* input, Image<OUTPUT>* output, StructuringElement2& se, Image<unsigned char>* mask= NULL)
    {
    int start= - se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            (*output)(i)= se.getMax(*input, i);
        }
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                //INPUT max= -numeric_limits<INPUT>::max();
	        //INPUT max= numeric_limits<INPUT>::min();
	      INPUT max= 0;
                for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*mask)(i + *sit) > 0 && (*input)(i + *sit) > max )
                        max= (*input)(i + *sit);
                (*output)(i)= max;
            }
    }
    }



    /**
    * binary opening method
    * @param input input binary image
    * @param output output binary image
    * @param se structuring element
    * @param c cache object for temporary images
    * @param mask the operation is performed in the foreground region of the mask
    */
    void binaryOpen(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, int* c, Image<unsigned char>* mask= NULL);
    
    /**
    * binary closing method
    * @param input input binary image
    * @param output output binary image
    * @param se structuring element
    * @param c cache object for temporary images
    * @param mask the operation is performed in the foreground region of the mask
    */
    void binaryClose(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, int* c, Image<unsigned char>* mask= NULL);

    /**
    * grayscale opening method
    * @param input input grayscale image
    * @param output outptu grayscale image
    * @param se structuring element
    * @param c cache object for temporary images
    * @param mask the operation is performed in the foreground region of the mask
    */
    /*template<typename INPUT, typename OUTPUT>
    void grayscaleOpen(Image<INPUT>* input, Image<OUTPUT>* output, StructuringElement2& se, int* c, Image<unsigned char>* mask= NULL)
    {
    Image<unsigned char>* tmp;
    c->get(&tmp);
    grayscaleErode(input, tmp, se, mask);
    grayscaleDilate(tmp, output, se, mask);
    c->put(tmp);
    }*/
    
    /**
    * grayscale closing method
    * @param input input grayscale image
    * @param output output grayscale image
    * @param se structuring element
    * @param c cache object for temporary images
    * @param mask the operation is performed in the foreground region of the mask
    */
    /*template<typename INPUT, typename OUTPUT>
    void grayscaleClose(Image<INPUT>* input, Image<OUTPUT>* output, StructuringElement2& se, int* c, Image<unsigned char>* mask= NULL)
    {
    Image<unsigned char>* tmp;
    c->get(&tmp);
    grayscaleDilate(input, tmp, se, mask);
    grayscaleErode(tmp, output, se, mask);
    c->put(tmp);
    }*/

    template<typename INPUT, typename OUTPUT>
    void areaOpening(Image<INPUT>& input, Image<OUTPUT>& output, int area, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<unsigned char> tmp;
        Image<unsigned char> flag;
        tmp.resizeImage(input);
        flag.resizeImage(input);
        tmp= 0;
        flag= 0;

        for ( unsigned int i= input.columns + 1; i < input.n - input.columns - 1; ++i )
            if ( !roi || (*roi)(i) > 0 )
                if ( input(i) <= input(i + 1) && input(i) <= input(i - 1) && input(i) <= input(i - input.columns) && input(i) <= input(i + input.columns) )
                    tmp(i)= 255;

        ComponentLabeling cl;
        Image<int> label;
        label.resizeImage(input);
        label= 0;
        cl.apply(tmp, label);

        int max= 0;
        for ( unsigned int i= 0; i < label.n; ++i )
            if ( label(i) > max )
                max= label(i);

        Vector<PriorityQueue<INPUT, int> > queues;
        Vector<Vector<int> > regions;
        Vector<int> sizes;
        queues.resize(max + 1);
        regions.resize(max + 1);
        sizes.resize(max + 1);
        sizes= 0;

        for ( unsigned int i= 0; i < label.n; ++i )
            if ( label(i) > 0 )
                sizes(label(i))++;
        for ( unsigned int i= 0; i < label.n; ++i )
            if ( sizes(label(i)) <= area && label(i) > 0 )
                queues(label(i)).push(-input(i), i);

        int pos;
        //INPUT init;
        INPUT last;

        output= input;
        StructuringElementSquare ses(3);
        ses.updateStride(input.columns);
        StructuringElementSquare::iterator it;

        flag= 0;
        for ( unsigned int i= 1; i < queues.size(); ++i )
        {
            if ( sizes(i) > area )
                continue;
            
            for ( unsigned int l= 0; l < queues(i).size(); ++l )
                flag(queues(i)[l].second)= 255;
            //init= (INPUT)(INT_MAX);
            if ( queues(i).size() > 0 )
            {
                int j= -queues(i).topP();
                //init = j;
                last= j;
                //printf("%d/%d\t", i, queues.size()); fflush(stdout);
                while ( 1 )
                {
                    if ( queues(i).size() == 0 )
                        break;
                    //printf("."); fflush(stdout);
                    pos= queues(i).top();
                    last= j;
                    j= -queues(i).topP();

                    if ( j < last || queues(i).size() <= 0 || int(regions(i).size()) > area )
                    {
                        for ( unsigned int k= 0; k < regions(i).size(); ++k )
                            output(regions(i)(k))= last;
                        break;
                    }

                    for ( it= ses.begin(); it != ses.end(); ++it )
                    {
                        if ( (!roi || (pos + *it >= 0 && pos + *it < int(input.size()) && (*roi)(pos + *it) > 0) ) )
                            if ( flag(pos + *it) == 0 )
                            {
                                queues(i).push(-input(pos + *it), pos + *it);
                                flag(pos + *it)= 255;
                            }
                    }

                    regions(i).push_back(pos);
                    
                    queues(i).pop();
                }
                for ( unsigned int k= 0; k < regions(i).size(); ++k )
                    tmp(regions(i)(k))= 0;
                for ( unsigned int k= 0; k < queues(i).size(); ++k )
                    tmp(queues(i)[k].second)= 0;
            }
        }
    }


    template<typename INPUT, typename OUTPUT>
    class GrayscaleAreaOpening: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleAreaOpening(int area);

        GrayscaleAreaOpening(const GrayscaleAreaOpening& g);

        ~GrayscaleAreaOpening();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleAreaOpening<INPUT, OUTPUT>::GrayscaleAreaOpening(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleAreaOpening " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAreaOpening<INPUT, OUTPUT>::GrayscaleAreaOpening(const GrayscaleAreaOpening& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAreaOpening<INPUT, OUTPUT>::~GrayscaleAreaOpening()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleAreaOpening<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        areaOpening(input, output, area, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void areaClosing(Image<INPUT>& input, Image<OUTPUT>& output, int area, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<unsigned char> tmp;
        Image<unsigned char> flag;
        Image<int> label;
        tmp.resizeImage(input);
        flag.resizeImage(input);
        label.resizeImage(input);
        tmp= 0;
        flag= 0;
        label= 0;

        for ( unsigned int i= input.columns + 1; i < input.n - input.columns - 1; ++i )
            if ( !roi || (*roi)(i) > 0 )
                if ( input(i) >= input(i + 1) && input(i) >= input(i - 1) && input(i) >= input(i - input.columns) && input(i) >= input(i + input.columns) )
                    tmp(i)= 255;

        ComponentLabeling cl;
        cl.apply(tmp, label);

        int max= 0;
        for ( unsigned int i= 0; i < label.n; ++i )
            if ( label(i) > max )
                max= label(i);

        Vector<PriorityQueue<INPUT, int> > queues;
        Vector<Vector<int> > regions;
        Vector<int> sizes;
        queues.resize(max + 1);
        regions.resize(max + 1);
        sizes.resize(max + 1);
        sizes= 0;

        for ( unsigned int i= 0; i < label.n; ++i )
            if ( label(i) > 0 )
                sizes(label(i))++;
        for ( unsigned int i= 0; i < label.n; ++i )
            if ( sizes(label(i)) <= area && label(i) > 0 )
                queues(label(i)).push(input(i), i);

        int pos;
        //INPUT init;
        INPUT last;

        output= input;
        StructuringElementSquare ses(3);
        ses.updateStride(input.columns);
        StructuringElementSquare::iterator it;

        flag= 0;
        for ( unsigned int i= 1; i < queues.size(); ++i )
        {
            if ( sizes(i) > area )
                continue;

            for ( unsigned int l= 0; l < queues(i).size(); ++l )
                flag(queues(i)[l].second)= 255;
            //init= (INPUT)(INT_MAX);
            
            //printf("a"); fflush(stdout);
            if ( queues(i).size() > 0 )
            {
                INPUT j= queues(i).topP();
                //init = j;
                last= j;

                //printf("e"); fflush(stdout);
                while ( 1 )
                {
                    if ( queues(i).size() == 0 )
                        break;
                    
                    pos= queues(i).top();
                    last= j;
                    j= queues(i).topP();

                    //printf("f"); fflush(stdout);
                    if ( j > last || queues(i).size() <= 0 || int(regions(i).size()) > area )
                    {
                        for ( unsigned int k= 0; k < regions(i).size(); ++k )
                            output(regions(i)(k))= last;
                        break;
                    }

                    //printf("g"); fflush(stdout);
                    for ( it= ses.begin(); it != ses.end(); ++it )
                    {
                        if ( !roi || ( pos + *it >= 0 && pos + *it < int(input.n ) && (*roi)(pos + *it) > 0) )
                            if ( flag(pos + *it) == 0 )
                            {
                                queues(i).push(input(pos + *it), pos + *it);
                                flag(pos + *it)= 255;
                            }
                    }

                    //printf("h"); fflush(stdout);
                    regions(i).push_back(pos);
                    //printf("i"); fflush(stdout);
                    queues(i).pop();
                    //printf("j"); fflush(stdout);
                }
                //printf("b"); fflush(stdout);
                for ( unsigned int k= 0; k < regions(i).size(); ++k )
                    tmp(regions(i)(k))= 0;
                //printf("c"); fflush(stdout);
                for ( unsigned int k= 0; k < queues(i).size(); ++k )
                    tmp(queues(i)[k].second)= 0;
                //printf("d"); fflush(stdout);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleAreaClosing: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleAreaClosing(int area);

        GrayscaleAreaClosing(const GrayscaleAreaClosing& g);

        ~GrayscaleAreaClosing();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleAreaClosing<INPUT, OUTPUT>::GrayscaleAreaClosing(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleAreaClosing " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAreaClosing<INPUT, OUTPUT>::GrayscaleAreaClosing(const GrayscaleAreaClosing& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAreaClosing<INPUT, OUTPUT>::~GrayscaleAreaClosing()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleAreaClosing<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        areaClosing(input, output, area, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class BlackAreaTopHat: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        BlackAreaTopHat(int area);

        BlackAreaTopHat(const BlackAreaTopHat& g);

        ~BlackAreaTopHat();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    BlackAreaTopHat<INPUT, OUTPUT>::BlackAreaTopHat(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "BlackAreaTopHat " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BlackAreaTopHat<INPUT, OUTPUT>::BlackAreaTopHat(const BlackAreaTopHat& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    BlackAreaTopHat<INPUT, OUTPUT>::~BlackAreaTopHat()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BlackAreaTopHat<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        areaClosing(input, output, area, roi, support);
        if ( !roi )
            for ( unsigned int i= 0; i < input.n; ++i )
                output(i)= output(i) - input(i);
        else
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= output(i) - input(i);
                else
                    output(i)= 0;
    }

    template<typename INPUT, typename OUTPUT>
    class WhiteAreaTopHat: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        WhiteAreaTopHat(int area);

        WhiteAreaTopHat(const WhiteAreaTopHat& g);

        ~WhiteAreaTopHat();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    WhiteAreaTopHat<INPUT, OUTPUT>::WhiteAreaTopHat(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "WhiteAreaTopHat " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    WhiteAreaTopHat<INPUT, OUTPUT>::WhiteAreaTopHat(const WhiteAreaTopHat& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    WhiteAreaTopHat<INPUT, OUTPUT>::~WhiteAreaTopHat()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void WhiteAreaTopHat<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        areaOpening(input, output, area, roi, support);
        if ( !roi )
            for ( unsigned int i= 0; i < output.n; ++i )
                output(i)= input(i) - output(i);
        else
            for ( unsigned int i= 0; i < output.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= input(i) - output(i);
                else
                    output(i)= 0;
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleOpen: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleOpen(int area);

        GrayscaleOpen(const GrayscaleOpen& g);

        ~GrayscaleOpen();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleOpen<INPUT, OUTPUT>::GrayscaleOpen(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleOpen " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleOpen<INPUT, OUTPUT>::GrayscaleOpen(const GrayscaleOpen& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleOpen<INPUT, OUTPUT>::~GrayscaleOpen()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleOpen<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        StructuringElementDisk se(area);
        se.updateStride(input.columns);

        grayscaleDilate(&input, &tmp, se, roi);
        grayscaleErode(&tmp, &output, se, roi);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleOpen<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(area, area, area, area);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleClose: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleClose(int area);

        GrayscaleClose(const GrayscaleClose& g);

        ~GrayscaleClose();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleClose<INPUT, OUTPUT>::GrayscaleClose(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleClose " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleClose<INPUT, OUTPUT>::GrayscaleClose(const GrayscaleClose& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleClose<INPUT, OUTPUT>::~GrayscaleClose()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleClose<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(area, area, area, area);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleClose<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        StructuringElementDisk se(area);
        se.updateStride(input.columns);

        grayscaleErode(&input, &tmp, se, roi);
        grayscaleDilate(&tmp, &output, se, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleWhiteTopHat: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleWhiteTopHat(int area);

        GrayscaleWhiteTopHat(const GrayscaleWhiteTopHat& g);

        ~GrayscaleWhiteTopHat();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleWhiteTopHat<INPUT, OUTPUT>::GrayscaleWhiteTopHat(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleWhiteTopHat " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleWhiteTopHat<INPUT, OUTPUT>::GrayscaleWhiteTopHat(const GrayscaleWhiteTopHat& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleWhiteTopHat<INPUT, OUTPUT>::~GrayscaleWhiteTopHat()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleWhiteTopHat<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(area, area, area, area);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleWhiteTopHat<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GrayscaleOpen<INPUT, OUTPUT> go(area);
        go.apply(input, output, roi, support);
        if ( !roi )
            for ( unsigned int i= 0; i < output.n; ++i )
                output(i)= input(i) - output(i);
        else
            for ( unsigned int i= 0; i < output.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= input(i) - output(i);
                else
                    output(i)= 0;
    }



    template<typename INPUT, typename OUTPUT>
    class GrayscaleBlackTopHat: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleBlackTopHat(int area);

        GrayscaleBlackTopHat(const GrayscaleBlackTopHat& g);

        ~GrayscaleBlackTopHat();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleBlackTopHat<INPUT, OUTPUT>::GrayscaleBlackTopHat(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleBlackTopHat " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBlackTopHat<INPUT, OUTPUT>::GrayscaleBlackTopHat(const GrayscaleBlackTopHat& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBlackTopHat<INPUT, OUTPUT>::~GrayscaleBlackTopHat()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleBlackTopHat<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(area, area, area, area);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleBlackTopHat<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GrayscaleClose<INPUT, OUTPUT> gc(area);
        gc.apply(input, output, roi, support);
        if ( !roi )
            for ( unsigned int i= 0; i < input.n; ++i )
                output(i)= output(i) - input(i);
        else
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= output(i) - input(i);
                else
                    output(i)= 0;
    }



    template<typename INPUT, typename OUTPUT>
    class GrayscaleDilate: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleDilate(int radius);

        GrayscaleDilate(const GrayscaleDilate& g);

        ~GrayscaleDilate();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleDilate<INPUT, OUTPUT>::GrayscaleDilate(int radius)
    : Transform2<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleDilate " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDilate<INPUT, OUTPUT>::GrayscaleDilate(const GrayscaleDilate& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDilate<INPUT, OUTPUT>::~GrayscaleDilate()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleDilate<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleDilate<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* /*support*/)
    {
        StructuringElementDisk sed(radius);
        sed.updateStride(input.columns);
        grayscaleDilate(&input, &output, sed, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleErode: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleErode(int radius);

        GrayscaleErode(const GrayscaleErode& g);

        ~GrayscaleErode();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleErode<INPUT, OUTPUT>::GrayscaleErode(int radius)
    : Transform2<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleErode " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleErode<INPUT, OUTPUT>::GrayscaleErode(const GrayscaleErode& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleErode<INPUT, OUTPUT>::~GrayscaleErode()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleErode<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleErode<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* /*support*/)
    {
        StructuringElementDisk sed(radius);
        sed.updateStride(input.columns);
        grayscaleErode(&input, &output, sed, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleBrightReconstruction: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleBrightReconstruction(int radius);

        GrayscaleBrightReconstruction(const GrayscaleBrightReconstruction& g);

        ~GrayscaleBrightReconstruction();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstruction<INPUT, OUTPUT>::GrayscaleBrightReconstruction(int radius)
    : Transform2<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleBrightReconstruction " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstruction<INPUT, OUTPUT>::GrayscaleBrightReconstruction(const GrayscaleBrightReconstruction& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstruction<INPUT, OUTPUT>::~GrayscaleBrightReconstruction()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleBrightReconstruction<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleBrightReconstruction<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GrayscaleDilate<INPUT, OUTPUT> gd(radius);
        GrayscaleErode<OUTPUT, OUTPUT> ge(1);
        Image<OUTPUT> tmp;
        Image<OUTPUT> tmp2;
        Image<OUTPUT> tmp3;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);
        tmp3.resizeImage(input);

        gd.apply(input, tmp, roi, support);

        //writeImage("dilated.png", tmp);
        int diff;

        do
        {
            ge.apply(tmp, tmp2, roi, support);
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( !roi || (*roi)(i) )
                tmp3(i)= input(i) > tmp2(i) ? input(i) : tmp2(i);

            diff= 0;
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( tmp3(i) != output(i) )
                    ++diff;
            output= tmp3;
            tmp= tmp3;
            //printf("%d\n", diff);
        }while ( diff > 0 );


    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleDarkReconstruction: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleDarkReconstruction(int radius);

        GrayscaleDarkReconstruction(const GrayscaleDarkReconstruction& g);

        ~GrayscaleDarkReconstruction();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstruction<INPUT, OUTPUT>::GrayscaleDarkReconstruction(int radius)
    : Transform2<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleDarkReconstruction " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstruction<INPUT, OUTPUT>::GrayscaleDarkReconstruction(const GrayscaleDarkReconstruction& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstruction<INPUT, OUTPUT>::~GrayscaleDarkReconstruction()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleDarkReconstruction<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleDarkReconstruction<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {

        GrayscaleDilate<OUTPUT, OUTPUT> gd(1);
        GrayscaleErode<INPUT, OUTPUT> ge(radius);
        Image<OUTPUT> tmp;
        Image<OUTPUT> tmp2;
        Image<OUTPUT> tmp3;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);
        tmp3.resizeImage(input);

        ge.apply(input, tmp, roi, support);

        //writeImage("dilated.png", tmp);
        int diff;

        do
        {
            gd.apply(tmp, tmp2, roi, support);
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( !roi || (*roi)(i) > 0 )
                    tmp3(i)= input(i) < tmp2(i) ? input(i) : tmp2(i);

            diff= 0;
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( tmp3(i) != output(i) )
                    ++diff;
            output= tmp3;
            tmp= tmp3;
            //printf("%d\n", diff);
        }while ( diff > 0 );
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleBrightReconstructionTopHat: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleBrightReconstructionTopHat(int area);

        GrayscaleBrightReconstructionTopHat(const GrayscaleBrightReconstructionTopHat& g);

        ~GrayscaleBrightReconstructionTopHat();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstructionTopHat<INPUT, OUTPUT>::GrayscaleBrightReconstructionTopHat(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleBrightReconstructionTopHat " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstructionTopHat<INPUT, OUTPUT>::GrayscaleBrightReconstructionTopHat(const GrayscaleBrightReconstructionTopHat& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstructionTopHat<INPUT, OUTPUT>::~GrayscaleBrightReconstructionTopHat()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleBrightReconstructionTopHat<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(area, area, area, area);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleBrightReconstructionTopHat<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GrayscaleBrightReconstruction<INPUT, OUTPUT> go(area);
        go.apply(input, output, roi, support);
        if ( !roi )
            for ( unsigned int i= 0; i < output.n; ++i )
                output(i)= input(i) - output(i);
        else
            for ( unsigned int i= 0; i < output.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= input(i) - output(i);
                else
                    output(i)= 0;
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleDarkReconstructionTopHat: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleDarkReconstructionTopHat(int area);

        GrayscaleDarkReconstructionTopHat(const GrayscaleDarkReconstructionTopHat& g);

        ~GrayscaleDarkReconstructionTopHat();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int area;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstructionTopHat<INPUT, OUTPUT>::GrayscaleDarkReconstructionTopHat(int area)
    : Transform2<INPUT, OUTPUT>()
    {
        this->area= area;
        std::stringstream ss;
        ss << "GrayscaleDarkReconstructionTopHat " << area;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstructionTopHat<INPUT, OUTPUT>::GrayscaleDarkReconstructionTopHat(const GrayscaleDarkReconstructionTopHat& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->area= g.area;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstructionTopHat<INPUT, OUTPUT>::~GrayscaleDarkReconstructionTopHat()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleDarkReconstructionTopHat<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(area, area, area, area);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleDarkReconstructionTopHat<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GrayscaleDarkReconstruction<INPUT, OUTPUT> gc(area);
        gc.apply(input, output, roi, support);
        if ( !roi )
            for ( unsigned int i= 0; i < input.n; ++i )
                output(i)= output(i) - input(i);
        else
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= output(i) - input(i);
                else
                    output(i)= 0;
    }


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleVerticalOpen: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleVerticalOpen(int width, int length);

        GrayscaleVerticalOpen(const GrayscaleVerticalOpen& g);

        ~GrayscaleVerticalOpen();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int width;
	int length;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleVerticalOpen<INPUT, OUTPUT>::GrayscaleVerticalOpen(int width, int length)
    : Transform2<INPUT, OUTPUT>()
    {
        this->width= width;
	this->length= length;
        std::stringstream ss;
        ss << "GrayscaleVerticalOpen " << width << " " << length;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleVerticalOpen<INPUT, OUTPUT>::GrayscaleVerticalOpen(const GrayscaleVerticalOpen& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->width= g.width;
	this->length= g.length;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleVerticalOpen<INPUT, OUTPUT>::~GrayscaleVerticalOpen()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleVerticalOpen<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        StructuringElementVertical se(width, length);
        se.updateStride(input.columns);

        grayscaleDilate(&input, &tmp, se, roi);
        grayscaleErode(&tmp, &output, se, roi);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleVerticalOpen<INPUT, OUTPUT>::getProposedBorder()
    {
	int b= (width > length) ? width : length;
        return Border2(b, b, b, b);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleVerticalClose: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleVerticalClose(int width, int length);

        GrayscaleVerticalClose(const GrayscaleVerticalClose& g);

        ~GrayscaleVerticalClose();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int width;
	int length;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleVerticalClose<INPUT, OUTPUT>::GrayscaleVerticalClose(int width, int length)
    : Transform2<INPUT, OUTPUT>()
    {
        this->width= width;
	this->length= length;
        std::stringstream ss;
        ss << "GrayscaleVerticalClose " << width << " " << length;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleVerticalClose<INPUT, OUTPUT>::GrayscaleVerticalClose(const GrayscaleVerticalClose& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->width= g.width;
	this->length= g.length;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleVerticalClose<INPUT, OUTPUT>::~GrayscaleVerticalClose()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleVerticalClose<INPUT, OUTPUT>::getProposedBorder()
    {
	int b= (width > length) ? width : length;
        return Border2(b, b, b, b);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleVerticalClose<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        StructuringElementVertical se(width, length);
        se.updateStride(input.columns);

        grayscaleErode(&input, &tmp, se, roi);
        grayscaleDilate(&tmp, &output, se, roi);
    }
    
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleHorizontalOpen: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleHorizontalOpen(int width, int length);

        GrayscaleHorizontalOpen(const GrayscaleHorizontalOpen& g);

        ~GrayscaleHorizontalOpen();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int width;
	int length;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleHorizontalOpen<INPUT, OUTPUT>::GrayscaleHorizontalOpen(int width, int length)
    : Transform2<INPUT, OUTPUT>()
    {
        this->width= width;
	this->length= length;
        std::stringstream ss;
        ss << "GrayscaleHorizontalOpen " << width << " " << length;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleHorizontalOpen<INPUT, OUTPUT>::GrayscaleHorizontalOpen(const GrayscaleHorizontalOpen& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->width= g.width;
	this->length= g.length;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleHorizontalOpen<INPUT, OUTPUT>::~GrayscaleHorizontalOpen()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleHorizontalOpen<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        StructuringElementHorizontal se(width, length);
        se.updateStride(input.columns);

        grayscaleDilate(&input, &tmp, se, roi);
        grayscaleErode(&tmp, &output, se, roi);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleHorizontalOpen<INPUT, OUTPUT>::getProposedBorder()
    {
	int b= (width > length) ? width : length;
        return Border2(b, b, b, b);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleHorizontalClose: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        GrayscaleHorizontalClose(int width, int length);

        GrayscaleHorizontalClose(const GrayscaleHorizontalClose& g);

        ~GrayscaleHorizontalClose();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int width;
	int length;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleHorizontalClose<INPUT, OUTPUT>::GrayscaleHorizontalClose(int width, int length)
    : Transform2<INPUT, OUTPUT>()
    {
        this->width= width;
	this->length= length;
        std::stringstream ss;
        ss << "GrayscaleHorizontalClose " << width << " " << length;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleHorizontalClose<INPUT, OUTPUT>::GrayscaleHorizontalClose(const GrayscaleHorizontalClose& g)
    : Transform2<INPUT, OUTPUT>(g)
    {
        this->width= g.width;
	this->length= g.length;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleHorizontalClose<INPUT, OUTPUT>::~GrayscaleHorizontalClose()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GrayscaleHorizontalClose<INPUT, OUTPUT>::getProposedBorder()
    {
	int b= (width > length) ? width : length;
        return Border2(b, b, b, b);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleHorizontalClose<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        StructuringElementHorizontal se(width, length);
        se.updateStride(input.columns);

        grayscaleErode(&input, &tmp, se, roi);
        grayscaleDilate(&tmp, &output, se, roi);
    }
    
    
}
#endif
