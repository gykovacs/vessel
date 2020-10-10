/* 
 * File:   RunningStatisticalFeatures.h
 * Author: gykovacs
 *
 * Created on 2011. március 10., 18:39
 */

#ifndef RUNNINGSTATISTICALFEATURES_H
#define	RUNNINGSTATISTICALFEATURES_H

#include <openipDS/RunningStructuringElements.h>
#include <openipDS/Transform2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class RunningFeature2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        RunningFeature2(RunningStructuringElement<INPUT>* se);

        RunningFeature2(const RunningFeature2& r);

        virtual ~RunningFeature2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int updateStride(int stride);

        RunningStructuringElement<INPUT>* copyRSE(RunningStructuringElement<INPUT>* rse);

        RunningStructuringElement<INPUT>* first;
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceFeature2: public RunningFeature2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using RunningFeature2<INPUT, OUTPUT>::copyRSE;
        using RunningFeature2<INPUT, OUTPUT>::first;

        RunningDifferenceFeature2(RunningStructuringElement<INPUT>* first, RunningStructuringElement<INPUT>* second);

        RunningDifferenceFeature2(const RunningDifferenceFeature2& r);

        virtual ~RunningDifferenceFeature2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int updateStride(int stride);

        RunningStructuringElement<INPUT>* second;
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMeanFeature2: public RunningDifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMeanFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMeanFeature2(const RunningDifferenceRingMeanFeature2& r);

        ~RunningDifferenceRingMeanFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingVarianceFeature2: public RunningDifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingVarianceFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingVarianceFeature2(const RunningDifferenceRingVarianceFeature2& r);

        ~RunningDifferenceRingVarianceFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMinFeature2: public RunningDifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMinFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMinFeature2(const RunningDifferenceRingMinFeature2& r);

        ~RunningDifferenceRingMinFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMaxFeature2: public RunningDifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMaxFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMaxFeature2(const RunningDifferenceRingMaxFeature2& r);

        ~RunningDifferenceRingMaxFeature2();
    };
    
    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMaxMeanFeature2: public RunningDifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMaxMeanFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMaxMeanFeature2(const RunningDifferenceRingMaxMeanFeature2& r);

        ~RunningDifferenceRingMaxMeanFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMaxMinFeature2: public RunningDifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMaxMinFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMaxMinFeature2(const RunningDifferenceRingMaxMinFeature2& r);

        ~RunningDifferenceRingMaxMinFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningRingMeanFeature2: public RunningFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningFeature2<INPUT, OUTPUT>::descriptor;
        using RunningFeature2<INPUT, OUTPUT>::apply;
        using RunningFeature2<INPUT, OUTPUT>::getProposedBorder;

        RunningRingMeanFeature2(int inner, int outer, int stride= 4000);

        RunningRingMeanFeature2(const RunningRingMeanFeature2& r);

        ~RunningRingMeanFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningRingVarianceFeature2: public RunningFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningFeature2<INPUT, OUTPUT>::descriptor;
        using RunningFeature2<INPUT, OUTPUT>::apply;
        using RunningFeature2<INPUT, OUTPUT>::getProposedBorder;

        RunningRingVarianceFeature2(int inner, int outer, int stride= 4000);

        RunningRingVarianceFeature2(const RunningRingVarianceFeature2& r);

        ~RunningRingVarianceFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningRingMinFeature2: public RunningFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningFeature2<INPUT, OUTPUT>::descriptor;
        using RunningFeature2<INPUT, OUTPUT>::apply;
        using RunningFeature2<INPUT, OUTPUT>::getProposedBorder;

        RunningRingMinFeature2(int inner, int outer, int stride= 4000);

        RunningRingMinFeature2(const RunningRingMinFeature2& r);

        ~RunningRingMinFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningRingMaxFeature2: public RunningFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningFeature2<INPUT, OUTPUT>::descriptor;
        using RunningFeature2<INPUT, OUTPUT>::apply;
        using RunningFeature2<INPUT, OUTPUT>::getProposedBorder;

        RunningRingMaxFeature2(int inner, int outer, int stride= 4000);

        RunningRingMaxFeature2(const RunningRingMaxFeature2& r);

        ~RunningRingMaxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RunningFeature2<INPUT, OUTPUT>::RunningFeature2(RunningStructuringElement<INPUT>* rse)
    :Transform2<INPUT, OUTPUT>()
    {
        this->first= rse;
    }

    template<typename INPUT, typename OUTPUT>
    RunningFeature2<INPUT, OUTPUT>::RunningFeature2(const RunningFeature2& r)
    :Transform2<INPUT, OUTPUT>(r)
    {
        this->first= r.first;
    }

    template<typename INPUT, typename OUTPUT>
    RunningFeature2<INPUT, OUTPUT>::~RunningFeature2()
    {
        delete first;
    }

    template<typename INPUT, typename OUTPUT>
    RunningStructuringElement<INPUT>* RunningFeature2<INPUT, OUTPUT>::copyRSE(RunningStructuringElement<INPUT>* rse)
    {
        if ( NULL != dynamic_cast<RunningStructuringElementMax<INPUT>*>(rse) )
            return new RunningStructuringElementMax<INPUT>(*(dynamic_cast<RunningStructuringElementMax<INPUT>*>(rse)));
        if ( NULL != dynamic_cast<RunningStructuringElementMin<INPUT>*>(rse) )
            return new RunningStructuringElementMin<INPUT>(*(dynamic_cast<RunningStructuringElementMin<INPUT>*>(rse)));
        if ( NULL != dynamic_cast<RunningStructuringElementMeanAndVariance<INPUT>*>(rse) )
            return new RunningStructuringElementMeanAndVariance<INPUT>(*(dynamic_cast<RunningStructuringElementMeanAndVariance<INPUT>*>(rse)));
        if ( NULL != dynamic_cast<RunningStructuringElementMean<INPUT>*>(rse) )
            return new RunningStructuringElementMean<INPUT>(*(dynamic_cast<RunningStructuringElementMean<INPUT>*>(rse)));
        return NULL;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 RunningFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int n= - this->first->se.getMin() / this->first->se.stride + 1;
        return Border2(n, n, n, n);
    }

    template<typename INPUT, typename OUTPUT>
    int RunningFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        return this->first->se.updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    void RunningFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->first->se.updateStride(input.columns);

        #pragma omp parallel
        {
            RunningStructuringElement<INPUT>* rse= copyRSE(first);
            int num= omp_get_num_threads();
            int t= omp_get_thread_num();

            rse->init(&input, support);
            int begin, end;
            
            begin= -this->first->se.getMin() + (input.n - 2*this->first->se.getMax())/num*t;
            end= -this->first->se.getMin() + (input.n - 2*this->first->se.getMax())/num*(t+1);
            rse->init(begin);

            if ( roi == NULL )
                while ( rse->position < end )
                {
                    output(rse->position)= rse->value();
                    if ( rse->position == end - 1 )
                        break;
                    rse->next();
                }
            else
                while ( rse->position < end )
                {
                    if ( (*roi)(rse->position) > 0 )
                        output(rse->position)= rse->value();
                    if ( rse->position == end - 1 )
                        break;
                    rse->next();
                }
            delete rse;
        }
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceFeature2<INPUT, OUTPUT>::RunningDifferenceFeature2(RunningStructuringElement<INPUT>* first, RunningStructuringElement<INPUT>* second)
    : RunningFeature2<INPUT, OUTPUT>(first)
    {
        this->second= second;
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceFeature2<INPUT, OUTPUT>::RunningDifferenceFeature2(const RunningDifferenceFeature2& r)
    : RunningFeature2<INPUT, OUTPUT>(r)
    {
        this->second= r.second;
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceFeature2<INPUT, OUTPUT>::~RunningDifferenceFeature2()
    {
        delete second;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 RunningDifferenceFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int n= -this->second->se.getMin() / this->second->se.stride + 1;
        return Border2(n, n, n, n);
    }

    template<typename INPUT, typename OUTPUT>
    int RunningDifferenceFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->second->se.updateStride(stride);
        return this->first->se.updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    void RunningDifferenceFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->first->se.updateStride(input.columns);
        this->second->se.updateStride(input.columns);

        #pragma omp parallel
        {
            RunningStructuringElement<INPUT>* rseFirst= copyRSE(first);
            RunningStructuringElement<INPUT>* rseSecond= copyRSE(second);
            int num= omp_get_num_threads();
            int t= omp_get_thread_num();

            rseFirst->init(&input, support);
            rseSecond->init(&input, support);

            int begin, end;
            int min, max;
            min= this->second->se.getMin() < this->first->se.getMin() ? this->second->se.getMin(): this->first->se.getMin();
            max= this->second->se.getMax() > this->first->se.getMax() ? this->second->se.getMax(): this->first->se.getMax();

            begin= -min + (input.n - 2*max)/num*t;
            end= -min + (input.n - 2*max)/num*(t+1);

            rseFirst->init(begin);
            rseSecond->init(begin);

            if ( roi == NULL )
            {
                while ( rseFirst->position < end )
                {
                    output(rseFirst->position)= rseFirst->value() - rseSecond->value();
                    if ( rseFirst->position == end - 1 )
                        break;
                    rseFirst->next();
                    rseSecond->next();
                }
            }
            else
                while ( rseFirst->position < end )
                {
                    if ( (*roi)(rseFirst->position) > 0 )
                        output(rseFirst->position)= rseFirst->value() - rseSecond->value();
                    if ( rseFirst->position == end - 1 )
                        break;
                    rseFirst->next();
                    rseSecond->next();
                }
            delete rseFirst;
            delete rseSecond;
        }
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMeanFeature2<INPUT, OUTPUT>::RunningRingMeanFeature2(int inner, int outer, int stride)
    : RunningFeature2<INPUT, OUTPUT>(new RunningStructuringElementMean<INPUT>(StructuringElementRing(inner, outer, stride)))
    {
        stringstream ss;
        ss << "RunningRingMeanFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMeanFeature2<INPUT, OUTPUT>::RunningRingMeanFeature2(const RunningRingMeanFeature2& r)
    : RunningFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMeanFeature2<INPUT, OUTPUT>::~RunningRingMeanFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingVarianceFeature2<INPUT, OUTPUT>::RunningRingVarianceFeature2(int inner, int outer, int stride)
    : RunningFeature2<INPUT, OUTPUT>(new RunningStructuringElementMeanAndVariance<INPUT>(StructuringElementRing(inner, outer, stride)))
    {
        stringstream ss;
        ss << "RunningRingVarianceFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingVarianceFeature2<INPUT, OUTPUT>::RunningRingVarianceFeature2(const RunningRingVarianceFeature2& r)
    : RunningFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingVarianceFeature2<INPUT, OUTPUT>::~RunningRingVarianceFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMinFeature2<INPUT, OUTPUT>::RunningRingMinFeature2(int inner, int outer, int stride)
    : RunningFeature2<INPUT, OUTPUT>(new RunningStructuringElementMin<INPUT>(StructuringElementRing(inner, outer, stride)))
    {
        stringstream ss;
        ss << "RunningRingMinFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMinFeature2<INPUT, OUTPUT>::RunningRingMinFeature2(const RunningRingMinFeature2& r)
    : RunningFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMinFeature2<INPUT, OUTPUT>::~RunningRingMinFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMaxFeature2<INPUT, OUTPUT>::RunningRingMaxFeature2(int inner, int outer, int stride)
    : RunningFeature2<INPUT, OUTPUT>(new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner, outer, stride)))
    {
        stringstream ss;
        ss << "RunningRingMaxFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMaxFeature2<INPUT, OUTPUT>::RunningRingMaxFeature2(const RunningRingMaxFeature2& r)
    : RunningFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningRingMaxFeature2<INPUT, OUTPUT>::~RunningRingMaxFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMeanFeature2<INPUT, OUTPUT>::RunningDifferenceRingMeanFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(new RunningStructuringElementMean<INPUT>((StructuringElementRing(inner1, outer1, stride))), new RunningStructuringElementMean<INPUT>((StructuringElementRing(inner2, outer2, stride))))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMeanFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMeanFeature2<INPUT, OUTPUT>::RunningDifferenceRingMeanFeature2(const RunningDifferenceRingMeanFeature2& r)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMeanFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMeanFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingVarianceFeature2<INPUT, OUTPUT>::RunningDifferenceRingVarianceFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(new RunningStructuringElementMeanAndVariance<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMeanAndVariance<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingVarianceFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingVarianceFeature2<INPUT, OUTPUT>::RunningDifferenceRingVarianceFeature2(const RunningDifferenceRingVarianceFeature2& r)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingVarianceFeature2<INPUT, OUTPUT>::~RunningDifferenceRingVarianceFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMinFeature2<INPUT, OUTPUT>::RunningDifferenceRingMinFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(new RunningStructuringElementMin<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMin<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMinFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMinFeature2<INPUT, OUTPUT>::RunningDifferenceRingMinFeature2(const RunningDifferenceRingMinFeature2& r)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMinFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMinFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMaxFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxFeature2(const RunningDifferenceRingMaxFeature2& r)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMaxFeature2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxMeanFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxMeanFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMean<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMaxMeanFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxMeanFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxMeanFeature2(const RunningDifferenceRingMaxMeanFeature2& r)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxMeanFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMaxMeanFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxMinFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxMinFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMin<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMaxMinFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxMinFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxMinFeature2(const RunningDifferenceRingMaxMinFeature2& r)
    : RunningDifferenceFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxMinFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMaxMinFeature2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRatioFeature2: public RunningFeature2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using RunningFeature2<INPUT, OUTPUT>::copyRSE;
        using RunningFeature2<INPUT, OUTPUT>::first;

        RunningDifferenceRatioFeature2(RunningStructuringElement<INPUT>* first, RunningStructuringElement<INPUT>* second);

        RunningDifferenceRatioFeature2(const RunningDifferenceRatioFeature2& r);

        virtual ~RunningDifferenceRatioFeature2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int updateStride(int stride);

        RunningStructuringElement<INPUT>* second;
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMeanRatioFeature2: public RunningDifferenceRatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMeanRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMeanRatioFeature2(const RunningDifferenceRingMeanRatioFeature2& r);

        ~RunningDifferenceRingMeanRatioFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingVarianceRatioFeature2: public RunningDifferenceRatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingVarianceRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingVarianceRatioFeature2(const RunningDifferenceRingVarianceRatioFeature2& r);

        ~RunningDifferenceRingVarianceRatioFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMinRatioFeature2: public RunningDifferenceRatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMinRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMinRatioFeature2(const RunningDifferenceRingMinRatioFeature2& r);

        ~RunningDifferenceRingMinRatioFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    class RunningDifferenceRingMaxRatioFeature2: public RunningDifferenceRatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::descriptor;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::getProposedBorder;
        using RunningDifferenceRatioFeature2<INPUT, OUTPUT>::apply;

        RunningDifferenceRingMaxRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride= 4000);

        RunningDifferenceRingMaxRatioFeature2(const RunningDifferenceRingMaxRatioFeature2& r);

        ~RunningDifferenceRingMaxRatioFeature2();
    };
    
    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMeanRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingMeanRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(new RunningStructuringElementMean<INPUT>((StructuringElementRing(inner1, outer1, stride))), new RunningStructuringElementMean<INPUT>((StructuringElementRing(inner2, outer2, stride))))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMeanRatioFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMeanRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingMeanRatioFeature2(const RunningDifferenceRingMeanRatioFeature2& r)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMeanRatioFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMeanRatioFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingVarianceRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingVarianceRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(new RunningStructuringElementMeanAndVariance<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMeanAndVariance<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingVarianceFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingVarianceRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingVarianceRatioFeature2(const RunningDifferenceRingVarianceRatioFeature2& r)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingVarianceRatioFeature2<INPUT, OUTPUT>::~RunningDifferenceRingVarianceRatioFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMinRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingMinRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(new RunningStructuringElementMin<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMin<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMinRatioFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMinRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingMinRatioFeature2(const RunningDifferenceRingMinRatioFeature2& r)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMinRatioFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMinRatioFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxRatioFeature2(int inner1, int outer1, int inner2, int outer2, int stride)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner1, outer1, stride)), new RunningStructuringElementMax<INPUT>(StructuringElementRing(inner2, outer2, stride)))
    {
        stringstream ss;
        ss << "RunningDifferenceRingMaxRatioFeature2 " << inner1 << " " << outer1 << " " << inner2 << " " << outer2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRingMaxRatioFeature2(const RunningDifferenceRingMaxRatioFeature2& r)
    : RunningDifferenceRatioFeature2<INPUT, OUTPUT>(r)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRingMaxRatioFeature2<INPUT, OUTPUT>::~RunningDifferenceRingMaxRatioFeature2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRatioFeature2(RunningStructuringElement<INPUT>* first, RunningStructuringElement<INPUT>* second)
    : RunningFeature2<INPUT, OUTPUT>(first)
    {
        this->second= second;
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRatioFeature2<INPUT, OUTPUT>::RunningDifferenceRatioFeature2(const RunningDifferenceRatioFeature2& r)
    : RunningFeature2<INPUT, OUTPUT>(r)
    {
        this->second= r.second;
    }

    template<typename INPUT, typename OUTPUT>
    RunningDifferenceRatioFeature2<INPUT, OUTPUT>::~RunningDifferenceRatioFeature2()
    {
        delete second;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 RunningDifferenceRatioFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int n= -this->second->se.getMin() / this->second->se.stride + 1;
        return Border2(n, n, n, n);
    }

    template<typename INPUT, typename OUTPUT>
    int RunningDifferenceRatioFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->second->se.updateStride(stride);
        return this->first->se.updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    void RunningDifferenceRatioFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->first->se.updateStride(input.columns);
        this->second->se.updateStride(input.columns);

        #pragma omp parallel
        {
            RunningStructuringElement<INPUT>* rseFirst= copyRSE(first);
            RunningStructuringElement<INPUT>* rseSecond= copyRSE(second);
            int num= omp_get_num_threads();
            int t= omp_get_thread_num();

            rseFirst->init(&input, support);
            rseSecond->init(&input, support);

            int begin, end;
            int min, max;
            min= this->second->se.getMin() < this->first->se.getMin() ? this->second->se.getMin(): this->first->se.getMin();
            max= this->second->se.getMax() > this->first->se.getMax() ? this->second->se.getMax(): this->first->se.getMax();

            begin= -min + (input.n - 2*max)/num*t;
            end= -min + (input.n - 2*max)/num*(t+1);

            rseFirst->init(begin);
            rseSecond->init(begin);

            if ( roi == NULL )
            {
                while ( rseFirst->position < end )
                {
                    output(rseFirst->position)= (rseFirst->value() - rseSecond->value())/rseFirst->value();
                    if ( rseFirst->position == end - 1 )
                        break;
                    rseFirst->next();
                    rseSecond->next();
                }
            }
            else
                while ( rseFirst->position < end )
                {
                    if ( (*roi)(rseFirst->position) > 0 )
                        output(rseFirst->position)= (rseFirst->value() - rseSecond->value())/rseFirst->value();
                    if ( rseFirst->position == end - 1 )
                        break;
                    rseFirst->next();
                    rseSecond->next();
                }
            delete rseFirst;
            delete rseSecond;
        }
    }
}

#endif	/* RUNNINGSTATISTICALFEATURES_H */

