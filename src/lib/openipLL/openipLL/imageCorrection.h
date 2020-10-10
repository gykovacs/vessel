/**
 * @file imageCorrection.h
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
 * Some usual image correction algorithms.
 */

#ifndef _IMAGE_CORRECTION_H_
#define _IMAGE_CORRECTION_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>
#include <openipDS/RunningStructuringElements.h>
#include <openipDS/StopperOpenMP.h>

#include <openipLL/imageCorrection.h>
#include <openipLL/imageIO.h>
#include <openipLL/ctmf.h>
#include <openipLL/morphology.h>

#include <openipSC/RealFunction.h>
#include <openipSC/rootFinding.h>

#include <openipDS/RunningStatisticalFeatures.h>

#include <math.h>

namespace openip
{
    /**
     * Derived variance function for gamma correction.
     */
    class DGammaFunction: public RealFunction
    {
    public:
        /**
         * constructor
         * @param v values
         */
        DGammaFunction(Vector<float>& v);

        /**
         * copy constructor
         */
        DGammaFunction(const DGammaFunction& g);

        /**
         * destructor
         */
        ~DGammaFunction();

        /**
         * function value at x
         * @param x evaluate the function at x
         * @return function value
         */
        virtual double value(double x);

        /**
         * function value of the derivative at x
         * @param x evaluate the derivative of the function at x
         * @return function value of the derivative
         */
        virtual double derivative(double x);

        /**
         * function value of the second derivative at x
         * @param x evaluate the second derivative of the function at x
         * @return function value of the second derivative
         */
        virtual double secondDerivative(double x);

        /** values in the variance */
        Vector<float> v;
    };

    /**
    * adaptive median filter
    * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
    */
    template<typename INPUT, typename OUTPUT>
    class AdaptiveMedianFilter: public Transform2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;
        using StructuringElementDisk::getMedian;
        using StructuringElementDisk::getMin;
        using StructuringElementDisk::getMax;
        using StructuringElementDisk::updateStride;

        /**
         * constructor
         * @param r radius
         */
        AdaptiveMedianFilter(int r= 3);

        /**
         * copy constructor
         * @param gc instance to copy
         */
        AdaptiveMedianFilter(const AdaptiveMedianFilter& gc);

        /**
         * destructor
         */
        ~AdaptiveMedianFilter();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    AdaptiveMedianFilter<INPUT, OUTPUT>::AdaptiveMedianFilter(int r)
    : StructuringElementDisk(r)
    {
        std::stringstream ss;
        ss << "AdaptiveMedianFilter " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveMedianFilter<INPUT, OUTPUT>::AdaptiveMedianFilter(const AdaptiveMedianFilter& gc)
    : StructuringElementDisk(gc)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveMedianFilter<INPUT, OUTPUT>::~AdaptiveMedianFilter()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void AdaptiveMedianFilter<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* , Image<unsigned char>* support)
    {
        INPUT max, min;
        input.getMinMax(min, max, support);
        this->updateStride(input.columns);

        INPUT median;
        float mean;

        for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
        {
            median= this->getMedian(input, i, support);
            mean= this->getMean(input, i, support);
            if ( median - (mean - min)/2.0 < input(i) && input(i) < median + (max - mean)/2.0 )
                output(i)= input(i);
            else
                output(i)= median;
        }
    }

    /**
     * gamma correction
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class GammaCorrection: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param gamma exponent
         */
        GammaCorrection(float gamma= 1.0);

        /**
         * copy constructor
         * @param gc instance to copy
         */
        GammaCorrection(const GammaCorrection& gc);

        /**
         * destructor
         */
        ~GammaCorrection();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** exponent */
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    GammaCorrection<INPUT, OUTPUT>::GammaCorrection(float gamma)
    {
        this->gamma= gamma;
        std::stringstream ss;
        ss << "GammaCorrection " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GammaCorrection<INPUT, OUTPUT>::GammaCorrection(const GammaCorrection& gc)
    {
        this->gamma= gc.gamma;
    }

    template<typename INPUT, typename OUTPUT>
    GammaCorrection<INPUT, OUTPUT>::~GammaCorrection()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GammaCorrection<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* )
    {
        if ( roi == NULL )
            for ( unsigned int i= 0; i < input.n; ++i )
                output(i)= pow(fabs(input(i)), gamma);
        else
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= pow(fabs(input(i)), gamma);
    }

    template<typename INPUT, typename OUTPUT>
    class NormalizedGammaCorrection: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param gamma exponent
         */
        NormalizedGammaCorrection(float gamma= 1.0);

        /**
         * copy constructor
         * @param gc instance to copy
         */
        NormalizedGammaCorrection(const NormalizedGammaCorrection& gc);

        /**
         * destructor
         */
        ~NormalizedGammaCorrection();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** exponent */
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    NormalizedGammaCorrection<INPUT, OUTPUT>::NormalizedGammaCorrection(float gamma)
    {
        this->gamma= gamma;
        std::stringstream ss;
        ss << "NormalizedGammaCorrection " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    NormalizedGammaCorrection<INPUT, OUTPUT>::NormalizedGammaCorrection(const NormalizedGammaCorrection& gc)
    {
        this->gamma= gc.gamma;
    }

    template<typename INPUT, typename OUTPUT>
    NormalizedGammaCorrection<INPUT, OUTPUT>::~NormalizedGammaCorrection()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void NormalizedGammaCorrection<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* )
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);
        if ( roi == NULL )
            for ( unsigned int i= 0; i < input.n; ++i )
                output(i)= pow(fabs(input(i) - min)/(float(max) - min), gamma);
        else
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= pow(fabs(input(i) - min)/(float(max) - min), gamma);
    }


    /**
     * background subtraction for illumination correction
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class BackgroundSubtraction: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        BackgroundSubtraction(int w1= 35, int w2= 25);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BackgroundSubtraction(const BackgroundSubtraction& b);

        /**
         * destructor
         */
        ~BackgroundSubtraction();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w1;
        /** method parameter */
        int w2;
    };
    
    template<typename INPUT, typename OUTPUT>
    class BackgroundEstimation: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        BackgroundEstimation(int w1= 35, int w2= 25);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BackgroundEstimation(const BackgroundEstimation& b);

        /**
         * destructor
         */
        ~BackgroundEstimation();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w1;
        /** method parameter */
        int w2;
    };

    template<typename INPUT, typename OUTPUT>
    class BackgroundEstimationMean: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        BackgroundEstimationMean(int w1= 35);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BackgroundEstimationMean(const BackgroundEstimationMean& b);

        /**
         * destructor
         */
        ~BackgroundEstimationMean();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w1;
    };

    template<typename INPUT, typename OUTPUT>
    class BackgroundEstimation3: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        BackgroundEstimation3(int w1= 35, int w2= 25);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BackgroundEstimation3(const BackgroundEstimation3& b);

        /**
         * destructor
         */
        ~BackgroundEstimation3();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w1;
        /** method parameter */
        int w2;
    };

    template<typename INPUT, typename OUTPUT>
    class BackgroundEstimationBright: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        BackgroundEstimationBright(int w1= 35, int w2= 25);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BackgroundEstimationBright(const BackgroundEstimationBright& b);

        /**
         * destructor
         */
        ~BackgroundEstimationBright();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w1;
        /** method parameter */
        int w2;
    };

    template<typename INPUT, typename OUTPUT>
    class BackgroundEstimationDark: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        BackgroundEstimationDark(int w1= 35, int w2= 25);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BackgroundEstimationDark(const BackgroundEstimationDark& b);

        /**
         * destructor
         */
        ~BackgroundEstimationDark();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w1;
        /** method parameter */
        int w2;
    };

    /**
     * histogram equalization
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class HistogramEqualization: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * default constructor
         */
        HistogramEqualization();

        /**
         * copy constuctor
         * @param b instance to copy
         */
        HistogramEqualization(const HistogramEqualization& b);

        /**
         * destructor
         */
        ~HistogramEqualization();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL, Image<unsigned char>* support= NULL);
    };
	
    /**
     * adaptive local contrast enhancement method
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class AdaptiveLocalContrastEnhancement: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w_ method parameter
         */
        AdaptiveLocalContrastEnhancement(int w_= 15);

        /**
         * copy constructor
         * @param b instance to copy
         */
        AdaptiveLocalContrastEnhancement(const AdaptiveLocalContrastEnhancement& b);

        /**
         * destructor
         */
        ~AdaptiveLocalContrastEnhancement();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w;
    };

    /**
     * adaptive histogram equalization
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class AdaptiveHistogramEqualization: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param h_ method parameter
         * @param r_ method parameter
         */
        AdaptiveHistogramEqualization(int h_= 15, float r_= 15);

        /**
         * copy constructor
         * @param b instance to copy
         */
        AdaptiveHistogramEqualization(const AdaptiveHistogramEqualization& b);

        /**
         * destructor
         */
        ~AdaptiveHistogramEqualization();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int h;
        /** method parameter */
        float r;
    };

    /**
     * desired average intensity
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class DesiredAverageIntensity: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param m_ method parameter
         * @param w_ method parameter
         */
        DesiredAverageIntensity(int m_= 15, int w_= 15);

        /**
         * copy constructor
         * @param b instance to copy
         */
        DesiredAverageIntensity(const DesiredAverageIntensity& b);

        /**
         * destructor
         */
        ~DesiredAverageIntensity();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int m;
        /** method parameter */
        int w;
    };

    /**
     * division by an over smoothed image
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class DivisionByAnOverSmoothed: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w_ method parameter
         */
        DivisionByAnOverSmoothed(int w_= 15);

        /**
         * copy constructor
         * @param b instance to copy
         */
        DivisionByAnOverSmoothed(const DivisionByAnOverSmoothed& b);

        /**
         * destructor
         */
        ~DivisionByAnOverSmoothed();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

#ifdef USE_OPENCL
/*
        std::string getOpenCLKernel();
        
        virtual void enqueueKernel(cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, Image<INPUT>& input);  
*/
#endif
        /** method parameter */
        int w;
    };

    /**
     * division by an over-smoothed
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class DivisionByAnOverSmoothedMean: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w_ method parameter
         */
        DivisionByAnOverSmoothedMean(int w_= 15);

        /**
         * copy constructor
         * @param b instance to copy
         */
        DivisionByAnOverSmoothedMean(const DivisionByAnOverSmoothedMean& b);

        /**
         * destructor
         */
        ~DivisionByAnOverSmoothedMean();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w;
    };

    /**
     * estimation of background luminosity and contrast variability
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class EstimationOfBackgroundLuminosityAndContrastVariability: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w_ method parameter
         */
        EstimationOfBackgroundLuminosityAndContrastVariability(int w_= 15);

        /**
         * copy constructor
         * @param b instance to copy
         */
        EstimationOfBackgroundLuminosityAndContrastVariability(const EstimationOfBackgroundLuminosityAndContrastVariability& b);

        /**
         * destructor
         */
        ~EstimationOfBackgroundLuminosityAndContrastVariability();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w;
    };


    /**
     * contrast limited adaptive histogram equalization
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class ContrastLimitedAdaptiveHistogramEqualization: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w_ method parameter
         * @param c_ contrast limit
         */
        ContrastLimitedAdaptiveHistogramEqualization(int w_= 15, float c_= 0.5);

        /**
         * copy constructor
         * @param b instance to copy
         */
        ContrastLimitedAdaptiveHistogramEqualization(const ContrastLimitedAdaptiveHistogramEqualization& b);

        /**
         * destructor
         */
        ~ContrastLimitedAdaptiveHistogramEqualization();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w;
        /** contrast limit */
        float cliplimit;
    };

    /**
     * morphological contrast enhancement
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class MorphologicalContrastEnhancement: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w_ method parameter
         */
        MorphologicalContrastEnhancement(int w_= 25);

        /**
         * copy constructor
         * @param b instance to copy
         */
        MorphologicalContrastEnhancement(const MorphologicalContrastEnhancement& b);

        /**
         * destructor
         */
        ~MorphologicalContrastEnhancement();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border2 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        int w;
    };

    /**
    * gamma correction
    * @param input input vector
    * @param output output vector
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    * @param gamma gamma value
    * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
    */
    template<typename INPUT, typename OUTPUT>
    void gammaCorrection(Vector<INPUT>* input, Vector<OUTPUT>* output, float gamma= 1.0, Vector<float>* mask= NULL);

    /**
    * adaptive local gamma correction
    * @param input input image
    * @param output output image
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    * @param w method parameter (window size)
    * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
    */
    template<typename INPUT, typename OUTPUT>
    float adaptiveLocalGammaCorrection(Image<INPUT>* input, Image<OUTPUT>* output, Image<unsigned char>* mask= NULL, int w= -1);


    /* -----------============== background subtraction ==============------------ */

    template<typename INPUT, typename OUTPUT>
    BackgroundSubtraction<INPUT, OUTPUT>::BackgroundSubtraction(int w1_, int w2_)
    : Transform2<INPUT, OUTPUT>(), w1(w1_), w2(w2_)
    {
        std::stringstream ss;
        ss << "BackgroundSubtraction" << " " << w1 << " " << w2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundSubtraction<INPUT, OUTPUT>::BackgroundSubtraction(const BackgroundSubtraction& b)
    : Transform2<INPUT, OUTPUT>(b), w1(b.w1), w2(b.w2)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundSubtraction<INPUT, OUTPUT>::~BackgroundSubtraction()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BackgroundSubtraction<INPUT, OUTPUT>::getProposedBorder()
    {
        if ( w1 > w2 )
            return Border2(w1/2, w1/2, w1/2, w1/2);
        else
            return Border2(w2/2, w2/2, w2/2, w2/2);
    }

    template<typename INPUT, typename OUTPUT>
    void BackgroundSubtraction<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        //printf("background subtr.\n");
        if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        if ( w2 == -1 )
            w2= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        RunningStructuringElementMean<INPUT> rse((StructuringElementSquare(w1)));
        rse.init(&input, support);

        Image<float> f;
        f.resizeImage(input);

        f= 0;
        output= 0;

        if ( support != NULL )
        {
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                if ( (*support)(i) > 0 )
                    (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }

            RunningStructuringElementMean<OUTPUT> rse2((StructuringElementSquare(w2)));
            rse2.init(&output, support);

            for ( int i= rse2.begin; i < rse2.end; ++i )
            {
                if ( (*mask)(i) > 0 )
                        (f)(i)= (input)(i) - rse2.mean;
                else
                    (f)(i)= 0;
                if ( rse2.position == rse2.end - 1 )
                    break;
                rse2.next();
            }
        }
        else
        {
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }

            RunningStructuringElementMean<OUTPUT> rse2((StructuringElementSquare(w2)));
            rse2.init(&output, support);

            for ( int i= rse2.begin; i < rse2.end; ++i )
            {
                (f)(i)= (input)(i) - rse2.mean;
                if ( rse2.position == rse2.end - 1 )
                    break;
                rse2.next();
            }
        }

        output= f;
    }
    
    template<typename INPUT, typename OUTPUT>
    BackgroundEstimation<INPUT, OUTPUT>::BackgroundEstimation(int w1_, int w2_)
    : Transform2<INPUT, OUTPUT>(), w1(w1_), w2(w2_)
    {
        std::stringstream ss;
        ss << "BackgroundEstimation" << " " << w1 << " " << w2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimation<INPUT, OUTPUT>::BackgroundEstimation(const BackgroundEstimation& b)
    : Transform2<INPUT, OUTPUT>(b), w1(b.w1), w2(b.w2)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimation<INPUT, OUTPUT>::~BackgroundEstimation()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BackgroundEstimation<INPUT, OUTPUT>::getProposedBorder()
    {
        if ( w1 > w2 )
            return Border2(w1/2, w1/2, w1/2, w1/2);
        else
            return Border2(w2/2, w2/2, w2/2, w2/2);
    }

    template<typename INPUT, typename OUTPUT>
    void BackgroundEstimation<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        if ( w2 == -1 )
            w2= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        RunningStructuringElementMean<INPUT> rse((StructuringElementSquare(w1)));
        rse.init(&input, support);

        Image<float> f;
        f.resizeImage(input);

        f= 0;
        output= 0;

        if ( support != NULL )
        {
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                if ( (*support)(i) > 0 )
                    (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }

            RunningStructuringElementMean<OUTPUT> rse2((StructuringElementSquare(w2)));
            rse2.init(&output, support);

            for ( int i= rse2.begin; i < rse2.end; ++i )
            {
                if ( (*mask)(i) > 0 )
                        (f)(i)= rse2.mean;
                else
                    (f)(i)= 0;
                if ( rse2.position == rse2.end - 1 )
                    break;
                rse2.next();
            }
        }
        else
        {
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }

            RunningStructuringElementMean<OUTPUT> rse2((StructuringElementSquare(w2)));
            rse2.init(&output, support);

            for ( int i= rse2.begin; i < rse2.end; ++i )
            {
                (f)(i)= rse2.mean;
                if ( rse2.position == rse2.end - 1 )
                    break;
                rse2.next();
            }
        }

        output= f;
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationMean<INPUT, OUTPUT>::BackgroundEstimationMean(int w1_)
    : Transform2<INPUT, OUTPUT>(), w1(w1_)
    {
        std::stringstream ss;
        ss << "BackgroundEstimationMean" << " " << w1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationMean<INPUT, OUTPUT>::BackgroundEstimationMean(const BackgroundEstimationMean& b)
    : Transform2<INPUT, OUTPUT>(b), w1(b.w1)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationMean<INPUT, OUTPUT>::~BackgroundEstimationMean()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BackgroundEstimationMean<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w1/2, w1/2, w1/2, w1/2);
    }

    template<typename INPUT, typename OUTPUT>
    void BackgroundEstimationMean<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        RunningStructuringElementMean<INPUT> rse((StructuringElementSquare(w1)));
        rse.init(&input, support);

        if ( support )
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                if ( (*mask)(i) > 0 )
                    (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }
        else
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimation3<INPUT, OUTPUT>::BackgroundEstimation3(int w1_, int w2_)
    : Transform2<INPUT, OUTPUT>(), w1(w1_), w2(w2_)
    {
        std::stringstream ss;
        ss << "BackgroundEstimation3" << " " << w1 << " " << w2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimation3<INPUT, OUTPUT>::BackgroundEstimation3(const BackgroundEstimation3& b)
    : Transform2<INPUT, OUTPUT>(b), w1(b.w1), w2(b.w2)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimation3<INPUT, OUTPUT>::~BackgroundEstimation3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BackgroundEstimation3<INPUT, OUTPUT>::getProposedBorder()
    {
        if ( w1 > w2 )
            return Border2(w1/2, w1/2, w1/2, w1/2);
        else
            return Border2(w2/2, w2/2, w2/2, w2/2);
    }

    template<typename INPUT, typename OUTPUT>
    void BackgroundEstimation3<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        if ( w2 == -1 )
            w2= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        RunningStructuringElementMean<INPUT> rse((StructuringElementSquare(w1)));
        rse.init(&input, support);

        if ( support )
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                if ( (*support)(i) > 0 )
                    (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }
        else
            for ( int i= rse.begin; i < rse.end; ++i )
            {
                (output)(i)= rse.mean;
                if ( rse.position == rse.end - 1 )
                    break;
                rse.next();
            }

        //output= 0;

        StructuringElementDisk sed(w2);
        sed.updateStride(input.columns);

        Image<OUTPUT> tmp;
        tmp.resizeImage(output);

        grayscaleDilate(&output, &tmp, sed, mask);

        output= tmp;
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationBright<INPUT, OUTPUT>::BackgroundEstimationBright(int w1_, int w2_)
    : Transform2<INPUT, OUTPUT>(), w1(w1_), w2(w2_)
    {
        std::stringstream ss;
        ss << "BackgroundEstimationBright" << " " << w1 << " " << w2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationBright<INPUT, OUTPUT>::BackgroundEstimationBright(const BackgroundEstimationBright& b)
    : Transform2<INPUT, OUTPUT>(b), w1(b.w1), w2(b.w2)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationBright<INPUT, OUTPUT>::~BackgroundEstimationBright()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BackgroundEstimationBright<INPUT, OUTPUT>::getProposedBorder()
    {
        if ( w1 > w2 )
            return Border2(w1/2, w1/2, w1/2, w1/2);
        else
            return Border2(w2/2, w2/2, w2/2, w2/2);
    }

    template<typename INPUT, typename OUTPUT>
    void BackgroundEstimationBright<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        if ( w2 == -1 )
            w2= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        Image<OUTPUT> tmp;
        Image<OUTPUT> tmp2;
        Image<OUTPUT> tmp3;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);
        tmp3.resizeImage(input);

        output= 0;

        GrayscaleDilate<INPUT, OUTPUT> gd(5);
        gd.apply(input, tmp2, mask, support);

        writeImage("gd.jpg", tmp2);
        BackgroundEstimationMean<INPUT, OUTPUT> bem(125);
        bem.apply(tmp2, tmp, mask, support);

        float total= 0;
        for ( int i= 0; i < mask->n; ++i )
            if ( (*mask)(i) > 0 )
                ++total;
        float sum= total;

        int pt= 0;
        while ( sum / total > 0.15 )
        {

            sum= 0;
            for ( int i= 0; i < input.n; ++i )
            {
                if ( input(i) > tmp(i) + pt )
                    sum++;
            }
            pt++;
            printf("%f\n", sum/total);fflush(stdout);
        }
        printf("----------\n"); fflush(stdout);

        output= tmp + pt;

        writeImage("threshold.png", output);
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationDark<INPUT, OUTPUT>::BackgroundEstimationDark(int w1_, int w2_)
    : Transform2<INPUT, OUTPUT>(), w1(w1_), w2(w2_)
    {
        std::stringstream ss;
        ss << "BackgroundEstimationDark" << " " << w1 << " " << w2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationDark<INPUT, OUTPUT>::BackgroundEstimationDark(const BackgroundEstimationDark& b)
    : Transform2<INPUT, OUTPUT>(b), w1(b.w1), w2(b.w2)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BackgroundEstimationDark<INPUT, OUTPUT>::~BackgroundEstimationDark()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BackgroundEstimationDark<INPUT, OUTPUT>::getProposedBorder()
    {
        if ( w1 > w2 )
            return Border2(w1/2, w1/2, w1/2, w1/2);
        else
            return Border2(w2/2, w2/2, w2/2, w2/2);
    }

    template<typename INPUT, typename OUTPUT>
    void BackgroundEstimationDark<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        if ( w2 == -1 )
            w2= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        Image<OUTPUT> tmp;
        Image<OUTPUT> tmp2;
        Image<OUTPUT> tmp3;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);
        tmp3.resizeImage(input);

        GaussianFilter2<INPUT, OUTPUT> gf(1, 5);
        gf.apply(input, tmp3, mask, support);
        input= tmp3;

        output= 255;

        for ( int i= 0; i < 1; ++i )
            for ( int j= 7; j < 10; ++j )
            {
                RunningDifferenceRingMeanFeature2<INPUT, OUTPUT> rdr(0, i, j, j+1);
                RunningDifferenceRingMaxMinFeature2<INPUT, OUTPUT> rdrmm(0, i, j, j+1);

                rdr.apply(input, tmp, mask, support);
                rdr.apply(input, tmp2, mask, support);

                for ( int i= 0; i < input.n; ++i )
                    if ( tmp(i) > -4 && tmp2(i) > -4 )
                        output(i)= input(i) - 1;
                /*else
                        output(i)= input(i) - 1;*/
            }

        BackgroundEstimationMean<INPUT, OUTPUT> bem(3);
        BackgroundEstimationMean<INPUT, OUTPUT> bem2(9);

        bem.apply(input, tmp, mask, support);
        bem2.apply(input, tmp2, mask, support);

        for ( int i= 0; i < input.n; ++i )
            if ( tmp2(i) < tmp(i) )
                output(i)= input(i) - 1;

        /*GrayscaleErode<INPUT, OUTPUT> gd(5);
        gd.apply(input, tmp2, mask, support);

        writeImage("gd.jpg", tmp2);
        BackgroundEstimationMean<INPUT, OUTPUT> bem(15);
        bem.apply(tmp2, tmp, mask, support);

        float total= 0;
        for ( int i= 0; i < mask->n; ++i )
            if ( (*mask)(i) > 0 )
                ++total;
        float sum= total;

        int pt= 0;
        while ( sum / total > 0.15 )
        {

            sum= 0;
            for ( int i= 0; i < input.n; ++i )
            {
                if ( (*mask)(i) && input(i) < tmp(i) - pt )
                    sum++;
            }
            pt++;
            printf("%f\n", sum/total);fflush(stdout);
        }
        printf("----------\n"); fflush(stdout);

        output= tmp - pt;*/

        /*if ( w1 == -1 )
            w1= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        if ( w2 == -1 )
            w2= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        Image<OUTPUT> tmp;
        Image<OUTPUT> tmp2;
        Image<OUTPUT> tmp3;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);
        tmp3.resizeImage(input);

        BackgroundEstimationMean<INPUT, OUTPUT> bem(w1);

        bem.apply(input, output, mask, support);

        GrayscaleErode<INPUT, OUTPUT> gd(2*w1);

        gd.apply(output, tmp, mask, support);

        //output= tmp;

        BackgroundEstimationMean<INPUT, OUTPUT> bem2(3*w1);

        bem2.apply(tmp, output, mask, support);*/

        writeImage("threshold.png", output);
    }

    /* -----------============== end of background subtraction ==============------------ */

    /* -----------============== histogram equalization ==============------------ */

    template<typename INPUT, typename OUTPUT>
    HistogramEqualization<INPUT, OUTPUT>::HistogramEqualization()
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "HistogramEqualization";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    HistogramEqualization<INPUT, OUTPUT>::HistogramEqualization(const HistogramEqualization& b)
    : Transform2<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    HistogramEqualization<INPUT, OUTPUT>::~HistogramEqualization()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 HistogramEqualization<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(0, 0, 0, 0);
    }

    template<typename INPUT, typename OUTPUT>
    void HistogramEqualization<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        Histogram h;

        INPUT min, max;
        input.getMinMax(min, max, support);
        if ( min < max )
        {
            //tprintf("compute histogram\n");
            h.computeHistogram(&input, 256, (INPUT)min, (INPUT)max, support);

            h.accumulate();

            std::vector<unsigned char> tmp;
            tmp.resize(h.size());

            //tprintf("operating\n");
            for ( unsigned int i= 0; i < input.n; ++i )
            {
                if ( !mask || (*mask)(i) )
                    (output)(i)= (float)(h.get((input)(i)))/(float)(input.n) * 255;
            }

            //tprintf("normalizing\n");
            output.normalize(0, 255, mask);
        }
        else
            output= input;
    }

    /* -----------============== histogram equalization ==============------------ */

    /* -----------============== adaptive local contrast enhancement ==============------------ */

    float sigm(float f, float mean, float var);

    template<typename INPUT, typename OUTPUT>
    AdaptiveLocalContrastEnhancement<INPUT, OUTPUT>::AdaptiveLocalContrastEnhancement(int w_)
    : Transform2<INPUT, OUTPUT>(), w(w_)
    {
        std::stringstream ss;
        ss << "AdaptiveLocalContrastEnhancement" << " " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveLocalContrastEnhancement<INPUT, OUTPUT>::AdaptiveLocalContrastEnhancement(const AdaptiveLocalContrastEnhancement& b)
    : Transform2<INPUT, OUTPUT>(b), w(b.w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveLocalContrastEnhancement<INPUT, OUTPUT>::~AdaptiveLocalContrastEnhancement()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 AdaptiveLocalContrastEnhancement<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2, w/2, w/2, w/2);
    }

    template<typename INPUT, typename OUTPUT>
    void AdaptiveLocalContrastEnhancement<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        RunningStructuringElementMeanAndVariance<INPUT> rse((StructuringElementSquare(w)));

        INPUT min, max;
        input.getMinMax(min, max, support);

        rse.init(&input);

        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( !mask || (*mask)(i) > 0 )
                if ( rse.variance > 0 )
                    (output)(rse.position)= 255 * ((sigm((input)(rse.position), rse.mean, rse.variance) - sigm(min, rse.mean, rse.variance))/(sigm(max, rse.mean, rse.variance) - sigm(min, rse.mean, rse.variance)));
            if ( rse.position == rse.end - 1 )
                break;
            rse.next();
        }
    }

    /* -----------============== end of adaptive local contrast enhancement ==============------------ */

    /* -----------============== adaptive histogram equalization ==============------------ */

    template<typename INPUT, typename OUTPUT>
    AdaptiveHistogramEqualization<INPUT, OUTPUT>::AdaptiveHistogramEqualization(int h_, float r_)
    : Transform2<INPUT, OUTPUT>(), h(h_), r(r_)
    {
        std::stringstream ss;
        ss << "AdaptiveHistogramEqualization" << " " << h << " " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveHistogramEqualization<INPUT, OUTPUT>::AdaptiveHistogramEqualization(const AdaptiveHistogramEqualization& b)
    : Transform2<INPUT, OUTPUT>(b), h(b.h), r(b.r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveHistogramEqualization<INPUT, OUTPUT>::~AdaptiveHistogramEqualization()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 AdaptiveHistogramEqualization<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(h/2, h/2, h/2, h/2);
    }

    template<typename INPUT, typename OUTPUT>
    void AdaptiveHistogramEqualization<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( h == -1 )
            h= input.columns > input.rows ? input.rows / 48 : input.columns / 48;
        StructuringElementSquare se(h);
        se.updateStride(input.columns);

        #pragma omp parallel for
        for ( int i= -se.min; i < (int)(input.n) - se.max; ++i )
        {
            if ( !mask || (*mask)(i) > 0 )
            {
                float ss= 0;
                for ( StructuringElementSquare::iterator sit= se.begin(); sit != se.end(); ++sit )
                {
                    if ( ! support || (*support)(i + *sit) > 0 )
                        if ( (input)(i) > (input)(i + *sit) )
                                ss+= 1;
                }
                ss/= h*h;

                (output)(i)= pow(ss, r);
            }
            else
                (output)(i)= 0;
        }
    }

    /* -----------============== end of adaptive histogram equalization ==============------------ */

    /* -----------============== desired average intensity ==============------------ */

    template<typename INPUT, typename OUTPUT>
    DesiredAverageIntensity<INPUT, OUTPUT>::DesiredAverageIntensity(int m_, int w_)
    : Transform2<INPUT, OUTPUT>(),m(m_), w(w_)
    {
        std::stringstream ss;
        ss << "DesiredAverageIntensity" << " " << m << " " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DesiredAverageIntensity<INPUT, OUTPUT>::DesiredAverageIntensity(const DesiredAverageIntensity& b)
    : Transform2<INPUT, OUTPUT>(b), m(b.m), w(b.w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DesiredAverageIntensity<INPUT, OUTPUT>::~DesiredAverageIntensity()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 DesiredAverageIntensity<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2+1, w/2+1, w/2+1, w/2+1);
    }

    template<typename INPUT, typename OUTPUT>
    void DesiredAverageIntensity<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        RunningStructuringElementMean<INPUT> rse((StructuringElementSquare(w)));
        rse.init(&input, support);

        output= 0;

        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( !mask || (*mask)(i) > 0 )
                (output)(i)= (input)(i) + m - rse.mean;
            if ( rse.position == rse.end - 1 )
                break;
            rse.next();
        }
    }

    /* -----------============== end of desired average intensity ==============------------ */

    /* -----------============== division by an over-smoothed ==============------------ */

    template<typename INPUT, typename OUTPUT>
    DivisionByAnOverSmoothed<INPUT, OUTPUT>::DivisionByAnOverSmoothed(int w_)
    : Transform2<INPUT, OUTPUT>(), w(w_)
    {
        std::stringstream ss;
        ss << "DivisionByAnOverSmoothed" << " " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DivisionByAnOverSmoothed<INPUT, OUTPUT>::DivisionByAnOverSmoothed(const DivisionByAnOverSmoothed& b)
    : Transform2<INPUT, OUTPUT>(b), w(b.w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DivisionByAnOverSmoothed<INPUT, OUTPUT>::~DivisionByAnOverSmoothed()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 DivisionByAnOverSmoothed<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2, w/2, w/2, w/2);
    }

    template<typename INPUT, typename OUTPUT>
    void DivisionByAnOverSmoothed<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        /*Image<unsigned char> tmp1;
        Image<unsigned char> tmp2;
        tmp1.resizeImage(input);
        tmp2.resizeImage(input);

        INPUT min, max;
        input.getMinMax(min, max, mask);
        input.normalize(0, 255);

        tmp1= input;
        tmp2= 0;*/

        MeanFeature2<INPUT, OUTPUT> mf(w/2);
        mf.updateStride(input.columns);
        output= 0;
        //ctmf(tmp1.data(), tmp2.data(), input.columns, input.rows, input.columns, input.columns, w, 1, 512);
        StopperOpenMP st;
        st.start();
#ifdef USE_OPENCL
        if ( mf.applyGPU(input, output, mask) )
#endif
        {
            printf("on CPU\n"); fflush(stdout);
        mf.apply(input, output, mask, support);
        }
        st.stop();
        cout << st.getElapsedTime();

        writeImage("aaa.png", output);
        /*for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= input(i)/255.0*(max - min);*/
        /*for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= tmp2(i);*/

        #pragma omp parallel for
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                if ( fabs(output(i)) > FLT_EPSILON )
                {
                    float tmp= ((float)((input)(i)))/((output)(i));

                    if ( tmp < 3.0 && tmp > 0.2 )
                        (output)(i)= tmp;
                    else
                        (output)(i)= input(i);
                }
                else
                    output(i)= 0.0;
            }
            else 
                output(i)= 0.0;
        }

        /*OUTPUT mino, maxo;
        output.getMinMax(mino, maxo, mask);
        printf("%f %f\n", mino, maxo);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                output(i)= output(i)/(maxo - mino) * (max - min) + min;
            }
        }*/
    }
    
#ifdef USE_OPENCL    
    
    /*template<typename INPUT, typename OUTPUT>
    std::string DivisionByAnOverSmoothed<INPUT, OUTPUT>::getOpenCLKernel()
    {
        std::string kernel;
        std::stringstream ss;
        ss.clear();

        ss << this->min;

        kernel= string("__kernel void convolution(const __global ") + type(typeid(INPUT)) + string("* input, ") +
                string("__global ") + type(typeid(OUTPUT)) + string("* output)\n")
                + string("{\n"
                "\tconst int idx= get_global_id(0) - (") + ss.str() + string(");\n"
                "\tfloat sum= 0;\n");

        ss.str("");
        ss << this->size()
        kernel+= string("float array[") +
        for ( fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( fit->first >= 0 )
            {
            kernel+= string("\tsum+= input[idx + ");
            }
            else
            {
                kernel+= string("\tsum+= input[idx ");
            }
            ss << fit->first;
            kernel+= ss.str();
            ss.str("");
            kernel+= string("] * (");
            ss << fit->second;
            kernel+= ss.str();
            ss.str("");
            kernel+= string(");\n");
        }

        kernel+= string("\toutput[idx]= (")+ type(typeid(OUTPUT))+string(")sum;\n");
        kernel+= string("}\n");

        return kernel;
    }
    
    template<typename INPUT, typename OUTPUT>
    void DivisionByAnOverSmoothed<INPUT, OUTPUT>::enqueueKernel(cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, Image<INPUT>& input)
    {
        
    }*/
    
#endif
    
    template<typename INPUT, typename OUTPUT>
    DivisionByAnOverSmoothedMean<INPUT, OUTPUT>::DivisionByAnOverSmoothedMean(int w_)
    : Transform2<INPUT, OUTPUT>(), w(w_)
    {
        std::stringstream ss;
        ss << "DivisionByAnOverSmoothedMean" << " " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DivisionByAnOverSmoothedMean<INPUT, OUTPUT>::DivisionByAnOverSmoothedMean(const DivisionByAnOverSmoothedMean& b)
    : Transform2<INPUT, OUTPUT>(b), w(b.w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DivisionByAnOverSmoothedMean<INPUT, OUTPUT>::~DivisionByAnOverSmoothedMean()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 DivisionByAnOverSmoothedMean<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2, w/2, w/2, w/2);
    }

    template<typename INPUT, typename OUTPUT>
    void DivisionByAnOverSmoothedMean<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        /*Image<unsigned char> tmp1;
        Image<unsigned char> tmp2;
        tmp1.resizeImage(input);
        tmp2.resizeImage(input);

        INPUT min, max;
        input.getMinMax(min, max, mask);
        input.normalize(0, 255);

        tmp1= input;
        tmp2= 0;*/

        //MedianFeature2<INPUT, OUTPUT> mf(w);
        RunningStructuringElementMean<INPUT> rse((StructuringElementDisk(w)));
        rse.init(&input, support);
        //ctmf(tmp1.data(), tmp2.data(), input.columns, input.rows, input.columns, input.columns, w, 1, 512);
        //mf.apply(input, output, mask);
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( (*mask)(i) )
                output(i)= rse.mean;
            if ( rse.position == rse.end - 1 )
                break;
            rse.next();
        }
        //rse.apply(input, output, mask);

        /*for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= input(i)/255.0*(max - min);*/
        /*for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= tmp2(i);*/

        #pragma omp parallel for
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                if ( output(i) > FLT_EPSILON )
                {
                    float tmp= ((float)((input)(i)))/((output)(i));

                    if ( tmp < 3.0 && tmp > 0.2 )
                        (output)(i)= tmp;
                    else
                        (output)(i)= input(i);
                }
                else
                    output(i)= 0.0;
            }
            else
                output(i)= 0.0;
        }

        /*OUTPUT mino, maxo;
        output.getMinMax(mino, maxo, mask);
        printf("%f %f\n", mino, maxo);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                output(i)= output(i)/(maxo - mino) * (max - min) + min;
            }
        }*/
    }

    /* -----------============== end of division by an over-smoothed ==============------------ */

    /* -----------============== estimation of background luminosity and contrast variability ==============------------ */

    template<typename INPUT, typename OUTPUT>
    EstimationOfBackgroundLuminosityAndContrastVariability<INPUT, OUTPUT>::EstimationOfBackgroundLuminosityAndContrastVariability( int w_)
    : Transform2<INPUT, OUTPUT>(), w(w_)
    {
        std::stringstream ss;
        ss << "EstimationOfBackgroundLuminosityAndContrastVariability" << " " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    EstimationOfBackgroundLuminosityAndContrastVariability<INPUT, OUTPUT>::EstimationOfBackgroundLuminosityAndContrastVariability(const EstimationOfBackgroundLuminosityAndContrastVariability& b)
    : Transform2<INPUT, OUTPUT>(b), w(b.w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    EstimationOfBackgroundLuminosityAndContrastVariability<INPUT, OUTPUT>::~EstimationOfBackgroundLuminosityAndContrastVariability()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 EstimationOfBackgroundLuminosityAndContrastVariability<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2, w/2, w/2, w/2);
    }

    template<typename INPUT, typename OUTPUT>
    void EstimationOfBackgroundLuminosityAndContrastVariability<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        Image<float> tmp;
        tmp.resizeImage(input);

        RunningStructuringElementMeanAndVariance<INPUT> rse((StructuringElementSquare(w)));
        rse.init(&input, support);

        output= 0;

        rse.init(&input, support);

        for ( unsigned int i= rse.begin; rse.position < rse.end; ++i )
        {
            if ( !mask || (*mask)(i) > 0 )
            {
                if ( rse.variance > FLT_EPSILON )
                    (output)(i)= ((input)(i) - rse.mean)/sqrt(rse.variance);
            }
            
            if ( rse.position == rse.end - 1 )
                break;
            rse.next();
        }
    }

    /* -----------============== end of estimation of background luminosity and contrast variability ==============------------ */

    /* -----------============== contrast limited adaptive histogram equalization ==============------------ */

    template<typename INPUT, typename OUTPUT>
    ContrastLimitedAdaptiveHistogramEqualization<INPUT, OUTPUT>::ContrastLimitedAdaptiveHistogramEqualization( int w_, float c_ )
    : Transform2<INPUT, OUTPUT>(), w(w_), cliplimit(c_)
    {
        std::stringstream ss;
        ss << "ContrastLimitedAdaptiveHistogramEqualization" << " " << w << " " << cliplimit;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ContrastLimitedAdaptiveHistogramEqualization<INPUT, OUTPUT>::ContrastLimitedAdaptiveHistogramEqualization(const ContrastLimitedAdaptiveHistogramEqualization& b)
    : Transform2<INPUT, OUTPUT>(b), w(b.w), cliplimit(b.cliplimit)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ContrastLimitedAdaptiveHistogramEqualization<INPUT, OUTPUT>::~ContrastLimitedAdaptiveHistogramEqualization()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 ContrastLimitedAdaptiveHistogramEqualization<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2, w/2, w/2, w/2);
    }

    template<typename INPUT, typename OUTPUT>
    void ContrastLimitedAdaptiveHistogramEqualization<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        RunningStructuringElementHistogram<INPUT> rseh((StructuringElementSquare(w)));
        rseh.init(&input, support);

        Image<float> tmp;
        Image<float> tmp2;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);
        Histogram h;

        tmp= 0;
        tmp2= 0;

        if ( support == NULL )
        {
            //#pragma omp parallel for
            for ( unsigned int i= -rseh.se.min; i < input.n - rseh.se.max; ++i )
            {
                /*float clipTotal= 0;
                float partialRank= 0;
                float incr;
                float redistr;

                for ( StructuringElementSquare::iterator sit= rseh.se->begin(); sit != rseh.se->end(); ++sit )
                {
                    if ( (tmp)(i + *sit) > cliplimit )
                        incr= cliplimit / (tmp)(i + *sit);
                    else
                        incr= 1;
                    clipTotal= clipTotal + (1 - incr);
                    if ( (input)(i) > (input)(i + *sit ) )
                        partialRank+= incr;
                }
                redistr= (clipTotal / rseh.se->size()) * (input)(i);
                (tmp2)(i)= partialRank + redistr;*/
                    h= rseh.h;
                    h.normalize();
                    /*float clipTotal= 0;
                    float partialRank= 0;
                    float incr;
                    float redistr;
                    int n= 0;*/

                    /*for ( StructuringElementSquare::iterator sit= rseh.se->begin(); sit != rseh.se->end(); ++sit )
                    {
                        if ( (*mask)(i + *sit) > 0 )
                        {
                            n++;
                            if ( (tmp)(i + *sit) > cliplimit )
                                incr= cliplimit / (tmp)(i + *sit);
                            else
                                incr= 1;
                            clipTotal= clipTotal + (1 - incr);
                            if ( (input)(i) > (input)(i + *sit ) )
                                partialRank+= incr;
                        }
                    }
                    redistr= (clipTotal / n) * (input)(i);
                    (tmp2)(i)= partialRank + redistr;*/
                    float a= 0;
                    for ( unsigned int j= 0; j < h.size(); ++j )
                        if ( h(j) > cliplimit )
                        {
                            a+= h(j) - cliplimit;
                            h(j)= cliplimit;
                        }
                    for ( unsigned int j= 0; j < h.size(); ++j )
                        h(j)+= a/h.size();
                    h.accumulate();

                    output(i)= h(input(i));
                    if ( rseh.position == rseh.end - 1 )
                        break;
                    rseh.next();
            }
        }
        else
        {
            //#pragma omp parallel for
            for ( unsigned int i= -rseh.se.min; i < input.n - rseh.se.max; ++i )
            {
                if ( (*mask)(i) > 0 )
                {
                    h= rseh.h;
                    h.normalize();
                    /*float clipTotal= 0;
                    float partialRank= 0;
                    float incr;
                    float redistr;
                    int n= 0;*/

                    /*for ( StructuringElementSquare::iterator sit= rseh.se->begin(); sit != rseh.se->end(); ++sit )
                    {
                        if ( (*mask)(i + *sit) > 0 )
                        {
                            n++;
                            if ( (tmp)(i + *sit) > cliplimit )
                                incr= cliplimit / (tmp)(i + *sit);
                            else
                                incr= 1;
                            clipTotal= clipTotal + (1 - incr);
                            if ( (input)(i) > (input)(i + *sit ) )
                                partialRank+= incr;
                        }
                    }
                    redistr= (clipTotal / n) * (input)(i);
                    (tmp2)(i)= partialRank + redistr;*/
                    float a= 0;
                    for ( unsigned int j= 0; j < h.size(); ++j )
                        if ( h(j) > cliplimit )
                        {
                            a+= h(j) - cliplimit;
                            h(j)= cliplimit;
                        }
                    for ( unsigned int j= 0; j < h.size(); ++j )
                        h(j)+= a/h.size();
                    h.accumulate();

                    output(i)= h(input(i));
                }
                if ( rseh.position == rseh.end - 1 )
                    break;
                rseh.next();
            }
        }

        /*if ( mask == NULL )
            output= tmp2;
        else
            #pragma omp parallel for
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*mask)(i) > 0 )
                    (output)(i)= (tmp2)(i);*/
    }

    /* -----------============== end of contrast limited adaptive histogram equalization ==============------------ */

    /* -----------============== morphological contrast enhancement ==============------------ */

    template<typename INPUT, typename OUTPUT>
    MorphologicalContrastEnhancement<INPUT, OUTPUT>::MorphologicalContrastEnhancement( int w_ )
    : Transform2<INPUT, OUTPUT>(), w(w_)
    {
        std::stringstream ss;
        ss << "MorphologicalContrastEnhancement" << " " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MorphologicalContrastEnhancement<INPUT, OUTPUT>::MorphologicalContrastEnhancement(const MorphologicalContrastEnhancement& b)
    : Transform2<INPUT, OUTPUT>(b), w(b.w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MorphologicalContrastEnhancement<INPUT, OUTPUT>::~MorphologicalContrastEnhancement()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 MorphologicalContrastEnhancement<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(w/2, w/2, w/2, w/2);
    }

    template<typename INPUT, typename OUTPUT>
    void MorphologicalContrastEnhancement<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* support)
    {
        if ( w == -1 )
            w= input.columns > input.rows ? input.rows / 48 : input.columns / 48;

        Image<unsigned char> tmp1;
        Image<unsigned char> tmp2;
        Image<unsigned char> tmpi;
        Image<unsigned char> tmpo;
        Image<float> tmp3;
        tmp1.resizeImage(input);
        tmp2.resizeImage(input);
        tmpi.resizeImage(input);
        tmpo.resizeImage(input);
        tmp3.resizeImage(input);

        tmpi= input;
        tmpo= output;
        StructuringElementSquare se(w, input.columns);

        grayscaleErode(&tmpi, &tmp2, se, support);
        grayscaleDilate(&tmp2, &tmp1, se, support);
        grayscaleDilate(&tmpi, &tmpo, se, support);
        grayscaleErode(&tmpo, &tmp2, se, support);

        #pragma omp parallel for
        for ( unsigned int i= 0; i < output.n; ++i )
            (tmp3)(i)= (int)((input)(i)) * 3 - (tmp1)(i) - (tmp2)(i);

        if ( mask == NULL )
            output= tmp3;
        else
            #pragma omp parallel for
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( (*mask)(i) > 0 )
                    (output)(i)= (tmp3)(i);
    }

    /* -----------============== end of morphological contrast enhancement ==============------------ */

    /**
     * exponential histogram equalization
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class ExponentialHistogramEqualization: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        ExponentialHistogramEqualization(float alpha);

        /**
         * copy constructor
         * @param b instance to copy
         */
        ExponentialHistogramEqualization(const ExponentialHistogramEqualization& b);

        /**
         * destructor
         */
        ~ExponentialHistogramEqualization();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        float alpha;
    };

    template<typename INPUT, typename OUTPUT>
    ExponentialHistogramEqualization<INPUT, OUTPUT>::ExponentialHistogramEqualization(float alpha)
    : Transform2<INPUT, OUTPUT>()
    {
        this->alpha= alpha;

        std::stringstream ss;
        ss << "ExponentialHistogramEqualization " << alpha;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ExponentialHistogramEqualization<INPUT, OUTPUT>::ExponentialHistogramEqualization(const ExponentialHistogramEqualization& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
        this->alpha= e.alpha;
    }

    template<typename INPUT, typename OUTPUT>
    ExponentialHistogramEqualization<INPUT, OUTPUT>::~ExponentialHistogramEqualization()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ExponentialHistogramEqualization<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        Histogram h;

        h.computeHistogram(&input, 256, (INPUT)0, (INPUT)255);

        h.accumulate();

        std::vector<unsigned char> tmp;
        tmp.resize(h.size());

        float minf= FLT_MAX, maxf= 0;
        float tmpf;
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            tmpf= (0 - 1/alpha*log(1 - float(h((input)(i))+1)/float((input).n + 2)));

            if ( tmpf < minf )
                minf= tmpf;
            if ( tmpf > maxf )
                maxf= tmpf;
        }

        //printf("%f %f\n", minf, maxf);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            output(i)= ((0.0 - 1/alpha*log(1 - float(h((input)(i))+1)/float((input).n + 2))) - minf)/(maxf - minf)*255;
        }
        output.normalize(0, 255);
    }

    /**
     * Rayleigh histogram equalization
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class RayleighHistogramEqualization: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        RayleighHistogramEqualization(float alpha);

        /**
         * copy constructor
         * @param b instance to copy
         */
        RayleighHistogramEqualization(const RayleighHistogramEqualization& b);

        /**
         * destructor
         */
        ~RayleighHistogramEqualization();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** method parameter */
        float alpha;
    };

    template<typename INPUT, typename OUTPUT>
    RayleighHistogramEqualization<INPUT, OUTPUT>::RayleighHistogramEqualization(float alpha)
    : Transform2<INPUT, OUTPUT>()
    {
        this->alpha= alpha;

        std::stringstream ss;
        ss << "RayleighHistogramEqualization " << alpha;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RayleighHistogramEqualization<INPUT, OUTPUT>::RayleighHistogramEqualization(const RayleighHistogramEqualization& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
        this->alpha= e.alpha;
    }

    template<typename INPUT, typename OUTPUT>
    RayleighHistogramEqualization<INPUT, OUTPUT>::~RayleighHistogramEqualization()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RayleighHistogramEqualization<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        Histogram h;

        h.computeHistogram(&input, 256, (INPUT)0, (INPUT)255);

        h.accumulate();

        std::vector<unsigned char> tmp;
        tmp.resize(h.size());

        float tmpf, minf, maxf;
        minf= FLT_MAX;
        maxf= 0;
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            tmpf= 0 + pow(2*alpha*alpha*log(1.0/(1.0-(float(h((input)(i)))+1)/(input.n+2))), 0.5);
            if ( tmpf < minf )
                minf= tmpf;
            if ( tmpf > maxf )
                maxf= tmpf;
            //(*output)(i)= (float)(h((*input)(i)))/(float)(*input).n * 255;
        }
        //printf("%f %f\n", minf, maxf);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            output(i)= ((0 + pow(2*alpha*alpha*log(1.0/(1.0-(float(h((input)(i)))+1)/(input.n+2))), 0.5)) - minf)/(maxf - minf) * 255;
        }

        output.normalize(0, 255);
    }

    /**
     * hyperbolic histogram equalization
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class HyperbolicHistogramEqualization: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        HyperbolicHistogramEqualization();

        /**
         * copy constructor
         * @param b instance to copy
         */
        HyperbolicHistogramEqualization(const HyperbolicHistogramEqualization& b);

        /**
         * destructor
         */
        ~HyperbolicHistogramEqualization();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    HyperbolicHistogramEqualization<INPUT, OUTPUT>::HyperbolicHistogramEqualization()
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "HyperbolicHistogramEqualization";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    HyperbolicHistogramEqualization<INPUT, OUTPUT>::HyperbolicHistogramEqualization(const HyperbolicHistogramEqualization& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    HyperbolicHistogramEqualization<INPUT, OUTPUT>::~HyperbolicHistogramEqualization()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void HyperbolicHistogramEqualization<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        Histogram h;

        h.computeHistogram(&input, 256, (INPUT)0, (INPUT)255);

        h.accumulate();

        std::vector<unsigned char> tmp;
        tmp.resize(h.size());

        for ( unsigned int i= 0; i < input.n; ++i )
        {
            (output)(i)= pow(pow(255.0,1/3.0)*h((input)(i)), 3.0);
            //(*output)(i)= (float)(h((*input)(i)))/(float)(*input).n * 255;
        }
        output.normalize(0, 255);
    }
    /* -----------============== adaptive global gamma correction ==============------------ */

    /**
     * adaptive global gamma correction
     * @param input input image
     * @param output output image
     * @param mask the operation is performed in the foreground (non-zero) region of the mask
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    float adaptiveGlobalGammaCorrection(Image<INPUT>* input, Image<OUTPUT>* output, Image<unsigned char>* mask)
    {
        return adaptiveLocalGammaCorrection(input, output, mask, -1);
    }

    /* -----------============== end of adaptive global gamma correction ==============------------ */



    /* -----------============== adaptive local gamma correction ==============------------ */

    /**
     * adaptive local gamma correction
     */
    template<typename INPUT, typename OUTPUT>
    class AdaptiveLocalGammaCorrection: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        AdaptiveLocalGammaCorrection(int w);

        /**
         * copy constructor
         * @param b instance to copy
         */
        AdaptiveLocalGammaCorrection(const AdaptiveLocalGammaCorrection& b);

        /**
         * destructor
         */
        ~AdaptiveLocalGammaCorrection();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** window size */
        int w;
    };

    template<typename INPUT, typename OUTPUT>
    AdaptiveLocalGammaCorrection<INPUT, OUTPUT>::AdaptiveLocalGammaCorrection(int w)
    : Transform2<INPUT, OUTPUT>()
    {
        this->w= w;
        std::stringstream ss;
        ss << "AdaptiveLocalGammaCorrection " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveLocalGammaCorrection<INPUT, OUTPUT>::AdaptiveLocalGammaCorrection(const AdaptiveLocalGammaCorrection& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveLocalGammaCorrection<INPUT, OUTPUT>::~AdaptiveLocalGammaCorrection()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void AdaptiveLocalGammaCorrection<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        StructuringElementDisk sed(w);
        sed.updateStride(input.columns);
        RunningStructuringElementLocalGamma<INPUT> rs(sed);
        rs.init(&input, support);
        rs.init(0);

        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( !roi || (*roi)(i) > 0 )
                output(i)= pow((float)(input(i) + 1)/257.0, rs.gamma)*255;
            if ( rs.position == rs.end - 1 )
                break;
            rs.next();
        }
    }

    /**
     * adaptive global gamma correction
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class AdaptiveGlobalGammaCorrection: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        AdaptiveGlobalGammaCorrection();

        /**
         * copy constructor
         * @param b instance to copy
         */
        AdaptiveGlobalGammaCorrection(const AdaptiveGlobalGammaCorrection& b);

        /**
         * destructor
         */
        ~AdaptiveGlobalGammaCorrection();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    AdaptiveGlobalGammaCorrection<INPUT, OUTPUT>::AdaptiveGlobalGammaCorrection()
    : Transform2<INPUT, OUTPUT>()
    {
        printf("ddd"); fflush(stdout);
        std::stringstream ss;
        ss << "AdaptiveGlobalGammaCorrection ";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveGlobalGammaCorrection<INPUT, OUTPUT>::AdaptiveGlobalGammaCorrection(const AdaptiveGlobalGammaCorrection& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AdaptiveGlobalGammaCorrection<INPUT, OUTPUT>::~AdaptiveGlobalGammaCorrection()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void AdaptiveGlobalGammaCorrection<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* )
    {
        Image<float> coeff;
        coeff.resizeImage(256, 256);

        for ( int i= 0; i < 256; ++i )
            for ( int j= 0; j < 256; ++j )
            {
                coeff(i,j)= (-(log(log((i+1)/257.0)/log((j+1)/257.0)))/(log((i+1)/257.0)-log((j+1)/257.0)));
            }

        Vector<float> histogram(256);
        histogram= 0;
        for ( unsigned int i= 0; i < input.n; ++i )
            histogram((input)(i))= histogram((input)(i)) + 1;

        float gamma= 0;
        float sum= 0;
        for ( unsigned int i= 0; i < histogram.size(); ++i )
        {
            for ( unsigned int j= 0; j < histogram.size(); ++j )
            {
                if ( i != j )
                {
                    gamma+= histogram(i)*histogram(j)*coeff(i,j);
                    sum+= histogram(i)*histogram(j);
                }
            }
        }
        gamma/= sum;

        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( !roi || (*roi)(i) > 0 )
                output(i)= pow((float)(input(i) + 1)/257.0, gamma)*255;
        }
    }

    /**
     * continuous adaptive local gamma correction
     * @see Aliaa A. A. Youssif, Atef Z. Ghalwash, Amr S. Ghoneim: Comparative Study of Contrast Enhancement and Illumination Equalization Methods for Retinal Vasculature Segmentation
     */
    template<typename INPUT, typename OUTPUT>
    class CAdaptiveLocalGammaCorrection: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        CAdaptiveLocalGammaCorrection(int w);

        /**
         * copy constructor
         * @param b instance to copy
         */
        CAdaptiveLocalGammaCorrection(const CAdaptiveLocalGammaCorrection& b);

        /**
         * destructor
         */
        ~CAdaptiveLocalGammaCorrection();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** window size */
        int w;
    };

    template<typename INPUT, typename OUTPUT>
    CAdaptiveLocalGammaCorrection<INPUT, OUTPUT>::CAdaptiveLocalGammaCorrection(int w)
    : Transform2<INPUT, OUTPUT>()
    {
        this->w= w;
        std::stringstream ss;
        ss << "CAdaptiveLocalGammaCorrection " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CAdaptiveLocalGammaCorrection<INPUT, OUTPUT>::CAdaptiveLocalGammaCorrection(const CAdaptiveLocalGammaCorrection& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CAdaptiveLocalGammaCorrection<INPUT, OUTPUT>::~CAdaptiveLocalGammaCorrection()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void CAdaptiveLocalGammaCorrection<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max);
        input.normalize(0.05,0.95);

        StructuringElementDisk sed(w);
        sed.updateStride(input.columns);

        float sum= 0;
        float gamma= 0;
        for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
        {
            sum= 0;
            gamma= 0;
            for ( StructuringElementDisk::iterator sit= sed.begin(); sit != sed.end(); ++sit )
            {
                for ( StructuringElementDisk::iterator sit2= sed.begin(); sit2 != sed.end(); ++sit2 )
                {
                    if ( input(i + *sit) != input(i + *sit2) )
                    {
                        gamma+= -((log(log(input(i + *sit))/log((input)(i + *sit2)))/(log(input(i + *sit2))-log(input(i + *sit)))));
                        sum+= 1;
                    }
                }
            }
            output(i)= pow(input(i), gamma/sum);
        }

        output.normalize(min, max);
    }

    template<typename INPUT, typename OUTPUT>
    float adaptiveLocalGammaCorrection(Image<INPUT>* input, Image<OUTPUT>* output, Image<unsigned char>* mask, int w)
    {
        Image<float>* gammas= new Image<float>(input->rows, input->columns);
        printf("%d\n", w);

        if ( w == -1 )
        {
            Vector<float> histogram(256);
            histogram= 0;
            for ( unsigned int i= 0; i < input->n; ++i )
                histogram((*input)(i))= histogram((*input)(i)) + 1;

            double gamma= 0;
            double sum= 0;
            double a= 0, b= 0, c= 0;
            for ( int i= 0; i < 256; ++i )
            {
                for ( int j= i; j < 256; ++j )
                {
                    if ( i != j )
                    {
                        gamma+= histogram(i)*histogram(j)*(-(log(log((i+1)/257.0)/log((j+1)/257.0)))/(log((i+1)/257.0)-log((j+1)/257.0)));
                        sum+= histogram(i)*histogram(j);
                    }
                    a += histogram(i)*histogram(j)*(log(log((i+1)/257.0)/log((j+1)/257.0)));
                    b += histogram(i)*histogram(j)*(log((j+1)*(j+1)/257.0/257.0) - log((i+1)*(j+1)/257.0/257.0));
                    if ( i != j )
                    c += histogram(i)*histogram(j)*(log(log((i+1)/257.0)/log((j+1)/257.0)))/(log((j+1)*(j+1)/257.0/257.0) - log((i+1)*(j+1)/257.0/257.0));
                }
            }

            printf("gamma: %f\n", gamma/input->size());
            printf("gamma: %f\n", gamma/input->size()/input->size());
            printf("gamma: %f\n", gamma/input->size()/(input->size()-1)*2);
            gamma/= sum;
            printf("gamma: %f\n", gamma);
            printf("gamma2: %f\n", c/ (input->size() * (input->size()-1) / 2));

            printf("new: %f %f %f\n", a, b, a/b);

	    if ( mask == NULL )
	    {
                for ( unsigned int i= 0; i < input->size(); ++i )
                    (*output)(i)= pow((float((*input)(i)) + 1)/257.0, gamma) * 255;
            }
            else
            {
                for ( unsigned int i= 0; i < input->size(); ++i )
                    if ( (*mask)(i) > 0 )
                        (*output)(i)= pow((float((*input)(i)) + 1)/257.0, gamma) * 255;
            }
            return gamma;
        }
        else
        {
            printf("local gamma"); fflush(stdout);
            StructuringElementDisk sed(w, input->columns);
            RunningStructuringElementLocalGamma<INPUT> rselg(&sed);
            rselg.init(input, mask);
            rselg.init(0);
            int i= 0;
            while ( rselg.position < (int)(input->n))
            {
                if ( rselg.position % input->columns == 0 )
                {
                    printf("%d ", i);
                    fflush(stdout);
                    ++i;
                }
                (*output)(rselg.position)= pow((float((*input)(rselg.position)) + 1)/257.0, rselg.gamma) * 255;
                if ( rselg.position == rselg.end - 1 )
                    break;
                rselg.next();
            }
        }
        delete gammas;
        return 0.0f;
    }

    template<typename INPUT, typename OUTPUT>
    void gammaCorrection(Vector<INPUT>* input, Vector<OUTPUT>* output, float gamma, Vector<float>* mask)
    {
        if ( mask == NULL )
            //#pragma omp parallel for
            for ( unsigned int i= 0; i < input->size(); ++i )
                (*output)(i)= pow(((float)(*input)(i)+1)/257.0, gamma)*255;
        else
            //#pragma omp parallel for
            for ( unsigned int i= 0; i < input->size(); ++i )
                if ( (*mask)(i) > 0 )
                    (*output)(i)= pow(((float)(*input)(i)+1)/257.0, gamma)*255;
    }

    /* -----------============== end of adaptive local gamma correction ==============------------ */
    
    template<typename INPUT, typename OUTPUT>
    class LocalMinimumGlobalMaximum: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        LocalMinimumGlobalMaximum(int w);

        /**
         * copy constructor
         * @param b instance to copy
         */
        LocalMinimumGlobalMaximum(const LocalMinimumGlobalMaximum& b);

        /**
         * destructor
         */
        ~LocalMinimumGlobalMaximum();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** window size */
        int w;
    };

    template<typename INPUT, typename OUTPUT>
    LocalMinimumGlobalMaximum<INPUT, OUTPUT>::LocalMinimumGlobalMaximum(int w)
    : Transform2<INPUT, OUTPUT>()
    {
        this->w= w;
        std::stringstream ss;
        ss << "LocalMinimumGlobalMaximum " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    LocalMinimumGlobalMaximum<INPUT, OUTPUT>::LocalMinimumGlobalMaximum(const LocalMinimumGlobalMaximum& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    LocalMinimumGlobalMaximum<INPUT, OUTPUT>::~LocalMinimumGlobalMaximum()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LocalMinimumGlobalMaximum<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* /*support*/)
    {
        INPUT minimum, maximum;
        input.getMinMax(minimum, maximum, roi);
        
        StructuringElementDisk sed(w);
        sed.updateStride(input.columns);
        
        INPUT minlocal;
        output= 0;
        if ( roi == NULL )
            for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
            {
                minlocal= sed.getMin(input, i);
                output(i)= (input(i) - minlocal)/(maximum - minlocal);
            }
        else
        {
            for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
            {
                if ( (*roi)(i) )
                {
                    minlocal= sed.getMin(input, i);
                    output(i)= (input(i) - minlocal)/(maximum - minlocal);
                }
            }
        }
    }
    
    template<typename INPUT, typename OUTPUT>
    class LocalMaximumGlobalMinimum: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        LocalMaximumGlobalMinimum(int w);

        /**
         * copy constructor
         * @param b instance to copy
         */
        LocalMaximumGlobalMinimum(const LocalMaximumGlobalMinimum& b);

        /**
         * destructor
         */
        ~LocalMaximumGlobalMinimum();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /** window size */
        int w;
    };

    template<typename INPUT, typename OUTPUT>
    LocalMaximumGlobalMinimum<INPUT, OUTPUT>::LocalMaximumGlobalMinimum(int w)
    : Transform2<INPUT, OUTPUT>()
    {
        this->w= w;
        std::stringstream ss;
        ss << "LocalMaximumGlobalMinimum " << w;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    LocalMaximumGlobalMinimum<INPUT, OUTPUT>::LocalMaximumGlobalMinimum(const LocalMaximumGlobalMinimum& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    LocalMaximumGlobalMinimum<INPUT, OUTPUT>::~LocalMaximumGlobalMinimum()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LocalMaximumGlobalMinimum<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* /*support*/)
    {
        INPUT minimum, maximum;
        input.getMinMax(minimum, maximum, roi);
        
        StructuringElementDisk sed(w);
        sed.updateStride(input.columns);
        
        INPUT maxlocal;
        output= 0;
        if ( roi == NULL )
            for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
            {
                maxlocal= sed.getMax(input, i);
                output(i)= (input(i) - minimum)/(maxlocal - minimum);
            }
        else
        {
            for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
            {
                if ( (*roi)(i) )
                {
                    maxlocal= sed.getMax(input, i);
                    output(i)= (input(i) - minimum)/(maxlocal - minimum);
                }
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    class LaplacianSharpening: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        LaplacianSharpening();

        /**
         * copy constructor
         * @param b instance to copy
         */
        LaplacianSharpening(const LaplacianSharpening& b);

        /**
         * destructor
         */
        ~LaplacianSharpening();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    LaplacianSharpening<INPUT, OUTPUT>::LaplacianSharpening()
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "LaplacianSharpening ";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    LaplacianSharpening<INPUT, OUTPUT>::LaplacianSharpening(const LaplacianSharpening& e)
    : Transform2<INPUT, OUTPUT>(e)
    {
    }

    template<typename INPUT, typename OUTPUT>
    LaplacianSharpening<INPUT, OUTPUT>::~LaplacianSharpening()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LaplacianSharpening<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        LaplacianFilter<INPUT, OUTPUT> laplace;

        laplace.apply(input, output, roi, support);
        for ( unsigned int i= 0; i < output.n; ++i )
            output(i)+= input(i);
    }

    template<typename INPUT, typename OUTPUT>
    class FullLocalRange: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        FullLocalRange(int r);

        /**
         * copy constructor
         * @param b instance to copy
         */
        FullLocalRange(const FullLocalRange& b);

        /**
         * destructor
         */
        ~FullLocalRange();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int r;
    };

    template<typename INPUT, typename OUTPUT>
    FullLocalRange<INPUT, OUTPUT>::FullLocalRange(int r)
    : Transform2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "FullLocalRange " << r;
        descriptor= ss.str();
        this->r= r;
    }

    template<typename INPUT, typename OUTPUT>
    FullLocalRange<INPUT, OUTPUT>::FullLocalRange(const FullLocalRange& e)
        : Transform2<INPUT, OUTPUT>(e), r(e.r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FullLocalRange<INPUT, OUTPUT>::~FullLocalRange()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FullLocalRange<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
    {
        StructuringElementDisk sed(r);
        sed.updateStride(input.columns);

        float mini= input.getMin();
        float maxi= input.getMax();

        for ( unsigned int i= -sed.getMin(); i < input.n - sed.getMax(); ++i )
        {
            float minv= FLT_MAX, maxv= -FLT_MAX;
            for ( unsigned int j= 0; j < sed.size(); ++j )
            {
                if ( input(i + sed(j)) < minv )
                    minv= input(i + sed(j));
                if ( input(i + sed(j)) > maxv )
                    maxv= input(i + sed(j));
            }

            if ( minv == maxv )
                continue;

            output(i)= (input(i) - minv)/(maxv - minv)*(maxi - mini) + mini;
        }
    }

    template<typename INPUT, typename OUTPUT>
    class AverageBackgroundSubtraction: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param w1 method parameter
         * @param w2 method parameter
         */
        AverageBackgroundSubtraction(int w1, int w2);

        /**
         * copy constructor
         * @param b instance to copy
         */
        AverageBackgroundSubtraction(const AverageBackgroundSubtraction& b);

        /**
         * destructor
         */
        ~AverageBackgroundSubtraction();

        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        BackgroundSubtraction<INPUT, OUTPUT>* bs;

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    AverageBackgroundSubtraction<INPUT, OUTPUT>::AverageBackgroundSubtraction(int w1, int w2)
    : Transform2<INPUT, OUTPUT>()
    {
        bs= new BackgroundSubtraction<INPUT, OUTPUT>(w1, w2);
        std::stringstream ss;
        ss << "AverageBackgroundSubtraction " << w1 << " " << w2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AverageBackgroundSubtraction<INPUT, OUTPUT>::AverageBackgroundSubtraction(const AverageBackgroundSubtraction& e)
        : Transform2<INPUT, OUTPUT>(e), bs(e.bs)
    {
    }

    template<typename INPUT, typename OUTPUT>
    AverageBackgroundSubtraction<INPUT, OUTPUT>::~AverageBackgroundSubtraction()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void AverageBackgroundSubtraction<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        bs->apply(input, tmp, roi, support);
        for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= (input(i) + tmp(i))/2;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 AverageBackgroundSubtraction<INPUT, OUTPUT>::getProposedBorder()
    {
        return bs->getProposedBorder();
    }
}

#endif
