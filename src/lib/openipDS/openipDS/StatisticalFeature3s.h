/* 
 * File:   StatisticalFeature3s.h
 * Author: gykovacs
 *
 * Created on July 21, 2011, 8:22 AM
 */

#ifndef STATISTICALFEATURE3S_H
#define	STATISTICALFEATURE3S_H

#include <openipDS/StructuringElement3s.h>
#include <openipDS/Feature3.h>
//#include <openipDS/RunningStructuringElements.h>

#include <openipDS/clFunctions.h>

#include <sstream>
#include <cmath>

namespace openip
{
    /**
     * StatisticalFeature2 is the base class for all statistical descriptors.
     */
    template<typename INPUT, typename OUTPUT>
    class StatisticalFeature3: public Feature3<INPUT, OUTPUT>, public StructuringElementSphere
    {
    public:
        using Feature3<INPUT, OUTPUT>::descriptor;
        using StructuringElementSphere::stride;
        using StructuringElementSphere::slicePitch;
        using StructuringElementSphere::updateStride;
        using Feature3<INPUT, OUTPUT>::apply;
        using Feature3<INPUT, OUTPUT>::getMin;
        using Feature3<INPUT, OUTPUT>::getMax;
        using StructuringElementSphere::getMin;
        using StructuringElementSphere::getMax;
        using StructuringElementSphere::getMean;
        using StructuringElementSphere::getMedian;
        using StructuringElementSphere::getVariance;
        using StructuringElementSphere::getMeanAndVariance;
        using StructuringElementSphere::getStandardDeviation;
        using StructuringElementSphere::computeMinMax;
        using StructuringElementSphere::getSlicesRowsColumns;
        using Feature3<INPUT, OUTPUT>::getProposedBorder;
#ifdef USE_OPENCL        
        using Feature3<INPUT, OUTPUT>::enqueueKernel;
        using Feature3<INPUT, OUTPUT>::getOpenCLKernel;
        using Feature3<INPUT, OUTPUT>::applyGPU;
#endif
        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        StatisticalFeature3(int r, int stride= 8000, int slicePitch= 64000000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        StatisticalFeature3(const StatisticalFeature3& s);

        /**
         * updates the stride
         * @param stride the new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride, int slicePitch);

        /**
         * returns the minimum relative position of row-major representation in the descriptor
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum relative position of row-major representation in the descriptor
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * using the proposed border, the feature can be computed in each position of the original image
         * @return the proposed border
         */
        virtual Border3 getProposedBorder();
        
        //void getCoordinate3D(int n, int& slices, int& rows, int& columns);
    };

    /*template<typename INPUT, typename OUTPUT>
    void StatisticalFeature3<INPUT, OUTPUT>::getCoordinate3D(int n, int& slices, int& rows, int& columns)
    {
        slices= n / this->slicePitch;
        if ( abs(n % this->slicePitch) > this->slicePitch / 2 )
            slices= n > 0 ? slices + 1 : slices - 1;
        n= n % this->slicePitch;
        rows= n / this->stride;
        if ( abs(n % this->stride) > this->stride/2 )
            rows= n > 0 ? rows + 1 : rows - 1;
        columns= n % this->stride;
        if ( abs(columns) > this->stride/2 )
            columns+= columns > 0 ? -this->stride : this->stride;
    }*/

    
    template<typename INPUT, typename OUTPUT>
    int StatisticalFeature3<INPUT, OUTPUT>::updateStride(int stride, int slicePitch)
    {
        return this->StructuringElementSphere::updateStride(stride, slicePitch);
    }

    template<typename INPUT, typename OUTPUT>
    Border3 StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder()
    {
        int s, r, c;
        this->getSlicesRowsColumns(s, r, c);
        return Border3(s/2 + 1, s/2 + 1, r/2 + 1, r/2 + 1, c/2 + 1, c/2 + 1);
    }

    template<typename INPUT, typename OUTPUT>
    StatisticalFeature3<INPUT, OUTPUT>::StatisticalFeature3(int r, int stride, int slicePitch)
    : StructuringElementSphere(r, stride, slicePitch)
    {
    }

    template<typename INPUT, typename OUTPUT>
    StatisticalFeature3<INPUT, OUTPUT>::StatisticalFeature3(const StatisticalFeature3& s)
    : StructuringElementSphere(s), Feature3<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int StatisticalFeature3<INPUT, OUTPUT>::getMin()
    {
        return this->StructuringElementSphere::getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int StatisticalFeature3<INPUT, OUTPUT>::getMax()
    {
        return this->StructuringElementSphere::getMax();
    }
    
    template<typename INPUT, typename OUTPUT>
    class MaxFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MaxFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MaxFeature3<INPUT, OUTPUT>::MaxFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "MaxFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MaxFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        return (OUTPUT)(this->getMax(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class MinFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MinFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MinFeature3<INPUT, OUTPUT>::MinFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "MinFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MinFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        return (OUTPUT)(this->getMin(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class MedianFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MedianFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MedianFeature3<INPUT, OUTPUT>::MedianFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "MedianFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MedianFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        return (OUTPUT)(this->getMedian(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class MeanFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MeanFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MeanFeature3<INPUT, OUTPUT>::MeanFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "MeanFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MeanFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        return (OUTPUT)(this->getMean(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class VarianceFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        VarianceFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    VarianceFeature3<INPUT, OUTPUT>::VarianceFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "VarianceFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT VarianceFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        return (OUTPUT)(this->getVariance(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class StandardDeviationFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        StandardDeviationFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    StandardDeviationFeature3<INPUT, OUTPUT>::StandardDeviationFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "StandardDeviationFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT StandardDeviationFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        return (OUTPUT)(this->getStandardDeviation(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class NormalizationFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        NormalizationFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        NormalizationFeature3(const NormalizationFeature3<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    NormalizationFeature3<INPUT, OUTPUT>::NormalizationFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "NormalizationFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationFeature3<INPUT, OUTPUT>::NormalizationFeature3(const NormalizationFeature3<INPUT, OUTPUT>& vf)
    : Feature3<INPUT, OUTPUT>(vf), StructuringElementSphere(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT NormalizationFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        float mean;
        float variance;
        this->getMeanAndVariance(input, n, mean, variance, support);
        variance= sqrt(variance);
        if ( fabs(variance) > 0.001 )
            return (input(n) - mean)/(variance);
        else
            return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    class SNRFeature3: public StatisticalFeature3<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature3<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature3<INPUT, OUTPUT>::getMin;
        using StatisticalFeature3<INPUT, OUTPUT>::getMax;
        using StatisticalFeature3<INPUT, OUTPUT>::stride;
        using StatisticalFeature3<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature3<INPUT, OUTPUT>::apply;
        using StatisticalFeature3<INPUT, OUTPUT>::applyMax;
        using StatisticalFeature3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        SNRFeature3(int r, int stride= 4000, int slicePitch= 64000000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    SNRFeature3<INPUT, OUTPUT>::SNRFeature3(int r, int stride, int slicePitch)
    : StatisticalFeature3<INPUT, OUTPUT>(r, stride, slicePitch)
    {
        std::stringstream ss;
        ss << "SNRFeature3 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SNRFeature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
        float mean, variance;
        this->getMeanAndVariance(input, n, mean, variance, support);

        if ( variance >= 1 )
            return (OUTPUT)(mean/sqrt(variance));
        else
            return 0;
    }

    /** Invariant Hu's Moment 1 feature
     *
     */

    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment1Feature3: public StatisticalFeature3<INPUT,OUTPUT>
    {
    public:
	using StatisticalFeature3<INPUT,OUTPUT>::descriptor;
	using StatisticalFeature3<INPUT,OUTPUT>::getMin;
	using StatisticalFeature3<INPUT,OUTPUT>::getMax;
	using StatisticalFeature3<INPUT,OUTPUT>::stride;
	using StatisticalFeature3<INPUT,OUTPUT>::updateStride;
	using StatisticalFeature3<INPUT,OUTPUT>::apply;
	using StatisticalFeature3<INPUT,OUTPUT>::getProposedBorder;

	InvariantHuMoment1Feature3(int r, int stride = 4000, int slicePitch = 64000000);

	OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support = NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment1Feature3<INPUT,OUTPUT>::InvariantHuMoment1Feature3(int r, int stride, int slicePitch)
	: StatisticalFeature3<INPUT,OUTPUT>(r,stride,slicePitch)
    {
	std::stringstream ss;
	ss << "InvariantHuMoment1Feature3 " << r;
	descriptor = ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment1Feature3<INPUT,OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
	int tmpx, tmpy, tmpz;
	float sum000 = 0;
	float sum200 = 0;
	float sum020 = 0;
	float sum002 = 0;


	float m000 = 0;
	float m100 = 0;
	float m010 = 0;
	float m001 = 0;


	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if( support == NULL || (*support)(n + *sit) > 0 )
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		m000 += input(n + *sit);
		m100 += tmpx * input(n + *sit);
		m010 += tmpy * input(n + *sit);
		m001 += tmpz * input(n + *sit);
	    }
	}

	m100 /= m000;
	m010 /= m000;
	m001 /= m000;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if(support == NULL || (*support)(n + *sit) > 0)
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		sum000 += input(n + *sit);
		sum200 += (tmpx - m100) * (tmpx - m100) * input(n + *sit);
		sum020 += (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum002 += (tmpz - m001) * (tmpz - m001) * input(n + *sit);
	    }
	}


	return (OUTPUT)(sum200 + sum020 + sum002) /  (sum000*sum000);
    }


    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment2Feature3: public StatisticalFeature3<INPUT,OUTPUT>
    {
    public:
	using StatisticalFeature3<INPUT,OUTPUT>::descriptor;
	using StatisticalFeature3<INPUT,OUTPUT>::getMin;
	using StatisticalFeature3<INPUT,OUTPUT>::getMax;
	using StatisticalFeature3<INPUT,OUTPUT>::stride;
	using StatisticalFeature3<INPUT,OUTPUT>::updateStride;
	using StatisticalFeature3<INPUT,OUTPUT>::apply;
	using StatisticalFeature3<INPUT,OUTPUT>::getProposedBorder;

	InvariantHuMoment2Feature3(int r, int stride = 4000, int slicePitch = 64000000);

	OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support = NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment2Feature3<INPUT,OUTPUT>::InvariantHuMoment2Feature3(int r, int stride, int slicePitch)
	: StatisticalFeature3<INPUT,OUTPUT>(r,stride,slicePitch)
    {
	std::stringstream ss;
	ss << "InvariantHuMoment2Feature3 " << r;
	descriptor = ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment2Feature3<INPUT,OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
	int tmpx, tmpy, tmpz;
	float sum000 = 0;
	float sum200 = 0;
	float sum020 = 0;
	float sum002 = 0;
	float sum110 = 0;
	float sum101 = 0;
	float sum011 = 0;

	float m000 = 0;
	float m100 = 0;
	float m010 = 0;
	float m001 = 0;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if( support == NULL || (*support)(n + *sit) > 0 )
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		m000 += input(n + *sit);
		m100 += tmpx * input(n + *sit);
		m010 += tmpy * input(n + *sit);
		m001 += tmpz * input(n + *sit);
	    }
	}

	m100 /= m000;
	m010 /= m000;
	m001 /= m000;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if(support == NULL || (*support)(n + *sit) > 0)
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		sum000 += input(n + *sit);
		sum200 += (tmpx - m100) * (tmpx - m100) * input(n + *sit);
		sum020 += (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum002 += (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum110 += (tmpx - m100) * (tmpy - m010) * input(n + *sit);
		sum101 += (tmpx - m100) * (tmpz - m001) * input(n + *sit);
		sum011 += (tmpy - m010) * (tmpz - m001) * input(n + *sit);
	    }
	}


	return (OUTPUT)(sum200*sum200 + sum020*sum020 + sum002*sum002 + 2*(sum110*sum110) + 2*(sum101*sum101) + 2*(sum011*sum011)) / pow(sum000,4);
    }

    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment3Feature3: public StatisticalFeature3<INPUT,OUTPUT>
    {
    public:
	using StatisticalFeature3<INPUT,OUTPUT>::descriptor;
	using StatisticalFeature3<INPUT,OUTPUT>::getMin;
	using StatisticalFeature3<INPUT,OUTPUT>::getMax;
	using StatisticalFeature3<INPUT,OUTPUT>::stride;
	using StatisticalFeature3<INPUT,OUTPUT>::updateStride;
	using StatisticalFeature3<INPUT,OUTPUT>::apply;
	using StatisticalFeature3<INPUT,OUTPUT>::getProposedBorder;

	InvariantHuMoment3Feature3(int r, int stride = 4000, int slicePitch = 64000000);

	OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support = NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment3Feature3<INPUT,OUTPUT>::InvariantHuMoment3Feature3(int r, int stride, int slicePitch)
	: StatisticalFeature3<INPUT,OUTPUT>(r,stride,slicePitch)
    {
	std::stringstream ss;
	ss << "InvariantHuMoment3Feature3 " << r;
	descriptor = ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment3Feature3<INPUT,OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
	int tmpx, tmpy, tmpz;
	float sum000 = 0;
	float sum200 = 0;
	float sum020 = 0;
	float sum002 = 0;
	float sum110 = 0;
	float sum101 = 0;
	float sum011 = 0;

	float m000 = 0;
	float m100 = 0;
	float m010 = 0;
	float m001 = 0;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if( support == NULL || (*support)(n + *sit) > 0 )
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		m000 += input(n + *sit);
		m100 += tmpx * input(n + *sit);
		m010 += tmpy * input(n + *sit);
		m001 += tmpz * input(n + *sit);
	    }
	}

	m100 /= m000;
	m010 /= m000;
	m001 /= m000;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if(support == NULL || (*support)(n + *sit) > 0)
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		sum000 += input(n + *sit);
		sum200 += (tmpx - m100) * (tmpx - m100) * input(n + *sit);
		sum020 += (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum002 += (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum110 += (tmpx - m100) * (tmpy - m010) * input(n + *sit);
		sum101 += (tmpx - m100) * (tmpz - m001) * input(n + *sit);
		sum011 += (tmpy - m010) * (tmpz - m001) * input(n + *sit);
	    }
	}


	return (OUTPUT)(pow(sum200,3) + 3*sum200*pow(sum110,2) + 3*sum200*pow(sum101,2) + 3*pow(sum110,2)*sum020 + 3*pow(sum101,2)*sum002
	      + pow(sum020,3) + 3*sum020*pow(sum011,2) + 3*pow(sum011,2)*sum002 + pow(sum002,3) + 6*sum110*sum101*sum011) / pow(sum000,6);
    }

    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment4Feature3: public StatisticalFeature3<INPUT,OUTPUT>
    {
    public:
	using StatisticalFeature3<INPUT,OUTPUT>::descriptor;
	using StatisticalFeature3<INPUT,OUTPUT>::getMin;
	using StatisticalFeature3<INPUT,OUTPUT>::getMax;
	using StatisticalFeature3<INPUT,OUTPUT>::stride;
	using StatisticalFeature3<INPUT,OUTPUT>::updateStride;
	using StatisticalFeature3<INPUT,OUTPUT>::apply;
	using StatisticalFeature3<INPUT,OUTPUT>::getProposedBorder;

	InvariantHuMoment4Feature3(int r, int stride = 4000, int slicePitch = 64000000);

	OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support = NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment4Feature3<INPUT,OUTPUT>::InvariantHuMoment4Feature3(int r, int stride, int slicePitch)
	: StatisticalFeature3<INPUT,OUTPUT>(r,stride,slicePitch)
    {
	std::stringstream ss;
	ss << "InvariantHuMoment4Feature3 " << r;
	descriptor = ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment4Feature3<INPUT,OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
	int tmpx, tmpy, tmpz;
	float sum000 = 0;
	float sum300 = 0;
	float sum030 = 0;
	float sum003 = 0;
	float sum210 = 0;
	float sum201 = 0;
	float sum120 = 0;
	float sum102 = 0;
	float sum021 = 0;
	float sum012 = 0;
	float sum111 = 0;

	float m000 = 0;
	float m100 = 0;
	float m010 = 0;
	float m001 = 0;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if( support == NULL || (*support)(n + *sit) > 0 )
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		m000 += input(n + *sit);
		m100 += tmpx * input(n + *sit);
		m010 += tmpy * input(n + *sit);
		m001 += tmpz * input(n + *sit);
	    }
	}

	m100 /= m000;
	m010 /= m000;
	m001 /= m000;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if(support == NULL || (*support)(n + *sit) > 0)
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		sum000 += input(n + *sit);
		sum300 += (tmpx - m100) * (tmpx - m100) * (tmpx - m100) * input(n + *sit);
		sum030 += (tmpy - m010) * (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum003 += (tmpz - m001) * (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum210 += (tmpx - m100) * (tmpx - m100) * (tmpy - m010) * input(n + *sit);
		sum201 += (tmpx - m100) * (tmpx - m100) * (tmpz - m001) * input(n + *sit);
		sum120 += (tmpx - m100) * (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum102 += (tmpx - m100) * (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum021 += (tmpy - m010) * (tmpy - m010) * (tmpz - m001) * input(n + *sit);
		sum012 += (tmpy - m010) * (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum111 += (tmpx - m001) * (tmpy - m010) * (tmpz - m001) * input(n + *sit);
	    }
	}


	return (OUTPUT)(pow(sum300,2) + pow(sum030,2) + pow(sum003,2) +
			3*pow(sum210,2) + 3*pow(sum201,2) + 3*pow(sum120,2) +
			3*pow(sum102,2) + 3*pow(sum021,2) + 3*pow(sum012,2) +
			6*pow(sum111,2) ) / pow(sum000,5);
    }


    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment5Feature3: public StatisticalFeature3<INPUT,OUTPUT>
    {
    public:
	using StatisticalFeature3<INPUT,OUTPUT>::descriptor;
	using StatisticalFeature3<INPUT,OUTPUT>::getMin;
	using StatisticalFeature3<INPUT,OUTPUT>::getMax;
	using StatisticalFeature3<INPUT,OUTPUT>::stride;
	using StatisticalFeature3<INPUT,OUTPUT>::updateStride;
	using StatisticalFeature3<INPUT,OUTPUT>::apply;
	using StatisticalFeature3<INPUT,OUTPUT>::getProposedBorder;

	InvariantHuMoment5Feature3(int r, int stride = 4000, int slicePitch = 64000000);

	OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support = NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment5Feature3<INPUT,OUTPUT>::InvariantHuMoment5Feature3(int r, int stride, int slicePitch)
	: StatisticalFeature3<INPUT,OUTPUT>(r,stride,slicePitch)
    {
	std::stringstream ss;
	ss << "InvariantHuMoment5Feature3 " << r;
	descriptor = ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment5Feature3<INPUT,OUTPUT>::apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support)
    {
	int tmpx, tmpy, tmpz;
	float sum000 = 0;
	float sum300 = 0;
	float sum030 = 0;
	float sum003 = 0;
	float sum210 = 0;
	float sum201 = 0;
	float sum120 = 0;
	float sum102 = 0;
	float sum021 = 0;
	float sum012 = 0;
	float sum111 = 0;

	float m000 = 0;
	float m100 = 0;
	float m010 = 0;
	float m001 = 0;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if( support == NULL || (*support)(n + *sit) > 0 )
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		m000 += input(n + *sit);
		m100 += tmpx * input(n + *sit);
		m010 += tmpy * input(n + *sit);
		m001 += tmpz * input(n + *sit);
	    }
	}

	m100 /= m000;
	m010 /= m000;
	m001 /= m000;

	for ( StructuringElementSphere::iterator sit = this->begin(); sit != this->end(); ++sit)
	{
	    if(support == NULL || (*support)(n + *sit) > 0)
	    {
		this->getCoordinates3D(*sit, tmpz, tmpy, tmpx);
		sum000 += input(n + *sit);
		sum300 += (tmpx - m100) * (tmpx - m100) * (tmpx - m100) * input(n + *sit);
		sum030 += (tmpy - m010) * (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum003 += (tmpz - m001) * (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum210 += (tmpx - m100) * (tmpx - m100) * (tmpy - m010) * input(n + *sit);
		sum201 += (tmpx - m100) * (tmpx - m100) * (tmpz - m001) * input(n + *sit);
		sum120 += (tmpx - m100) * (tmpy - m010) * (tmpy - m010) * input(n + *sit);
		sum102 += (tmpx - m100) * (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum021 += (tmpy - m010) * (tmpy - m010) * (tmpz - m001) * input(n + *sit);
		sum012 += (tmpy - m010) * (tmpz - m001) * (tmpz - m001) * input(n + *sit);
		sum111 += (tmpx - m001) * (tmpy - m010) * (tmpz - m001) * input(n + *sit);
	    }
	}


	return (OUTPUT) ( pow(sum300,2) + 2*sum300*sum120 + 2*sum300*sum102 + 2*sum210*sum030 +
			  2*sum201*sum003 + pow(sum030,2) + 2*sum030*sum012 + 2*sum021*sum003 +
			  pow(sum003,2) + pow(sum210,2) + 2*sum210*sum012 + pow(sum201,2) +
			  2*sum201*sum021 + pow(sum120,2) + 2*sum120*sum102 + pow(sum102,2) +
			  pow(sum021,2) + pow(sum012,2)) / pow(sum000,5);
    }


















































}

#endif	/* STATISTICALFEATURE3S_H */

