/* 
 * File:   FourierFeature1.h
 * Author: gykovacs
 *
 * Created on December 2, 2011, 1:39 PM
 */

#ifndef FOURIERFEATURE1_H
#define	FOURIERFEATURE1_H

#include <openipSC/FourierTransformer1.h>
#include <openipDS/StatisticalFeature2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class HorizontalFourierMaxMagnitudeFeature2: public StatisticalFeature2<INPUT,OUTPUT>
    {
        public:
            using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
            using StatisticalFeature2<INPUT, OUTPUT>::getMin;
            using StatisticalFeature2<INPUT, OUTPUT>::getMax;
            using StatisticalFeature2<INPUT, OUTPUT>::stride;
            using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
            using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
            using StatisticalFeature2<INPUT, OUTPUT>::apply;
            using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

            HorizontalFourierMaxMagnitudeFeature2(int r, int n);

            HorizontalFourierMaxMagnitudeFeature2(const HorizontalFourierMaxMagnitudeFeature2& h);

            ~HorizontalFourierMaxMagnitudeFeature2();

            void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
            
            OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

            std::vector<std::complex<double> > tmp;
            FourierTransformer1* ft;
            int radius;
            int coord;
    };
    
    template<typename INPUT, typename OUTPUT>
    HorizontalFourierMaxMagnitudeFeature2<INPUT, OUTPUT>::HorizontalFourierMaxMagnitudeFeature2(int r, int n)
    : StatisticalFeature2<INPUT, OUTPUT>(r, 4000)
    {
        tmp.resize(2*r + 1);
        ft= new FourierTransformer1(2*r + 1);
        
        radius= r;
        coord= n;
        
        std::stringstream ss;
        ss << "HorizontalFourierMaxMagnitudeFeature2 " << r << " " << n;
        this->descriptor= ss.str();
    }
    
    template<typename INPUT, typename OUTPUT>
    HorizontalFourierMaxMagnitudeFeature2<INPUT, OUTPUT>::HorizontalFourierMaxMagnitudeFeature2(const HorizontalFourierMaxMagnitudeFeature2& h)
    : StatisticalFeature2<INPUT, OUTPUT>(h)
    {
        tmp= h.tmp;
        ft= h.ft;
        radius= h.radius;
        coord= h.coord;
    }
    
    template<typename INPUT, typename OUTPUT>
    HorizontalFourierMaxMagnitudeFeature2<INPUT, OUTPUT>::~HorizontalFourierMaxMagnitudeFeature2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void HorizontalFourierMaxMagnitudeFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }
    
    template<typename INPUT, typename OUTPUT>
    OUTPUT HorizontalFourierMaxMagnitudeFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* /*support*/)
    {
        double array[radius*2+1];
        for ( int i= 0; i < radius*2+1; ++i )
                array[i]= input(n - radius + i);
        
        ft->transform(array, &(tmp[0]));
        /*for ( int i= 0; i < coord; ++i )
            tmp[radius*2+1-i]= tmp[i]= std::complex<double>(0,0);*/
        for ( int i= 0; i < coord; ++i )
            tmp[radius + i]= tmp[radius - i]= std::complex<double>(0,0);
        float max= 0;
        ft->inverseTransform(&(tmp[0]), array);
        
        return array[radius];
        
        
        for ( int i= 0; i < coord; ++i )
            max+= log(norm(tmp[i]));
        
        return log(norm(tmp[coord]) + 1);
        
        for ( int i= 0; i < coord; ++i )
            if ( max < log(norm(tmp[i])))
                max= log(norm(tmp[i]));
        
        return max;
        
        return log(norm(tmp[coord]) + 1);
        
        for ( unsigned int i= 0; i < tmp.size(); ++i )
            if ( log(norm(tmp[i]) + 1) > max )
                max= log(norm(tmp[i]) + 1);
        //printf("%f ", max);
        if ( max < 1000000 )
            return max;
        else
            return 0;
    }
}

#endif	/* FOURIERFEATURE1_H */

