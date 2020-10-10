/**
 * @file LowLevelTransforms.h
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
 * Some low level image transforms.
 */

#ifndef LOWLEVELTRANSFORM_H
#define	LOWLEVELTRANSFORM_H

#include <float.h>
#include <algorithm>

#include <openipDS/Image.h>

#include <openipLL/morphology.h>
#include <openipLL/imageIO.h>
#include <openipLL/ROIDetector.h>
#include <openipLL/ComponentLabeling.h>
#include <openipML/statisticalFunctions.h>
#include <openipML/StatisticalTests.h>
#include <openipML/Noises.h>
#include <openipDS/Stopper.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/MatchedGaborFilter2.h>
#include <openipDS/drawing.h>
#include <openipDS/OrientationEstimators.h>

namespace openip
{
   template<typename INPUT, typename OUTPUT>
    class InversionTransform2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        InversionTransform2();

        /**
         * copy constructor
         * @param a instance to copy
         */
        InversionTransform2(const InversionTransform2& a);

        /**
         * destructor
         */
        ~InversionTransform2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InversionTransform2<INPUT, OUTPUT>::InversionTransform2()
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "InversionTransform2";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    InversionTransform2<INPUT, OUTPUT>::InversionTransform2(const InversionTransform2& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
    }

    template<typename INPUT, typename OUTPUT>
    InversionTransform2<INPUT, OUTPUT>::~InversionTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void InversionTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        if ( roi == NULL )
            for ( unsigned int i= 0; i < output.n; ++i )
                    output(i)= 255 - input(i);
        else
            for ( unsigned int i= 0; i < output.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= 255 - input(i);
    }

    /**
     * AmplifiedOrientation2 estimates the orientation of image content
     */
    template<typename INPUT, typename OUTPUT>
    class AmplifiedOrientation2: public Transform2<INPUT, OUTPUT>
    {
    public:
        /**
         * default constructor
         */
        AmplifiedOrientation2();

        /**
         * copy constructor
         * @param a instance to copy
         */
        AmplifiedOrientation2(const AmplifiedOrientation2& a);

        /**
         * destructor
         */
        ~AmplifiedOrientation2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);
    };

    /**
     * AmplifiedOrientation estimates the orientation of image content
     */
    template<typename INPUT, typename OUTPUT>
    class AmplifiedOrientation1: public Transform2<INPUT, OUTPUT>
    {
    public:
        /**
         * default constructor
         */
        AmplifiedOrientation1();

        /**
         * copy constructor
         * @param a instance to copy
         */
        AmplifiedOrientation1(const AmplifiedOrientation1& a);

        /**
         * destructor
         */
        ~AmplifiedOrientation1();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    AmplifiedOrientation1<INPUT, OUTPUT>::AmplifiedOrientation1()
    {
    }

    template<typename INPUT, typename OUTPUT>
    AmplifiedOrientation1<INPUT, OUTPUT>::AmplifiedOrientation1(const AmplifiedOrientation1& a)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AmplifiedOrientation1<INPUT, OUTPUT>::~AmplifiedOrientation1()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void AmplifiedOrientation1<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* )
    {
        Image<float> gximg(input.rows, input.columns);
        Image<float> gyimg(input.rows, input.columns);;
        Image<float> gximgconv(input.rows, input.columns);
        Image<float> gyimgconv(input.rows, input.columns);
        Image<float> tmpi(input.rows, input.columns);
        Image<unsigned char> spikes(input.rows, input.columns);
        Image<unsigned char> roi(input.rows, input.columns);
        spikes= input;

        SobelFilterX<INPUT, OUTPUT> gx(input.columns);
        SobelFilterY<INPUT, OUTPUT> gy(input.columns);

        gx.updateStride(input.columns);
        gy.updateStride(input.columns);
        gx.apply(input, gximg);
        gy.apply(input, gyimg);

        tmpi= gximg;
        tmpi.normalize(0, 255);
        writeImage("gximg.png", tmpi, tmpi, tmpi);
        tmpi= gyimg;
        tmpi.normalize(0, 255);
        writeImage("gyimg.png", tmpi, tmpi, tmpi);

        float tmp;
        for ( unsigned int i= 0; i < gximg.n; ++i )
        {
            tmp= sqrt(gximg(i)*gximg(i) + gyimg(i)*gyimg(i));
            gximg(i)= gximg(i) * tmp;
            gyimg(i)= gyimg(i) * tmp;
        }

        tmpi= gximg;
        tmpi.normalize(0, 255);
        writeImage("gximg2.png", tmpi, tmpi, tmpi);
        tmpi= gyimg;
        tmpi.normalize(0, 255);
        writeImage("gyimg2.png", tmpi, tmpi, tmpi);

        GaussianFilter2<float, float> gf2(8.0, 25, input.columns);

        gximgconv= gximg;
        gyimgconv= gyimg;
        gf2.apply(gximg, gximgconv);
        gf2.apply(gyimg, gyimgconv);

        tmpi= gximgconv;
        tmpi.normalize(0, 255);
        writeImage("gximgconv.png", tmpi, tmpi, tmpi);
        tmpi= gyimgconv;
        tmpi.normalize(0, 255);
        writeImage("gyimgconv.png", tmpi, tmpi, tmpi);

        for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= atan2(gyimgconv(i),gximgconv(i));

        int x, y;
        for ( int i= 0; i < 2000; ++i )
        {
            x= rand()%input.rows;
            y= rand()%input.columns;
                drawSpike(spikes, x, y, atan2(gyimgconv(x,y), gximgconv(x,y)), 10, (unsigned char)255);
        }

        writeImage("spikes.png", spikes, spikes, spikes);

    }

    /**
     * top hat transform (not implemented yet)
     */
    template<typename INPUT, typename OUTPUT>
    class TopHatTransform: public Transform2<INPUT, OUTPUT>
    {
    public:

    };

    template<typename INPUT, typename OUTPUT>
    class NormalizationTransform2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        NormalizationTransform2();

        /**
         * copy constructor
         * @param a instance to copy
         */
        NormalizationTransform2(const NormalizationTransform2& a);

        /**
         * destructor
         */
        ~NormalizationTransform2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    NormalizationTransform2<INPUT, OUTPUT>::NormalizationTransform2()
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "NormalizationTransform2";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationTransform2<INPUT, OUTPUT>::NormalizationTransform2(const NormalizationTransform2& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationTransform2<INPUT, OUTPUT>::~NormalizationTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void NormalizationTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        float mean, var;
        input.getMeanAndVariance(mean, var, roi);
        var= sqrt(var);

        /*writeImage("input.jpg", input);
        writeImage("output.jpg", output);
        writeImage("roi.jpg", *roi);
        printf("image variance = %f %f\n", mean, var);*/
        
        if ( var > FLT_EPSILON )
        {
            if ( roi == NULL )
                for ( unsigned int i= 0; i < output.n; ++i )
                        output(i)= (float(input(i)) - mean)/var;
            else
                for ( unsigned int i= 0; i < output.n; ++i )
                    if ( (*roi)(i) > 0 )
                        output(i)= (float(input(i)) - mean)/var;
        }
        else
        {
            output= 0;
        }
    }
    
    template<typename INPUT, typename OUTPUT>
    class NormalizationIntoRange: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        NormalizationIntoRange(float a, float b);

        /**
         * copy constructor
         * @param a instance to copy
         */
        NormalizationIntoRange(const NormalizationIntoRange& a);

        /**
         * destructor
         */
        ~NormalizationIntoRange();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        float a;
        float b;
    };

    template<typename INPUT, typename OUTPUT>
    NormalizationIntoRange<INPUT, OUTPUT>::NormalizationIntoRange(float a, float b)
    : Transform2<INPUT, OUTPUT>()
    {
        this->a= a;
        this->b= b;
        
        std::stringstream ss;
        ss << "NormalizationIntoRange " << a << " " << b;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationIntoRange<INPUT, OUTPUT>::NormalizationIntoRange(const NormalizationIntoRange& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
        this->a= a.a;
        this->b= a.b;
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationIntoRange<INPUT, OUTPUT>::~NormalizationIntoRange()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void NormalizationIntoRange<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);

        float d= max - min;
        if ( d == 0 )
        {
            output= input;
        }
        else
        {
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( !roi || (*roi)(i) )
                {
                    output(i)= (input(i) - min)/d*(b-a) + a;
                }
                else
                    output(i)= 0;
        }
    }

    template<typename INPUT, typename OUTPUT>
    class VesselPostprocessingTransform2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        VesselPostprocessingTransform2();

        /**
         * copy constructor
         * @param a instance to copy
         */
        VesselPostprocessingTransform2(const VesselPostprocessingTransform2& a);

        /**
         * destructor
         */
        ~VesselPostprocessingTransform2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    VesselPostprocessingTransform2<INPUT, OUTPUT>::VesselPostprocessingTransform2()
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "VesselPostprocessingTransform2";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    VesselPostprocessingTransform2<INPUT, OUTPUT>::VesselPostprocessingTransform2(const VesselPostprocessingTransform2& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VesselPostprocessingTransform2<INPUT, OUTPUT>::~VesselPostprocessingTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VesselPostprocessingTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        /*StructuringElementDisk sed(2);
        sed.updateStride(input.columns);
        binaryDilate(&input, &output, sed);
        binaryDilate(&output, &input, sed);
        binaryErode(&input, &output, sed);
        binaryErode(&output, &input, sed);
        output= input;*/
    }
    
    template<typename INPUT, typename OUTPUT>
    class ParametricMorphologicalReconstruction2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        ParametricMorphologicalReconstruction2(float a, float b);

        /**
         * copy constructor
         * @param a instance to copy
         */
        ParametricMorphologicalReconstruction2(const ParametricMorphologicalReconstruction2& a);

        /**
         * destructor
         */
        ~ParametricMorphologicalReconstruction2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        float a;
        float b;
    };

    template<typename INPUT, typename OUTPUT>
    ParametricMorphologicalReconstruction2<INPUT, OUTPUT>::ParametricMorphologicalReconstruction2(float a, float b)
    : Transform2<INPUT, OUTPUT>()
    {
        this->a= a;
        this->b= b;
        
        std::stringstream ss;
        ss << "ParametricMorphologicalReconstruction2 " << a << " " << b;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ParametricMorphologicalReconstruction2<INPUT, OUTPUT>::ParametricMorphologicalReconstruction2(const ParametricMorphologicalReconstruction2& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
        this->a= a.a;
        this->b= a.b;
    }

    template<typename INPUT, typename OUTPUT>
    ParametricMorphologicalReconstruction2<INPUT, OUTPUT>::~ParametricMorphologicalReconstruction2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ParametricMorphologicalReconstruction2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);

        Image<INPUT> tmp;
        tmp.resizeImage(input);
        
        float threshold= min + (max - min)*b;
        for ( unsigned int i= 0; i < input.n; ++i )
            if ( input(i) > threshold )
                tmp(i)= min;
            else
                tmp(i)= input(i);
        StructuringElementDisk sed(a);
        sed.updateStride(input.columns);
        
        grayscaleDilate(&tmp, &output, sed);
        
        for ( unsigned int i= 0; i < output.n; ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) > threshold )
                    output(i)= input(i) - output(i);
                else
                    output(i)= 0;
            }
            else
                (output)(i)= 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    class ParametricMorphologicalReconstructionROI2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        ParametricMorphologicalReconstructionROI2(float a, float b);

        /**
         * copy constructor
         * @param a instance to copy
         */
        ParametricMorphologicalReconstructionROI2(const ParametricMorphologicalReconstructionROI2& a);

        /**
         * destructor
         */
        ~ParametricMorphologicalReconstructionROI2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        float a;
        float b;
    };

    template<typename INPUT, typename OUTPUT>
    ParametricMorphologicalReconstructionROI2<INPUT, OUTPUT>::ParametricMorphologicalReconstructionROI2(float a, float b)
    : Transform2<INPUT, OUTPUT>()
    {
        this->a= a;
        this->b= b;
        
        std::stringstream ss;
        ss << "ParametricMorphologicalReconstructionROI2 " << a << " " << b;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ParametricMorphologicalReconstructionROI2<INPUT, OUTPUT>::ParametricMorphologicalReconstructionROI2(const ParametricMorphologicalReconstructionROI2& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
        this->a= a.a;
        this->b= a.b;
    }

    template<typename INPUT, typename OUTPUT>
    ParametricMorphologicalReconstructionROI2<INPUT, OUTPUT>::~ParametricMorphologicalReconstructionROI2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ParametricMorphologicalReconstructionROI2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);
        
        printf("%p\n", roi);

        Image<INPUT> tmp;
        tmp.resizeImage(input);
        
        float threshold= min + (max - min)*b;
        for ( unsigned int i= 0; i < input.n; ++i )
            if ( input(i) > threshold )
                tmp(i)= min;
            else
                tmp(i)= input(i);
        StructuringElementDisk sed(a);
        sed.updateStride(input.columns);
        
        grayscaleDilate(&tmp, &output, sed);
        
        for ( unsigned int i= 0; i < output.n; ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) > threshold )
                    output(i)= input(i) - output(i);
                else
                {
                    output(i)= 0;
                    if ( roi )
                    (*roi)(i)= 0;
                }
            }
            else
                (output)(i)= 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    class ATransform: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        ATransform(float a);

        /**
         * copy constructor
         * @param a instance to copy
         */
        ATransform(const ATransform& a);

        /**
         * destructor
         */
        ~ATransform();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        float a;
    };

    template<typename INPUT, typename OUTPUT>
    ATransform<INPUT, OUTPUT>::ATransform(float a)
    : Transform2<INPUT, OUTPUT>()
    {
        this->a= a;
        
        std::stringstream ss;
        ss << "ATransform " << a;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ATransform<INPUT, OUTPUT>::ATransform(const ATransform& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
        this->a= a.a;
    }

    template<typename INPUT, typename OUTPUT>
    ATransform<INPUT, OUTPUT>::~ATransform()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ATransform<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);

        float threshold= min + (max - min)*a;
        
        for ( unsigned int i= 0; i < output.n; ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) > threshold )
                    output(i)= input(i) * (1 + a);
                else
                    output(i)= input(i) * a;
            }
            else
                (output)(i)= 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    class BTransform: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        BTransform(float a);

        /**
         * copy constructor
         * @param a instance to copy
         */
        BTransform(const BTransform& a);

        /**
         * destructor
         */
        ~BTransform();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        float a;
    };

    template<typename INPUT, typename OUTPUT>
    BTransform<INPUT, OUTPUT>::BTransform(float a)
    : Transform2<INPUT, OUTPUT>()
    {
        this->a= a;
        
        std::stringstream ss;
        ss << "BTransform " << a;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BTransform<INPUT, OUTPUT>::BTransform(const BTransform& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
        this->a= a.a;
    }

    template<typename INPUT, typename OUTPUT>
    BTransform<INPUT, OUTPUT>::~BTransform()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BTransform<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);
        
        float threshold= min + (max - min) * a;

        for ( unsigned int i= 0; i < output.n; ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) < threshold )
                    output(i)= input(i) * (threshold - min);
                else
                    output(i)= input(i) * (max - threshold);
            }
            else
                (output)(i)= 0;
    }

    template<typename INPUT, typename OUTPUT>
    class BrightForegroundFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;

        BrightForegroundFeature2(int r, int w2, int h2, int d2, float alpha= 0, float sign= 0.2, int stride= 4000);

        BrightForegroundFeature2(const BrightForegroundFeature2& f);

        ~BrightForegroundFeature2();

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int updateStride(int stride);

        virtual int getMin();
        virtual int getMax();

        void translate(Pixel1 n);

        StructuringElementDisk* sed;
        StructuringElementParallelStripes* seps;
        StructuringElementOneStripe* seos1;
        StructuringElementOneStripe* seos2;
        TwoSampleTTest<float>* tstt;
        FTest<float>* ft;
        Vector<float> a;
        Vector<float> b;
    };

    template<typename INPUT, typename OUTPUT>
    BrightForegroundFeature2<INPUT, OUTPUT>::BrightForegroundFeature2(int r, int w2, int h2, int d2, float alpha, float sign, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(0, stride)
    {
        this->sed= new StructuringElementDisk(r, stride);
        this->seps= new StructuringElementParallelStripes(w2, h2, d2, stride);
        this->seos1= new StructuringElementOneStripe(w2, h2, d2, stride);
        this->seos2= new StructuringElementOneStripe(w2, h2, -d2, stride);
        this->seps->rotate(alpha);
        this->seps->computeMinMax();
        this->seos1->rotate(alpha);
        this->seos2->rotate(alpha);
        this->seos1->computeMinMax();
        this->seos2->computeMinMax();
        this->sed->computeMinMax();
        tstt= new TwoSampleTTest<float>(sign);
        ft= new FTest<float>(sign);
        a.resize(sed->size());
        b.resize(seps->size());
        printf("a\n"); fflush(stdout);
        for ( unsigned int i= 0; i < sed->size(); ++i )
            printf("%d ", (*sed)(i));
        printf("b\n");
        for ( unsigned int i= 0; i < seps->size(); ++i )
            printf("%d ", (*seps)(i));
        printf("\n");
    }

    template<typename INPUT, typename OUTPUT>
    void BrightForegroundFeature2<INPUT, OUTPUT>::translate(Pixel1 n)
    {
        sed->translate(n);
        seps->translate(n);
    }

    template<typename INPUT, typename OUTPUT>
    BrightForegroundFeature2<INPUT, OUTPUT>::BrightForegroundFeature2(const BrightForegroundFeature2 &f)
        : StatisticalFeature2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BrightForegroundFeature2<INPUT, OUTPUT>::~BrightForegroundFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BrightForegroundFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int tmp= abs(seps->getMax()/seps->stride);
        //tmp= tmp + sqrt(tmp);
        tmp*= 2;
        return Border2(tmp, tmp, tmp, tmp);
    }

    template<typename INPUT, typename OUTPUT>
    int BrightForegroundFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        tprintf("updateStride\n");
        this->sed->updateStride(stride);
        this->sed->computeMinMax();
        this->seps->updateStride(stride);
        this->seps->computeMinMax();
        this->stride= stride;
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int BrightForegroundFeature2<INPUT, OUTPUT>::getMin()
    {
        return seps->getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int BrightForegroundFeature2<INPUT, OUTPUT>::getMax()
    {
        return seps->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT BrightForegroundFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        //tprintf("%d ",n);
        if ( support == 0 )
        {
            int i= 0;
            for ( StructuringElementDisk::iterator it= sed->begin(); it != sed->end(); ++it )
            {
                a(i++)= input(n + *it);
            }

            i= 0;
            for ( StructuringElementParallelStripes::iterator it= seps->begin(); it != seps->end(); ++it )
            {
                b(i++)= input(n + *it);
            }
        }
        else
        {
            a.clear();
            b.clear();
            for ( StructuringElementDisk::iterator it= sed->begin(); it != sed->end(); ++it )
            {
                if ( (*support)(n + *it ) )
                    a.push_back(input(n + *it));
            }

            for ( StructuringElementParallelStripes::iterator it= seps->begin(); it != seps->end(); ++it )
            {
                if ( (*support)(n + *it ) )
                    b.push_back(input(n + *it));
            }
        }

        /*if ( fabs(a.getMean() - b.getMean()) > 5 )
        {
            printf("%f,%f ", a.getMean(), b.getMean()); fflush(stdout);
            return 1;
        }
        else
            return 0;*/


        int res= 0;
        if ( tstt->test(a,b) == 0 || ft->test(a,b) == 0 )
        {
            res= 1;
            //printf("%f,%f ", a.getMean(), b.getMean()); fflush(stdout);
            /*for ( int i= 0; i < a.size(); ++i )
                if ( a(i) < 10 )
                {
                    printf("%f ", a(i)); fflush(stdout);
                }*/
        }
        else
            res= 0;

        if ( res == 1 && a.getMean() <= b.getMean() )
            res= 0;
        return res;
    }

    template<typename INPUT, typename OUTPUT>
    void BrightForegroundFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
        {
            if ( !roi || (*roi)(i) )
            {
                printf("%d ", i); fflush(stdout);
                output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    class BrightRotatedForegroundFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::operator [];

        BrightRotatedForegroundFeature2(int r, int w, int h, int d, float step, float sign, int stride= 4000);

        BrightRotatedForegroundFeature2(const BrightRotatedForegroundFeature2& r);

        ~BrightRotatedForegroundFeature2();

        virtual void apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi= NULL, Image<unsigned char> *support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    BrightRotatedForegroundFeature2<INPUT, OUTPUT>::BrightRotatedForegroundFeature2(int r, int w, int h, int d, float step, float sign, int stride)
    : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int i= 0; i < 1/step; ++i )
        {
            this->push_back(new BrightForegroundFeature2<INPUT, OUTPUT>(r, w, h, d, i*step*M_PI, sign, stride));
        }
        this->computeMinMax();

        std::stringstream ss;
        ss << "BrightRotatedForegroundFeature2 " << r << " " << w << " " << h << " " << d << " " << step << " " << sign << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BrightRotatedForegroundFeature2<INPUT, OUTPUT>::BrightRotatedForegroundFeature2(const BrightRotatedForegroundFeature2 &r)
    : FeatureSet2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BrightRotatedForegroundFeature2<INPUT, OUTPUT>::~BrightRotatedForegroundFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BrightRotatedForegroundFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        /*Image<float> f;
        f.resizeImage(input);

        f= 0;*/



        tprintf("bright apply\n");
        updateStride(input.columns);
        computeMinMax();

        /*dynamic_cast<BrightForegroundFeature2<INPUT, OUTPUT>*>(this->operator[](0))->translate(100200);
        printf("%d\n", dynamic_cast<BrightForegroundFeature2<INPUT, OUTPUT>*>(this->operator[](0))->seps->stride);
        printf("%d\n", dynamic_cast<BrightForegroundFeature2<INPUT, OUTPUT>*>(this->operator[](0))->sed->stride);
        dynamic_cast<BrightForegroundFeature2<INPUT, OUTPUT>*>(this->operator[](0))->seps->set(f);
        dynamic_cast<BrightForegroundFeature2<INPUT, OUTPUT>*>(this->operator[](0))->sed->set(f);
        dynamic_cast<BrightForegroundFeature2<INPUT, OUTPUT>*>(this->operator[](0))->translate(-100200);*/

        /*f.normalize(0, 255);
        writeImage("filter.jpg", f);*/

        output= 0;


        OUTPUT res, tmp;
        if ( roi == NULL )
        {
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
            {
                res= -numeric_limits<OUTPUT>::max();

                for ( unsigned int j= 0; j < this->size(); ++j )
                {
                    tmp= (*this)[j]->apply(input, i, support);
                    if ( tmp > res )
                        res= tmp;
                }

                output(i)= res;
            }
        }
        else
        {
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
                if ( (*roi)(i) > 0 )
                {
                    if ( i % 10000 == 0 )
                    {
                        printf("%d/%d ", i, input.n);
                        fflush(stdout);
                    }
                    res= -numeric_limits<OUTPUT>::max();

                    for ( unsigned int j= 0; j < this->size(); ++j )
                    {
                        tmp= (*this)[j]->apply(input, i, support);
                        if ( tmp > res )
                            res= tmp;
                    }

                    output(i)= res;
                }
        }
    }





    template<typename INPUT, typename OUTPUT>
    class DarkForegroundFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;

        DarkForegroundFeature2(int r, int w2, int h2, int d2, float alpha= 0, float sign= 0.2, int stride= 4000);

        DarkForegroundFeature2(const DarkForegroundFeature2& f);

        ~DarkForegroundFeature2();

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int updateStride(int stride);

        virtual int getMin();
        virtual int getMax();

        void translate(Pixel1 n);

        StructuringElementDisk* sed;
        StructuringElementParallelStripes* seps;
        TwoSampleTTest<float>* tstt;
        FTest<float>* ft;
        Vector<float> a;
        Vector<float> b;
    };

    template<typename INPUT, typename OUTPUT>
    DarkForegroundFeature2<INPUT, OUTPUT>::DarkForegroundFeature2(int r, int w2, int h2, int d2, float alpha, float sign, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(0, stride)
    {
        this->sed= new StructuringElementDisk(r, stride);
        this->seps= new StructuringElementParallelStripes(w2, h2, d2, stride);
        this->seps->rotate(alpha);
        this->seps->computeMinMax();
        this->sed->computeMinMax();
        tstt= new TwoSampleTTest<float>(sign);
        ft= new FTest<float>(sign);
        a.resize(sed->size());
        b.resize(seps->size());
        printf("a\n"); fflush(stdout);
        for ( unsigned int i= 0; i < sed->size(); ++i )
            printf("%d ", (*sed)(i));
        printf("b\n");
        for ( unsigned int i= 0; i < seps->size(); ++i )
            printf("%d ", (*seps)(i));
        printf("\n");
    }

    template<typename INPUT, typename OUTPUT>
    void DarkForegroundFeature2<INPUT, OUTPUT>::translate(Pixel1 n)
    {
        sed->translate(n);
        seps->translate(n);
    }

    template<typename INPUT, typename OUTPUT>
    DarkForegroundFeature2<INPUT, OUTPUT>::DarkForegroundFeature2(const DarkForegroundFeature2 &f)
        : StatisticalFeature2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DarkForegroundFeature2<INPUT, OUTPUT>::~DarkForegroundFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 DarkForegroundFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int tmp= abs(seps->getMax()/seps->stride);
        //tmp= tmp + sqrt(tmp);
        tmp*= 2;
        return Border2(tmp, tmp, tmp, tmp);
    }

    template<typename INPUT, typename OUTPUT>
    int DarkForegroundFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        tprintf("updateStride\n");
        this->sed->updateStride(stride);
        this->sed->computeMinMax();
        this->seps->updateStride(stride);
        this->seps->computeMinMax();
        this->stride= stride;
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int DarkForegroundFeature2<INPUT, OUTPUT>::getMin()
    {
        return seps->getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int DarkForegroundFeature2<INPUT, OUTPUT>::getMax()
    {
        return seps->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT DarkForegroundFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        //tprintf("%d ",n);
        if ( support == 0 )
        {
            int i= 0;
            for ( StructuringElementDisk::iterator it= sed->begin(); it != sed->end(); ++it )
            {
                a(i++)= input(n + *it);
            }

            i= 0;
            for ( StructuringElementParallelStripes::iterator it= seps->begin(); it != seps->end(); ++it )
            {
                b(i++)= input(n + *it);
            }
        }
        else
        {
            a.clear();
            b.clear();
            for ( StructuringElementDisk::iterator it= sed->begin(); it != sed->end(); ++it )
            {
                if ( (*support)(n + *it ) )
                    a.push_back(input(n + *it));
            }

            for ( StructuringElementParallelStripes::iterator it= seps->begin(); it != seps->end(); ++it )
            {
                if ( (*support)(n + *it ) )
                    b.push_back(input(n + *it));
            }
        }

        int res= ((tstt->test(a,b) == 0) || ft->test(a,b) == 0 ) ? 1 : 0;

        if ( res == 1 && a.getMean() >= b.getMean() )
            res= 0;
        return res;
    }

    template<typename INPUT, typename OUTPUT>
    void DarkForegroundFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
        {
            if ( !roi || (*roi)(i) )
            {
                printf("%d ", i); fflush(stdout);
                output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    class DarkRotatedForegroundFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::operator [];

        DarkRotatedForegroundFeature2(int r, int w, int h, int d, float step, float sign, int stride= 4000);

        DarkRotatedForegroundFeature2(const DarkRotatedForegroundFeature2& r);

        ~DarkRotatedForegroundFeature2();

        virtual void apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi= NULL, Image<unsigned char> *support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    DarkRotatedForegroundFeature2<INPUT, OUTPUT>::DarkRotatedForegroundFeature2(int r, int w, int h, int d, float step, float sign, int stride)
    : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int i= 0; i < 1/step; ++i )
        {
            this->push_back(new DarkForegroundFeature2<INPUT, OUTPUT>(r, w, h, d, i*step*M_PI, sign, stride));
        }
        this->computeMinMax();

        std::stringstream ss;
        ss << "DarkRotatedForegroundFeature2 " << r << " " << w << " " << h << " " << d << " " << step << " " << sign << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DarkRotatedForegroundFeature2<INPUT, OUTPUT>::DarkRotatedForegroundFeature2(const DarkRotatedForegroundFeature2 &r)
    : FeatureSet2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DarkRotatedForegroundFeature2<INPUT, OUTPUT>::~DarkRotatedForegroundFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void DarkRotatedForegroundFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        OUTPUT res, tmp;
        if ( roi == NULL )
        {
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
            {
                res= -numeric_limits<OUTPUT>::max();

                for ( unsigned int j= 0; j < this->size(); ++j )
                {
                    tmp= (*this)[j]->apply(input, i, support);
                    if ( tmp > res )
                        res= tmp;
                }

                output(i)= res;
            }
        }
        else
        {
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
                if ( (*roi)(i) > 0 )
                {
                    res= -numeric_limits<OUTPUT>::max();

                    for ( unsigned int j= 0; j < this->size(); ++j )
                    {
                        tmp= (*this)[j]->apply(input, i, support);
                        if ( tmp > res )
                            res= tmp;
                    }

                    output(i)= res;
                }
        }
    }

    template<typename INPUT, typename OUTPUT>
    class FillSupport: public Transform2<INPUT, OUTPUT>
    {
      public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        FillSupport();

        /**
         * copy constructor
         * @param a instance to copy
         */
        FillSupport(const FillSupport& a);

        /**
         * destructor
         */
        ~FillSupport();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FillSupport<INPUT, OUTPUT>::FillSupport()
        : Transform2<INPUT, OUTPUT>()
    {
        this->descriptor= std::string("FillSupport");
    }

    template<typename INPUT, typename OUTPUT>
    FillSupport<INPUT, OUTPUT>::FillSupport(const FillSupport& u)
        : Transform2<INPUT, OUTPUT>(u)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FillSupport<INPUT, OUTPUT>::~FillSupport()
    {
    }

    float meanOrientation(Vector<float> orientations, Vector<float> magnitudes);
    
    template<typename INPUT, typename OUTPUT>
    void FillSupport<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
      Vector<float> to;
      Vector<float> tm;
      to.push_back(M_PI/2+0.03);
      to.push_back(-M_PI/2-0.03);
      tm.push_back(1);
      tm.push_back(1);
      
      //printf("%f\n", meanOrientation(to, tm));
      
	Image<unsigned char> roinew;
        tprintf("resizing new roi\n");
	roinew.resizeImage(*roi);
	roinew= *roi;
	Image<unsigned char> roio;
        tprintf("resizing roio\n");
	roio.resizeImage(*roi);
	roio= *roi;
	Image<unsigned char> tmpimage;
        tprintf("resizing tmpimage\n");
	tmpimage.resizeImage(*roi);
	tmpimage= 0;
	StructuringElementDisk sedd(5, input.columns);
        tprintf("start erosion\n");
	binaryErode(roi, &tmpimage, sedd);
	tprintf("erosion finished\n");
	*roi= tmpimage;
      
        output= input;

        if ( roi == NULL || support == NULL )
            return;
	
        tprintf("set new borders\n");
	Border2 b(45, 45, 45, 45, BORDER_MODE_ZERO);
	input.setBorder(b);
	output.setBorder(b);
	roinew.setBorder(b);
        tprintf("set border for roi\n");
        Image<unsigned char> tir, tis;
        tir.resizeImage(*roi);
        tir= *roi;
        tir.setBorder(b);
        tprintf("set roi\n");
        //*roi= ti;
	//roi->setBorder(b);
        tprintf("set border for support\n");
        tis.resizeImage(*support);
        tis= *support;
        tis.setBorder(b);
	//support->setBorder(b);

	Image<float> input2;
        tprintf("resizing input2\n");
	input2.resizeImage(input);
	input2= input;
	
	SobelFilterX<float, float> fsx;
	SobelFilterY<float, float> fsy;
	fsx.updateStride(tis.columns);
	fsy.updateStride(tis.columns);
	
	
	
	Image<unsigned char> roi2;
        tprintf("resizing roi2\n");
	roi2.resizeImage(tir);
	roi2= tir;
	Image<unsigned char> roioriginal;
        tprintf("resizing roioriginal\n");
	roioriginal.resizeImage(tir);
	roioriginal= tir;
	
	//writeImage("roi0.bmp", roi2);
	
	
	
	tprintf("extending image, iteration: ");
	for ( int l= 0; l < input.columns/20; ++l )
	{
	  printf("%d ", l); fflush(stdout);
	  StructuringElementDisk sed((l+5));
	  sed.updateStride(tis.columns);
#pragma omp parallel for
	for ( int i= 0; i < tis.rows; ++i )
	{
	  float gx, gy, ats, atc, sum, magn;
	double t;
	int n;
	int num;
	Vector<float> orientations;
	Vector<float> magnitudes;
            for ( int j= 0; j < tis.columns; ++j )
	    {
	      n= i*tis.columns + j;
	      ats= 0;
	      atc= 0;
	      sum= 0;
	      num= 0;
                if ( (tis)(i,j) && !(tir)(i,j) && tir.isRealImagePixel(n) && Region2::isOuterContour4((tir), n) )
                {
		  Vector<float> magns;
		  Vector<float> ints;
		  
		  for ( unsigned int k= 0; k < sed.size(); ++k )
		  {
		    if ( 0 <= int(n + sed(k)) && n + sed(k) < int(roioriginal.n) && (roioriginal)(n + sed(k)) )
		    {
		      gx= fsx.apply(input2, n + sed(k));
		      gy= fsy.apply(input2, n + sed(k));
		      t= atan2(gy, gx);
		      magn= sqrt(gx*gx + gy*gy);
		      magns.push_back(magn);
		      ints.push_back(input2(n + sed(k)));
		    }
		  }
		  
		  sort(magns.begin(), magns.end());
		  float threshold= magns(magns.size()*0.8);
		  
		  orientations.clear();
		  magnitudes.clear();
		  for ( unsigned int k= 0; k < sed.size(); ++k )
		  {
		    if ( 0 <= int(n + sed(k)) && n + sed(k) < int(roioriginal.n) && (roioriginal)(n + sed(k)) )
		    {
		      ++num;
		      gx= fsx.apply(input2, n + sed(k));
		      gy= fsy.apply(input2, n + sed(k));
		      t= atan2(gy, gx);
		      
		      magn= sqrt(gx*gx + gy*gy);
		      
		      
		      if (  magn > threshold )
		      {
			orientations.push_back(t);
			magnitudes.push_back(magn);
			
			ats+= sin(t)*magn;
			atc+= cos(t)*magn;
			sum+= magn;
		      }
		    }
		  }
		  if ( (ats == 0 && atc == 0) )
		  {
		    continue;
		  }
		  
		  ats/= sum;
		  atc/= sum;
		  
		  t= atan2(atc, ats);
		  
		  t= meanOrientation(orientations, magnitudes);
		  
		  t += M_PI/2;
		  float dist= (l+1);
		  float di= floor((dist)*sin(t)+0.5);
		  float dj= floor((dist)*cos(t)+0.5);
		  
		  if ( (tir)(i + di, j + dj) )
		  {
		    if ( output(i,j) < ints.getMean() - 1.5*ints.getStandardDeviation() || output(i,j) > ints.getMean() + 1.5*ints.getStandardDeviation() )
		    {
		      output(i,j)= output(i + di, j + dj);
		      {
			(roinew)(i,j)= 0;
		      }
		    }
		    (roi2)(i,j)= 255;
		  }
		  else if ( (tir)(i - di, j - dj) )
		  {
		    if ( output(i,j) < ints.getMean() - 1.5*ints.getStandardDeviation() || output(i,j) > ints.getMean() + 1.5*ints.getStandardDeviation() )
		    {
		      output(i,j)= (output)(i - di, j - dj);
		      
		      {
			(roinew)(i,j)= 0;
		      }
		    }
		    (roi2)(i,j)= 255;
		  }
		  else
		  {
		    roi2(i,j)= 255;
		    output(i,j)= ints.getMean();
		  }
                }
	    }
	}
	tir= roi2;
	}
	printf("\n");
	tir= roio;
        printf("finished\n"); fflush(stdout);
        /*tir.removeBorder();
	input.removeBorder();
	output.removeBorder();*/
	//roi->removeBorder();
        *roi= tir;
        /*tis.removeBorder();
	support->removeBorder();*/
	
	//writeImage("roi2.bmp", roi2);
    }

    template<typename INPUT, typename OUTPUT>
    class ThinObjectBoundaries: public Transform2<INPUT, OUTPUT>
    {
      public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        ThinObjectBoundaries();

        /**
         * copy constructor
         * @param a instance to copy
         */
        ThinObjectBoundaries(const ThinObjectBoundaries& a);

        /**
         * destructor
         */
        ~ThinObjectBoundaries();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        //virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<unsigned char>& mask);
    };

    template<typename INPUT, typename OUTPUT>
    ThinObjectBoundaries<INPUT, OUTPUT>::ThinObjectBoundaries()
        : Transform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    ThinObjectBoundaries<INPUT, OUTPUT>::ThinObjectBoundaries(const ThinObjectBoundaries& tob)
        : Transform2<INPUT, OUTPUT>(tob)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ThinObjectBoundaries<INPUT, OUTPUT>::~ThinObjectBoundaries()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThinObjectBoundaries<INPUT, OUTPUT>::apply(Image<INPUT>& image, Image<unsigned char>& mask)
    {
        StructuringElementSquare ses(3, image.columns);
        ses.updateStride(image.columns);
        Vector<int> bg;
        Vector<int> fg;
        float meanbg;
        float meanfg;
        Vector<int> toDelete;

        printf("%d\n", ses.size());

        do
        {

        toDelete.clear();
        for ( int i= 0; i < mask.n; ++i )
        {
            if ( mask(i) == 0 )
                continue;

            bg.clear();
            fg.clear();
            meanbg= 0;
            meanfg= 0;
            for ( int j= 0; j < ses.size(); ++j )
                if ( ses(j) != 0 )
		{
                    if ( mask(i + ses(j)) == 0 )
                        bg.push_back(i + ses(j));
                    else if ( mask(i + ses(j)) == 255 )
                        fg.push_back(i + ses(j));
		}
            if ( bg.size() < 3 )
                continue;
            if ( fg.size() < 3 )
                continue;
            if ( fg.size() > 5 )
                continue;
            if ( bg.size() > 5 )
                continue;

            for ( int j= 0; j < bg.size(); ++j )
                meanbg+= image(bg[j]);
            for ( int j= 0; j < fg.size(); ++j )
                meanfg+= image(fg[j]);
            meanbg/= bg.size();
            meanfg/= fg.size();

            //printf("%f,%f\n", meanbg, meanfg);

            if ( fabs(image(i) - meanbg) < fabs(image(i) - meanfg)*1.5 )
            {
                toDelete.push_back(i);
                mask(i)= 254;
            }
        }

        printf("%d\n", toDelete.size());

        for ( int i= 0; i < toDelete.size(); ++i )
            mask(toDelete(i))= 0;
        } while ( toDelete.size() > 0 );
    }

    template<typename INPUT, typename OUTPUT>
    class ThinObjectBoundaries2: public Transform2<INPUT, OUTPUT>
    {
      public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        ThinObjectBoundaries2();

        /**
         * copy constructor
         * @param a instance to copy
         */
        ThinObjectBoundaries2(const ThinObjectBoundaries2& a);

        /**
         * destructor
         */
        ~ThinObjectBoundaries2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        //virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<unsigned char>& mask);
    };

    template<typename INPUT, typename OUTPUT>
    ThinObjectBoundaries2<INPUT, OUTPUT>::ThinObjectBoundaries2()
        : Transform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    ThinObjectBoundaries2<INPUT, OUTPUT>::ThinObjectBoundaries2(const ThinObjectBoundaries2& tob)
        : Transform2<INPUT, OUTPUT>(tob)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ThinObjectBoundaries2<INPUT, OUTPUT>::~ThinObjectBoundaries2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThinObjectBoundaries2<INPUT, OUTPUT>::apply(Image<INPUT>& image, Image<unsigned char>& mask)
    {
        StructuringElementSquare ses(3, image.columns);
        ses.updateStride(image.columns);
        Vector<int> bg;
        Vector<int> fg;
        float meanbg;
        float meanfg;
        Vector<int> toDelete;

        printf("%d\n", ses.size());

        do
        {

        toDelete.clear();
        for ( int i= 0; i < mask.n; ++i )
        {
            if ( !mask(i) )
                continue;

            bg.clear();
            fg.clear();
            meanbg= 0;
            meanfg= 0;
            for ( int j= 0; j < ses.size(); ++j )
                if ( ses(j) != 0 )
		{
                    if ( !mask(i + ses(j)) )
                        bg.push_back(i + ses(j));
                    else
                        fg.push_back(i + ses(j));
		}
            if ( bg.size() == 0 )
                continue;
            if ( fg.size() == 0 )
                continue;
            if ( fg.size() > 5 )
                continue;
            if ( bg.size() > 5 )
                continue;

            /*for ( int j= 0; j < bg.size(); ++j )
                meanbg+= image(bg[j]);
            for ( int j= 0; j < fg.size(); ++j )
                meanfg+= image(fg[j]);
            meanbg/= bg.size();
            meanfg/= fg.size();*/

            meanbg= image(bg(0));
            meanfg= image(fg(0));
            for ( int j= 0; j < bg.size(); ++j )
                if ( image(bg(j)) < meanbg )
                    meanbg= image(bg(j));
            for ( int j= 0; j < fg.size(); ++j )
                if ( image(fg(j)) > meanfg )
                    meanfg= image(fg(j));

            //printf("%f,%f\n", meanbg, meanfg);

            if ( fabs(image(i) - meanbg) < fabs(image(i) - meanfg) )
                toDelete.push_back(i);
        }

        printf("%d\n", toDelete.size());

        for ( int i= 0; i < toDelete.size(); ++i )
            mask(toDelete(i))= 0;
        } while ( toDelete.size() > 0 );
    }

    template<typename INPUT, typename OUTPUT>
    class ThinVesselTransform: public Transform2<INPUT, OUTPUT>
    {
      public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        ThinVesselTransform(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param a instance to copy
         */
        ThinVesselTransform(const ThinVesselTransform& a);

        /**
         * destructor
         */
        ~ThinVesselTransform();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float sigma;
        float theta0;
        float step;
        float theta1;
        float lambda;
        float psi;
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    ThinVesselTransform<INPUT, OUTPUT>::ThinVesselTransform(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : Transform2<INPUT, OUTPUT>()
    {
        this->sigma= sigma;
        this->theta0= theta0;
        this->step= step;
        this->theta1= theta1;
        this->lambda= lambda;
        this->psi= psi;
        this->gamma= gamma;

        stringstream ss;
        ss << "ThinVesselTransform " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ThinVesselTransform<INPUT, OUTPUT>::ThinVesselTransform(const ThinVesselTransform& tob)
        : Transform2<INPUT, OUTPUT>(tob)
    {
        this->sigma= tob.sigma;
        this->theta0= tob.theta0;
        this->step= tob.step;
        this->theta1= tob.theta1;
        this->lambda= tob.lambda;
        this->psi= tob.psi;
        this->gamma= tob.gamma;
        this->descriptor= tob.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    ThinVesselTransform<INPUT, OUTPUT>::~ThinVesselTransform()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThinVesselTransform<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        printf("-------\n"); fflush(stdout);
        MatchedGaborFilterR2<INPUT, OUTPUT> mgf(sigma, theta0, step, theta1, lambda, psi, gamma);
        mgf.apply(input, output, roi, support);
        printf("%f %f\n", output.getMin(), output.getMax());
        output.normalize(0, 255);
        writeImage("tvt.png", output);
        StandardDeviationFeature2<INPUT, OUTPUT> sdf(4);
        Image<OUTPUT> sdo;
        sdo.resizeImage(input);

        /*sdf.apply(input, sdo, roi, support);

        printf("%f %f\n", sdo.getMin(), sdo.getMax());
        sdo.normalize(0, 255);
        writeImage("sdo.png", sdo);*/
        for ( unsigned int i= 0; i < output.n; ++i )
            if ( (*roi)(i) )
                output(i)= output(i)*(1.0/(1.0 + input(i)));
        printf("%f %f\n", output.getMin(), output.getMax());
    }

    template<typename INPUT, typename OUTPUT>
    class LimitRangeTransform2: public Transform2<INPUT, OUTPUT>
    {
      public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        LimitRangeTransform2(int limit);

        /**
         * copy constructor
         * @param a instance to copy
         */
        LimitRangeTransform2(const LimitRangeTransform2& a);

        /**
         * destructor
         */
        ~LimitRangeTransform2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int limit;
    };

    template<typename INPUT, typename OUTPUT>
    LimitRangeTransform2<INPUT, OUTPUT>::LimitRangeTransform2(int limit)
        : Transform2<INPUT, OUTPUT>()
    {
        this->limit= limit;

        stringstream ss;
        ss << "LimitRangeTransform2 " << limit;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    LimitRangeTransform2<INPUT, OUTPUT>::LimitRangeTransform2(const LimitRangeTransform2& tob)
        : Transform2<INPUT, OUTPUT>(tob)
    {
    }

    template<typename INPUT, typename OUTPUT>
    LimitRangeTransform2<INPUT, OUTPUT>::~LimitRangeTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LimitRangeTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* /*support*/)
    {
        int changed= 1;
        StructuringElementDisk sed(1);
        sed.updateStride(input.columns);

        Image<INPUT> tmp;
        tmp= input;
        output= input;
        OUTPUT min;

        while ( changed )
        {
            changed= 0;
            for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
            {
                if ( !roi || (*roi)(i) )
                {
                    min= output(i);
                    for ( unsigned int j= 0; j < sed.size(); ++j )
                          if ( output(i + sed(j)) < min )
                            min= output(i + sed(j));
                    if ( output(i) - min > limit )
                    {
                          tmp(i)--;
                          changed++;
                    }
                }
            }

            printf("%d ", changed); fflush(stdout);
            output= tmp;
        }
    }

    template<typename INPUT, typename OUTPUT>
    class VisualizeEstimatedOrientation: public Transform2<INPUT, OUTPUT>
    {
      public:
        VisualizeEstimatedOrientation(Feature2<INPUT, OUTPUT>* estimator, int sampling, float theta0, float thetas, float theta1);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int n, Image<OUTPUT>& output, Image<unsigned char>* support= NULL);

        Feature2<INPUT, OUTPUT>* estimator;
        int sampling;
        float theta0, thetas, theta1;
    };

    template<typename INPUT, typename OUTPUT>
    VisualizeEstimatedOrientation<INPUT, OUTPUT>::VisualizeEstimatedOrientation(Feature2<INPUT, OUTPUT>* estimator, int sampling, float theta0, float thetas, float theta1)
        : Transform2<INPUT, OUTPUT>()
    {
        this->estimator= estimator;
        this->sampling= sampling;
        this->theta0= theta0;
        this->thetas= thetas;
        this->theta1= theta1;
    }

    template<typename INPUT, typename OUTPUT>
    void VisualizeEstimatedOrientation<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        int begin= estimator->getMin();
        int end= estimator->getMax();
        int n= input.n / sampling;

        srand(time(NULL));
        while ( n > 0 )
        {
            int smp= rand()%input.n;
            if ( smp > -begin && smp < input.n - end && (!roi || (*roi)(smp)) )
            {
                apply(input, smp, output, support);
                --n;
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    void VisualizeEstimatedOrientation<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<OUTPUT>& output, Image<unsigned char>* support)
    {
        int idx= estimator->apply(input, n, support);
        float a= theta0 + idx*thetas;

        drawSpike(output, n / input.columns, n % input.columns, a, 5, (OUTPUT)255);
    }
}

#endif	/* LOWLEVELTRANSFORM_H */

