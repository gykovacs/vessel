#ifndef LBPFEATURE2S_H
#define LBPFEATURE2S_H

#include <openipDS/Filter2.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/RunningStructuringElements.h>

#include <openipDS/clFunctions.h>

#include <sstream>
#include <cmath>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class LBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        LBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        LBPFeature2(const LBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<int> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    LBPFeature2<INPUT, OUTPUT>::LBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "LBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    LBPFeature2<INPUT, OUTPUT>::LBPFeature2(const LBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int LBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        if ( N == 0 )
        {
            pos.resize(4);
            pos(0)= -ds;
            pos(1)= ds;
            pos(2)= -ds*stride;
            pos(4)= ds*stride;
        }

        if ( N == 1 )
        {
            pos.resize(4);
            pos(0)= -ds-ds*stride;
            pos(1)=-ds+ds*stride;
            pos(2)= ds-ds*stride;
            pos(4)= ds+ds*stride;
        }

        if ( N == 2 )
        {
            pos.resize(8);
            pos(0)= -ds;
            pos(1)= ds;
            pos(2)= -ds*stride;
            pos(4)= ds*stride;
            pos(5)= -ds-ds*stride;
            pos(6)=-ds+ds*stride;
            pos(7)= ds-ds*stride;
            pos(8)= ds+ds*stride;
        }
        
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int LBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return -ds-ds*stride;
    }

    template<typename INPUT, typename OUTPUT>
    int LBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return ds+ds*stride;
    }

    template<typename INPUT, typename OUTPUT>
    void LBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( unsigned int i= -getMin(); i < input.n - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( unsigned int i= -getMin(); i < input.n - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT LBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;

        for ( unsigned int i= 0; i < pos.size(); ++i )
        {
            if ( n + pos(i) >= 0 && n + pos(i) < int(input.n) && (!support || (*support)(n + pos(i))) )
                if ( input(n + pos(i)) > input(n) )
                    out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }


    template<typename INPUT, typename OUTPUT>
    class RotationInvariantLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RotationInvariantLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RotationInvariantLBPFeature2(const RotationInvariantLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<int> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RotationInvariantLBPFeature2<INPUT, OUTPUT>::RotationInvariantLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RotationInvariantLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RotationInvariantLBPFeature2<INPUT, OUTPUT>::RotationInvariantLBPFeature2(const RotationInvariantLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RotationInvariantLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        if ( N == 0 )
        {
            pos.resize(4);
            pos(0)= -ds;
            pos(1)= ds;
            pos(2)= -ds*stride;
            pos(4)= ds*stride;
        }

        if ( N == 1 )
        {
            pos.resize(4);
            pos(0)= -ds-ds*stride;
            pos(1)=-ds+ds*stride;
            pos(2)= ds-ds*stride;
            pos(4)= ds+ds*stride;
        }

        if ( N == 2 )
        {
            pos.resize(8);
            pos(0)= -ds;
            pos(1)= ds;
            pos(2)= -ds*stride;
            pos(4)= ds*stride;
            pos(5)= -ds-ds*stride;
            pos(6)=-ds+ds*stride;
            pos(7)= ds-ds*stride;
            pos(8)= ds+ds*stride;
        }
        
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RotationInvariantLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return -ds-ds*stride;
    }

    template<typename INPUT, typename OUTPUT>
    int RotationInvariantLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return ds+ds*stride;
    }

    template<typename INPUT, typename OUTPUT>
    void RotationInvariantLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( unsigned int i= -getMin(); i < input.n - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( unsigned int i= -getMin(); i < input.n - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RotationInvariantLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        unsigned int minOut= INT_MAX;

        for ( unsigned int i= 0; i < pos.size(); ++i )
        {
            if ( n + pos(i) >= 0 && n + pos(i) < int(input.n) && (!support || (*support)(n + pos(i))) )
                if ( input(n + pos(i)) > input(n) )
                    out|= mask;
            mask*= 2;
        }

        for ( unsigned int i= 0; i < 32; ++i )
        {
            out = (out << 1) | (out >> (sizeof(out)*CHAR_BIT - 1));
            if ( out < minOut )
                minOut= out;
        }

        return OUTPUT(minOut);
    }


    template<typename INPUT, typename OUTPUT>
    class RadialMeanLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialMeanLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialMeanLBPFeature2(const RadialMeanLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialMeanLBPFeature2<INPUT, OUTPUT>::RadialMeanLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialMeanLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialMeanLBPFeature2<INPUT, OUTPUT>::RadialMeanLBPFeature2(const RadialMeanLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMeanLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i < N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMeanLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMeanLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialMeanLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialMeanLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        //unsigned int minOut= INT_MAX;

        for ( unsigned int i= 0; i < pos.size(); ++i )
        {
            if ( pos(i).getMean(input, n, support) > input(n) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }

    template<typename INPUT, typename OUTPUT>
    class RadialMeanSuccLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialMeanSuccLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialMeanSuccLBPFeature2(const RadialMeanSuccLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::RadialMeanSuccLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialMeanSuccLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::RadialMeanSuccLBPFeature2(const RadialMeanSuccLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i <= N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialMeanSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        //unsigned int minOut= INT_MAX;

        for ( int i= 0; i < int(pos.size())-1; ++i )
        {
            if ( pos(i).getMean(input, n, support) > pos(i+1).getMean(input, n, support) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }


    template<typename INPUT, typename OUTPUT>
    class RadialMinLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialMinLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialMinLBPFeature2(const RadialMinLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialMinLBPFeature2<INPUT, OUTPUT>::RadialMinLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialMinLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialMinLBPFeature2<INPUT, OUTPUT>::RadialMinLBPFeature2(const RadialMinLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMinLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i < N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMinLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMinLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialMinLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialMinLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        //unsigned int minOut= INT_MAX;

        for ( unsigned int i= 0; i < pos.size(); ++i )
        {
            if ( pos(i).getMin(input, n, support) > input(n) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }

    template<typename INPUT, typename OUTPUT>
    class RadialMaxLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialMaxLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialMaxLBPFeature2(const RadialMaxLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialMaxLBPFeature2<INPUT, OUTPUT>::RadialMaxLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialMaxLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialMaxLBPFeature2<INPUT, OUTPUT>::RadialMaxLBPFeature2(const RadialMaxLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMaxLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i < N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMaxLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMaxLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialMaxLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialMaxLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        //unsigned int minOut= INT_MAX;

        for ( unsigned int i= 0; i < pos.size(); ++i )
        {
            if ( pos(i).getMax(input, n, support) > input(n) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }

    template<typename INPUT, typename OUTPUT>
    class RadialMinSuccLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialMinSuccLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialMinSuccLBPFeature2(const RadialMinSuccLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialMinSuccLBPFeature2<INPUT, OUTPUT>::RadialMinSuccLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialMinSuccLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialMinSuccLBPFeature2<INPUT, OUTPUT>::RadialMinSuccLBPFeature2(const RadialMinSuccLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMinSuccLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i <= N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMinSuccLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMinSuccLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialMinSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialMinSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        //unsigned int minOut= INT_MAX;

        for ( int i= 0; i < int(pos.size())-1; ++i )
        {
            if ( pos(i).getMin(input, n, support) > pos(i+1).getMin(input, n, support) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }

    template<typename INPUT, typename OUTPUT>
    class RadialMaxSuccLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialMaxSuccLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialMaxSuccLBPFeature2(const RadialMaxSuccLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::RadialMaxSuccLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialMaxSuccLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::RadialMaxSuccLBPFeature2(const RadialMaxSuccLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i <= N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < int(input.n) - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialMaxSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        //unsigned int minOut= INT_MAX;

        for ( unsigned int i= 0; i < pos.size()-1; ++i )
        {
            if ( pos(i).getMax(input, n, support) > pos(i+1).getMax(input, n, support) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }

    template<typename INPUT, typename OUTPUT>
    class RadialStandardDeviationSuccLBPFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        RadialStandardDeviationSuccLBPFeature2(int N, int ds, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        RadialStandardDeviationSuccLBPFeature2(const RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual int updateStride(int stride);

        int N;
        int ds;
        Vector<StructuringElementRing> pos;
        int stride;
    };

    template<typename INPUT, typename OUTPUT>
    RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::RadialStandardDeviationSuccLBPFeature2(int N, int ds, int stride)
    : Feature2<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "RadialStandardDeviationSuccLBPFeature2 " << N << " " << ds;
        descriptor= ss.str();
        this->N= N;
        this->ds= ds;
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT>
    RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::RadialStandardDeviationSuccLBPFeature2(const RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>& vf)
        : Feature2<INPUT, OUTPUT>(vf), N(vf.N), ds(vf.ds), pos(vf.pos), stride(vf.stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;

        pos.clear();
        for ( int i= 0; i <= N; ++i )
            pos.push_back(StructuringElementRing(i*ds, (i+1)*ds, stride));
	
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::getMin()
    {
        return pos(pos.size()-1).getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::getMax()
    {
        return pos(pos.size()-1).getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( stride != input.columns )
            updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( int i= -getMin(); i < input.n - getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= -getMin(); i < input.n - getMax(); ++i )
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RadialStandardDeviationSuccLBPFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        unsigned int out= 0;
        unsigned int mask= 1;
        unsigned int minOut= INT_MAX;

        for ( int i= 0; i < pos.size()-1; ++i )
        {
            if ( pos(i).getStandardDeviation(input, n, support) > pos(i+1).getStandardDeviation(input, n, support) )
                out|= mask;
            mask*= 2;
        }

        return OUTPUT(out);
    }
}

#endif // LBPFEATURE2S_H
