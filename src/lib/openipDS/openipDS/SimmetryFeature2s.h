#ifndef SIMMETRYFEATURE2S_H
#define SIMMETRYFEATURE2S_H

#include <openipDS/Feature2.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/StatisticalFeature2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class SimmetryFeature2: public Feature2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using StructuringElementDisk::stride;
        using StructuringElementDisk::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using StructuringElementDisk::getMin;
        using StructuringElementDisk::getMax;
        using StructuringElementDisk::getMean;
        using StructuringElementDisk::getMedian;
        using StructuringElementDisk::getVariance;
        using StructuringElementDisk::getSkewness;
        using StructuringElementDisk::getKurtosis;
        using StructuringElementDisk::getMeanAndVariance;
        using StructuringElementDisk::getStandardDeviation;
        using StructuringElementDisk::computeMinMax;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        SimmetryFeature2(int r, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        SimmetryFeature2(const SimmetryFeature2& s);

        /**
         * updates the stride
         * @param stride the new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

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
        virtual Border2 getProposedBorder();

        void getCoordinate2D(int n, int& rows, int& columns);

    };

    template<typename INPUT, typename OUTPUT>
    void SimmetryFeature2<INPUT, OUTPUT>::getCoordinate2D(int n, int& rows, int& columns)
    {
        rows= n / this->stride;
        if ( abs(n % this->stride) > this->stride/2 )
            rows= n > 0 ? rows + 1 : rows - 1;
        columns= n % this->stride;
        if ( abs(columns) > this->stride/2 )
            columns+= columns > 0 ? -this->stride : this->stride;
    }


    template<typename INPUT, typename OUTPUT>
    int SimmetryFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        return this->StructuringElementDisk::updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 SimmetryFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        computeMinMax();
        int w= getMax() / stride + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    SimmetryFeature2<INPUT, OUTPUT>::SimmetryFeature2(int r, int stride)
    : StructuringElementDisk(r, stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    SimmetryFeature2<INPUT, OUTPUT>::SimmetryFeature2(const SimmetryFeature2& s)
        : StructuringElementDisk(s), Feature2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int SimmetryFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->StructuringElementDisk::getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int SimmetryFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->StructuringElementDisk::getMax();
    }

    template<typename INPUT, typename OUTPUT>
    class AxisSimmetryFeature2: public SimmetryFeature2<INPUT, OUTPUT>
    {
    public:
        using SimmetryFeature2<INPUT, OUTPUT>::descriptor;
        using SimmetryFeature2<INPUT, OUTPUT>::stride;
        using SimmetryFeature2<INPUT, OUTPUT>::updateStride;
        using SimmetryFeature2<INPUT, OUTPUT>::apply;
        using SimmetryFeature2<INPUT, OUTPUT>::getMin;
        using SimmetryFeature2<INPUT, OUTPUT>::getMax;
        using SimmetryFeature2<INPUT, OUTPUT>::computeMinMax;
        using SimmetryFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        AxisSimmetryFeature2(float angle, int r, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        AxisSimmetryFeature2(const AxisSimmetryFeature2& s);

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    AxisSimmetryFeature2<INPUT, OUTPUT>::AxisSimmetryFeature2(float angle, int r, int stride)
    : SimmetryFeature2<INPUT, OUTPUT>(r, stride)
    {
        this->angle= angle;
        stringstream ss;
        ss << "AxisSimmetryFeature2 " << angle << " " << r << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AxisSimmetryFeature2<INPUT, OUTPUT>::AxisSimmetryFeature2(const AxisSimmetryFeature2& s)
        : SimmetryFeature2<INPUT, OUTPUT>(s), angle(s.angle)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT AxisSimmetryFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int tmpx, tmpy;
        float sum= 0;

        float e00= cos(angle);
        float e01= -sin(angle);
        float e10= sin(angle);
        float e11= cos(angle);

        int e0, e1;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                if ( tmpy > 0 )
                {
                    e0= e00*tmpy + e01*tmpx;
                    e1= e10*tmpy + e11*tmpx;
                    int c1= e0 * input.columns + e1;
                    //printf("%f %f %d\n", e0, e1, c1);
                    e0= e00*(-tmpy) + e01*tmpx;
                    e1= e10*(-tmpy) + e11*tmpx;
                    int c2= e0 * input.columns + e1;
                    //printf("%f %f %d\n", e0, e1, c2);
                    sum+= fabs(input(n + c1) - input(n + c2));
                }
            }
        }

        return sum;
    }

    template<typename INPUT, typename OUTPUT>
    class AxisAntiSimmetryFeature2: public SimmetryFeature2<INPUT, OUTPUT>
    {
    public:
        using SimmetryFeature2<INPUT, OUTPUT>::descriptor;
        using SimmetryFeature2<INPUT, OUTPUT>::stride;
        using SimmetryFeature2<INPUT, OUTPUT>::updateStride;
        using SimmetryFeature2<INPUT, OUTPUT>::apply;
        using SimmetryFeature2<INPUT, OUTPUT>::getMin;
        using SimmetryFeature2<INPUT, OUTPUT>::getMax;
        using SimmetryFeature2<INPUT, OUTPUT>::computeMinMax;
        using SimmetryFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        AxisAntiSimmetryFeature2(float angle, int r, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        AxisAntiSimmetryFeature2(const AxisAntiSimmetryFeature2& s);

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    AxisAntiSimmetryFeature2<INPUT, OUTPUT>::AxisAntiSimmetryFeature2(float angle, int r, int stride)
    : SimmetryFeature2<INPUT, OUTPUT>(r, stride)
    {
        this->angle= angle;
        stringstream ss;
        ss << "AxisAntiSimmetryFeature2 " << angle << " " << r << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AxisAntiSimmetryFeature2<INPUT, OUTPUT>::AxisAntiSimmetryFeature2(const AxisAntiSimmetryFeature2& s)
        : SimmetryFeature2<INPUT, OUTPUT>(s), angle(s.angle)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT AxisAntiSimmetryFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int tmpx, tmpy;
        float sum= 0;

        float e00= cos(angle);
        float e01= -sin(angle);
        float e10= sin(angle);
        float e11= cos(angle);

        int e0, e1;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                if ( tmpy > 0 )
                {
                    e0= e00*tmpy + e01*tmpx;
                    e1= e10*tmpy + e11*tmpx;
                    int c1= e0 * input.columns + e1;
                    //printf("%f %f %d\n", e0, e1, c1);
                    e0= e00*(-tmpy) + e01*tmpx;
                    e1= e10*(-tmpy) + e11*tmpx;
                    int c2= e0 * input.columns + e1;
                    //printf("%f %f %d\n", e0, e1, c2);
                    sum+= fabs(input(n + c1) - input(n + c2));
                }
            }
        }

        return -sum;
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedAxisSimmetryFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedAxisSimmetryFeature2(int r, float angle0, float angles, float angle1, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedAxisSimmetryFeature2(const MatchedAxisSimmetryFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedAxisSimmetryFeature2<INPUT, OUTPUT>::MatchedAxisSimmetryFeature2(int r, float angle0, float angles, float angle1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( float a= angle0; a <= angle1; a+= angles )
            this->push_back(new AxisSimmetryFeature2<INPUT, OUTPUT>(a, r, stride));

        stringstream ss;
        ss << "MatchedAxisSimmetryFeature2 " << r << " " << angle0 << " " << angles << " " << angle1 << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedAxisSimmetryFeature2<INPUT, OUTPUT>::MatchedAxisSimmetryFeature2(const MatchedAxisSimmetryFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedAxisAntiSimmetryFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedAxisAntiSimmetryFeature2(int r, float angle0, float angles, float angle1, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedAxisAntiSimmetryFeature2(const MatchedAxisAntiSimmetryFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedAxisAntiSimmetryFeature2<INPUT, OUTPUT>::MatchedAxisAntiSimmetryFeature2(int r, float angle0, float angles, float angle1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( float a= angle0; a <= angle1; a+= angles )
            this->push_back(new AxisAntiSimmetryFeature2<INPUT, OUTPUT>(a, r, stride));

        stringstream ss;
        ss << "MatchedAxisAntiSimmetryFeature2 " << r << " " << angle0 << " " << angles << " " << angle1 << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedAxisAntiSimmetryFeature2<INPUT, OUTPUT>::MatchedAxisAntiSimmetryFeature2(const MatchedAxisAntiSimmetryFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxNormalizationFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxNormalizationFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxNormalizationFeature2(const MatchedMaxNormalizationFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxNormalizationFeature2<INPUT, OUTPUT>::MatchedMaxNormalizationFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new NormalizationFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxNormalizationFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxNormalizationFeature2<INPUT, OUTPUT>::MatchedMaxNormalizationFeature2(const MatchedMaxNormalizationFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinNormalizationFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinNormalizationFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinNormalizationFeature2(const MatchedMinNormalizationFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinNormalizationFeature2<INPUT, OUTPUT>::MatchedMinNormalizationFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new NormalizationFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinNormalizationFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinNormalizationFeature2<INPUT, OUTPUT>::MatchedMinNormalizationFeature2(const MatchedMinNormalizationFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxMeanFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxMeanFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxMeanFeature2(const MatchedMaxMeanFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxMeanFeature2<INPUT, OUTPUT>::MatchedMaxMeanFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new MeanFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxMeanFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxMeanFeature2<INPUT, OUTPUT>::MatchedMaxMeanFeature2(const MatchedMaxMeanFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinMeanFeature2: public MinFeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinMeanFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinMeanFeature2(const MatchedMinMeanFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinMeanFeature2<INPUT, OUTPUT>::MatchedMinMeanFeature2(int n0, int ns, int n1, int stride)
        : MinFeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new MeanFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinMeanFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinMeanFeature2<INPUT, OUTPUT>::MatchedMinMeanFeature2(const MatchedMinMeanFeature2& s)
        : MinFeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxVarianceFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxVarianceFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxVarianceFeature2(const MatchedMaxVarianceFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxVarianceFeature2<INPUT, OUTPUT>::MatchedMaxVarianceFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new VarianceFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxVarianceFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxVarianceFeature2<INPUT, OUTPUT>::MatchedMaxVarianceFeature2(const MatchedMaxVarianceFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinVarianceFeature2: public MinFeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinVarianceFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinVarianceFeature2(const MatchedMinVarianceFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinVarianceFeature2<INPUT, OUTPUT>::MatchedMinVarianceFeature2(int n0, int ns, int n1, int stride)
        : MinFeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new VarianceFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinMeanFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinVarianceFeature2<INPUT, OUTPUT>::MatchedMinVarianceFeature2(const MatchedMinVarianceFeature2& s)
        : MinFeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxStandardDeviationFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxStandardDeviationFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxStandardDeviationFeature2(const MatchedMaxStandardDeviationFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxStandardDeviationFeature2<INPUT, OUTPUT>::MatchedMaxStandardDeviationFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new StandardDeviationFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxStandardDeviationFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxStandardDeviationFeature2<INPUT, OUTPUT>::MatchedMaxStandardDeviationFeature2(const MatchedMaxStandardDeviationFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinStandardDeviationFeature2: public MinFeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinStandardDeviationFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinStandardDeviationFeature2(const MatchedMinStandardDeviationFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinStandardDeviationFeature2<INPUT, OUTPUT>::MatchedMinStandardDeviationFeature2(int n0, int ns, int n1, int stride)
        : MinFeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new StandardDeviationFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinStandardDeviationFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinStandardDeviationFeature2<INPUT, OUTPUT>::MatchedMinStandardDeviationFeature2(const MatchedMinStandardDeviationFeature2& s)
        : MinFeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxSNRFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxSNRFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxSNRFeature2(const MatchedMaxSNRFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxSNRFeature2<INPUT, OUTPUT>::MatchedMaxSNRFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new SNRFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxSNRFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxSNRFeature2<INPUT, OUTPUT>::MatchedMaxSNRFeature2(const MatchedMaxSNRFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinSNRFeature2: public MinFeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinSNRFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinSNRFeature2(const MatchedMinSNRFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinSNRFeature2<INPUT, OUTPUT>::MatchedMinSNRFeature2(int n0, int ns, int n1, int stride)
        : MinFeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new SNRFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinSNRFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinSNRFeature2<INPUT, OUTPUT>::MatchedMinSNRFeature2(const MatchedMinSNRFeature2& s)
        : MinFeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxKurtosisFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxKurtosisFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxKurtosisFeature2(const MatchedMaxKurtosisFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxKurtosisFeature2<INPUT, OUTPUT>::MatchedMaxKurtosisFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new KurtosisFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxKurtosisFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxKurtosisFeature2<INPUT, OUTPUT>::MatchedMaxKurtosisFeature2(const MatchedMaxKurtosisFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinKurtosisFeature2: public MinFeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinKurtosisFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinKurtosisFeature2(const MatchedMinKurtosisFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinKurtosisFeature2<INPUT, OUTPUT>::MatchedMinKurtosisFeature2(int n0, int ns, int n1, int stride)
        : MinFeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new KurtosisFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinKurtosisFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinKurtosisFeature2<INPUT, OUTPUT>::MatchedMinKurtosisFeature2(const MatchedMinKurtosisFeature2& s)
        : MinFeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxSkewnessFeature2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMaxSkewnessFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMaxSkewnessFeature2(const MatchedMaxSkewnessFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxSkewnessFeature2<INPUT, OUTPUT>::MatchedMaxSkewnessFeature2(int n0, int ns, int n1, int stride)
        : FeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new SkewnessFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMaxSkewnessFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxSkewnessFeature2<INPUT, OUTPUT>::MatchedMaxSkewnessFeature2(const MatchedMaxSkewnessFeature2& s)
        : FeatureSet2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinSkewnessFeature2: public MinFeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::descriptor;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::push_back;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        MatchedMinSkewnessFeature2(int n0, int ns, int n1, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        MatchedMinSkewnessFeature2(const MatchedMinSkewnessFeature2& s);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinSkewnessFeature2<INPUT, OUTPUT>::MatchedMinSkewnessFeature2(int n0, int ns, int n1, int stride)
        : MinFeatureSet2<INPUT, OUTPUT>()
    {
        for ( int n= n0; n <= n1; n+= ns )
            this->push_back(new SkewnessFeature2<INPUT, OUTPUT>(n, stride));

        stringstream ss;
        ss << "MatchedMinSkewnessFeature2 " << n0 << " " << ns << " " << n1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinSkewnessFeature2<INPUT, OUTPUT>::MatchedMinSkewnessFeature2(const MatchedMinSkewnessFeature2& s)
        : MinFeatureSet2<INPUT, OUTPUT>(s)
    {
    }
}
#endif // SIMMETRYFEATURE2S_H
