/*
 * File:   RegionFeatures.h
 * Author: gykovacs
 *
 * Created on 2011. március 4., 18:45
 */

#ifndef REGIONFEATURES2_H
#define	REGIONFEATURES2_H

#include <openipDS/RegionFeature2.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipLL/morphology.h>
#include <openipLL/thresholding.h>
#include <openipDS/LBPFeature2s.h>

#include <iostream>

namespace openip
{
    template<typename INPUT>
    class RegionAreaFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::getDescriptor;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionAreaFeature2();

        RegionAreaFeature2(const RegionAreaFeature2& r);

        ~RegionAreaFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionAreaFeature2<INPUT>::RegionAreaFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionAreaFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionAreaFeature2<INPUT>::RegionAreaFeature2(const RegionAreaFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionAreaFeature2<INPUT>::~RegionAreaFeature2()
    {
    }

    template<typename INPUT>
    float RegionAreaFeature2<INPUT>::apply(Image<INPUT>& /*input*/, Region2& r)
    {
        return r.size();
    }

    template<typename INPUT>
    class RegionPerimeterFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::getDescriptor;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterFeature2();

        RegionPerimeterFeature2(const RegionPerimeterFeature2& r);

        ~RegionPerimeterFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionPerimeterFeature2<INPUT>::RegionPerimeterFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionPerimeterFeature2<INPUT>::RegionPerimeterFeature2(const RegionPerimeterFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionPerimeterFeature2<INPUT>::~RegionPerimeterFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterFeature2<INPUT>::apply(Image<INPUT>& /*input*/, Region2& r)
    {
        float p= 0.0f;

        for ( Region2::iterator it= r.begin(); it != r.end(); ++it )
        {
            if ( r.find(*it + 1 ) == r.end() || r.find(*it - 1) == r.end() || r.find(*it - r.stride) == r.end() || r.find(*it + r.stride) == r.end() )
                p+= 1.0;
        }

        return p;
    }
    
    template<typename INPUT>
    class RegionMajorityFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionMajorityFeature2();

        RegionMajorityFeature2(const RegionMajorityFeature2& r);

        ~RegionMajorityFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionMajorityFeature2<INPUT>::RegionMajorityFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionMajorityFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionMajorityFeature2<INPUT>::RegionMajorityFeature2(const RegionMajorityFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionMajorityFeature2<INPUT>::~RegionMajorityFeature2()
    {
    }

    template<typename INPUT>
    float RegionMajorityFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Vector<int> tmp(256);
        tmp= 0;
        
        for ( unsigned int i= 0; i < r.size(); ++i )
            ++tmp(input(r[i]));
        
        int max= 0;
        for ( int i= 1; i < 256; ++i )
            if ( tmp(i) > tmp(max) )
                max= i;

        return max;
    }
    
    template<typename INPUT>
    class RegionMaximumFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionMaximumFeature2();

        RegionMaximumFeature2(const RegionMaximumFeature2& r);

        ~RegionMaximumFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionMaximumFeature2<INPUT>::RegionMaximumFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionMaximumFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionMaximumFeature2<INPUT>::RegionMaximumFeature2(const RegionMaximumFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionMaximumFeature2<INPUT>::~RegionMaximumFeature2()
    {
    }

    template<typename INPUT>
    float RegionMaximumFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        INPUT max= -std::numeric_limits<INPUT>::max();

        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( max < input(r(i)) )
                max= input(r(i));

        return max;
    }
    
    template<typename INPUT>
    class RegionStandardDeviationFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionStandardDeviationFeature2();

        RegionStandardDeviationFeature2(const RegionStandardDeviationFeature2& r);

        ~RegionStandardDeviationFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionStandardDeviationFeature2<INPUT>::RegionStandardDeviationFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionStandardDeviationFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionStandardDeviationFeature2<INPUT>::RegionStandardDeviationFeature2(const RegionStandardDeviationFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionStandardDeviationFeature2<INPUT>::~RegionStandardDeviationFeature2()
    {
    }

    template<typename INPUT>
    float RegionStandardDeviationFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<float> tmp;
        tmp.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp, NULL, NULL);
        else
            tmp= input;

        debugFeatures(tmp);
        return r.getStandardDeviation(tmp);
    }
    
    template<typename INPUT>
    class RegionMeanFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionMeanFeature2();

        RegionMeanFeature2(const RegionMeanFeature2& r);

        ~RegionMeanFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionMeanFeature2<INPUT>::RegionMeanFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionMeanFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionMeanFeature2<INPUT>::RegionMeanFeature2(const RegionMeanFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionMeanFeature2<INPUT>::~RegionMeanFeature2()
    {
    }

    template<typename INPUT>
    float RegionMeanFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float res= 0;
        
        if ( transform == NULL )
            for ( unsigned int i= 0; i < r.size(); ++i )
                res+= input(r[i]);
        else
        {
            Image<INPUT> tmp;
            tmp.resizeImage(input);

            transform->apply(input, tmp);

            debugFeatures(tmp);

            for ( unsigned int i= 0; i < r.size(); ++i )
                res+= tmp(r[i]);
        }
        
        return res/= r.size();
    }
    
    template<typename INPUT>
    class RegionCircularityFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionCircularityFeature2();

        RegionCircularityFeature2(const RegionCircularityFeature2& r);

        ~RegionCircularityFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionCircularityFeature2<INPUT>::RegionCircularityFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCircularityFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionCircularityFeature2<INPUT>::RegionCircularityFeature2(const RegionCircularityFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionCircularityFeature2<INPUT>::~RegionCircularityFeature2()
    {
    }

    template<typename INPUT>
    float RegionCircularityFeature2<INPUT>::apply(Image<INPUT>& /*input*/, Region2& r)
    {
        float p= 0.0f;

        for ( Region2::iterator it= r.begin(); it != r.end(); ++it )
        {
            if ( r.find(*it + 1 ) == r.end() || r.find(*it - 1) == r.end() || r.find(*it - r.stride) == r.end() || r.find(*it + r.stride) == r.end() )
                p+= 1.0;
        }

        return 4*M_PI*r.size()/(p*p);
    }
    
    template<typename INPUT>
    class RegionAreaToImageFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionAreaToImageFeature2();

        RegionAreaToImageFeature2(const RegionAreaToImageFeature2& r);

        ~RegionAreaToImageFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionAreaToImageFeature2<INPUT>::RegionAreaToImageFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionAreaToImageFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionAreaToImageFeature2<INPUT>::RegionAreaToImageFeature2(const RegionAreaToImageFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionAreaToImageFeature2<INPUT>::~RegionAreaToImageFeature2()
    {
    }

    template<typename INPUT>
    float RegionAreaToImageFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        return float(r.size()) / input.n;
    }
    
    template<typename INPUT>
    class RegionRelativeMeanFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionRelativeMeanFeature2();

        RegionRelativeMeanFeature2(const RegionRelativeMeanFeature2& r);

        ~RegionRelativeMeanFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionRelativeMeanFeature2<INPUT>::RegionRelativeMeanFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionRelativeMeanFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionRelativeMeanFeature2<INPUT>::RegionRelativeMeanFeature2(const RegionRelativeMeanFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionRelativeMeanFeature2<INPUT>::~RegionRelativeMeanFeature2()
    {
    }

    template<typename INPUT>
    float RegionRelativeMeanFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float res= 0;
        
        {
            INPUT min, max;
            r.getMinMax(input, min, max);
            
            for ( unsigned int i= 0; i < r.size(); ++i )
                res+= input(r[i]);
            res= res/r.size();
            res= (res - min)/float(max - min);
            
        }
        
        return res;
    }
    
    template<typename INPUT>
    class RegionGlobalRelativeMeanFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionGlobalRelativeMeanFeature2();

        RegionGlobalRelativeMeanFeature2(const RegionGlobalRelativeMeanFeature2& r);

        ~RegionGlobalRelativeMeanFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionGlobalRelativeMeanFeature2<INPUT>::RegionGlobalRelativeMeanFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionGlobalRelativeMeanFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionGlobalRelativeMeanFeature2<INPUT>::RegionGlobalRelativeMeanFeature2(const RegionGlobalRelativeMeanFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionGlobalRelativeMeanFeature2<INPUT>::~RegionGlobalRelativeMeanFeature2()
    {
    }

    template<typename INPUT>
    float RegionGlobalRelativeMeanFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float res= 0;
        
        if ( transform == NULL )
        {
            INPUT min, max;
            input.getMinMax(min, max);
            
            for ( unsigned int i= 0; i < r.size(); ++i )
                res+= input(r[i]);
            res= res/r.size();
            res= (res - min)/float(max - min);
            
        }
        else
        {
            Image<INPUT> tmp;
            tmp.resizeImage(input);
            Feature2<INPUT, INPUT>* f= dynamic_cast<Feature2<INPUT, INPUT>* >(transform);
            if ( f != NULL )
                f->updateStride(input.columns);
            f->apply(input, tmp);
            for ( unsigned int i= 0; i < r.size(); ++i )
                res+= tmp(r[i]);
            INPUT min, max;
            tmp.getMinMax(min, max);
            res= res/r.size();
            res= (res - min)/float(max - min);
        }
        
        return res;
    }
    
    template<typename INPUT>
    class RegionSNRFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionSNRFeature2();

        RegionSNRFeature2(const RegionSNRFeature2& r);

        ~RegionSNRFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionSNRFeature2<INPUT>::RegionSNRFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionSNRFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionSNRFeature2<INPUT>::RegionSNRFeature2(const RegionSNRFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionSNRFeature2<INPUT>::~RegionSNRFeature2()
    {
    }

    template<typename INPUT>
    float RegionSNRFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float mean, var;

        Image<float> tmp;
        tmp.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp, NULL, NULL);
        else
            tmp= input;

        debugFeatures(tmp);
        r.getMeanAndVariance(tmp, mean, var);
        
        return mean/sqrt(var);
    }
    
    template<typename INPUT>
    class RegionEntropyFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionEntropyFeature2(int bins);

        RegionEntropyFeature2(const RegionEntropyFeature2& r);

        ~RegionEntropyFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int bins;
    };

    template<typename INPUT>
    RegionEntropyFeature2<INPUT>::RegionEntropyFeature2(int bins)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionEntropyFeature2 " << bins;
        descriptor= ss.str();
        reqTransform= true;
        this->bins= bins;
        transform= NULL;
    }

    template<typename INPUT>
    RegionEntropyFeature2<INPUT>::RegionEntropyFeature2(const RegionEntropyFeature2& r)
        : RegionFeature2<INPUT>(r), bins(r.bins)
    {
    }

    template<typename INPUT>
    RegionEntropyFeature2<INPUT>::~RegionEntropyFeature2()
    {
    }

    template<typename INPUT>
    float RegionEntropyFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float entropy= 0;

        if ( transform == 0 )
        {
            debugFeatures(input);
            float minimum= r.getMin(input);
            float maximum= r.getMax(input);
            float d= (maximum - minimum)/(bins - 1);

            Vector<float> h(bins);
            h= 0;

            for ( unsigned int i= 0; i < r.size(); ++i )
                h((int)floor((input(r(i)) - minimum)/d))++;

            h/= r.size();

            for ( unsigned int i= 0; i < h.size(); ++i )
                if ( h(i) > 0 )
                    entropy+= float(h(i))*log(float(h(i)));
        }
        else
        {
            Image<INPUT> tmp;
            tmp.resizeImage(input);
            tmp= 0;
            VectorTransform2<INPUT, INPUT>* f= dynamic_cast<VectorTransform2<INPUT, INPUT>* >(transform);

            printf("%s\n", f->getDescriptor().c_str());
            transform->apply(input, tmp, NULL, NULL);

            debugFeatures(tmp);

            float minimum= r.getMin(tmp);
            float maximum= r.getMax(tmp);
            float d= (maximum - minimum)/(bins - 1);

            Vector<float> h(bins);
            h= 0;

            for ( unsigned int i= 0; i < r.size(); ++i )
                h((int)floor((tmp(r(i)) - minimum)/d))++;

            h/= r.size();

            for ( unsigned int i= 0; i < h.size(); ++i )
                if ( h(i) > 0 )
                    entropy+= float(h(i))*log(float(h(i)));
        }

        return entropy;
    }
    
    template<typename INPUT>
    class RegionLineMagnitudeFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionLineMagnitudeFeature2();

        RegionLineMagnitudeFeature2(const RegionLineMagnitudeFeature2& r);

        ~RegionLineMagnitudeFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionLineMagnitudeFeature2<INPUT>::RegionLineMagnitudeFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionLineMagnitudeFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionLineMagnitudeFeature2<INPUT>::RegionLineMagnitudeFeature2(const RegionLineMagnitudeFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionLineMagnitudeFeature2<INPUT>::~RegionLineMagnitudeFeature2()
    {
    }

    template<typename INPUT>
    float RegionLineMagnitudeFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        MatchedLineSegmentFilter2<INPUT, float> mlsf(15, 30, 2, 0, 0.2, 3.1, 1, 1, 1, 0);
        mlsf.updateStride(input.columns);
        mlsf.computeMinMax();
        
        Image<float> tmp;
        tmp.resizeImage(input);
        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( r(i) >= -mlsf.getMin() && r(i) < int(input.n) - mlsf.getMax())
            tmp(r(i))= mlsf.apply(input, r(i));
        
        float max= 0;
        for ( unsigned int i= 0; i < r.size(); ++i )
                max+= fabs(tmp(r(i)));
        
        max/= r.size();
        return max;
    }
    
    template<typename INPUT>
    class RegionMaxLineMagnitudeFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionMaxLineMagnitudeFeature2();

        RegionMaxLineMagnitudeFeature2(const RegionMaxLineMagnitudeFeature2& r);

        ~RegionMaxLineMagnitudeFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionMaxLineMagnitudeFeature2<INPUT>::RegionMaxLineMagnitudeFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionMaxLineMagnitudeFeature2";
        descriptor= ss.str();
        reqTransform= false;
    }

    template<typename INPUT>
    RegionMaxLineMagnitudeFeature2<INPUT>::RegionMaxLineMagnitudeFeature2(const RegionMaxLineMagnitudeFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionMaxLineMagnitudeFeature2<INPUT>::~RegionMaxLineMagnitudeFeature2()
    {
    }

    template<typename INPUT>
    float RegionMaxLineMagnitudeFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        MatchedLineSegmentFilter2<INPUT, float> mlsf(15, 30, 2, 0, 0.2, 3.1, 1, 1, 1, 0);
        mlsf.updateStride(input.columns);
        mlsf.computeMinMax();
        
        Image<float> tmp;
        tmp.resizeImage(input);
        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( r(i) >= -mlsf.getMin() && r(i) < int(input.n) - mlsf.getMax())
            tmp(r(i))= mlsf.apply(input, r(i));
        
        float max= 0;
        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( tmp(r(i)) > max )
                max= tmp(r(i));
        
        return max;
    }
    
    template<typename INPUT>
    class RegionEnergyFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionEnergyFeature2();

        RegionEnergyFeature2(const RegionEnergyFeature2& r);

        ~RegionEnergyFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionEnergyFeature2<INPUT>::RegionEnergyFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionEnergyFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionEnergyFeature2<INPUT>::RegionEnergyFeature2(const RegionEnergyFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionEnergyFeature2<INPUT>::~RegionEnergyFeature2()
    {
    }

    template<typename INPUT>
    float RegionEnergyFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float sum= 0;

        if ( transform == NULL )
        {
            debugFeatures(input);
            for ( unsigned int i= 0; i < r.size(); ++i )
                    sum+= fabs(input(r(i)));
        }
        else
        {
            Image<float> tmp;
            tmp.resizeImage(input);
            transform->apply(input, tmp, NULL, NULL);

            debugFeatures(tmp);
            for ( unsigned int i= 0; i < r.size(); ++i )
                    sum+= fabs(tmp(r(i)));

        }

        return sum/r.size();
    }
    
    template<typename INPUT>
    class RegionTotalVariationFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionTotalVariationFeature2();

        RegionTotalVariationFeature2(const RegionTotalVariationFeature2& r);

        ~RegionTotalVariationFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionTotalVariationFeature2<INPUT>::RegionTotalVariationFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionTotalVariationFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionTotalVariationFeature2<INPUT>::RegionTotalVariationFeature2(const RegionTotalVariationFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionTotalVariationFeature2<INPUT>::~RegionTotalVariationFeature2()
    {
    }

    template<typename INPUT>
    float RegionTotalVariationFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<float> tmp;
        tmp.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp, NULL, NULL);
        else
            tmp= input;

        debugFeatures(tmp);
        float sum= 0;
        for ( unsigned int i= 0; i < r.size(); ++i )
        {
            sum+= (tmp(r(i)) - tmp(r(i)+1))*(tmp(r(i)) - tmp(r(i)+1));
            sum+= (tmp(r(i)) - tmp(r(i)-1))*(tmp(r(i)) - tmp(r(i)-1));
            sum+= (tmp(r(i)) - tmp(r(i)+tmp.columns))*(tmp(r(i)) - tmp(r(i)+tmp.columns));
            sum+= (tmp(r(i)) - tmp(r(i)-tmp.columns))*(tmp(r(i)) - tmp(r(i)-tmp.columns));
        }
        
        sum/= r.size();
        
        return sum/4;
    }
    
    template<typename INPUT>
    class RegionThresholdFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionThresholdFeature2(float th);

        RegionThresholdFeature2(const RegionThresholdFeature2& r);

        ~RegionThresholdFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        float th;
    };

    template<typename INPUT>
    RegionThresholdFeature2<INPUT>::RegionThresholdFeature2(float th)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionThresholdFeature2 " << th;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->th= th;
    }

    template<typename INPUT>
    RegionThresholdFeature2<INPUT>::RegionThresholdFeature2(const RegionThresholdFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
        this->th= r.th;
    }

    template<typename INPUT>
    RegionThresholdFeature2<INPUT>::~RegionThresholdFeature2()
    {
    }

    template<typename INPUT>
    float RegionThresholdFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        INPUT min, max;

        Image<float> tmp;
        tmp.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp);
        else
            tmp= input;

        debugFeatures(tmp);

        r.getMinMax(tmp, min, max);

        float res= 0;
        float threshold= min + (max - min)*th;
        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( tmp(r(i)) > threshold )
                res+= 1;
        return res/r.size();
    }
    
    template<typename INPUT>
    class RegionCenterFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionCenterFeature2();

        RegionCenterFeature2(const RegionCenterFeature2& r);

        ~RegionCenterFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };
    
    template<typename INPUT>
    RegionCenterFeature2<INPUT>::RegionCenterFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCenterFeature2";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionCenterFeature2<INPUT>::RegionCenterFeature2(const RegionCenterFeature2& r)
    : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionCenterFeature2<INPUT>::~RegionCenterFeature2()
    {
    }

    template<typename INPUT>
    float RegionCenterFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        int cr, cc;
        cr= 0; cc= 0;
        int ct, rt;
        int n;
        
        for ( unsigned int i= 0; i < r.size(); ++i )
        {
            input.getCoordinate2D(r(i), ct, rt);
            cr+= rt;
            cc+= ct;
        }
        cr= float(cr)/r.size();
        cc= float(cc)/r.size();
        
        input.getCoordinate1D(cr, cc, n);
        
        if ( transform == NULL )
        {
            return input(n);
        }
        else
        {
            Feature2<INPUT, float>* f;
            f= dynamic_cast<Feature2<INPUT, float>* >(f);
            if ( f == NULL )
                return 0;
            else
                return f->apply(input, n);
        }
    }

    template<typename INPUT>
    class RegionPerimeterMeanDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterMeanDifferenceFeature2(int tickness);

        RegionPerimeterMeanDifferenceFeature2(const RegionPerimeterMeanDifferenceFeature2& r);

        ~RegionPerimeterMeanDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterMeanDifferenceFeature2<INPUT>::RegionPerimeterMeanDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterMeanDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterMeanDifferenceFeature2<INPUT>::RegionPerimeterMeanDifferenceFeature2(const RegionPerimeterMeanDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterMeanDifferenceFeature2<INPUT>::~RegionPerimeterMeanDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterMeanDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getMean(input) - perimeter.getMean(input);
    }

    template<typename INPUT>
    class RegionPerimeterMinDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterMinDifferenceFeature2(int tickness);

        RegionPerimeterMinDifferenceFeature2(const RegionPerimeterMinDifferenceFeature2& r);

        ~RegionPerimeterMinDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterMinDifferenceFeature2<INPUT>::RegionPerimeterMinDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterMinDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterMinDifferenceFeature2<INPUT>::RegionPerimeterMinDifferenceFeature2(const RegionPerimeterMinDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterMinDifferenceFeature2<INPUT>::~RegionPerimeterMinDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterMinDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getMin(input) - perimeter.getMin(input);
    }

    template<typename INPUT>
    class RegionPerimeterMaxDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterMaxDifferenceFeature2(int tickness);

        RegionPerimeterMaxDifferenceFeature2(const RegionPerimeterMaxDifferenceFeature2& r);

        ~RegionPerimeterMaxDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterMaxDifferenceFeature2<INPUT>::RegionPerimeterMaxDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterMaxDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterMaxDifferenceFeature2<INPUT>::RegionPerimeterMaxDifferenceFeature2(const RegionPerimeterMaxDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterMaxDifferenceFeature2<INPUT>::~RegionPerimeterMaxDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterMaxDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getMax(input) - perimeter.getMax(input);
    }

    template<typename INPUT>
    class RegionPerimeterVarianceDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterVarianceDifferenceFeature2(int tickness);

        RegionPerimeterVarianceDifferenceFeature2(const RegionPerimeterVarianceDifferenceFeature2& r);

        ~RegionPerimeterVarianceDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterVarianceDifferenceFeature2<INPUT>::RegionPerimeterVarianceDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterVarianceDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterVarianceDifferenceFeature2<INPUT>::RegionPerimeterVarianceDifferenceFeature2(const RegionPerimeterVarianceDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterVarianceDifferenceFeature2<INPUT>::~RegionPerimeterVarianceDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterVarianceDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getVariance(input) - perimeter.getVariance(input);
    }

    template<typename INPUT>
    class RegionPerimeterStandardDeviationDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterStandardDeviationDifferenceFeature2(int tickness);

        RegionPerimeterStandardDeviationDifferenceFeature2(const RegionPerimeterStandardDeviationDifferenceFeature2& r);

        ~RegionPerimeterStandardDeviationDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterStandardDeviationDifferenceFeature2<INPUT>::RegionPerimeterStandardDeviationDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterStandardDeviationDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterStandardDeviationDifferenceFeature2<INPUT>::RegionPerimeterStandardDeviationDifferenceFeature2(const RegionPerimeterStandardDeviationDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterStandardDeviationDifferenceFeature2<INPUT>::~RegionPerimeterStandardDeviationDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterStandardDeviationDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getStandardDeviation(input) - perimeter.getStandardDeviation(input);
    }

    template<typename INPUT>
    class RegionPerimeterSkewnessDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterSkewnessDifferenceFeature2(int tickness);

        RegionPerimeterSkewnessDifferenceFeature2(const RegionPerimeterSkewnessDifferenceFeature2& r);

        ~RegionPerimeterSkewnessDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterSkewnessDifferenceFeature2<INPUT>::RegionPerimeterSkewnessDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterSkewnessDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterSkewnessDifferenceFeature2<INPUT>::RegionPerimeterSkewnessDifferenceFeature2(const RegionPerimeterSkewnessDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterSkewnessDifferenceFeature2<INPUT>::~RegionPerimeterSkewnessDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterSkewnessDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getSkewness(input) - perimeter.getSkewness(input);
    }

    template<typename INPUT>
    class RegionPerimeterKurtosisDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterKurtosisDifferenceFeature2(int tickness);

        RegionPerimeterKurtosisDifferenceFeature2(const RegionPerimeterKurtosisDifferenceFeature2& r);

        ~RegionPerimeterKurtosisDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterKurtosisDifferenceFeature2<INPUT>::RegionPerimeterKurtosisDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterKurtosisDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterKurtosisDifferenceFeature2<INPUT>::RegionPerimeterKurtosisDifferenceFeature2(const RegionPerimeterKurtosisDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterKurtosisDifferenceFeature2<INPUT>::~RegionPerimeterKurtosisDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterKurtosisDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return r.getKurtosis(input) - perimeter.getKurtosis(input);
    }

    template<typename INPUT>
    class RegionRangeFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionRangeFeature2();

        RegionRangeFeature2(const RegionRangeFeature2& r);

        ~RegionRangeFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionRangeFeature2<INPUT>::RegionRangeFeature2()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionRangeFeature2 ";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionRangeFeature2<INPUT>::RegionRangeFeature2(const RegionRangeFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionRangeFeature2<INPUT>::~RegionRangeFeature2()
    {
    }

    template<typename INPUT>
    float RegionRangeFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<float> tmp;
        tmp.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp, NULL, NULL);
        else
            tmp= input;

        debugFeatures(tmp);
        return r.getMax(tmp) - r.getMin(tmp);
    }

    template<typename INPUT>
    class RegionPerimeterRangeDifferenceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionPerimeterRangeDifferenceFeature2(int tickness);

        RegionPerimeterRangeDifferenceFeature2(const RegionPerimeterRangeDifferenceFeature2& r);

        ~RegionPerimeterRangeDifferenceFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int tickness;
    };

    template<typename INPUT>
    RegionPerimeterRangeDifferenceFeature2<INPUT>::RegionPerimeterRangeDifferenceFeature2(int tickness)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionPerimeterRangeDifferenceFeature2 " << tickness;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->tickness= tickness;
    }

    template<typename INPUT>
    RegionPerimeterRangeDifferenceFeature2<INPUT>::RegionPerimeterRangeDifferenceFeature2(const RegionPerimeterRangeDifferenceFeature2& r)
        : RegionFeature2<INPUT>(r), tickness(r.tickness)
    {
    }

    template<typename INPUT>
    RegionPerimeterRangeDifferenceFeature2<INPUT>::~RegionPerimeterRangeDifferenceFeature2()
    {
    }

    template<typename INPUT>
    float RegionPerimeterRangeDifferenceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        StructuringElementDisk sed(tickness);
        sed.updateStride(input.columns);
        PixelSet1 tmp= binaryDilate(r, sed);
        PixelSet1 perimeter= tmp / r;

        return (r.getMax(input) - r.getMin(input)) - (perimeter.getMax(input) - perimeter.getMin(input));
    }

    template<typename INPUT>
    class RegionHistogramFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionHistogramFeature2(int bins);

        RegionHistogramFeature2(const RegionHistogramFeature2& r);

        ~RegionHistogramFeature2();

        virtual int dimensions();

        virtual float apply(Image<INPUT>& input, Region2& r);

        void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
    };

    template<typename INPUT>
    RegionHistogramFeature2<INPUT>::RegionHistogramFeature2(int bins)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionHistogramFeature2 " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
    }

    template<typename INPUT>
    RegionHistogramFeature2<INPUT>::RegionHistogramFeature2(const RegionHistogramFeature2& r)
        : RegionFeature2<INPUT>(r), bins(r.bins)
    {
    }

    template<typename INPUT>
    RegionHistogramFeature2<INPUT>::~RegionHistogramFeature2()
    {
    }

    template<typename INPUT>
    int RegionHistogramFeature2<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    float RegionHistogramFeature2<INPUT>::apply(Image<INPUT>& /*input*/, Region2& /*r*/)
    {
        printf("This should not be called ever: RegionHistogramFeature2::apply\n"); fflush(stdout);
        return 0.0;
    }

    template<typename INPUT>
    void RegionHistogramFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        if ( transform == 0 )
        {
            debugFeatures(input);
            float minimum= r.getMin(input);
            float maximum= r.getMax(input);
            float d= (maximum - minimum)/(bins - 1);

            Vector<float> h(bins);
            h= 0;

            for ( unsigned int i= 0; i < r.size(); ++i )
                h((int)floor((input(r(i)) - minimum)/d))++;

            h/= r.size();

            for ( unsigned int i= 0; i < h.size(); ++i )
                output(actual + i)= h(i);
        }
        else
        {
            Image<INPUT> tmp;
            tmp.resizeImage(input);
            tmp= 0;
            VectorTransform2<INPUT, INPUT>* f= dynamic_cast<VectorTransform2<INPUT, INPUT>* >(transform);

            printf("%s\n", f->getDescriptor().c_str());
            transform->apply(input, tmp, NULL, NULL);

            debugFeatures(tmp);
            float minimum= r.getMin(tmp);
            float maximum= r.getMax(tmp);
            float d= (maximum - minimum)/(bins - 1);

            Vector<float> h(bins);
            h= 0;

            for ( unsigned int i= 0; i < r.size(); ++i )
                h((int)floor((tmp(r(i)) - minimum)/d))++;

            h/= r.size();

            for ( unsigned int i= 0; i < h.size(); ++i )
                output(actual + i)= h(i);
        }
    }

    template<typename INPUT>
    class RegionNumberOfRegionsThreshold: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionNumberOfRegionsThreshold(float th);

        RegionNumberOfRegionsThreshold(const RegionNumberOfRegionsThreshold& r);

        ~RegionNumberOfRegionsThreshold();

        virtual float apply(Image<INPUT>& input, Region2& r);

        float th;
    };

    template<typename INPUT>
    RegionNumberOfRegionsThreshold<INPUT>::RegionNumberOfRegionsThreshold(float th)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionNumberOfRegionsThreshold " << th;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->th= th;
    }

    template<typename INPUT>
    RegionNumberOfRegionsThreshold<INPUT>::RegionNumberOfRegionsThreshold(const RegionNumberOfRegionsThreshold& r)
        : RegionFeature2<INPUT>(r), th(r.th)
    {
    }

    template<typename INPUT>
    RegionNumberOfRegionsThreshold<INPUT>::~RegionNumberOfRegionsThreshold()
    {
    }

    template<typename INPUT>
    float RegionNumberOfRegionsThreshold<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float min, max;

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<float> tmp2;
        tmp2.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp2, NULL, NULL);
        else
            tmp2= input;

        debugFeatures(tmp2);
        r.getMinMax(tmp2, min, max);

        float threshold= min + (max - min)*th;
        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( tmp2(r(i)) > threshold )
                tmp(r(i))= 1;

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        return vr.size();
    }

    template<typename INPUT>
    class RegionAverageSizeOfRegionsThreshold: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionAverageSizeOfRegionsThreshold(float th);

        RegionAverageSizeOfRegionsThreshold(const RegionAverageSizeOfRegionsThreshold& r);

        ~RegionAverageSizeOfRegionsThreshold();

        virtual float apply(Image<INPUT>& input, Region2& r);

        float th;
    };

    template<typename INPUT>
    RegionAverageSizeOfRegionsThreshold<INPUT>::RegionAverageSizeOfRegionsThreshold(float th)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionAverageSizeOfRegionsThreshold " << th;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->th= th;
    }

    template<typename INPUT>
    RegionAverageSizeOfRegionsThreshold<INPUT>::RegionAverageSizeOfRegionsThreshold(const RegionAverageSizeOfRegionsThreshold& r)
        : RegionFeature2<INPUT>(r), th(r.th)
    {
    }

    template<typename INPUT>
    RegionAverageSizeOfRegionsThreshold<INPUT>::~RegionAverageSizeOfRegionsThreshold()
    {
    }

    template<typename INPUT>
    float RegionAverageSizeOfRegionsThreshold<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        float min, max;

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<float> tmp2;
        tmp2.resizeImage(input);
        if ( transform )
            transform->apply(input, tmp2, NULL, NULL);
        else
            tmp2= input;

        debugFeatures(tmp2);
        r.getMinMax(tmp2, min, max);

        float threshold= min + (max - min)*th;
        for ( unsigned int i= 0; i < r.size(); ++i )
            if ( tmp2(r(i)) > threshold )
                tmp(r(i))= 1;

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sum+= vr(i).size();
        sum/= vr.size();

        return sum;
    }

    template<typename INPUT>
    class RegionOtsuNumberOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionOtsuNumberOfRegions();

        RegionOtsuNumberOfRegions(const RegionOtsuNumberOfRegions& r);

        ~RegionOtsuNumberOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionOtsuNumberOfRegions<INPUT>::RegionOtsuNumberOfRegions()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionOtsuNumberOfRegions";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionOtsuNumberOfRegions<INPUT>::RegionOtsuNumberOfRegions(const RegionOtsuNumberOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionOtsuNumberOfRegions<INPUT>::~RegionOtsuNumberOfRegions()
    {
    }

    template<typename INPUT>
    float RegionOtsuNumberOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        OtsuThresholding2<INPUT, unsigned char> otsu(256);
        otsu.apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        return vr.size();
    }

    template<typename INPUT>
    class RegionOtsuAverageSizeOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionOtsuAverageSizeOfRegions();

        RegionOtsuAverageSizeOfRegions(const RegionOtsuAverageSizeOfRegions& r);

        ~RegionOtsuAverageSizeOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionOtsuAverageSizeOfRegions<INPUT>::RegionOtsuAverageSizeOfRegions()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionOtsuAverageSizeOfRegions";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionOtsuAverageSizeOfRegions<INPUT>::RegionOtsuAverageSizeOfRegions(const RegionOtsuAverageSizeOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionOtsuAverageSizeOfRegions<INPUT>::~RegionOtsuAverageSizeOfRegions()
    {
    }

    template<typename INPUT>
    float RegionOtsuAverageSizeOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        OtsuThresholding2<INPUT, unsigned char> otsu(256);
        otsu.apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sum+= vr(i).size();

        return sum/vr.size();
    }

    template<typename INPUT>
    class RegionOtsuAveragePerimeterOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::apply;

        RegionOtsuAveragePerimeterOfRegions();

        RegionOtsuAveragePerimeterOfRegions(const RegionOtsuAveragePerimeterOfRegions& r);

        ~RegionOtsuAveragePerimeterOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);
    };

    template<typename INPUT>
    RegionOtsuAveragePerimeterOfRegions<INPUT>::RegionOtsuAveragePerimeterOfRegions()
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionOtsuAveragePerimeterOfRegions";
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
    }

    template<typename INPUT>
    RegionOtsuAveragePerimeterOfRegions<INPUT>::RegionOtsuAveragePerimeterOfRegions(const RegionOtsuAveragePerimeterOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
    }

    template<typename INPUT>
    RegionOtsuAveragePerimeterOfRegions<INPUT>::~RegionOtsuAveragePerimeterOfRegions()
    {
    }

    template<typename INPUT>
    float RegionOtsuAveragePerimeterOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        OtsuThresholding2<INPUT, unsigned char> otsu(256);
        otsu.apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sum+= vr(i).perimeter();

        return sum/vr.size();
    }

    template<typename INPUT>
    class RegionOtsuSizeHistogramOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::apply;

        RegionOtsuSizeHistogramOfRegions(int bins);

        RegionOtsuSizeHistogramOfRegions(const RegionOtsuSizeHistogramOfRegions& r);

        ~RegionOtsuSizeHistogramOfRegions();

        int dimensions();

        virtual float apply(Image<INPUT>& input, Region2& r, int actual, Vector<float>& output);

        int bins;
    };

    template<typename INPUT>
    RegionOtsuSizeHistogramOfRegions<INPUT>::RegionOtsuSizeHistogramOfRegions(int bins)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionOtsuSizeHistogramOfRegions " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        bins= this->bins;
    }

    template<typename INPUT>
    RegionOtsuSizeHistogramOfRegions<INPUT>::RegionOtsuSizeHistogramOfRegions(const RegionOtsuSizeHistogramOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
    }

    template<typename INPUT>
    RegionOtsuSizeHistogramOfRegions<INPUT>::~RegionOtsuSizeHistogramOfRegions()
    {
    }

    template<typename INPUT>
    int RegionOtsuSizeHistogramOfRegions<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    float RegionOtsuSizeHistogramOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r, int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        OtsuThresholding2<INPUT, unsigned char> otsu(256);
        otsu.apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<int> sizes(vr.size());
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sizes(i)= vr(i).size();

        float minimum= r.getMin(input);
        float maximum= r.getMax(input);
        float d= (maximum - minimum)/(bins - 1);

        Vector<float> h(bins);
        h= 0;

        if ( d != 0 )
            for ( unsigned int i= 0; i < r.size(); ++i )
                h((int)floor((float)sizes(i) - minimum)/d)++;
        else
            h(0)= 1;

        h/= r.size();

        for ( unsigned int i= 0; i < h.size(); ++i )
            output(actual + i)= h(i);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sum+= vr(i).size();

        return sum/vr.size();
    }

    template<typename INPUT>
    class RegionOtsuPerimeterHistogramOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::apply;

        RegionOtsuPerimeterHistogramOfRegions(int bins);

        RegionOtsuPerimeterHistogramOfRegions(const RegionOtsuPerimeterHistogramOfRegions& r);

        ~RegionOtsuPerimeterHistogramOfRegions();

        int dimensions();

        virtual float apply(Image<INPUT>& input, Region2& r, int actual, Vector<float>& output);

        int bins;
    };

    template<typename INPUT>
    RegionOtsuPerimeterHistogramOfRegions<INPUT>::RegionOtsuPerimeterHistogramOfRegions(int bins)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionOtsuPerimeterHistogramOfRegions " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        bins= this->bins;
    }

    template<typename INPUT>
    RegionOtsuPerimeterHistogramOfRegions<INPUT>::RegionOtsuPerimeterHistogramOfRegions(const RegionOtsuPerimeterHistogramOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
    }

    template<typename INPUT>
    RegionOtsuPerimeterHistogramOfRegions<INPUT>::~RegionOtsuPerimeterHistogramOfRegions()
    {
    }

    template<typename INPUT>
    int RegionOtsuPerimeterHistogramOfRegions<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    float RegionOtsuPerimeterHistogramOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r, int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        OtsuThresholding2<INPUT, unsigned char> otsu(256);
        otsu.apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<int> sizes(vr.size());
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sizes(i)= vr(i).perimeter();

        float minimum= r.getMin(input);
        float maximum= r.getMax(input);
        float d= (maximum - minimum)/(bins - 1);

        Vector<float> h(bins);
        h= 0;

        if ( d != 0 )
            for ( unsigned int i= 0; i < r.size(); ++i )
                h((int)floor((float)sizes(i) - minimum)/d)++;
        else
            h(0)= 1;

        h/= r.size();

        for ( unsigned int i= 0; i < h.size(); ++i )
            output(actual + i)= h(i);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sum+= vr(i).size();

        return sum/vr.size();
    }





    /** general adaptive threshold based region features*/

    template<typename INPUT>
    class RegionGeneralThresholdNumberOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdNumberOfRegions(int threshold);

        RegionGeneralThresholdNumberOfRegions(const RegionGeneralThresholdNumberOfRegions& r);

        ~RegionGeneralThresholdNumberOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdNumberOfRegions<INPUT>::RegionGeneralThresholdNumberOfRegions(int threshold)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdNumberOfRegions " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdNumberOfRegions<INPUT>::RegionGeneralThresholdNumberOfRegions(const RegionGeneralThresholdNumberOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdNumberOfRegions<INPUT>::~RegionGeneralThresholdNumberOfRegions()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdNumberOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        return vr.size();
    }

    template<typename INPUT>
    class RegionGeneralThresholdAverageSizeOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdAverageSizeOfRegions(int threshold);

        RegionGeneralThresholdAverageSizeOfRegions(const RegionGeneralThresholdAverageSizeOfRegions& r);

        ~RegionGeneralThresholdAverageSizeOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdAverageSizeOfRegions<INPUT>::RegionGeneralThresholdAverageSizeOfRegions(int threshold)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdAverageSizeOfRegions " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdAverageSizeOfRegions<INPUT>::RegionGeneralThresholdAverageSizeOfRegions(const RegionGeneralThresholdAverageSizeOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdAverageSizeOfRegions<INPUT>::~RegionGeneralThresholdAverageSizeOfRegions()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdAverageSizeOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sum+= vr(i).size();

        if ( vr.size() > 0 )
            return sum/vr.size();
        else
            return 0;
    }


    template<typename INPUT>
    class RegionGeneralThresholdSizeHistogramOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdSizeHistogramOfRegions(int threshold, int bins);

        RegionGeneralThresholdSizeHistogramOfRegions(const RegionGeneralThresholdSizeHistogramOfRegions& r);

        ~RegionGeneralThresholdSizeHistogramOfRegions();

        virtual int dimensions();

        float apply(Image<INPUT>& input, Region2& r);

        void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdSizeHistogramOfRegions<INPUT>::RegionGeneralThresholdSizeHistogramOfRegions(int threshold, int bins)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdSizeHistogramOfRegions " << threshold << " " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdSizeHistogramOfRegions<INPUT>::RegionGeneralThresholdSizeHistogramOfRegions(const RegionGeneralThresholdSizeHistogramOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdSizeHistogramOfRegions<INPUT>::~RegionGeneralThresholdSizeHistogramOfRegions()
    {
    }

    template<typename INPUT>
    int RegionGeneralThresholdSizeHistogramOfRegions<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    float RegionGeneralThresholdSizeHistogramOfRegions<INPUT>::apply(Image<INPUT>& /*input*/, Region2& /*r*/)
    {
        printf("This should not be called ever: RegionHistogramFeature2::apply\n"); fflush(stdout);
        return 0.0;
    }

    template<typename INPUT>
    void RegionGeneralThresholdSizeHistogramOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<int> sizes(vr.size());
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sizes(i)= vr(i).size();

        float minimum= sizes.getMin();
        float maximum= sizes.getMax();
        float d= (maximum - minimum)/(bins - 1);

        Vector<float> h(bins);
        h= 0;

        if ( d != 0 )
            for ( unsigned int i= 0; i < sizes.size(); ++i )
                h((int)floor(((float)sizes(i) - minimum)/d))++;
        else
            h(0)= 1;

        h/= sizes.size();

        for ( unsigned int i= 0; i < h.size(); ++i )
            output(actual + i)= h(i);
    }

    template<typename INPUT>
    class RegionGeneralThresholdAveragePerimeterOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdAveragePerimeterOfRegions(int threshold);

        RegionGeneralThresholdAveragePerimeterOfRegions(const RegionGeneralThresholdAveragePerimeterOfRegions& r);

        ~RegionGeneralThresholdAveragePerimeterOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdAveragePerimeterOfRegions<INPUT>::RegionGeneralThresholdAveragePerimeterOfRegions(int threshold)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdAveragePerimeterOfRegions " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdAveragePerimeterOfRegions<INPUT>::RegionGeneralThresholdAveragePerimeterOfRegions(const RegionGeneralThresholdAveragePerimeterOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdAveragePerimeterOfRegions<INPUT>::~RegionGeneralThresholdAveragePerimeterOfRegions()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdAveragePerimeterOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        float sum= 0;
        for ( unsigned int i= 0; i < vr.size(); ++i )
        {
            sum+= vr(i).perimeter();
        }

        if ( vr.size() > 0 )
            return sum/vr.size();
        else
            return 0;
    }

    template<typename INPUT>
    class RegionGeneralThresholdStdDevSizeOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdStdDevSizeOfRegions(int threshold);

        RegionGeneralThresholdStdDevSizeOfRegions(const RegionGeneralThresholdStdDevSizeOfRegions& r);

        ~RegionGeneralThresholdStdDevSizeOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdStdDevSizeOfRegions<INPUT>::RegionGeneralThresholdStdDevSizeOfRegions(int threshold)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdStdDevSizeOfRegions " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdStdDevSizeOfRegions<INPUT>::RegionGeneralThresholdStdDevSizeOfRegions(const RegionGeneralThresholdStdDevSizeOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdStdDevSizeOfRegions<INPUT>::~RegionGeneralThresholdStdDevSizeOfRegions()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdStdDevSizeOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<float> res(vr.size());

        for ( unsigned int i= 0; i < vr.size(); ++i )
        {
            res(i)= vr(i).size();
        }

        return res.getStandardDeviation();
    }

    template<typename INPUT>
    class RegionGeneralThresholdStdDevPerimeterOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdStdDevPerimeterOfRegions(int threshold);

        RegionGeneralThresholdStdDevPerimeterOfRegions(const RegionGeneralThresholdStdDevPerimeterOfRegions& r);

        ~RegionGeneralThresholdStdDevPerimeterOfRegions();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdStdDevPerimeterOfRegions<INPUT>::RegionGeneralThresholdStdDevPerimeterOfRegions(int threshold)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdStdDevPerimeterOfRegions " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdStdDevPerimeterOfRegions<INPUT>::RegionGeneralThresholdStdDevPerimeterOfRegions(const RegionGeneralThresholdStdDevPerimeterOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdStdDevPerimeterOfRegions<INPUT>::~RegionGeneralThresholdStdDevPerimeterOfRegions()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdStdDevPerimeterOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<float> res(vr.size());

        for ( unsigned int i= 0; i < vr.size(); ++i )
        {
            res(i)= vr(i).perimeter();
        }

        return res.getStandardDeviation();
    }

    template<typename INPUT>
    class RegionGeneralThresholdNumberOfRegionsSizeInRange: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdNumberOfRegionsSizeInRange(int threshold, int a, int b);

        RegionGeneralThresholdNumberOfRegionsSizeInRange(const RegionGeneralThresholdNumberOfRegionsSizeInRange& r);

        ~RegionGeneralThresholdNumberOfRegionsSizeInRange();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
        int a;
        int b;
    };

    template<typename INPUT>
    RegionGeneralThresholdNumberOfRegionsSizeInRange<INPUT>::RegionGeneralThresholdNumberOfRegionsSizeInRange(int threshold, int a, int b)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdNumberOfRegionsSizeInRange " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
        this->a= a;
        this->b= b;
    }

    template<typename INPUT>
    RegionGeneralThresholdNumberOfRegionsSizeInRange<INPUT>::RegionGeneralThresholdNumberOfRegionsSizeInRange(const RegionGeneralThresholdNumberOfRegionsSizeInRange& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
        a= r.a;
        b= r.b;
    }

    template<typename INPUT>
    RegionGeneralThresholdNumberOfRegionsSizeInRange<INPUT>::~RegionGeneralThresholdNumberOfRegionsSizeInRange()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdNumberOfRegionsSizeInRange<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<float> res(vr.size());

        float num= 0;

        for ( unsigned int i= 0; i < vr.size(); ++i )
        {
            if ( int(vr.size()) > a && int(vr.size()) < b )
                num+= 1;
        }

        return num;
    }

    template<typename INPUT>
    class RegionGeneralThresholdAverageCircularity: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdAverageCircularity(int threshold);

        RegionGeneralThresholdAverageCircularity(const RegionGeneralThresholdAverageCircularity& r);

        ~RegionGeneralThresholdAverageCircularity();

        virtual float apply(Image<INPUT>& input, Region2& r);

        int threshold;
        int a;
        int b;
    };

    template<typename INPUT>
    RegionGeneralThresholdAverageCircularity<INPUT>::RegionGeneralThresholdAverageCircularity(int threshold)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdAverageCircularity " << threshold;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdAverageCircularity<INPUT>::RegionGeneralThresholdAverageCircularity(const RegionGeneralThresholdAverageCircularity& r)
        : RegionFeature2<INPUT>(r)
    {
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdAverageCircularity<INPUT>::~RegionGeneralThresholdAverageCircularity()
    {
    }

    template<typename INPUT>
    float RegionGeneralThresholdAverageCircularity<INPUT>::apply(Image<INPUT>& input, Region2& r)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<float> res(vr.size());

        float num= 0;

        for ( unsigned int i= 0; i < vr.size(); ++i )
        {
            num+= vr(i).size()/vr(i).perimeter();
        }

        return num/vr.size();
    }

    template<typename INPUT>
    class RegionGeneralThresholdPerimeterHistogramOfRegions: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionGeneralThresholdPerimeterHistogramOfRegions(int threshold, int bins);

        RegionGeneralThresholdPerimeterHistogramOfRegions(const RegionGeneralThresholdPerimeterHistogramOfRegions& r);

        ~RegionGeneralThresholdPerimeterHistogramOfRegions();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
        int threshold;
    };

    template<typename INPUT>
    RegionGeneralThresholdPerimeterHistogramOfRegions<INPUT>::RegionGeneralThresholdPerimeterHistogramOfRegions(int threshold, int bins)
    : RegionFeature2<INPUT>(threshold)
    {
        std::stringstream ss;
        ss << "RegionGeneralThresholdPerimeterHistogramOfRegions " << threshold << " " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
        this->threshold= threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdPerimeterHistogramOfRegions<INPUT>::RegionGeneralThresholdPerimeterHistogramOfRegions(const RegionGeneralThresholdPerimeterHistogramOfRegions& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
        threshold= r.threshold;
    }

    template<typename INPUT>
    RegionGeneralThresholdPerimeterHistogramOfRegions<INPUT>::~RegionGeneralThresholdPerimeterHistogramOfRegions()
    {
    }

    template<typename INPUT>
    int RegionGeneralThresholdPerimeterHistogramOfRegions<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    void RegionGeneralThresholdPerimeterHistogramOfRegions<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);

        ExtractRegions er;
        Vector<Region2> vr;
        er.apply(tmp, vr);

        Vector<int> sizes(vr.size());
        for ( unsigned int i= 0; i < vr.size(); ++i )
            sizes(i)= vr(i).perimeter();

        float minimum= sizes.getMin();
        float maximum= sizes.getMax();
        float d= (maximum - minimum)/(bins - 1);

        Vector<float> h(bins);
        h= 0;

        if ( d != 0 )
            for ( unsigned int i= 0; i < sizes.size(); ++i )
                h((int)floor(((float)sizes(i) - minimum)/d))++;
        else
            h(0)= 1;

        h/= sizes.size();

        for ( unsigned int i= 0; i < h.size(); ++i )
            output(actual + i)= h(i);
    }

    template<typename INPUT>
    class RegionCooccuranceDiagonalFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionCooccuranceDiagonalFeature2(int bins);

        RegionCooccuranceDiagonalFeature2(const RegionCooccuranceDiagonalFeature2& r);

        ~RegionCooccuranceDiagonalFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
    };

    template<typename INPUT>
    RegionCooccuranceDiagonalFeature2<INPUT>::RegionCooccuranceDiagonalFeature2(int bins)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCooccuranceDiagonalFeature2 " << " " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
    }

    template<typename INPUT>
    RegionCooccuranceDiagonalFeature2<INPUT>::RegionCooccuranceDiagonalFeature2(const RegionCooccuranceDiagonalFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
    }

    template<typename INPUT>
    RegionCooccuranceDiagonalFeature2<INPUT>::~RegionCooccuranceDiagonalFeature2()
    {
    }

    template<typename INPUT>
    int RegionCooccuranceDiagonalFeature2<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    void RegionCooccuranceDiagonalFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        /*tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(tmp);*/

        float minimum= transformed.getMin(&roi);
        float maximum= transformed.getMax(&roi);
        float d= (maximum - minimum)/(bins - 1);
        Matrix<int> cooc(bins, bins);
        cooc.resize(bins, bins);
        cooc= 0;

        Image<int> labels;
        labels.resizeImage(transformed);
        for ( unsigned int i= 0; i < transformed.n; ++i )
            labels(i)= (int)floor(((float)transformed(i) - minimum)/d);

        if ( d != 0 )
        {
            for ( unsigned int i= 0; i < input.n; ++i )
            {
                if ( roi(i) )
                {
                    if ( i + 1 >= 0 && i + 1 < input.n && roi(i + 1))
                    {
                        cooc(labels(i),labels(i+1))++;
                        cooc(labels(i+1),labels(i))++;
                    }

                    if ( i - 1 >= 0 && i - 1 < input.n && roi(i - 1))
                    {
                        cooc(labels(i),labels(i-1))++;
                        cooc(labels(i-1),labels(i))++;
                    }

                    if ( i - input.columns >= 0 && i - input.columns < input.n && roi(i - input.columns))
                    {
                        cooc(labels(i),labels(i - input.columns))++;
                        cooc(labels(i - input.columns),labels(i))++;
                    }

                    if ( i + input.columns >= 0 && i + input.columns < input.n && roi(i + input.columns))
                    {
                        cooc(labels(i),labels(i + input.columns))++;
                        cooc(labels(i + input.columns),labels(i))++;
                    }
                }
            }
        }

        Vector<float> res(cooc.rows);
        for ( int i= 0; i < cooc.rows; ++i )
            res(i)= cooc(i,i);
        res.normalize(0, 1);
        for ( unsigned int i= 0; i < res.size(); ++i )
            output(actual + i)= res(i);
    }

    template<typename INPUT>
    class RegionCooccuranceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionCooccuranceFeature2(int bins);

        RegionCooccuranceFeature2(const RegionCooccuranceFeature2& r);

        ~RegionCooccuranceFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
    };

    template<typename INPUT>
    RegionCooccuranceFeature2<INPUT>::RegionCooccuranceFeature2(int bins)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCooccuranceFeature2 " << " " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
    }

    template<typename INPUT>
    RegionCooccuranceFeature2<INPUT>::RegionCooccuranceFeature2(const RegionCooccuranceFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
    }

    template<typename INPUT>
    RegionCooccuranceFeature2<INPUT>::~RegionCooccuranceFeature2()
    {
    }

    template<typename INPUT>
    int RegionCooccuranceFeature2<INPUT>::dimensions()
    {
        return (bins + 1)*bins*0.5;
    }

    template<typename INPUT>
    void RegionCooccuranceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        /*debugFeatures(tmp);*/

        float minimum= transformed.getMin(&roi);
        float maximum= transformed.getMax(&roi);
        float d= (maximum - minimum)/(bins - 1);
        Matrix<float> cooc(bins, bins);
        cooc= 0;

        int n= 0;
        if ( d != 0 )
        {
            Image<int> labels;
            labels.resizeImage(input.rows, input.columns);
            for ( unsigned int i= 0; i < transformed.n; ++i )
                if ( roi(i) )
                    labels(i)= (int)floor(((float)transformed(i) - minimum)/d);

            for ( unsigned int i= 0; i < input.n; ++i )
            {
                if ( roi(i) )
                {
                    if ( i + 1 >= 0 && i + 1 < input.n && roi(i + 1))
                    {
                        cooc(labels(i),labels(i+1))++;
                        cooc(labels(i+1),labels(i))++;
                        ++n;
                    }

                    if ( i - 1 >= 0 && i - 1 < input.n && roi(i - 1))
                    {
                        cooc(labels(i),labels(i-1))++;
                        cooc(labels(i-1),labels(i))++;
                        ++n;
                    }

                    if ( i - input.columns >= 0 && i - input.columns < input.n && roi(i - input.columns))
                    {
                        cooc(labels(i),labels(i - input.columns))++;
                        cooc(labels(i - input.columns),labels(i))++;
                        ++n;
                    }

                    if ( i + input.columns >= 0 && i + input.columns < input.n && roi(i + input.columns))
                    {
                        cooc(labels(i),labels(i + input.columns))++;
                        cooc(labels(i + input.columns),labels(i))++;
                        ++n;
                    }
                }
            }
        }



        int k= 0;
        for ( int i= 0; i < cooc.rows; ++i )
            for ( int j= i; j < cooc.columns; ++j )
            {
                output(actual + k)= cooc(i,j)/(n*2.0);
                ++k;
            }
    }




    template<typename INPUT>
    class RegionCooccuranceThresholdFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionCooccuranceThresholdFeature2(int bins, float th);

        RegionCooccuranceThresholdFeature2(const RegionCooccuranceThresholdFeature2& r);

        ~RegionCooccuranceThresholdFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
        float th;
    };

    template<typename INPUT>
    RegionCooccuranceThresholdFeature2<INPUT>::RegionCooccuranceThresholdFeature2(int bins, float th)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCooccuranceThresholdFeature2 " << " " << bins << " " << th;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
        this->th= th;
    }

    template<typename INPUT>
    RegionCooccuranceThresholdFeature2<INPUT>::RegionCooccuranceThresholdFeature2(const RegionCooccuranceThresholdFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
        th= r.th;
    }

    template<typename INPUT>
    RegionCooccuranceThresholdFeature2<INPUT>::~RegionCooccuranceThresholdFeature2()
    {
    }

    template<typename INPUT>
    int RegionCooccuranceThresholdFeature2<INPUT>::dimensions()
    {
        return 2*bins*bins;
    }

    template<typename INPUT>
    void RegionCooccuranceThresholdFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        /*debugFeatures(tmp);*/

        float minimum= transformed.getMin(&roi);
        float maximum= transformed.getMax(&roi);
        float d= (maximum - minimum)/(bins - 1);
        Matrix<float> coocbg(bins, bins);
        coocbg= 0;
        Matrix<float> coocfg(bins, bins);
        coocfg= 0;

        int nbg= 0;
        int nfg= 0;

        Image<unsigned char> thresholded;
        thresholded.resizeImage(input);
        thresholded= 0;
        ShanbagThresholding2<INPUT, unsigned char> li(256);
        li.apply(input, thresholded, &roi, &roi);

        Vector<int> pos;
        pos.push_back(1);
        pos.push_back(-1);
        pos.push_back(input.columns);
        pos.push_back(-input.columns);
        if ( d != 0 )
        {
            Image<int> labels;
            labels.resizeImage(input.rows, input.columns);
            for ( unsigned int i= 0; i < transformed.n; ++i )
                if ( roi(i) )
                    labels(i)= (int)floor(((float)transformed(i) - minimum)/d);

            for ( unsigned int i= 0; i < input.n; ++i )
            {
                if ( roi(i) )
                {
                    for ( unsigned int j= 0; j < pos.size(); ++j )
                    {
                        if ( i + pos(j) >= 0 && i + pos(j) < (input.n) && roi(i + pos(j)))
                        {
                            if ( thresholded(i) )
                            {
                                coocfg(labels(i),labels(i+1))++;
                                coocfg(labels(i+1),labels(i))++;
                                ++nfg;
                            }
                            else
                            {
                                coocbg(labels(i),labels(i+1))++;
                                coocbg(labels(i+1),labels(i))++;
                                ++nbg;
                            }
                        }
                    }
                }
            }
        }



        int k= 0;
        for ( int i= 0; i < coocfg.rows; ++i )
            for ( int j= 0; j < coocfg.columns; ++j )
            {
                output(actual + k)= coocfg(i,j)/(nfg*2.0);
                ++k;
            }
        for ( int i= 0; i < coocbg.rows; ++i )
            for ( int j= 0; j < coocbg.columns; ++j )
            {
                output(actual + k)= coocbg(i,j)/(nbg*2.0);
                ++k;
            }
    }






    template<typename INPUT>
    class RegionCooccuranceALBPFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionCooccuranceALBPFeature2(int N, int ds, int rs);

        RegionCooccuranceALBPFeature2(const RegionCooccuranceALBPFeature2& r);

        ~RegionCooccuranceALBPFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        LBPFeature2<INPUT, float>* lbp;
        int rs;
    };

    template<typename INPUT>
    RegionCooccuranceALBPFeature2<INPUT>::RegionCooccuranceALBPFeature2(int N, int ds, int rs)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCooccuranceALBPFeature2 " << N << " " << ds << " " << rs;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->rs= rs;
        lbp= new LBPFeature2<INPUT, float>(N, ds);
    }

    template<typename INPUT>
    RegionCooccuranceALBPFeature2<INPUT>::RegionCooccuranceALBPFeature2(const RegionCooccuranceALBPFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        this->rs= r.rs;
        this->lbp= new LBPFeature2<INPUT, float>(r.lbp->N, r.lbp->ds);
    }

    template<typename INPUT>
    RegionCooccuranceALBPFeature2<INPUT>::~RegionCooccuranceALBPFeature2()
    {
    }

    template<typename INPUT>
    int RegionCooccuranceALBPFeature2<INPUT>::dimensions()
    {
        int bins;
        if ( lbp->N == 0 )
            bins= 16;
        if ( lbp->N == 1 )
            bins= 16;
        if ( lbp->N == 2 )
            bins= 256;
        return bins*bins*4;
    }

    template<typename INPUT>
    void RegionCooccuranceALBPFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        transformed= 0;

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        debugFeatures(transformed);




        int bins;
        if ( lbp->N == 0 )
            bins= 16;
        if ( lbp->N == 1 )
            bins= 16;
        if ( lbp->N == 2 )
            bins= 256;

        Vector<Matrix<float> > cooc(4);
        Vector<int> pos;
        pos.push_back(rs);
        pos.push_back(rs*input.columns);
        pos.push_back(rs + rs*input.columns);
        pos.push_back(rs - rs*input.columns);

        for ( unsigned int i= 0; i < cooc.size(); ++i )
        {
            cooc(i).resize(bins, bins);
            cooc(i)= 0;
        }

        Vector<int> n(4);
        n= 0;

        Image<float> labels;
        labels.resizeImage(input.rows, input.columns);
        lbp->apply(input, labels, &roi, &roi);


        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( roi(i) )
            {
                for ( unsigned int j= 0; j < cooc.size(); ++j )
                {
                    if ( int(i) + pos(j) >= 0 && int(i) + pos(j) < int(input.n) && roi(i + pos(j)) )
                    {
                        cooc(j)(labels(i), labels(i + pos(j)))++;
                        cooc(j)(labels(i + pos(j)), labels(i))++;
                        n(j)++;
                    }
                }
            }
        }



        int k= 0;
        for ( unsigned int l= 0; l < cooc.size(); ++l )
            for ( int i= 0; i < cooc(l).rows; ++i )
                for ( int j= 0; j < cooc(l).columns; ++j )
                {
                    output(actual + k)= cooc(l)(i,j)/((float)n(l));
                    ++k;
                }
    }

    template<typename INPUT>
    class RegionCooccuranceARadialLBPFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionCooccuranceARadialLBPFeature2(int N, int ds, int rs, int ns, int m);

        RegionCooccuranceARadialLBPFeature2(const RegionCooccuranceARadialLBPFeature2& r);

        ~RegionCooccuranceARadialLBPFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        Feature2<INPUT, float>* lbp;
        int rs;
        int m;
        int ds;
        int N;
        int ns;
    };

    template<typename INPUT>
    RegionCooccuranceARadialLBPFeature2<INPUT>::RegionCooccuranceARadialLBPFeature2(int N, int ds, int rs, int ns, int m)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCooccuranceARadialLBPFeature2 " << N << " " << ds << " " << rs << " " << ns << " " << m;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->rs= rs;
        this->m= m;
        this->ds= ds;
        this->N= N;
        this->ns= ns;
        if ( m == 0 )
            lbp= new RadialMeanLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 1 )
            lbp= new RadialMinLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 2 )
            lbp= new RadialMaxLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 3 )
            lbp= new RadialMeanSuccLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 4 )
            lbp= new RadialMinSuccLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 5 )
            lbp= new RadialMaxSuccLBPFeature2<INPUT, float>(N, ds);
    }

    template<typename INPUT>
    RegionCooccuranceARadialLBPFeature2<INPUT>::RegionCooccuranceARadialLBPFeature2(const RegionCooccuranceARadialLBPFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        this->rs= r.rs;
        this->ds= r.ds;
        this->ns= r.ns;
        this->N= r.N;
        this->m= r.m;

        if ( m == 0 )
            lbp= new RadialMeanLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 1 )
            lbp= new RadialMinLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 2 )
            lbp= new RadialMaxLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 3 )
            lbp= new RadialMeanSuccLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 4 )
            lbp= new RadialMinSuccLBPFeature2<INPUT, float>(N, ds);
        else if ( m == 5 )
            lbp= new RadialMaxSuccLBPFeature2<INPUT, float>(N, ds);
    }

    template<typename INPUT>
    RegionCooccuranceARadialLBPFeature2<INPUT>::~RegionCooccuranceARadialLBPFeature2()
    {
    }

    template<typename INPUT>
    int RegionCooccuranceARadialLBPFeature2<INPUT>::dimensions()
    {
        int n;
        if ( N == 4 )
            n= 16;
        int bins= 0;
        for( int i= 0; i < n; ++i )
            for ( int j= i; j < n; ++j )
                ++bins;

        return bins*4;
    }

    template<typename INPUT>
    void RegionCooccuranceARadialLBPFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        transformed= 0;

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        debugFeatures(transformed);

        int bins;
        if ( N == 4 )
            bins= 16;

        Matrix<float> cooc;

        Vector<int> pos;

        float dphi= 2*M_PI/ns;
        for ( int i= 0; i < ns; ++i )
        {
            float phi= dphi*i;
            float r= rs*sin(phi);
            float c= rs*cos(phi);

            pos.push_back(c + r*input.columns);
        }

        cooc.resize(bins, bins);

        int n= 0;

        Image<float> labels;
        labels.resizeImage(input.rows, input.columns);
        labels= 0;
        lbp->apply(input, labels, &roi, &roi);

        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( roi(i) )
            {
                for ( unsigned int j= 0; j < pos.size(); ++j )
                {
                    if ( i + pos(j) >= 0 && i + pos(j) < (input.n) && roi(i + pos(j)) )
                    {
                        cooc(labels(i), labels(i + pos(j)))++;
                        cooc(labels(i + pos(j)), labels(i))++;
                        n++;
                    }
                }
            }
        }

        int k= 0;
        for ( int i= 0; i < cooc.rows; ++i )
            for ( int j= i; j < cooc.columns; ++j )
            {
                output(actual + k)= cooc(i,j)/((float)n);
                ++k;
            }
    }



    template<typename INPUT>
    class RegionCooccuranceRALBPFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionCooccuranceRALBPFeature2(int N, int ds, int rs);

        RegionCooccuranceRALBPFeature2(const RegionCooccuranceRALBPFeature2& r);

        ~RegionCooccuranceRALBPFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        RotationInvariantLBPFeature2<INPUT, float>* lbp;
        int rs;
    };

    template<typename INPUT>
    RegionCooccuranceRALBPFeature2<INPUT>::RegionCooccuranceRALBPFeature2(int N, int ds, int rs)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionCooccuranceRALBPFeature2 " << N << " " << ds << " " << rs;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->rs= rs;
        lbp= new RotationInvariantLBPFeature2<INPUT, float>(N, ds);
    }

    template<typename INPUT>
    RegionCooccuranceRALBPFeature2<INPUT>::RegionCooccuranceRALBPFeature2(const RegionCooccuranceRALBPFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        this->rs= r.rs;
        this->lbp= new LBPFeature2<INPUT, float>(r.lbp->N, r.lbp->ds);
    }

    template<typename INPUT>
    RegionCooccuranceRALBPFeature2<INPUT>::~RegionCooccuranceRALBPFeature2()
    {
    }

    template<typename INPUT>
    int RegionCooccuranceRALBPFeature2<INPUT>::dimensions()
    {
        int bins;
        if ( lbp->N == 0 )
            bins= 16;
        if ( lbp->N == 1 )
            bins= 16;
        if ( lbp->N == 2 )
            bins= 256;
        return bins*bins*4;
    }

    template<typename INPUT>
    void RegionCooccuranceRALBPFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        transformed= 0;

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        debugFeatures(transformed);




        int bins;
        if ( lbp->N == 0 )
            bins= 16;
        if ( lbp->N == 1 )
            bins= 16;
        if ( lbp->N == 2 )
            bins= 256;

        Vector<Matrix<float> > cooc(4);
        Vector<int> pos;
        pos.push_back(rs);
        pos.push_back(rs*input.columns);
        pos.push_back(rs + rs*input.columns);
        pos.push_back(rs - rs*input.columns);

        for ( int i= 0; i < cooc.size(); ++i )
        {
            cooc(i).resize(bins, bins);
            cooc(i)= 0;
        }

        Vector<int> n(4);
        n= 0;

        Image<float> labels;
        labels.resizeImage(input.rows, input.columns);
        lbp->apply(input, labels, &roi, &roi);


        for ( int i= 0; i < input.n; ++i )
        {
            if ( roi(i) )
            {
                for ( int j= 0; j < cooc.size(); ++j )
                {
                    if ( i + pos(j) >= 0 && i + pos(j) < input.n && roi(i + pos(j)) )
                    {
                        cooc(j)(labels(i), labels(i + pos(j)))++;
                        cooc(j)(labels(i + pos(j)), labels(i))++;
                        n(j)++;
                    }
                }
            }
        }



        int k= 0;
        for ( int l= 0; l < cooc.size(); ++l )
            for ( int i= 0; i < cooc(l).rows; ++i )
                for ( int j= 0; j < cooc(l).columns; ++j )
                {
                    output(actual + k)= cooc(l)(i,j)/((float)n(l));
                    ++k;
                }
    }

    /** test features */

    template<typename INPUT>
    class RegionShellsMeanFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionShellsMeanFeature2(int ds, int ns);

        RegionShellsMeanFeature2(const RegionShellsMeanFeature2& r);

        ~RegionShellsMeanFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int ds;
        int ns;
    };

    template<typename INPUT>
    RegionShellsMeanFeature2<INPUT>::RegionShellsMeanFeature2(int ds, int ns)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionShellsMeanFeature2 " << ds << " " << ns;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->ds= ds;
        this->ns= ns;
    }

    template<typename INPUT>
    RegionShellsMeanFeature2<INPUT>::RegionShellsMeanFeature2(const RegionShellsMeanFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        this->ds= r.ds;
        this->ns= r.ns;
    }

    template<typename INPUT>
    RegionShellsMeanFeature2<INPUT>::~RegionShellsMeanFeature2()
    {
    }

    template<typename INPUT>
    int RegionShellsMeanFeature2<INPUT>::dimensions()
    {
        return ns;
    }

    template<typename INPUT>
    void RegionShellsMeanFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> mask;
        mask.resizeImage(input);
        mask= 0;

        r.set(roi, (unsigned char)255);

        transformed= 0;

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        debugFeatures(transformed);

        StructuringElementDisk sed(ds, input.columns);

        Vector<float> results(ns);
        results= 0;

        int n;
        for ( int i= 0; i < ns; ++i )
        {
            n= 0;

            mask= roi;
            binaryErode(&roi, &tmp, sed, NULL);
            mask-= tmp;

            for ( unsigned int j= 0; j < roi.n; ++j)
                if ( mask(j) )
                {
                    ++n;
                    results(i)+= input(j);
                }
            if ( n > 0 )
                results(i)/= n;
            else
                results(i)= 0;

            roi= tmp;
        }

        for ( unsigned int j= 0; j < results.size(); ++j )
            output(actual + j)= results(j);
    }


    template<typename INPUT>
    class RegionShellsVarianceFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionShellsVarianceFeature2(int ds, int ns);

        RegionShellsVarianceFeature2(const RegionShellsVarianceFeature2& r);

        ~RegionShellsVarianceFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int ds;
        int ns;
    };

    template<typename INPUT>
    RegionShellsVarianceFeature2<INPUT>::RegionShellsVarianceFeature2(int ds, int ns)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionShellsVarianceFeature2 " << ds << " " << ns;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->ds= ds;
        this->ns= ns;
    }

    template<typename INPUT>
    RegionShellsVarianceFeature2<INPUT>::RegionShellsVarianceFeature2(const RegionShellsVarianceFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        this->ds= r.ds;
        this->ns= r.ns;
    }

    template<typename INPUT>
    RegionShellsVarianceFeature2<INPUT>::~RegionShellsVarianceFeature2()
    {
    }

    template<typename INPUT>
    int RegionShellsVarianceFeature2<INPUT>::dimensions()
    {
        return ns;
    }

    template<typename INPUT>
    void RegionShellsVarianceFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> mask;
        mask.resizeImage(input);
        mask= 0;

        r.set(roi, (unsigned char)255);

        transformed= 0;

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        debugFeatures(transformed);

        StructuringElementDisk sed(ds, input.columns);

        Vector<float> results(ns);
        results= 0;

        for ( int i= 0; i < ns; ++i )
        {
            mask= roi;
            binaryErode(&roi, &tmp, sed, NULL);
            mask-= tmp;

            results(i)= input.getVariance(&mask);

            roi= tmp;
        }

        for ( unsigned int j= 0; j < results.size(); ++j )
            output(actual + j)= results(j);
    }

    template<typename INPUT>
    class RegionShellsStandardDeviationFeature2: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionShellsStandardDeviationFeature2(int ds, int ns);

        RegionShellsStandardDeviationFeature2(const RegionShellsStandardDeviationFeature2& r);

        ~RegionShellsStandardDeviationFeature2();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int ds;
        int ns;
    };

    template<typename INPUT>
    RegionShellsStandardDeviationFeature2<INPUT>::RegionShellsStandardDeviationFeature2(int ds, int ns)
    : RegionFeature2<INPUT>()
    {
        std::stringstream ss;
        ss << "RegionShellsStandardDeviationFeature2 " << ds << " " << ns;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->ds= ds;
        this->ns= ns;
    }

    template<typename INPUT>
    RegionShellsStandardDeviationFeature2<INPUT>::RegionShellsStandardDeviationFeature2(const RegionShellsStandardDeviationFeature2& r)
        : RegionFeature2<INPUT>(r)
    {
        this->ds= r.ds;
        this->ns= r.ns;
    }

    template<typename INPUT>
    RegionShellsStandardDeviationFeature2<INPUT>::~RegionShellsStandardDeviationFeature2()
    {
    }

    template<typename INPUT>
    int RegionShellsStandardDeviationFeature2<INPUT>::dimensions()
    {
        return ns;
    }

    template<typename INPUT>
    void RegionShellsStandardDeviationFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        Image<unsigned char> mask;
        mask.resizeImage(input);
        mask= 0;

        r.set(roi, (unsigned char)255);

        transformed= 0;

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, &roi, NULL);

        debugFeatures(transformed);

        StructuringElementDisk sed(ds, input.columns);

        Vector<float> results(ns);
        results= 0;

        for ( int i= 0; i < ns; ++i )
        {
            mask= roi;
            binaryErode(&roi, &tmp, sed, NULL);
            mask-= tmp;

            results(i)= input.getStandardDeviation(&mask);

            roi= tmp;
        }

        for ( unsigned int j= 0; j < results.size(); ++j )
            output(actual + j)= results(j);
    }

    template<typename INPUT>
    class RegionHistogramOfMaximaGaussianResponse: public RegionFeature2<INPUT>
    {
    public:
        using RegionFeature2<INPUT>::descriptor;
        using RegionFeature2<INPUT>::requiresTransform;
        using RegionFeature2<INPUT>::reqTransform;
        using RegionFeature2<INPUT>::transform;
        using RegionFeature2<INPUT>::debugFeatures;
        using RegionFeature2<INPUT>::dimensions;
        using RegionFeature2<INPUT>::tm;
        using RegionFeature2<INPUT>::apply;

        RegionHistogramOfMaximaGaussianResponse(int bins);

        RegionHistogramOfMaximaGaussianResponse(const RegionHistogramOfMaximaGaussianResponse& r);

        ~RegionHistogramOfMaximaGaussianResponse();

        virtual int dimensions();

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);

        int bins;
    };

    template<typename INPUT>
    RegionHistogramOfMaximaGaussianResponse<INPUT>::RegionHistogramOfMaximaGaussianResponse(int bins)
    : RegionFeature2<INPUT>(3)
    {
        std::stringstream ss;
        ss << "RegionHistogramOfMaximaGaussianResponse " << bins;
        descriptor= ss.str();
        reqTransform= true;
        transform= NULL;
        this->bins= bins;
    }

    template<typename INPUT>
    RegionHistogramOfMaximaGaussianResponse<INPUT>::RegionHistogramOfMaximaGaussianResponse(const RegionHistogramOfMaximaGaussianResponse& r)
        : RegionFeature2<INPUT>(r)
    {
        bins= r.bins;
    }

    template<typename INPUT>
    RegionHistogramOfMaximaGaussianResponse<INPUT>::~RegionHistogramOfMaximaGaussianResponse()
    {
    }

    template<typename INPUT>
    int RegionHistogramOfMaximaGaussianResponse<INPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT>
    void RegionHistogramOfMaximaGaussianResponse<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        Image<INPUT> transformed;
        transformed.resizeImage(input);

        if ( transform == NULL )
            transformed= input;
        else
            transform->apply(input, transformed, NULL, NULL);

        Image<unsigned char> roi;
        roi.resizeImage(input);
        roi= 0;

        r.set(roi, (unsigned char)255);

        Image<unsigned char> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        tm->apply(transformed, tmp, &roi, &roi);

        debugFeatures(input);

        MatchedGaussianFilter2<INPUT, float> mgf(9.0, 1.0, 1.0, 1.0, -1.0, 0.0);
        mgf.setMatchType(1.0, 1.0, 5.0, MATCHED_GAUSSIAN_FILTER_SIGMA);
        mgf.updateStride(input.columns);

        Vector<float> results;
        for ( int i= -mgf.getMin(); i < int(input.n) - mgf.getMax(); ++i )
            if ( tmp(i) && roi(i) && input(i) >= input(i + 1) && input(i) >= input(i-1) && input(i) >= input(i + input.columns) && input(i) >= input(i - input.columns))
                results.push_back(mgf.apply(input, i, &roi));

        float minimum= results.getMin();
        float maximum= results.getMax();
        float d= (maximum - minimum)/(bins - 1);

        Vector<float> h(bins);
        h= 0;

        if ( d != 0 )
            for ( unsigned int i= 0; i < results.size(); ++i )
                h((int)floor(((float)results(i) - minimum)/d))++;
        else
            h(0)= 1;

        h/= results.size();

        for ( unsigned int i= 0; i < h.size(); ++i )
            output(actual + i)= h(i);
    }
}

#endif	/* REGIONFEATURES_H */

